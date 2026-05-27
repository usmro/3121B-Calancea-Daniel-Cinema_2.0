#pragma once
#include "UITerminal.h"

class UIAdmin : public UITerminal {
public:
    explicit UIAdmin(Cinematograf& cinema);
    void run();

private:
    void afiseazaMeniu() const;
    void adaugaFilm();
    void stergeFilm();
    void adaugaSala();
    void stergeSala();
};
