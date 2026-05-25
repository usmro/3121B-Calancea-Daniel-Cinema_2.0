#pragma once
#include "Rezervare.h"

class RezervareOnline : public Rezervare {
private:
    std::string emailClient;
    bool        confirmareTrimiasa;

public:
    RezervareOnline(Film* film, Sala* sala, int rand, int col,
                    const std::string& email);

    std::string getEmailClient()    const;
    bool        isConfirmareTrimisa() const;
    void        trimitConfirmare();

    void        afiseaza() const override;
    std::string getTip()   const override { return "online"; }
};
