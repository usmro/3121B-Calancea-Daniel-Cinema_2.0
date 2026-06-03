#include "../include/Film.h"
#include <iostream>
#include <iomanip>

int Film::nextId = 1;

Film::Film(const std::string& titlu, const std::string& gen,
           int durata, TipFilm tip, const std::string& descriere)
    : id(nextId++), titlu(titlu), gen(gen),
      durata(durata), tip(tip), descriere(descriere) {}

std::string Film::tipToString() const {
    return tip == TipFilm::_3D ? "3D" : "2D";
}

void Film::afiseaza() const {
    std::cout << std::left
              << std::setw(4)  << id
              << std::setw(28) << titlu
              << std::setw(8)  << tipToString()
              << std::setw(14) << gen
              << durata << " min\n";
}
