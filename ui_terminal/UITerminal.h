#pragma once
#include <string>
#include <iostream>
#include "../include/Cinematograf.h"

// Coduri ANSI pentru culori si formatare
namespace Color {
    inline const std::string RESET    = "\033[0m";
    inline const std::string BOLD     = "\033[1m";
    inline const std::string RED      = "\033[31m";
    inline const std::string GREEN    = "\033[32m";
    inline const std::string YELLOW   = "\033[33m";
    inline const std::string BLUE     = "\033[34m";
    inline const std::string MAGENTA  = "\033[35m";
    inline const std::string CYAN     = "\033[36m";
    inline const std::string WHITE    = "\033[37m";
}

class UITerminal {
protected:
    Cinematograf& cinema;

    void clearScreen()                                  const;
    void printBox(const std::string& titlu,
                  int latime = 50)                      const;
    void printSeparator(int latime = 50)                const;
    void printSuccess(const std::string& mesaj)         const;
    void printError(const std::string& mesaj)           const;
    void printInfo(const std::string& mesaj)            const;

    int         readInt(const std::string& prompt)      const;
    std::string readString(const std::string& prompt)   const;

public:
    explicit UITerminal(Cinematograf& cinema);
    virtual ~UITerminal() = default;
    virtual void run() = 0;
};
