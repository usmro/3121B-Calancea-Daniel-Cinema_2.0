#include "SalaWidget.h"
#include "RezervareDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTabWidget>

SalaWidget::SalaWidget(Cinematograf& cinema, int filmId, QWidget* parent)
    : QDialog(parent), cinema(cinema), filmId(filmId)
{
    setWindowTitle("Selecteaza Locul");
    setMinimumSize(520, 480);
    setStyleSheet("QDialog { background:#f5f6fa; }");
    setupUI();
}

void SalaWidget::setupUI() {
    QVBoxLayout* main = new QVBoxLayout(this);
    main->setContentsMargins(20, 16, 20, 16);
    main->setSpacing(12);

    // Titlu
    Film* film = nullptr;
    for (auto* f : cinema.getFilme())
        if (f->getId() == filmId) { film = f; break; }

    QLabel* title = new QLabel(this);
    if (film)
        title->setText(QString("<b>%1</b> — Selecteaza locul")
                       .arg(QString::fromStdString(film->getTitlu())));
    else
        title->setText("Selecteaza locul");
    title->setStyleSheet("font-size:17px; color:#2c3e50;");
    main->addWidget(title);

    // Legenda
    QHBoxLayout* legend = new QHBoxLayout();
    auto mkLeg = [&](const QString& color, const QString& text) {
        QLabel* l = new QLabel(this);
        l->setText(QString("<span style='background:%1;color:%1;'>___</span> %2")
                   .arg(color, text));
        l->setStyleSheet("font-size:11px; color:#555;");
        legend->addWidget(l);
    };
    QLabel* leg1 = new QLabel("■ Liber", this);  leg1->setStyleSheet("color:#27ae60; font-size:11px;");
    QLabel* leg2 = new QLabel("■ VIP",   this);  leg2->setStyleSheet("color:#f39c12; font-size:11px;");
    QLabel* leg3 = new QLabel("■ Student",this); leg3->setStyleSheet("color:#3498db; font-size:11px;");
    QLabel* leg4 = new QLabel("■ Ocupat",this);  leg4->setStyleSheet("color:#e74c3c; font-size:11px;");
    legend->addWidget(leg1); legend->addWidget(leg2);
    legend->addWidget(leg3); legend->addWidget(leg4);
    legend->addStretch();
    main->addLayout(legend);

    // Tab-uri pentru fiecare sala
    QTabWidget* tabs = new QTabWidget(this);
    tabs->setStyleSheet("QTabBar::tab { padding:6px 16px; font-size:12px; }");

    auto sali = cinema.getSali();
    if (sali.empty()) {
        QLabel* noSala = new QLabel("Nu exista sali disponibile.", this);
        main->addWidget(noSala);
    }

    for (Sala* sala : sali) {
        QWidget* page = new QWidget();
        QVBoxLayout* pagelay = new QVBoxLayout(page);
        pagelay->setSpacing(8);

        // Ecran
        QLabel* screen = new QLabel("[ E C R A N ]", page);
        screen->setAlignment(Qt::AlignCenter);
        screen->setStyleSheet(
            "background:#bdc3c7; color:#7f8c8d; border-radius:4px;"
            "padding:4px; font-size:11px; font-weight:bold;");
        pagelay->addWidget(screen);

        // Grid locuri
        QScrollArea* scroll = new QScrollArea(page);
        scroll->setWidgetResizable(true);
        scroll->setStyleSheet("border:none;");

        QWidget* grid_w = new QWidget();
        QGridLayout* grid = new QGridLayout(grid_w);
        grid->setSpacing(4);

        int salaId = sala->getId();
        int rows = sala->getNrRanduri();
        int cols = sala->getNrColoane();

        for (int r = 0; r < rows; ++r) {
            QLabel* rowLbl = new QLabel(QString::number(r+1), grid_w);
            rowLbl->setStyleSheet("font-size:11px; color:#aaa;");
            rowLbl->setFixedWidth(20);
            grid->addWidget(rowLbl, r, 0);

            for (int c = 0; c < cols; ++c) {
                QPushButton* btn = new QPushButton(grid_w);
                btn->setFixedSize(36, 32);

                if (!sala->esteLiber(r, c)) {
                    btn->setText("X");
                    btn->setEnabled(false);
                    btn->setStyleSheet(
                        "background:#e74c3c; color:white; border-radius:4px;"
                        "font-size:11px; font-weight:bold;");
                } else {
                    TipLoc tip = sala->getTipLoc(r, c);
                    QString bg, label;
                    if (tip == TipLoc::VIP) {
                        bg = "#f39c12"; label = "V";
                    } else if (tip == TipLoc::STUDENT) {
                        bg = "#3498db"; label = "S";
                    } else {
                        bg = "#27ae60"; label = "";
                    }
                    btn->setText(label);
                    btn->setStyleSheet(
                        QString("QPushButton { background:%1; color:white; border-radius:4px;"
                                "font-size:11px; font-weight:bold; }"
                                "QPushButton:hover { background:#2c3e50; }").arg(bg));

                    connect(btn, &QPushButton::clicked, this,
                            [this, salaId, r, c]() { onLocSelectat(salaId, r, c); });
                }
                grid->addWidget(btn, r, c + 1);
            }
        }

        scroll->setWidget(grid_w);
        pagelay->addWidget(scroll);

        QLabel* info = new QLabel(
            QString("%1 locuri libere din %2")
                .arg(sala->getNumarLocuriLibere())
                .arg(rows * cols), page);
        info->setStyleSheet("font-size:12px; color:#7f8c8d;");
        pagelay->addWidget(info);

        tabs->addTab(page, QString::fromStdString(sala->getNume()));
    }

    main->addWidget(tabs);

    QPushButton* closeBtn = new QPushButton("Inchide", this);
    closeBtn->setStyleSheet(
        "padding:8px 20px; background:#ecf0f1; border-radius:8px;"
        "font-size:13px; color:#7f8c8d;");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
    main->addWidget(closeBtn, 0, Qt::AlignRight);
}

void SalaWidget::onLocSelectat(int salaId, int rand, int col) {
    RezervareDialog dlg(cinema, filmId, salaId, rand, col, this);
    if (dlg.exec() == QDialog::Accepted) {
        // Reinitializeaza UI dupa rezervare
        QLayout* old = this->layout();
        QLayoutItem* item;
        while ((item = old->takeAt(0))) {
            delete item->widget();
            delete item;
        }
        delete old;
        setupUI();
    }
}
