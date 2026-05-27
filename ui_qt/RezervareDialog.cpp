#include "RezervareDialog.h"
#include "../include/Film.h"
#include "../include/Sala.h"
#include "../include/Exceptii.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <ctime>

RezervareDialog::RezervareDialog(Cinematograf& cinema,
                                 int filmId, int salaId,
                                 int rand, int col,
                                 QWidget* parent)
    : QDialog(parent), cinema(cinema),
      filmId(filmId), salaId(salaId),
      rand(rand), col(col)
{
    setWindowTitle("Rezervare bilet");
    setFixedSize(420, 360);
    setupUI();
}

void RezervareDialog::setupUI() {
    // Gaseste filmul
    Film* film = nullptr;
    for (auto* f : cinema.getFilme())
        if (f->getId() == filmId) { film = f; break; }

    Sala* sala = nullptr;
    try { sala = cinema.getSala(salaId); } catch (...) {}

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(14);

    // Titlu
    QLabel* title = new QLabel("Confirmare rezervare", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50;");
    layout->addWidget(title);

    QFrame* separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("color: #ecf0f1;");
    layout->addWidget(separator);

    // Info film
    if (film) {
        QLabel* filmLabel = new QLabel(
            QString("%1  (%2)")
                .arg(QString::fromStdString(film->getTitlu()))
                .arg(QString::fromStdString(film->tipToString())),
            this);
        filmLabel->setStyleSheet("font-size: 15px; color: #34495e; font-weight: bold;");
        layout->addWidget(filmLabel);
    }

    // Info loc
    QString tipLocStr = "Standard";
    if (sala) {
        TipLoc tip = sala->getTipLoc(rand, col);
        if (tip == TipLoc::VIP)     tipLocStr = "VIP";
        if (tip == TipLoc::STUDENT) tipLocStr = "Student";
    }
    QLabel* locLabel = new QLabel(
        QString("Rand %1, Coloana %2  —  %3")
            .arg(rand + 1).arg(col + 1).arg(tipLocStr),
        this);
    locLabel->setStyleSheet("font-size: 13px; color: #7f8c8d;");
    layout->addWidget(locLabel);

    // Calcul pret
    double pret = film ? (film->getTip() == TipFilm::_3D ? 45.0 : 30.0) : 0;
    if (sala) {
        TipLoc tip = sala->getTipLoc(rand, col);
        if (tip == TipLoc::VIP)     pret *= 1.50;
        if (tip == TipLoc::STUDENT) pret *= 0.80;
    }
    std::time_t now = std::time(nullptr);
    std::tm* t = std::localtime(&now);
    bool weekend = (t->tm_wday == 0 || t->tm_wday == 6);
    if (weekend) pret *= 1.15;

    QLabel* pretLabel = new QLabel(
        QString("Pret: %1 lei%2")
            .arg(pret, 0, 'f', 0)
            .arg(weekend ? "  (include majorare weekend)" : ""),
        this);
    pretLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #27ae60;");
    layout->addWidget(pretLabel);

    // Email
    QLabel* emailLabel = new QLabel("Email pentru confirmare:", this);
    emailLabel->setStyleSheet("font-size: 13px; color: #7f8c8d;");
    layout->addWidget(emailLabel);

    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("exemplu@gmail.com");
    emailInput->setStyleSheet(
        "padding: 10px; border: 1.5px solid #ddd;"
        "border-radius: 8px; font-size: 14px;"
    );
    layout->addWidget(emailInput);

    layout->addStretch();

    // Butoane
    QHBoxLayout* btnRow = new QHBoxLayout();

    QPushButton* cancelBtn = new QPushButton("Anuleaza", this);
    cancelBtn->setStyleSheet(
        "QPushButton { background: #ecf0f1; color: #7f8c8d; border: none;"
        "  border-radius: 8px; padding: 10px 20px; font-size: 13px; }"
        "QPushButton:hover { background: #bdc3c7; }"
    );
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    QPushButton* confirmBtn = new QPushButton("Confirma rezervarea", this);
    confirmBtn->setStyleSheet(
        "QPushButton { background: #3498db; color: white; border: none;"
        "  border-radius: 8px; padding: 10px 20px; font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background: #2980b9; }"
    );
    connect(confirmBtn, &QPushButton::clicked, this, &RezervareDialog::onConfirm);

    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(confirmBtn);
    layout->addLayout(btnRow);
}

void RezervareDialog::onConfirm() {
    std::string email = emailInput->text().trimmed().toStdString();

    if (email.empty()) {
        QMessageBox::warning(this, "Email lipsa",
                             "Te rugam sa introduci adresa de email.");
        return;
    }

    try {
        cinema.realizeazaRezervareOnline(filmId, salaId, rand, col, email);
        QMessageBox::information(this, "Rezervare confirmata!",
            QString("Rezervarea a fost realizata cu succes!\n"
                    "Confirmarea a fost trimisa la: %1")
                .arg(QString::fromStdString(email)));
        accept();
    } catch (const CinemaException& e) {
        QMessageBox::critical(this, "Eroare",
                              QString::fromStdString(e.what()));
    }
}
