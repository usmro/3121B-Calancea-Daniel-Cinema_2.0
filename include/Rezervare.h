#pragma once
#include <string>
#include <ctime>
#include "Film.h"
#include "Sala.h"

class Rezervare {
protected:
    static int nextId;
    int      id;
    Film*    film;
    Sala*    sala;
    int      rand;
    int      col;
    TipLoc   tipLoc;
    std::time_t dataRezervare;
    double   pretFinal;

    static bool esteWeekend(std::time_t t);

public:
    Rezervare(Film* film, Sala* sala, int rand, int col);
    virtual ~Rezervare() = default;

    int     getId()    const { return id; }
    Film*   getFilm()  const { return film; }
    Sala*   getSala()  const { return sala; }
    int     getRand()  const { return rand; }
    int     getCol()   const { return col; }
    TipLoc  getTipLoc()const { return tipLoc; }
    double  getPret()  const { return pretFinal; }

    double calculeazaPret() const;

    virtual void        afiseaza() const;
    virtual std::string getTip()   const { return "fizica"; }
};
