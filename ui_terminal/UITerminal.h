#pragma once
#include <string>
#include "../include/Cinematograf.h"

namespace Color {
    inline const std::string RESET   = "\033[0m";
    inline const std::string BOLD    = "\033[1m";
    inline const std::string RED     = "\033[31m";
    inline const std::string GREEN   = "\033[32m";
    inline const std::string YELLOW  = "\033[33m";
    inline const std::string CYAN    = "\033[36m";
}

class UITerminal {
protected:
    Cinematograf& cinema;
    explicit UITerminal(Cinematograf& cinema);

    void clearScreen()                         const;
    void printBox(const std::string& title)    const;
    void printSeparator()                      const;
    void printSuccess(const std::string& msg)  const;
    void printError(const std::string& msg)    const;
    void printInfo(const std::string& msg)     const;
    int  readInt(const std::string& prompt,
                 int min, int max)             const;
    std::string readString(const std::string& prompt) const;

    void afiseazaFilme()    const;
    void afiseazaLocuri()   const;
    void cautaFilme()       const;
    void afiseazaRezervari()const;
};
