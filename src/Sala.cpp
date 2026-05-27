#include "../include/Sala.h"
#include "../include/Exceptii.h"
#include <iostream>

int Sala::nextId = 1;

Sala::Sala(const std::string& nume, int nrRanduri, int nrColoane)
    : id(nextId++), nume(nume), nrRanduri(nrRanduri), nrColoane(nrColoane),
      ocupat(nrRanduri, std::vector<bool>(nrColoane, false)),
      tipuriLocuri(nrRanduri, std::vector<TipLoc>(nrColoane, TipLoc::STANDARD)) {}

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

void Sala::setTipLoc(int rand, int col, TipLoc t) {
    valideazaIndex(rand, col);
    tipuriLocuri[rand][col] = t;
}

void Sala::ocupa(int rand, int col) {
    valideazaIndex(rand, col);
    if (ocupat[rand][col]) throw LocOcupatException(rand, col);
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
    int cnt = 0;
    for (int r = 0; r < nrRanduri; ++r)
        for (int c = 0; c < nrColoane; ++c)
            if (!ocupat[r][c]) ++cnt;
    return cnt;
}

void Sala::afiseazaLocuri() const {
    std::cout << "     ";
    for (int c = 0; c < nrColoane; ++c)
        std::cout << " " << (c + 1) << " ";
    std::cout << "\n";

    for (int r = 0; r < nrRanduri; ++r) {
        std::cout << "  " << (r + 1) << "  ";
        for (int c = 0; c < nrColoane; ++c) {
            if (ocupat[r][c]) {
                std::cout << "\033[31m[X]\033[0m";
            } else {
                TipLoc t = tipuriLocuri[r][c];
                if      (t == TipLoc::VIP)     std::cout << "\033[33m[V]\033[0m";
                else if (t == TipLoc::STUDENT)  std::cout << "\033[36m[S]\033[0m";
                else                            std::cout << "\033[32m[ ]\033[0m";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n  \033[32m[ ]\033[0m Liber  "
              << "\033[33m[V]\033[0m VIP  "
              << "\033[36m[S]\033[0m Student  "
              << "\033[31m[X]\033[0m Ocupat\n";
}
