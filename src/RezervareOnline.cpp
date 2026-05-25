#include "../include/RezervareOnline.h"
#include "../include/Film.h"
#include "../include/Sala.h"
#include <iostream>

RezervareOnline::RezervareOnline(Film* film, Sala* sala,
                                 int rand, int col,
                                 const std::string& email)
    : Rezervare(film, sala, rand, col),
      emailClient(email),
      confirmareTrimiasa(false)
{}

std::string RezervareOnline::getEmailClient()      const { return emailClient; }
bool        RezervareOnline::isConfirmareTrimisa() const { return confirmareTrimiasa; }

void RezervareOnline::trimitConfirmare() {
    // In productie: integrat cu un serviciu de email (SMTP / API)
    // Deocamdata: simuleaza trimiterea
    std::cout << "\033[32m[EMAIL]\033[0m Confirmare trimisa la: "
              << emailClient << "\n";
    confirmareTrimiasa = true;
}

void RezervareOnline::afiseaza() const {
    Rezervare::afiseaza();
    std::cout << "         Email: " << emailClient;
    if (confirmareTrimiasa)
        std::cout << "  \033[32m[confirmat]\033[0m";
    else
        std::cout << "  \033[33m[neconfirmat]\033[0m";
    std::cout << "\n";
}
