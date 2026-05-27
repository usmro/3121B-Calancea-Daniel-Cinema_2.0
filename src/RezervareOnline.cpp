#include "../include/RezervareOnline.h"
#include <iostream>

RezervareOnline::RezervareOnline(Film* film, Sala* sala, int rand, int col,
                                 const std::string& email)
    : Rezervare(film, sala, rand, col),
      emailClient(email), confirmareTrimiasa(false) {}

void RezervareOnline::trimitConfirmare() {
    std::cout << "\033[32m[EMAIL]\033[0m Confirmare trimisa la: "
              << emailClient << "\n";
    confirmareTrimiasa = true;
}

void RezervareOnline::afiseaza() const {
    Rezervare::afiseaza();
    std::cout << "         Email: " << emailClient
              << (confirmareTrimiasa ? "  \033[32m[confirmat]\033[0m"
                                    : "  \033[33m[neprelucrat]\033[0m")
              << "\n";
}
