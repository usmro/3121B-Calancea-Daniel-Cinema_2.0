#include "../include/Cinematograf.h"
#include "../include/Exceptii.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

Cinematograf::Cinematograf(const std::string& nume) : nume(nume) {}
std::string Cinematograf::getNume() const { return nume; }

Film* Cinematograf::findFilm(int filmId) const {
    for (auto& f : filme)
        if (f.getId() == filmId) return const_cast<Film*>(&f);
    throw FilmNotFoundException(filmId);
}

Sala* Cinematograf::findSala(int salaId) const {
    for (auto& s : sali)
        if (s->getId() == salaId) return s.get();
    throw SalaNotFoundException(salaId);
}

void Cinematograf::adaugaFilm(const Film& film) { filme.push_back(film); }

void Cinematograf::stergeFilm(int filmId) {
    auto it = std::find_if(filme.begin(), filme.end(),
                           [filmId](const Film& f){ return f.getId() == filmId; });
    if (it == filme.end()) throw FilmNotFoundException(filmId);
    filme.erase(it);
}

void Cinematograf::adaugaSala(Sala* sala) {
    sali.push_back(std::unique_ptr<Sala>(sala));
}

void Cinematograf::stergeSala(int salaId) {
    auto it = std::find_if(sali.begin(), sali.end(),
                           [salaId](const auto& s){ return s->getId() == salaId; });
    if (it == sali.end()) throw SalaNotFoundException(salaId);
    sali.erase(it);
}

void Cinematograf::afiseazaFilme() const {
    if (filme.empty()) { std::cout << "  Nu exista filme in program.\n"; return; }
    std::cout << "\033[1m" << std::left
              << std::setw(4)  << "ID"
              << std::setw(28) << "Titlu"
              << std::setw(8)  << "Tip"
              << std::setw(14) << "Gen"
              << "Durata\033[0m\n"
              << std::string(60, '-') << "\n";
    for (const auto& f : filme) f.afiseaza();
}

void Cinematograf::afiseazaLocuri(int salaId) const {
    Sala* sala = findSala(salaId);
    std::cout << "\033[1m  Sala: " << sala->getNume()
              << "  (" << sala->getNumarLocuriLibere() << " locuri libere)\033[0m\n\n"
              << "          [ ECRAN ]\n\n";
    sala->afiseazaLocuri();
}

void Cinematograf::afiseazaRezervari() const {
    if (rezervari.empty()) { std::cout << "  Nu exista rezervari.\n"; return; }
    std::cout << "\033[1m" << std::left
              << std::setw(5)  << "ID"
              << std::setw(22) << "Film"
              << std::setw(8)  << "Sala"
              << std::setw(7)  << "Loc"
              << std::setw(10) << "Tip loc"
              << std::setw(8)  << "Pret"
              << "  Mod     Data\n\033[0m"
              << std::string(80, '-') << "\n";
    for (const auto& r : rezervari) r->afiseaza();
}

void Cinematograf::realizeazaRezervare(int filmId, int salaId, int rand, int col) {
    Film* film = findFilm(filmId);
    Sala* sala = findSala(salaId);
    sala->ocupa(rand, col);
    rezervari.push_back(std::make_unique<Rezervare>(film, sala, rand, col));
}

void Cinematograf::realizeazaRezervareOnline(int filmId, int salaId,
                                              int rand, int col,
                                              const std::string& email) {
    Film* film = findFilm(filmId);
    Sala* sala = findSala(salaId);
    sala->ocupa(rand, col);
    auto rez = std::make_unique<RezervareOnline>(film, sala, rand, col, email);
    rez->trimitConfirmare();
    rezervari.push_back(std::move(rez));
}

void Cinematograf::anuleazaRezervare(int rezervareId) {
    auto it = std::find_if(rezervari.begin(), rezervari.end(),
                           [rezervareId](const auto& r){ return r->getId() == rezervareId; });
    if (it == rezervari.end()) throw RezervareNotFoundException(rezervareId);
    (*it)->getSala()->elibereaza((*it)->getRand(), (*it)->getCol());
    rezervari.erase(it);
}

std::vector<Film*> Cinematograf::cautaFilme(const std::string& titlu) const {
    std::vector<Film*> rez;
    std::string q = titlu;
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);
    for (auto& f : filme) {
        std::string t = f.getTitlu();
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        if (t.find(q) != std::string::npos)
            rez.push_back(const_cast<Film*>(&f));
    }
    return rez;
}

std::vector<Film*> Cinematograf::filtreazaDupaTip(TipFilm tip) const {
    std::vector<Film*> rez;
    for (auto& f : filme)
        if (f.getTip() == tip) rez.push_back(const_cast<Film*>(&f));
    return rez;
}

std::vector<Film*> Cinematograf::filtreazaDupaGen(const std::string& gen) const {
    std::vector<Film*> rez;
    for (auto& f : filme)
        if (f.getGen() == gen) rez.push_back(const_cast<Film*>(&f));
    return rez;
}

std::vector<Film*> Cinematograf::filtreazaDisponibile() const {
    std::vector<Film*> rez;
    for (auto& f : filme)
        for (auto& s : sali)
            if (s->areLocuriLibere()) { rez.push_back(const_cast<Film*>(&f)); break; }
    return rez;
}

std::vector<Film*> Cinematograf::getFilme() const {
    std::vector<Film*> rez;
    for (auto& f : filme) rez.push_back(const_cast<Film*>(&f));
    return rez;
}

Sala* Cinematograf::getSala(int salaId) const { return findSala(salaId); }

std::vector<Rezervare*> Cinematograf::getRezervari() const {
    std::vector<Rezervare*> rez;
    for (auto& r : rezervari) rez.push_back(r.get());
    return rez;
}

std::vector<Sala*> Cinematograf::getSali() const {
    std::vector<Sala*> rez;
    for (auto& s : sali) rez.push_back(s.get());
    return rez;
}
