#pragma once
#include "UITerminal.h"

class UIAngajat : public UITerminal {
public:
    explicit UIAngajat(Cinematograf& cinema);
    void run();

private:
    void afiseazaMeniu() const;
    void realizeazaRezervare();
    void anuleazaRezervare();
};
