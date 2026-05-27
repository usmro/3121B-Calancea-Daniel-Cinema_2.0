#include "../include/Exceptii.h"
#include "UIAdmin.h"
#include <iostream>

UIAdmin::UIAdmin(Cinematograf& cinema) : UITerminal(cinema) {}

void UIAdmin::afiseazaMeniu() const {
    clearScreen();
    printBox("ADMINISTRATOR — Cinema Central");
    std::cout << "\n"
              << "  " << Color::YELLOW << "── Vizualizare ──" << Color::RESET << "\n"
              << "  [1] Afiseaza filme\n"
              << "  [2] Afiseaza locuri sala\n"
              << "  [3] Cauta filme\n"
              << "  [4] Afiseaza rezervari\n"
              << "\n"
              << "  " << Color::YELLOW << "── Gestiune ──" << Color::RESET << "\n"
              << "  [5] Adauga film\n"
              << "  [6] Sterge film\n"
              << "  [7] Adauga sala\n"
              << "  [8] Sterge sala\n"
              << "\n"
              << "  [0] Iesire\n";
    printSeparator();
}

void UIAdmin::adaugaFilm() {
    clearScreen();
    printBox("Adauga Film");
    std::string titlu  = readString("Titlu: ");
    std::string gen    = readString("Gen: ");
    int durata         = readInt("Durata (min): ", 1, 500);
    int tip            = readInt("Tip [1=2D, 2=3D]: ", 1, 2);
    std::string descr  = readString("Descriere: ");
    TipFilm tipFilm    = (tip == 2) ? TipFilm::_3D : TipFilm::_2D;
    try {
        cinema.adaugaFilm(Film(titlu, gen, durata, tipFilm, descr));
        printSuccess("Filmul a fost adaugat!");
    } catch (const CinemaException& e) {
        printError(e.what());
    }
    readString("Apasa Enter...");
}

void UIAdmin::stergeFilm() {
    clearScreen();
    printBox("Sterge Film");
    cinema.afiseazaFilme();
    int id = readInt("\nID film de sters: ", 1, 9999);
    try {
        cinema.stergeFilm(id);
        printSuccess("Filmul a fost sters!");
    } catch (const CinemaException& e) {
        printError(e.what());
    }
    readString("Apasa Enter...");
}

void UIAdmin::adaugaSala() {
    clearScreen();
    printBox("Adauga Sala");
    std::string nume = readString("Nume sala: ");
    int randuri      = readInt("Numar randuri: ", 1, 20);
    int coloane      = readInt("Numar coloane: ", 1, 20);
    try {
        cinema.adaugaSala(new Sala(nume, randuri, coloane));
        printSuccess("Sala a fost adaugata!");
    } catch (const CinemaException& e) {
        printError(e.what());
    }
    readString("Apasa Enter...");
}

void UIAdmin::stergeSala() {
    clearScreen();
    printBox("Sterge Sala");
    auto sali = cinema.getSali();
    for (auto* s : sali)
        std::cout << "  [" << s->getId() << "] " << s->getNume() << "\n";
    int id = readInt("\nID sala de sters: ", 1, 9999);
    try {
        cinema.stergeSala(id);
        printSuccess("Sala a fost stearsa!");
    } catch (const CinemaException& e) {
        printError(e.what());
    }
    readString("Apasa Enter...");
}

void UIAdmin::run() {
    while (true) {
        afiseazaMeniu();
        int opt = readInt("> ", 0, 8);
        switch (opt) {
            case 1: afiseazaFilme();    break;
            case 2: afiseazaLocuri();   break;
            case 3: cautaFilme();       break;
            case 4: afiseazaRezervari();break;
            case 5: adaugaFilm();       break;
            case 6: stergeFilm();       break;
            case 7: adaugaSala();       break;
            case 8: stergeSala();       break;
            case 0: return;
        }
    }
}
