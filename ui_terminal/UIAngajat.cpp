#include "../include/Exceptii.h"
#include "UIAngajat.h"
#include <iostream>

UIAngajat::UIAngajat(Cinematograf& cinema) : UITerminal(cinema) {}

void UIAngajat::afiseazaMeniu() const {
    clearScreen();
    printBox("ANGAJAT — Cinema Central");
    std::cout << "\n"
              << "  [1] Afiseaza filme\n"
              << "  [2] Afiseaza locuri sala\n"
              << "  [3] Cauta filme\n"
              << "  [4] Afiseaza rezervari\n"
              << "  [5] Realizeaza rezervare\n"
              << "  [6] Anuleaza rezervare\n"
              << "  [0] Iesire\n";
    printSeparator();
}

void UIAngajat::realizeazaRezervare() {
    clearScreen();
    printBox("Realizeaza Rezervare");
    cinema.afiseazaFilme();
    int filmId = readInt("\nID film: ", 1, 9999);
    auto sali = cinema.getSali();
    if (sali.empty()) { printError("Nu exista sali!"); readString(""); return; }
    std::cout << "\n  Sali disponibile:\n";
    for (auto* s : sali)
        std::cout << "    [" << s->getId() << "] " << s->getNume()
                  << " (" << s->getNumarLocuriLibere() << " locuri libere)\n";
    int salaId = readInt("ID sala: ", 1, 9999);
    try {
        cinema.afiseazaLocuri(salaId);
        int rand = readInt("Rand: ", 1, 99) - 1;
        int col  = readInt("Coloana: ", 1, 99) - 1;
        cinema.realizeazaRezervare(filmId, salaId, rand, col);
        printSuccess("Rezervare realizata cu succes!");
    } catch (const CinemaException& e) {
        printError(e.what());
    }
    readString("Apasa Enter...");
}

void UIAngajat::anuleazaRezervare() {
    clearScreen();
    printBox("Anuleaza Rezervare");
    cinema.afiseazaRezervari();
    int id = readInt("\nID rezervare de anulat: ", 1, 9999);
    try {
        cinema.anuleazaRezervare(id);
        printSuccess("Rezervarea a fost anulata!");
    } catch (const CinemaException& e) {
        printError(e.what());
    }
    readString("Apasa Enter...");
}

void UIAngajat::run() {
    while (true) {
        afiseazaMeniu();
        int opt = readInt("> ", 0, 6);
        switch (opt) {
            case 1: afiseazaFilme();       break;
            case 2: afiseazaLocuri();      break;
            case 3: cautaFilme();          break;
            case 4: afiseazaRezervari();   break;
            case 5: realizeazaRezervare(); break;
            case 6: anuleazaRezervare();   break;
            case 0: return;
        }
    }
}
