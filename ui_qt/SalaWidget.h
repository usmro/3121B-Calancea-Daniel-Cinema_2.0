#pragma once
#include <QDialog>
#include "../include/Cinematograf.h"

class SalaWidget : public QDialog {
    Q_OBJECT
public:
    SalaWidget(Cinematograf& cinema, int filmId, QWidget* parent = nullptr);

private:
    Cinematograf& cinema;
    int filmId;
    void setupUI();
    void onLocSelectat(int salaId, int rand, int col);
};
