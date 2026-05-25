#include "UIAngajat.h"
#include "../include/Exceptii.h"
#include <iostream>

UIAngajat::UIAngajat(Cinematograf& cinema) : UITerminal(cinema) {}

void UIAngajat::run() {
    meniuPrincipal();
}

void UIAngajat::meniuPrincipal() {
    while (true) {
        clearScreen();
        printBox("CINEMA SISTEM  —  Angajat", 50);
        std::cout << "\n"
                  << Color::GREEN << "  [1]" << Color::RESET << " Afiseaza filme\n"
                  << Color::GREEN << "  [2]" << Color::RESET << " Afiseaza locuri sala\n"
                  << Color::GREEN << "  [3]" << Color::RESET << " Afiseaza rezervari\n"
                  << Color::GREEN << "  [4]" << Color::RESET << " Cauta filme\n"
                  << Color::CYAN  << "  [5]" << Color::RESET << " Realizeaza rezervare\n"
                  << Color::RED   << "  [0]" << Color::RESET << " Iesire\n\n";

        int opt = readInt("Optiune");
        switch (opt) {
            case 1: afiseazaFilme();       break;
            case 2: afiseazaLocuri();      break;
            case 3: afiseazaRezervari();   break;
            case 4: cautaFilme();          break;
            case 5: realizeazaRezervare(); break;
            case 0: return;
            default: printError("Optiune invalida."); break;
        }
        std::cout << "\n"; readString("Apasati ENTER pentru a continua...");
    }
}

void UIAngajat::afiseazaFilme() {
    clearScreen();
    printBox("FILME DISPONIBILE", 50);
    std::cout << "\n";
    cinema.afiseazaFilme();
}

void UIAngajat::afiseazaLocuri() {
    clearScreen();
    printBox("AFISARE LOCURI SALA", 50);
    int salaId = readInt("ID sala");
    try {
        std::cout << "\n";
        cinema.afiseazaLocuri(salaId);
    } catch (const CinemaException& e) {
        printError(e.what());
    }
}

void UIAngajat::afiseazaRezervari() {
    clearScreen();
    printBox("REZERVARI EXISTENTE", 50);
    std::cout << "\n";
    cinema.afiseazaRezervari();
}

void UIAngajat::cautaFilme() {
    clearScreen();
    printBox("CAUTA FILME", 50);
    std::cout << "\n";
    std::cout << Color::CYAN
              << "  [1] Dupa titlu\n"
              << "  [2] Dupa tip (2D/3D)\n"
              << "  [3] Dupa gen\n"
              << "  [4] Doar disponibile\n"
              << Color::RESET;
    int opt = readInt("Optiune");

    std::vector<Film*> rezultat;
    if (opt == 1) {
        std::string titlu = readString("Titlu (partial)");
        rezultat = cinema.cautaFilme(titlu);
    } else if (opt == 2) {
        int t = readInt("Tip (1=2D, 2=3D)");
        rezultat = cinema.filtreazaDupaTip(t == 2 ? TipFilm::_3D : TipFilm::_2D);
    } else if (opt == 3) {
        std::string gen = readString("Gen");
        rezultat = cinema.filtreazaDupaGen(gen);
    } else if (opt == 4) {
        rezultat = cinema.filtreazaDisponibile();
    }

    std::cout << "\n";
    if (rezultat.empty())
        printInfo("Niciun film gasit.");
    else {
        printInfo(std::to_string(rezultat.size()) + " film(e) gasite:");
        std::cout << "\n";
        for (auto* f : rezultat) f->afiseaza();
    }
}

void UIAngajat::realizeazaRezervare() {
    clearScreen();
    printBox("REZERVARE NOUA", 50);
    cinema.afiseazaFilme();
    int filmId = readInt("ID film");
    int salaId = readInt("ID sala");

    try {
        cinema.afiseazaLocuri(salaId);
    } catch (const CinemaException& e) {
        printError(e.what());
        return;
    }

    int rand = readInt("Rand (incepand cu 1)") - 1;
    int col  = readInt("Coloana (incepand cu 1)") - 1;

    try {
        cinema.realizeazaRezervare(filmId, salaId, rand, col);
        printSuccess("Rezervare realizata cu succes!");
    } catch (const CinemaException& e) {
        printError(e.what());
    }
}
