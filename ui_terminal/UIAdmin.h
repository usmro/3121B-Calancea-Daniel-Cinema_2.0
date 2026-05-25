#pragma once
#include "UITerminal.h"

class UIAdmin : public UITerminal {
private:
    void meniuPrincipal();

    // Gestiune (doar admin)
    void adaugaFilm();
    void stergeFilm();
    void adaugaSala();
    void stergeSala();

    // Vizualizare (shared cu angajat)
    void afiseazaFilme();
    void afiseazaLocuri();
    void afiseazaRezervari();
    void cautaFilme();

public:
    explicit UIAdmin(Cinematograf& cinema);
    void run() override;
};
