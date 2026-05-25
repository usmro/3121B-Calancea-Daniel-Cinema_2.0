#include "UIAdmin.h"
#include "../include/Exceptii.h"
#include <iostream>

UIAdmin::UIAdmin(Cinematograf& cinema) : UITerminal(cinema) {}

void UIAdmin::run() {
    meniuPrincipal();
}

void UIAdmin::meniuPrincipal() {
    while (true) {
        clearScreen();
        printBox("CINEMA SISTEM  —  Administrator", 50);
        std::cout << "\n"
                  << Color::GREEN  << "  [1]" << Color::RESET << " Afiseaza filme\n"
                  << Color::GREEN  << "  [2]" << Color::RESET << " Afiseaza locuri sala\n"
                  << Color::GREEN  << "  [3]" << Color::RESET << " Afiseaza rezervari\n"
                  << Color::GREEN  << "  [4]" << Color::RESET << " Cauta filme\n"
                  << Color::YELLOW << "  [5]" << Color::RESET << " Adauga film\n"
                  << Color::YELLOW << "  [6]" << Color::RESET << " Sterge film\n"
                  << Color::YELLOW << "  [7]" << Color::RESET << " Adauga sala\n"
                  << Color::YELLOW << "  [8]" << Color::RESET << " Sterge sala\n"
                  << Color::RED    << "  [0]" << Color::RESET << " Iesire\n\n";

        int opt = readInt("Optiune");
        switch (opt) {
            case 1: afiseazaFilme();    break;
            case 2: afiseazaLocuri();   break;
            case 3: afiseazaRezervari(); break;
            case 4: cautaFilme();       break;
            case 5: adaugaFilm();       break;
            case 6: stergeFilm();       break;
            case 7: adaugaSala();       break;
            case 8: stergeSala();       break;
            case 0: return;
            default: printError("Optiune invalida."); break;
        }
        std::cout << "\n"; readString("Apasati ENTER pentru a continua...");
    }
}

// ── Vizualizare ──────────────────────────────────────────────────────────────
void UIAdmin::afiseazaFilme() {
    clearScreen();
    printBox("FILME DISPONIBILE", 50);
    std::cout << "\n";
    cinema.afiseazaFilme();
}

void UIAdmin::afiseazaLocuri() {
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

void UIAdmin::afiseazaRezervari() {
    clearScreen();
    printBox("REZERVARI EXISTENTE", 50);
    std::cout << "\n";
    cinema.afiseazaRezervari();
}

void UIAdmin::cautaFilme() {
    clearScreen();
    printBox("CAUTA FILME", 50);
    std::cout << "\n";
    std::cout << Color::CYAN << "  [1] Dupa titlu\n"
              << "  [2] Dupa tip (2D/3D)\n"
              << "  [3] Dupa gen\n"
              << "  [4] Doar disponibile (locuri libere)\n" << Color::RESET;
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
    if (rezultat.empty()) {
        printInfo("Niciun film gasit.");
    } else {
        printInfo(std::to_string(rezultat.size()) + " film(e) gasite:");
        std::cout << "\n";
        for (auto* f : rezultat) f->afiseaza();
    }
}

// ── Gestiune (doar admin) ─────────────────────────────────────────────────────
void UIAdmin::adaugaFilm() {
    clearScreen();
    printBox("ADAUGA FILM", 50);
    std::string titlu  = readString("Titlu");
    std::string gen    = readString("Gen");
    int         durata = readInt("Durata (minute)");
    int         tip    = readInt("Tip (1=2D, 2=3D)");
    std::string desc   = readString("Descriere (optional)");

    Film f(titlu, gen, durata,
           tip == 2 ? TipFilm::_3D : TipFilm::_2D, desc);
    cinema.adaugaFilm(f);
    printSuccess("Film adaugat cu succes!");
}

void UIAdmin::stergeFilm() {
    clearScreen();
    printBox("STERGE FILM", 50);
    cinema.afiseazaFilme();
    int id = readInt("ID film de sters");
    try {
        cinema.stergeFilm(id);
        printSuccess("Film sters.");
    } catch (const CinemaException& e) {
        printError(e.what());
    }
}

void UIAdmin::adaugaSala() {
    clearScreen();
    printBox("ADAUGA SALA", 50);
    std::string nume   = readString("Numele salii");
    int         randuri = readInt("Numar randuri");
    int         coloane = readInt("Numar coloane");
    cinema.adaugaSala(new Sala(nume, randuri, coloane));
    printSuccess("Sala adaugata cu succes!");
}

void UIAdmin::stergeSala() {
    clearScreen();
    printBox("STERGE SALA", 50);
    int id = readInt("ID sala de sters");
    try {
        cinema.stergeSala(id);
        printSuccess("Sala stearsa.");
    } catch (const CinemaException& e) {
        printError(e.what());
    }
}
