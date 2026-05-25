#pragma once
#include <ctime>
#include <string>
#include "Tipuri.h"

class Film;
class Sala;

class Rezervare {
protected:
    int         id;
    Film*       film;
    Sala*       sala;
    int         rand;
    int         col;
    TipLoc      tipLoc;
    std::time_t dataRezervare;
    double      pretFinal;

    static int nextId;

public:
    Rezervare(Film* film, Sala* sala, int rand, int col);
    virtual ~Rezervare() = default;

    // Getteri
    int         getId()            const;
    Film*       getFilm()          const;
    Sala*       getSala()          const;
    int         getRand()          const;
    int         getCol()           const;
    TipLoc      getTipLoc()        const;
    double      getPretFinal()     const;
    std::time_t getDataRezervare() const;

    // Logica pret:
    //   baza:    2D = 30 lei, 3D = 45 lei
    //   VIP:     +50%   Student: -20%
    //   weekend: +15%
    double calculeazaPret() const;

    virtual void        afiseaza() const;
    virtual std::string getTip()   const { return "fizica"; }

private:
    static bool esteWeekend(std::time_t t);
};
