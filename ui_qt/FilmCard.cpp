#include "FilmCard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

FilmCard::FilmCard(Film* film, QWidget* parent)
    : QFrame(parent), film(film)
{
    setFixedSize(260, 300);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(
        "QFrame {"
        "  background: white;"
        "  border: 1px solid #e0e0e0;"
        "  border-radius: 12px;"
        "}"
        "QFrame:hover {"
        "  border: 2px solid #3498db;"
        "}"
    );
    setupUI();
}

void FilmCard::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(8);

    // Poster placeholder
    QLabel* poster = new QLabel(this);
    poster->setFixedHeight(120);
    poster->setAlignment(Qt::AlignCenter);
    poster->setText("FILM");
    poster->setStyleSheet(
        "background: #dfe6e9;"
        "border-radius: 8px;"
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #b2bec3;"
        "letter-spacing: 4px;"
    );
    layout->addWidget(poster);

    // Titlu
    QLabel* title = new QLabel(QString::fromStdString(film->getTitlu()), this);
    title->setStyleSheet("font-size: 15px; font-weight: bold; color: #2c3e50;");
    title->setWordWrap(true);
    layout->addWidget(title);

    // Tip + Gen
    QHBoxLayout* meta = new QHBoxLayout();

    QString tipColor = (film->getTip() == TipFilm::_3D) ? "#9b59b6" : "#e67e22";
    QLabel* tip = new QLabel(QString::fromStdString(film->tipToString()), this);
    tip->setFixedSize(34, 20);
    tip->setAlignment(Qt::AlignCenter);
    tip->setStyleSheet(QString(
        "background: %1; color: white;"
        "border-radius: 4px; font-size: 11px; font-weight: bold;"
    ).arg(tipColor));

    QLabel* gen = new QLabel(QString::fromStdString(film->getGen()), this);
    gen->setStyleSheet("font-size: 12px; color: #7f8c8d;");

    meta->addWidget(tip);
    meta->addSpacing(6);
    meta->addWidget(gen);
    meta->addStretch();
    layout->addLayout(meta);

    // Pret + durata
    double pretBaza = (film->getTip() == TipFilm::_3D) ? 45.0 : 30.0;
    QLabel* pret = new QLabel(
        QString("de la %1 lei  •  %2 min")
            .arg(pretBaza, 0, 'f', 0)
            .arg(film->getDurata()), this);
    pret->setStyleSheet("font-size: 12px; color: #27ae60; font-weight: bold;");
    layout->addWidget(pret);

    layout->addStretch();

    // Buton rezerva
    QPushButton* btn = new QPushButton("Rezerva loc", this);
    btn->setStyleSheet(
        "QPushButton {"
        "  background: #3498db; color: white;"
        "  border: none; border-radius: 8px;"
        "  padding: 9px; font-size: 13px; font-weight: bold;"
        "}"
        "QPushButton:hover { background: #2980b9; }"
        "QPushButton:pressed { background: #2471a3; }"
    );
    connect(btn, &QPushButton::clicked, this, [this]() {
        emit filmSelectat(film->getId());
    });
    layout->addWidget(btn);
}
