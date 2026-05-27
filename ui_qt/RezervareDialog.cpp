#include "RezervareDialog.h"
#include "../include/Rezervare.h"
#include "../include/Persistenta.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

static const std::string FISIER_REZERVARI = "rezervari.csv";

RezervareDialog::RezervareDialog(Cinematograf& cinema, int filmId, int salaId,
                                 int rand, int col, QWidget* parent)
    : QDialog(parent), cinema(cinema),
      filmId(filmId), salaId(salaId), rand(rand), col(col)
{
    setWindowTitle("Confirmare Rezervare");
    setFixedSize(420, 360);
    setStyleSheet("QDialog { background:#f5f6fa; }");

    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->setContentsMargins(24, 20, 24, 20);
    lay->setSpacing(12);

    QLabel* header = new QLabel("Rezervare Loc", this);
    header->setStyleSheet("font-size:20px; font-weight:bold; color:#2c3e50;");
    lay->addWidget(header);

    try {
        Sala* sala  = cinema.getSala(salaId);
        Film* film  = nullptr;
        for (auto* f : cinema.getFilme())
            if (f->getId() == filmId) { film = f; break; }

        if (film) {
            QLabel* filmLbl = new QLabel(
                QString("<b>Film:</b> %1 (%2)")
                    .arg(QString::fromStdString(film->getTitlu()))
                    .arg(QString::fromStdString(film->tipToString())), this);
            filmLbl->setStyleSheet("font-size:13px; color:#34495e;");
            lay->addWidget(filmLbl);
        }

        QLabel* locLbl = new QLabel(
            QString("<b>Loc:</b> Rand %1, Coloana %2")
                .arg(rand + 1).arg(col + 1), this);
        locLbl->setStyleSheet("font-size:13px; color:#34495e;");
        lay->addWidget(locLbl);

        TipLoc tipLoc = sala->getTipLoc(rand, col);
        QString tipStr = tipLoc == TipLoc::VIP ? "VIP (+50%)" :
                         tipLoc == TipLoc::STUDENT ? "Student (-20%)" : "Standard";
        QLabel* tipLbl = new QLabel(QString("<b>Tip loc:</b> %1").arg(tipStr), this);
        tipLbl->setStyleSheet("font-size:13px; color:#34495e;");
        lay->addWidget(tipLbl);

        if (film) {
            double pret = (film->getTip() == TipFilm::_3D) ? 45.0 : 30.0;
            if (tipLoc == TipLoc::VIP)     pret *= 1.5;
            if (tipLoc == TipLoc::STUDENT) pret *= 0.8;
            QLabel* pretLbl = new QLabel(
                QString("<b>Pret:</b> %1 lei").arg(pret), this);
            pretLbl->setStyleSheet("font-size:14px; color:#27ae60; font-weight:bold;");
            lay->addWidget(pretLbl);
        }
    } catch (...) {}

    lay->addSpacing(8);

    QLabel* emailLbl = new QLabel("Email pentru confirmare:", this);
    emailLbl->setStyleSheet("font-size:13px; color:#34495e;");
    lay->addWidget(emailLbl);

    emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText("exemplu@email.com");
    emailEdit->setStyleSheet(
        "padding:9px 12px; border:1.5px solid #ddd; border-radius:8px;"
        "font-size:13px; background:white;");
    lay->addWidget(emailEdit);

    lay->addStretch();

    QHBoxLayout* btnRow = new QHBoxLayout();
    QPushButton* cancelBtn = new QPushButton("Anuleaza", this);
    cancelBtn->setStyleSheet(
        "padding:10px 20px; border-radius:8px; font-size:13px;"
        "background:#ecf0f1; color:#7f8c8d;");
    QPushButton* confirmBtn = new QPushButton("Confirma Rezervarea", this);
    confirmBtn->setStyleSheet(
        "padding:10px 20px; border-radius:8px; font-size:13px; font-weight:bold;"
        "background:#27ae60; color:white;");

    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(confirmBtn);
    lay->addLayout(btnRow);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(confirmBtn, &QPushButton::clicked, this, &RezervareDialog::onConfirm);
}

void RezervareDialog::onConfirm() {
    QString email = emailEdit->text().trimmed();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Eroare", "Introduceti adresa de email!");
        return;
    }
    try {
        cinema.realizeazaRezervareOnline(filmId, salaId, rand, col, email.toStdString());

        // Salveaza imediat dupa rezervare pentru a persista datele
        Persistenta::salveaza(FISIER_REZERVARI, cinema);

        QMessageBox::information(this, "Succes",
            "Rezervarea a fost confirmata!\nO confirmare a fost trimisa la: " + email);
        accept();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Eroare", QString::fromStdString(e.what()));
    }
}
