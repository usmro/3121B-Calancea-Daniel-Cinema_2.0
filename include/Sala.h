#pragma once
#include <string>
#include <vector>
#include "Tipuri.h"

class Sala {
private:
    int         id;
    std::string nume;
    int         nrRanduri;
    int         nrColoane;

    std::vector<std::vector<bool>>    ocupat;      // true = loc ocupat
    std::vector<std::vector<TipLoc>>  tipuriLocuri;

    static int nextId;

public:
    Sala(const std::string& nume, int nrRanduri, int nrColoane);

    // Getteri
    int         getId()        const;
    std::string getNume()      const;
    int         getNrRanduri() const;
    int         getNrColoane() const;

    // Operatii pe locuri
    bool   esteLiber(int rand, int col)            const;
    TipLoc getTipLoc(int rand, int col)            const;
    void   setTipLoc(int rand, int col, TipLoc tip);
    void   ocupa(int rand, int col);
    void   elibereaza(int rand, int col);

    // Statistici
    bool areLocuriLibere()     const;
    int  getNumarLocuriLibere() const;

    // Afisare
    void afiseazaLocuri() const;

private:
    void valideazaIndex(int rand, int col) const;
};
