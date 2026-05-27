#pragma once
#include <QDialog>
#include <QLineEdit>
#include "../include/Cinematograf.h"

class RezervareDialog : public QDialog {
    Q_OBJECT

public:
    RezervareDialog(Cinematograf& cinema,
                    int filmId, int salaId,
                    int rand, int col,
                    QWidget* parent = nullptr);

private:
    Cinematograf& cinema;
    int filmId, salaId, rand, col;
    QLineEdit* emailInput;

    void setupUI();
    void onConfirm();
};
