#include "UITerminal.h"
#include <limits>

UITerminal::UITerminal(Cinematograf& cinema) : cinema(cinema) {}

void UITerminal::clearScreen() const {
    std::cout << "\033[2J\033[H";
}

void UITerminal::printBox(const std::string& titlu, int latime) const {
    std::string linie(latime, '=');
    std::cout << Color::BLUE << Color::BOLD
              << "+" << linie << "+\n"
              << "|  " << Color::YELLOW << titlu
              << Color::BLUE << std::string(latime - 2 - titlu.size(), ' ') << "|\n"
              << "+" << linie << "+"
              << Color::RESET << "\n";
}

void UITerminal::printSeparator(int latime) const {
    std::cout << Color::BLUE
              << "+" << std::string(latime, '-') << "+"
              << Color::RESET << "\n";
}

void UITerminal::printSuccess(const std::string& mesaj) const {
    std::cout << Color::GREEN << Color::BOLD
              << "  [OK] " << Color::RESET
              << Color::GREEN << mesaj
              << Color::RESET << "\n";
}

void UITerminal::printError(const std::string& mesaj) const {
    std::cout << Color::RED << Color::BOLD
              << "  [ERR] " << Color::RESET
              << Color::RED << mesaj
              << Color::RESET << "\n";
}

void UITerminal::printInfo(const std::string& mesaj) const {
    std::cout << Color::CYAN
              << "  [*] " << mesaj
              << Color::RESET << "\n";
}

int UITerminal::readInt(const std::string& prompt) const {
    int val;
    while (true) {
        std::cout << Color::CYAN << "  " << prompt << Color::RESET << " > ";
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printError("Introduceti un numar valid.");
    }
}

std::string UITerminal::readString(const std::string& prompt) const {
    std::string val;
    std::cout << Color::CYAN << "  " << prompt << Color::RESET << " > ";
    std::getline(std::cin, val);
    return val;
}
