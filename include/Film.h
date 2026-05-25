#pragma once
#include <string>
#include "Tipuri.h"

class Film {
private:
    int         id;
    std::string titlu;
    std::string gen;
    int         durata;       // minute
    TipFilm     tip;
    std::string descriere;
    std::string posterPath;

    static int nextId;

public:
    Film(const std::string& titlu, const std::string& gen,
         int durata, TipFilm tip,
         const std::string& descriere = "",
         const std::string& posterPath = "");

    // Getteri
    int         getId()         const;
    std::string getTitlu()      const;
    std::string getGen()        const;
    int         getDurata()     const;
    TipFilm     getTip()        const;
    std::string getDescriere()  const;
    std::string getPosterPath() const;

    // Setteri
    void setPosterPath(const std::string& path);

    // Utilitare
    std::string tipToString() const;
    void        afiseaza()    const;
};
