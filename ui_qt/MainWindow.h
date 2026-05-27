#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include "../include/Cinematograf.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Cinematograf& cinema, QWidget* parent = nullptr);

private slots:
    void onFiltrareSchimbata();
    void onFilmSelectat(int filmId);

private:
    Cinematograf& cinema;
    QLineEdit*    searchBar;
    QComboBox*    tipFilter;
    QWidget*      filmsContainer;
    QGridLayout*  filmsLayout;

    void setupUI();
    void incarcaFilme(const std::vector<Film*>& filme);
    void stergeCarduri();
};
