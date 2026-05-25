#pragma once
#include "UITerminal.h"

class UIAngajat : public UITerminal {
private:
    void meniuPrincipal();

    void afiseazaFilme();
    void afiseazaLocuri();
    void realizeazaRezervare();
    void afiseazaRezervari();
    void cautaFilme();

public:
    explicit UIAngajat(Cinematograf& cinema);
    void run() override;
};
