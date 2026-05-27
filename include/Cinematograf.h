#pragma once
#include <memory>
#include <string>
#include <vector>
#include "ICinemaService.h"
#include "Film.h"
#include "Sala.h"
#include "Rezervare.h"
#include "RezervareOnline.h"

class Cinematograf : public ICinemaService {
private:
    std::string  nume;
    std::vector<Film>                      filme;
    std::vector<std::unique_ptr<Sala>>     sali;
    std::vector<std::unique_ptr<Rezervare>> rezervari;

    Film* findFilm(int filmId) const;
    Sala* findSala(int salaId) const;

public:
    explicit Cinematograf(const std::string& nume);
    ~Cinematograf() override = default;

    std::string getNume() const;

    void adaugaFilm(const Film& film)       override;
    void stergeFilm(int filmId)             override;
    void adaugaSala(Sala* sala)             override;
    void stergeSala(int salaId)             override;

    void afiseazaFilme()              const override;
    void afiseazaLocuri(int salaId)   const override;
    void afiseazaRezervari()          const override;

    void realizeazaRezervare(int filmId, int salaId,
                             int rand, int col)          override;
    void realizeazaRezervareOnline(int filmId, int salaId,
                                   int rand, int col,
                                   const std::string& email) override;
    void anuleazaRezervare(int rezervareId)              override;

    std::vector<Film*>      cautaFilme(const std::string& titlu) const override;
    std::vector<Film*>      filtreazaDupaTip(TipFilm tip)        const override;
    std::vector<Film*>      filtreazaDupaGen(const std::string& gen) const override;
    std::vector<Film*>      filtreazaDisponibile()               const override;

    std::vector<Film*>      getFilme()          const override;
    Sala*                   getSala(int salaId) const override;
    std::vector<Rezervare*> getRezervari()      const override;

    std::vector<Sala*>      getSali()           const;
};
