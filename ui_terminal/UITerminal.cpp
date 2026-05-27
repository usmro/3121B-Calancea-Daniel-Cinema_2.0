#include "../include/Exceptii.h"
#include "UITerminal.h"
#include <iostream>
#include <limits>

UITerminal::UITerminal(Cinematograf& cinema) : cinema(cinema) {}

void UITerminal::clearScreen() const {
    std::cout << "\033[2J\033[H";
}

void UITerminal::printBox(const std::string& title) const {
    std::string border(title.size() + 4, '=');
    std::cout << Color::CYAN << Color::BOLD
              << "  " << border << "\n"
              << "  | " << title << " |\n"
              << "  " << border << "\n"
              << Color::RESET;
}

void UITerminal::printSeparator() const {
    std::cout << Color::CYAN << "  " << std::string(50, '-') << Color::RESET << "\n";
}

void UITerminal::printSuccess(const std::string& msg) const {
    std::cout << Color::GREEN << "  [OK] " << msg << Color::RESET << "\n";
}

void UITerminal::printError(const std::string& msg) const {
    std::cout << Color::RED << "  [EROARE] " << msg << Color::RESET << "\n";
}

void UITerminal::printInfo(const std::string& msg) const {
    std::cout << Color::YELLOW << "  [INFO] " << msg << Color::RESET << "\n";
}

int UITerminal::readInt(const std::string& prompt, int min, int max) const {
    int val;
    while (true) {
        std::cout << "  " << prompt;
        if (std::cin >> val && val >= min && val <= max) {
            std::cin.ignore();
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printError("Valoare invalida. Introduceti un numar intre "
                   + std::to_string(min) + " si " + std::to_string(max));
    }
}

std::string UITerminal::readString(const std::string& prompt) const {
    std::cout << "  " << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

void UITerminal::afiseazaFilme() const {
    clearScreen();
    printBox("Lista Filme");
    cinema.afiseazaFilme();
    std::cout << "\n";
    readString("Apasa Enter pentru a continua...");
}

void UITerminal::afiseazaLocuri() const {
    clearScreen();
    printBox("Vizualizare Locuri Sala");
    auto sali = cinema.getSali();
    if (sali.empty()) { printInfo("Nu exista sali."); readString(""); return; }
    std::cout << "\n  Sali disponibile:\n";
    for (auto* s : sali)
        std::cout << "    [" << s->getId() << "] " << s->getNume() << "\n";
    int id = readInt("ID sala: ", 1, 9999);
    try {
        cinema.afiseazaLocuri(id);
    } catch (const CinemaException& e) {
        printError(e.what());
    }
    readString("\n  Apasa Enter pentru a continua...");
}

void UITerminal::cautaFilme() const {
    clearScreen();
    printBox("Cautare Filme");
    std::string q = readString("Titlu (partial): ");
    auto rezultat = cinema.cautaFilme(q);
    if (rezultat.empty()) {
        printInfo("Niciun film gasit.");
    } else {
        std::cout << "\n";
        for (auto* f : rezultat) f->afiseaza();
    }
    readString("\n  Apasa Enter pentru a continua...");
}

void UITerminal::afiseazaRezervari() const {
    clearScreen();
    printBox("Lista Rezervari");
    cinema.afiseazaRezervari();
    std::cout << "\n";
    readString("Apasa Enter pentru a continua...");
}
