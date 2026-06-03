#pragma once
#include <string>
#include "Tipuri.h"

class Film {
private:
    static int nextId;
    int         id;
    std::string titlu;
    std::string gen;
    int         durata;
    TipFilm     tip;
    std::string descriere;
    std::string posterPath;

public:
    Film(const std::string& titlu, const std::string& gen,
         int durata, TipFilm tip, const std::string& descriere = "");

    int         getId()        const { return id; }
    std::string getTitlu()     const { return titlu; }
    std::string getGen()       const { return gen; }
    int         getDurata()    const { return durata; }
    TipFilm     getTip()       const { return tip; }
    std::string getDescriere() const { return descriere; }
    std::string getPosterPath()const { return posterPath; }

    void setPosterPath(const std::string& path) { posterPath = path; }

    std::string tipToString() const;
    void        afiseaza()    const;
};
