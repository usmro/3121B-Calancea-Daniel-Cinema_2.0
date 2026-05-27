#include "MainWindow.h"
#include "FilmCard.h"
#include "SalaWidget.h"
#include "../include/Film.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>

MainWindow::MainWindow(Cinematograf& cinema, QWidget* parent)
    : QMainWindow(parent), cinema(cinema)
{
    setWindowTitle("Cinema Central — Rezervare Bilete");
    setMinimumSize(920, 640);
    setupUI();
    incarcaFilme(cinema.getFilme());
}

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    setStyleSheet("QMainWindow { background: #f5f6fa; }");

    QVBoxLayout* root = new QVBoxLayout(central);
    root->setContentsMargins(24, 20, 24, 20);
    root->setSpacing(16);

    QLabel* title = new QLabel("Cinema Central", this);
    title->setStyleSheet("font-size:26px; font-weight:bold; color:#2c3e50;");
    root->addWidget(title);

    QLabel* subtitle = new QLabel("Alege filmul si rezerva-ti locul", this);
    subtitle->setStyleSheet("font-size:14px; color:#95a5a6;");
    root->addWidget(subtitle);

    QHBoxLayout* filterRow = new QHBoxLayout();
    filterRow->setSpacing(10);

    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Cauta dupa titlu...");
    searchBar->setStyleSheet(
        "padding:9px 14px; border:1.5px solid #ddd;"
        "border-radius:8px; font-size:14px; background:white;");

    tipFilter = new QComboBox(this);
    tipFilter->addItems({"Toate tipurile", "2D", "3D"});
    tipFilter->setStyleSheet(
        "padding:8px 12px; border:1.5px solid #ddd;"
        "border-radius:8px; font-size:13px; background:white;");
    tipFilter->setFixedWidth(160);

    filterRow->addWidget(searchBar, 3);
    filterRow->addWidget(tipFilter, 1);
    root->addLayout(filterRow);

    connect(searchBar, &QLineEdit::textChanged, this, &MainWindow::onFiltrareSchimbata);
    connect(tipFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFiltrareSchimbata);

    QScrollArea* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("border:none; background:transparent;");

    filmsContainer = new QWidget();
    filmsContainer->setStyleSheet("background:transparent;");
    filmsLayout = new QGridLayout(filmsContainer);
    filmsLayout->setSpacing(18);
    filmsLayout->setContentsMargins(0, 0, 0, 0);

    scroll->setWidget(filmsContainer);
    root->addWidget(scroll);
}

void MainWindow::stergeCarduri() {
    QLayoutItem* item;
    while ((item = filmsLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }
}

void MainWindow::incarcaFilme(const std::vector<Film*>& filme) {
    stergeCarduri();
    if (filme.empty()) {
        QLabel* msg = new QLabel("Nu exista filme disponibile.", filmsContainer);
        msg->setStyleSheet("font-size:16px; color:#95a5a6; padding:40px;");
        msg->setAlignment(Qt::AlignCenter);
        filmsLayout->addWidget(msg, 0, 0, 1, 3);
        return;
    }
    const int COLS = 3;
    int row = 0, col = 0;
    for (Film* f : filme) {
        FilmCard* card = new FilmCard(f, filmsContainer);
        connect(card, &FilmCard::filmSelectat, this, &MainWindow::onFilmSelectat);
        filmsLayout->addWidget(card, row, col);
        if (++col >= COLS) { col = 0; ++row; }
    }
}

void MainWindow::onFiltrareSchimbata() {
    std::vector<Film*> rezultat = cinema.getFilme();
    QString q = searchBar->text().trimmed().toLower();
    if (!q.isEmpty()) rezultat = cinema.cautaFilme(q.toStdString());

    if (tipFilter->currentIndex() == 1) {
        std::vector<Film*> tmp;
        for (auto* f : rezultat) if (f->getTip() == TipFilm::_2D) tmp.push_back(f);
        rezultat = tmp;
    } else if (tipFilter->currentIndex() == 2) {
        std::vector<Film*> tmp;
        for (auto* f : rezultat) if (f->getTip() == TipFilm::_3D) tmp.push_back(f);
        rezultat = tmp;
    }
    incarcaFilme(rezultat);
}

void MainWindow::onFilmSelectat(int filmId) {
    SalaWidget dlg(cinema, filmId, this);
    dlg.exec();
    incarcaFilme(cinema.getFilme());
}
