#pragma once
#include <QFrame>
#include "../include/Film.h"

class FilmCard : public QFrame {
    Q_OBJECT

public:
    explicit FilmCard(Film* film, QWidget* parent = nullptr);

signals:
    void filmSelectat(int filmId);

private:
    Film* film;
    void setupUI();
};
