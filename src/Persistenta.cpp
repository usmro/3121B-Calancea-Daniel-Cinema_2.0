#include "../include/Persistenta.h"
#include "../include/RezervareOnline.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Format CSV: filmId;salaId;rand;col;isOnline;email
// Exemplu fizica:  1;1;0;3;0;
// Exemplu online:  2;1;1;2;1;client@gmail.com

void Persistenta::salveaza(const std::string& fisier, const Cinematograf& cinema) {
    std::ofstream out(fisier);
    if (!out) {
        std::cerr << "[PERSISTENTA] Nu pot salva in: " << fisier << "\n";
        return;
    }
    for (Rezervare* r : cinema.getRezervari()) {
        RezervareOnline* ro = dynamic_cast<RezervareOnline*>(r);
        out << r->getFilm()->getId() << ";"
            << r->getSala()->getId() << ";"
            << r->getRand()          << ";"
            << r->getCol()           << ";"
            << (ro ? 1 : 0)          << ";"
            << (ro ? ro->getEmail() : "") << "\n";
    }
    std::cout << "[PERSISTENTA] " << cinema.getRezervari().size()
              << " rezervari salvate in " << fisier << "\n";
}

void Persistenta::incarca(const std::string& fisier, Cinematograf& cinema) {
    std::ifstream in(fisier);
    if (!in) return; // prima rulare, fisierul nu exista inca

    int loaded = 0;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;

        try {
            int filmId, salaId, rand, col, isOnline;
            std::string email;

            std::getline(ss, token, ';'); filmId   = std::stoi(token);
            std::getline(ss, token, ';'); salaId   = std::stoi(token);
            std::getline(ss, token, ';'); rand     = std::stoi(token);
            std::getline(ss, token, ';'); col      = std::stoi(token);
            std::getline(ss, token, ';'); isOnline = std::stoi(token);
            if (isOnline) std::getline(ss, email);

            // Suprima output-ul (trimitConfirmare) in timpul incarcarii
            std::cout.setstate(std::ios::failbit);
            if (isOnline && !email.empty())
                cinema.realizeazaRezervareOnline(filmId, salaId, rand, col, email);
            else
                cinema.realizeazaRezervare(filmId, salaId, rand, col);
            std::cout.clear();

            ++loaded;
        } catch (...) {
            std::cout.clear(); // restaureaza cout in caz de eroare
        }
    }

    if (loaded > 0)
        std::cout << "[PERSISTENTA] " << loaded
                  << " rezervari incarcate din " << fisier << "\n";
}
