#include "FilmCard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

FilmCard::FilmCard(Film* film, QWidget* parent)
    : QFrame(parent), film(film)
{
    setFixedSize(260, 300);
    setStyleSheet(
        "FilmCard { background: white; border-radius: 12px;"
        "border: 1px solid #e0e0e0; }"
        "FilmCard:hover { border: 1.5px solid #3498db; }");

    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->setContentsMargins(12, 12, 12, 12);
    lay->setSpacing(6);

    // Poster placeholder
    QLabel* poster = new QLabel(this);
    poster->setFixedHeight(130);
    poster->setAlignment(Qt::AlignCenter);
    poster->setText("FILM");
    poster->setStyleSheet(
        "background: #dfe6e9; border-radius: 8px;"
        "font-size: 14px; color: #95a5a6; font-weight: bold;");
    lay->addWidget(poster);

    // Titlu
    QLabel* title = new QLabel(QString::fromStdString(film->getTitlu()), this);
    title->setStyleSheet("font-size: 14px; font-weight: bold; color: #2c3e50;");
    title->setWordWrap(true);
    lay->addWidget(title);

    // Badges
    QHBoxLayout* badges = new QHBoxLayout();
    badges->setSpacing(6);

    QLabel* tipBadge = new QLabel(QString::fromStdString(film->tipToString()), this);
    QString tipColor = (film->getTip() == TipFilm::_3D) ? "#8e44ad" : "#e67e22";
    tipBadge->setStyleSheet(
        "background:" + tipColor + "; color:white; border-radius:4px;"
        "padding:2px 8px; font-size:11px; font-weight:bold;");
    tipBadge->setFixedHeight(20);

    QLabel* genBadge = new QLabel(QString::fromStdString(film->getGen()), this);
    genBadge->setStyleSheet(
        "background:#ecf0f1; color:#7f8c8d; border-radius:4px;"
        "padding:2px 8px; font-size:11px;");
    genBadge->setFixedHeight(20);

    badges->addWidget(tipBadge);
    badges->addWidget(genBadge);
    badges->addStretch();
    lay->addLayout(badges);

    // Pret si durata
    double pretBaza = (film->getTip() == TipFilm::_3D) ? 45.0 : 30.0;
    QLabel* info = new QLabel(
        QString("de la %1 lei  •  %2 min")
            .arg(pretBaza).arg(film->getDurata()), this);
    info->setStyleSheet("font-size:12px; color:#27ae60; font-weight:bold;");
    lay->addWidget(info);

    lay->addStretch();

    // Buton
    QPushButton* btn = new QPushButton("Rezerva loc", this);
    btn->setStyleSheet(
        "QPushButton { background:#3498db; color:white; border-radius:8px;"
        "padding:8px; font-size:13px; font-weight:bold; }"
        "QPushButton:hover { background:#2980b9; }"
        "QPushButton:pressed { background:#1a6fa3; }");
    lay->addWidget(btn);

    connect(btn, &QPushButton::clicked, this, [this]() {
        emit filmSelectat(this->film->getId());
    });
}
