#include "../include/Sala.h"
#include "../include/Exceptii.h"
#include <iostream>

int Sala::nextId = 1;

Sala::Sala(const std::string& nume, int nrRanduri, int nrColoane)
    : id(nextId++), nume(nume),
      nrRanduri(nrRanduri), nrColoane(nrColoane),
      ocupat(nrRanduri, std::vector<bool>(nrColoane, false)),
      tipuriLocuri(nrRanduri, std::vector<TipLoc>(nrColoane, TipLoc::STANDARD))
{}

int         Sala::getId()        const { return id; }
std::string Sala::getNume()      const { return nume; }
int         Sala::getNrRanduri() const { return nrRanduri; }
int         Sala::getNrColoane() const { return nrColoane; }

void Sala::valideazaIndex(int rand, int col) const {
    if (rand < 0 || rand >= nrRanduri || col < 0 || col >= nrColoane)
        throw IndexInvalidException(rand, col);
}

bool Sala::esteLiber(int rand, int col) const {
    valideazaIndex(rand, col);
    return !ocupat[rand][col];
}

TipLoc Sala::getTipLoc(int rand, int col) const {
    valideazaIndex(rand, col);
    return tipuriLocuri[rand][col];
}

void Sala::setTipLoc(int rand, int col, TipLoc tip) {
    valideazaIndex(rand, col);
    tipuriLocuri[rand][col] = tip;
}

void Sala::ocupa(int rand, int col) {
    valideazaIndex(rand, col);
    if (ocupat[rand][col])
        throw LocOcupatException(rand, col);
    ocupat[rand][col] = true;
}

void Sala::elibereaza(int rand, int col) {
    valideazaIndex(rand, col);
    ocupat[rand][col] = false;
}

bool Sala::areLocuriLibere() const {
    for (int r = 0; r < nrRanduri; ++r)
        for (int c = 0; c < nrColoane; ++c)
            if (!ocupat[r][c]) return true;
    return false;
}

int Sala::getNumarLocuriLibere() const {
    int count = 0;
    for (int r = 0; r < nrRanduri; ++r)
        for (int c = 0; c < nrColoane; ++c)
            if (!ocupat[r][c]) ++count;
    return count;
}

void Sala::afiseazaLocuri() const {
    // Antet cu numere coloane
    std::cout << "     ";
    for (int c = 0; c < nrColoane; ++c)
        std::cout << " " << c + 1 << " ";
    std::cout << "\n";

    for (int r = 0; r < nrRanduri; ++r) {
        std::cout << "  " << r + 1 << "  ";
        for (int c = 0; c < nrColoane; ++c) {
            if (ocupat[r][c]) {
                // Rosu pentru ocupat
                std::cout << "\033[31m[X]\033[0m";
            } else {
                // Verde pentru liber; V = VIP, S = Student
                std::string simbol = " ";
                if (tipuriLocuri[r][c] == TipLoc::VIP)     simbol = "V";
                else if (tipuriLocuri[r][c] == TipLoc::STUDENT) simbol = "S";
                std::cout << "\033[32m[" << simbol << "]\033[0m";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n  \033[32m[ ] Liber\033[0m  "
              << "\033[31m[X] Ocupat\033[0m  "
              << "\033[32m[V] VIP\033[0m  "
              << "\033[32m[S] Student\033[0m\n";
}
