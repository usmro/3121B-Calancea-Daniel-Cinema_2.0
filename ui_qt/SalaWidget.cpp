#include "SalaWidget.h"
#include "RezervareDialog.h"
#include "../include/Sala.h"
#include "../include/Film.h"
#include "../include/Exceptii.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

SalaWidget::SalaWidget(Cinematograf& cinema, int filmId, QWidget* parent)
    : QDialog(parent), cinema(cinema), filmId(filmId)
{
    setWindowTitle("Selecteaza locul");
    setMinimumSize(520, 460);
    setupUI();
}

void SalaWidget::setupUI() {
    // Curata layout-ul existent daca e un refresh
    if (layout()) {
        QLayoutItem* item;
        while ((item = layout()->takeAt(0))) {
            delete item->widget();
            delete item;
        }
        delete layout();
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(14);

    // Gaseste filmul
    Film* film = nullptr;
    for (auto* f : cinema.getFilme())
        if (f->getId() == filmId) { film = f; break; }

    // Header cu info film
    if (film) {
        QLabel* filmInfo = new QLabel(
            QString("%1  —  %2")
                .arg(QString::fromStdString(film->getTitlu()))
                .arg(QString::fromStdString(film->tipToString())),
            this);
        filmInfo->setStyleSheet(
            "font-size: 17px; font-weight: bold; color: #2c3e50;");
        mainLayout->addWidget(filmInfo);
    }

    // Ecran
    QLabel* ecran = new QLabel("[ E C R A N ]", this);
    ecran->setAlignment(Qt::AlignCenter);
    ecran->setStyleSheet(
        "background: #dfe6e9; border-radius: 6px;"
        "padding: 6px; font-weight: bold;"
        "color: #95a5a6; letter-spacing: 6px; font-size: 13px;");
    mainLayout->addWidget(ecran);

    // Scroll area pentru grila de locuri
    QScrollArea* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("border: none;");

    QWidget* gridContainer = new QWidget();
    QVBoxLayout* gridWrap = new QVBoxLayout(gridContainer);
    gridWrap->setContentsMargins(0, 8, 0, 8);

    // Afiseaza toate salile disponibile
    bool oriceSala = false;
    for (auto* sala : cinema.getSali()) {
        oriceSala = true;

        QLabel* salaLabel = new QLabel(
            QString("Sala: %1  (%2 locuri libere)")
                .arg(QString::fromStdString(sala->getNume()))
                .arg(sala->getNumarLocuriLibere()),
            this);
        salaLabel->setStyleSheet(
            "font-size: 13px; color: #7f8c8d; font-weight: bold; margin-top: 8px;");
        gridWrap->addWidget(salaLabel);

        QGridLayout* seatGrid = new QGridLayout();
        seatGrid->setSpacing(5);

        for (int r = 0; r < sala->getNrRanduri(); ++r) {
            // Numar rand
            QLabel* randLabel = new QLabel(QString::number(r + 1), this);
            randLabel->setFixedSize(20, 38);
            randLabel->setAlignment(Qt::AlignCenter);
            randLabel->setStyleSheet("color: #95a5a6; font-size: 11px;");
            seatGrid->addWidget(randLabel, r, 0);

            for (int c = 0; c < sala->getNrColoane(); ++c) {
                QPushButton* seat = new QPushButton(this);
                seat->setFixedSize(38, 38);

                if (!sala->esteLiber(r, c)) {
                    seat->setEnabled(false);
                    seat->setStyleSheet(
                        "background: #e74c3c; border-radius: 6px; border: none;");
                    seat->setToolTip("Ocupat");
                } else {
                    TipLoc tip = sala->getTipLoc(r, c);
                    QString stil, label;
                    if (tip == TipLoc::VIP) {
                        stil = "QPushButton { background:#f39c12; border-radius:6px;"
                               "  border:none; color:white; font-weight:bold; font-size:11px;}"
                               "QPushButton:hover{background:#e67e22;}";
                        label = "V";
                    } else if (tip == TipLoc::STUDENT) {
                        stil = "QPushButton { background:#2ecc71; border-radius:6px;"
                               "  border:none; color:white; font-weight:bold; font-size:11px;}"
                               "QPushButton:hover{background:#27ae60;}";
                        label = "S";
                    } else {
                        stil = "QPushButton { background:#2ecc71; border-radius:6px; border:none;}"
                               "QPushButton:hover{background:#27ae60;}";
                        label = "";
                    }
                    seat->setStyleSheet(stil);
                    seat->setText(label);
                    seat->setToolTip(
                        QString("Rand %1, Col %2").arg(r + 1).arg(c + 1));

                    int capR = r, capC = c, capSalaId = sala->getId();
                    connect(seat, &QPushButton::clicked, this,
                            [this, capR, capC, capSalaId]() {
                                onLocSelectat(capR, capC, capSalaId);
                            });
                }
                seatGrid->addWidget(seat, r, c + 1);
            }
        }

        QWidget* gridW = new QWidget();
        gridW->setLayout(seatGrid);
        gridWrap->addWidget(gridW);
    }

    if (!oriceSala) {
        QLabel* noSala = new QLabel("Nu exista sali disponibile.", this);
        noSala->setStyleSheet("color: #7f8c8d; font-size: 14px; padding: 20px;");
        gridWrap->addWidget(noSala);
    }

    scroll->setWidget(gridContainer);
    mainLayout->addWidget(scroll);

    // Legenda
    QHBoxLayout* legend = new QHBoxLayout();

    auto addLegend = [&](const QString& bg, const QString& text) {
        QLabel* dot = new QLabel(this);
        dot->setFixedSize(16, 16);
        dot->setStyleSheet(
            QString("background: %1; border-radius: 3px;").arg(bg));
        legend->addWidget(dot);
        QLabel* lbl = new QLabel(text, this);
        lbl->setStyleSheet("font-size: 12px; color: #7f8c8d;");
        legend->addWidget(lbl);
        legend->addSpacing(14);
    };

    addLegend("#2ecc71", "Liber");
    addLegend("#e74c3c", "Ocupat");
    addLegend("#f39c12", "VIP  (+50%)");
    addLegend("#2ecc71", "S = Student  (-20%)");
    legend->addStretch();
    mainLayout->addLayout(legend);
}

void SalaWidget::onLocSelectat(int rand, int col, int salaId) {
    RezervareDialog dlg(cinema, filmId, salaId, rand, col, this);
    if (dlg.exec() == QDialog::Accepted)
        refresh();
}

void SalaWidget::refresh() {
    setupUI();
}
