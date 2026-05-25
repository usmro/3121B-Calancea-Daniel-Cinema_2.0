#pragma once
#include <string>
#include <vector>
#include "Tipuri.h"

class Film;
class Sala;
class Rezervare;

class ICinemaService {
public:
    virtual ~ICinemaService() = default;

    // --- Gestiune filme (doar admin) ---
    virtual void adaugaFilm(const Film& film) = 0;
    virtual void stergeFilm(int filmId) = 0;

    // --- Gestiune sali (doar admin) ---
    virtual void adaugaSala(Sala* sala) = 0;
    virtual void stergeSala(int salaId) = 0;

    // --- Afisare (admin + angajat) ---
    virtual void afiseazaFilme() const = 0;
    virtual void afiseazaLocuri(int salaId) const = 0;
    virtual void afiseazaRezervari() const = 0;

    // --- Rezervari ---
    virtual void realizeazaRezervare(int filmId, int salaId,
                                     int rand, int col) = 0;
    virtual void realizeazaRezervareOnline(int filmId, int salaId,
                                           int rand, int col,
                                           const std::string& email) = 0;
    virtual void anuleazaRezervare(int rezervareId) = 0;

    // --- Cautare si filtrare ---
    virtual std::vector<Film*> cautaFilme(const std::string& titlu) const = 0;
    virtual std::vector<Film*> filtreazaDupaTip(TipFilm tip) const = 0;
    virtual std::vector<Film*> filtreazaDupaGen(const std::string& gen) const = 0;
    virtual std::vector<Film*> filtreazaDisponibile() const = 0;

    // --- Getteri pentru UI ---
    virtual std::vector<Film*>     getFilme()     const = 0;
    virtual Sala*                  getSala(int salaId) const = 0;
    virtual std::vector<Rezervare*> getRezervari() const = 0;
};
