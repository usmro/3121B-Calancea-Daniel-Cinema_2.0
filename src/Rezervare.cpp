#include "../include/Rezervare.h"
#include "../include/Film.h"
#include "../include/Sala.h"
#include <iostream>
#include <iomanip>
#include <ctime>

int Rezervare::nextId = 1;

Rezervare::Rezervare(Film* film, Sala* sala, int rand, int col)
    : id(nextId++), film(film), sala(sala),
      rand(rand), col(col),
      tipLoc(sala->getTipLoc(rand, col)),
      dataRezervare(std::time(nullptr)),
      pretFinal(0.0)
{
    pretFinal = calculeazaPret();
}

int         Rezervare::getId()            const { return id; }
Film*       Rezervare::getFilm()          const { return film; }
Sala*       Rezervare::getSala()          const { return sala; }
int         Rezervare::getRand()          const { return rand; }
int         Rezervare::getCol()           const { return col; }
TipLoc      Rezervare::getTipLoc()        const { return tipLoc; }
double      Rezervare::getPretFinal()     const { return pretFinal; }
std::time_t Rezervare::getDataRezervare() const { return dataRezervare; }

bool Rezervare::esteWeekend(std::time_t t) {
    std::tm* tm = std::localtime(&t);
    // 0 = duminica, 6 = sambata
    return (tm->tm_wday == 0 || tm->tm_wday == 6);
}

double Rezervare::calculeazaPret() const {
    // Pret de baza
    double pret = (film->getTip() == TipFilm::_3D) ? 45.0 : 30.0;

    // Modificator tip loc
    switch (tipLoc) {
        case TipLoc::VIP:     pret *= 1.50; break;
        case TipLoc::STUDENT: pret *= 0.80; break;
        default: break;
    }

    // Modificator weekend
    if (esteWeekend(dataRezervare))
        pret *= 1.15;

    return pret;
}

void Rezervare::afiseaza() const {
    char buf[20];
    std::tm* tm = std::localtime(&dataRezervare);
    std::strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", tm);

    std::string tipLocStr;
    switch (tipLoc) {
        case TipLoc::VIP:     tipLocStr = "VIP";     break;
        case TipLoc::STUDENT: tipLocStr = "Student"; break;
        default:              tipLocStr = "Standard"; break;
    }

    std::cout << std::left
              << std::setw(5)  << id
              << std::setw(22) << film->getTitlu()
              << std::setw(8)  << sala->getNume()
              << "R" << std::setw(3) << rand + 1
              << "C" << std::setw(3) << col + 1
              << std::setw(10) << tipLocStr
              << std::setw(8)  << std::fixed << std::setprecision(0) << pretFinal << " lei"
              << "  [" << getTip() << "]"
              << "  " << buf << "\n";
}
