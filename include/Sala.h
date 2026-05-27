#pragma once
#include <string>
#include <vector>
#include "Tipuri.h"

class Sala {
private:
    static int nextId;
    int         id;
    std::string nume;
    int         nrRanduri;
    int         nrColoane;
    std::vector<std::vector<bool>>    ocupat;
    std::vector<std::vector<TipLoc>>  tipuriLocuri;

    void valideazaIndex(int rand, int col) const;

public:
    Sala(const std::string& nume, int nrRanduri, int nrColoane);

    int         getId()      const { return id; }
    std::string getNume()    const { return nume; }
    int         getNrRanduri() const { return nrRanduri; }
    int         getNrColoane() const { return nrColoane; }

    bool    esteLiber(int rand, int col)           const;
    TipLoc  getTipLoc(int rand, int col)           const;
    void    setTipLoc(int rand, int col, TipLoc t);
    void    ocupa(int rand, int col);
    void    elibereaza(int rand, int col);

    bool areLocuriLibere()      const;
    int  getNumarLocuriLibere() const;
    void afiseazaLocuri()       const;
};
