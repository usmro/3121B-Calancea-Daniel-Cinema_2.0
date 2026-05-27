#pragma once
#include "Rezervare.h"
#include <string>

class RezervareOnline : public Rezervare {
private:
    std::string emailClient;
    bool        confirmareTrimiasa;

public:
    RezervareOnline(Film* film, Sala* sala, int rand, int col,
                    const std::string& email);

    std::string getEmail()             const { return emailClient; }
    bool        isConfirmareTrimisa()  const { return confirmareTrimiasa; }
    void        trimitConfirmare();

    void        afiseaza() const override;
    std::string getTip()   const override { return "online"; }
};
