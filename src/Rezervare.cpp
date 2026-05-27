#include "../include/Rezervare.h"
#include <iostream>
#include <iomanip>
#include <ctime>

int Rezervare::nextId = 1;

bool Rezervare::esteWeekend(std::time_t t) {
    std::tm* tm = std::localtime(&t);
    return tm->tm_wday == 0 || tm->tm_wday == 6;
}

Rezervare::Rezervare(Film* film, Sala* sala, int rand, int col)
    : id(nextId++), film(film), sala(sala), rand(rand), col(col),
      tipLoc(sala->getTipLoc(rand, col)),
      dataRezervare(std::time(nullptr)),
      pretFinal(calculeazaPret()) {}

double Rezervare::calculeazaPret() const {
    double pret = (film->getTip() == TipFilm::_3D) ? 45.0 : 30.0;
    if      (tipLoc == TipLoc::VIP)     pret *= 1.5;
    else if (tipLoc == TipLoc::STUDENT) pret *= 0.8;
    if (esteWeekend(dataRezervare)) pret *= 1.15;
    return pret;
}

void Rezervare::afiseaza() const {
    std::tm* tm = std::localtime(&dataRezervare);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%d.%m.%Y", tm);

    std::string tipLocStr =
        tipLoc == TipLoc::VIP     ? "VIP" :
        tipLoc == TipLoc::STUDENT ? "Student" : "Standard";

    std::cout << std::left
              << std::setw(5)  << id
              << std::setw(22) << film->getTitlu()
              << std::setw(8)  << sala->getNume()
              << "R" << std::setw(2) << (rand+1)
              << "C" << std::setw(3) << (col+1)
              << std::setw(10) << tipLocStr
              << std::setw(8)  << pretFinal
              << "  " << getTip()
              << "  " << buf << "\n";
}
