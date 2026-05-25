#include "../include/Film.h"
#include <iostream>
#include <iomanip>

int Film::nextId = 1;

Film::Film(const std::string& titlu, const std::string& gen,
           int durata, TipFilm tip,
           const std::string& descriere,
           const std::string& posterPath)
    : id(nextId++), titlu(titlu), gen(gen),
      durata(durata), tip(tip),
      descriere(descriere), posterPath(posterPath) {}

int         Film::getId()         const { return id; }
std::string Film::getTitlu()      const { return titlu; }
std::string Film::getGen()        const { return gen; }
int         Film::getDurata()     const { return durata; }
TipFilm     Film::getTip()        const { return tip; }
std::string Film::getDescriere()  const { return descriere; }
std::string Film::getPosterPath() const { return posterPath; }

void Film::setPosterPath(const std::string& path) { posterPath = path; }

std::string Film::tipToString() const {
    return (tip == TipFilm::_3D) ? "3D" : "2D";
}

void Film::afiseaza() const {
    std::cout << std::left
              << std::setw(4)  << id
              << std::setw(28) << titlu
              << std::setw(8)  << tipToString()
              << std::setw(14) << gen
              << durata << " min\n";
}
