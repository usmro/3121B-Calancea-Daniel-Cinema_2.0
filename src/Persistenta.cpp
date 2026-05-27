#include "../include/Persistenta.h"
#include "../include/RezervareOnline.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Fisierele in care se salveaza datele
static const std::string F_FILME     = "filme.csv";
static const std::string F_SALI      = "sali.csv";
static const std::string F_REZERVARI = "rezervari.csv";

bool Persistenta::existaFisier(const std::string& fisier) {
    std::ifstream f(fisier);
    return f.good();
}

// ── FILME ────────────────────────────────────────────────────────────────────
// Format: id;titlu;gen;durata;tip(0=2D/1=3D);descriere

void Persistenta::salveazaFilme(const std::string& fisier, const Cinematograf& cinema) {
    std::ofstream out(fisier);
    if (!out) { std::cerr << "[PERSISTENTA] Nu pot salva filmele.\n"; return; }
    for (Film* f : cinema.getFilme()) {
        out << f->getId()        << ";"
            << f->getTitlu()     << ";"
            << f->getGen()       << ";"
            << f->getDurata()    << ";"
            << (f->getTip() == TipFilm::_3D ? 1 : 0) << ";"
            << f->getDescriere() << "\n";
    }
}

void Persistenta::incarcaFilme(const std::string& fisier, Cinematograf& cinema) {
    std::ifstream in(fisier);
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        try {
            int id, durata, tipInt;
            std::string titlu, gen, descriere;
            std::getline(ss, token, ';'); id      = std::stoi(token);
            std::getline(ss, titlu,  ';');
            std::getline(ss, gen,    ';');
            std::getline(ss, token, ';'); durata  = std::stoi(token);
            std::getline(ss, token, ';'); tipInt  = std::stoi(token);
            std::getline(ss, descriere);
            TipFilm tip = (tipInt == 1) ? TipFilm::_3D : TipFilm::_2D;
            cinema.adaugaFilm(Film(id, titlu, gen, durata, tip, descriere));
        } catch (...) {}
    }
}

// ── SALI ─────────────────────────────────────────────────────────────────────
// Format: id;nume;nrRanduri;nrColoane;tipLocuri(0,1,2,0,0,... flat row-by-row)

void Persistenta::salveazaSali(const std::string& fisier, const Cinematograf& cinema) {
    std::ofstream out(fisier);
    if (!out) { std::cerr << "[PERSISTENTA] Nu pot salva salile.\n"; return; }
    for (Sala* s : cinema.getSali()) {
        out << s->getId()        << ";"
            << s->getNume()      << ";"
            << s->getNrRanduri() << ";"
            << s->getNrColoane() << ";";
        // Serializeaza tipurile de locuri flat
        bool first = true;
        for (int r = 0; r < s->getNrRanduri(); ++r) {
            for (int c = 0; c < s->getNrColoane(); ++c) {
                if (!first) out << ",";
                out << static_cast<int>(s->getTipLoc(r, c));
                first = false;
            }
        }
        out << "\n";
    }
}

void Persistenta::incarcaSali(const std::string& fisier, Cinematograf& cinema) {
    std::ifstream in(fisier);
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        try {
            int id, nrRanduri, nrColoane;
            std::string nume, tipLocuriStr;
            std::getline(ss, token,      ';'); id        = std::stoi(token);
            std::getline(ss, nume,        ';');
            std::getline(ss, token,      ';'); nrRanduri = std::stoi(token);
            std::getline(ss, token,      ';'); nrColoane = std::stoi(token);
            std::getline(ss, tipLocuriStr);

            Sala* sala = new Sala(id, nume, nrRanduri, nrColoane);

            // Restaureaza tipurile de locuri
            std::stringstream tss(tipLocuriStr);
            std::string val;
            for (int r = 0; r < nrRanduri && std::getline(tss, val, ','); ++r) {
                for (int c = 0; c < nrColoane && std::getline(tss, val, ','); ) {
                    // primul val e deja citit inainte de inner loop
                    TipLoc t = static_cast<TipLoc>(std::stoi(val));
                    sala->setTipLoc(r, c, t);
                    ++c;
                    if (c < nrColoane) std::getline(tss, val, ',');
                }
            }
            // Recitire corecta a tipurilor
            // Resetam si recitim corect
            delete sala;
            sala = new Sala(id, nume, nrRanduri, nrColoane);
            std::stringstream tss2(tipLocuriStr);
            for (int r = 0; r < nrRanduri; ++r) {
                for (int c = 0; c < nrColoane; ++c) {
                    std::string v;
                    std::getline(tss2, v, ',');
                    if (!v.empty())
                        sala->setTipLoc(r, c, static_cast<TipLoc>(std::stoi(v)));
                }
            }
            cinema.adaugaSala(sala);
        } catch (...) {}
    }
}

// ── REZERVARI ────────────────────────────────────────────────────────────────
// Format: filmId;salaId;rand;col;isOnline;email

void Persistenta::salveazaRezervari(const std::string& fisier, const Cinematograf& cinema) {
    std::ofstream out(fisier);
    if (!out) { std::cerr << "[PERSISTENTA] Nu pot salva rezervarile.\n"; return; }
    for (Rezervare* r : cinema.getRezervari()) {
        RezervareOnline* ro = dynamic_cast<RezervareOnline*>(r);
        out << r->getFilm()->getId() << ";"
            << r->getSala()->getId() << ";"
            << r->getRand()          << ";"
            << r->getCol()           << ";"
            << (ro ? 1 : 0)          << ";"
            << (ro ? ro->getEmail() : "") << "\n";
    }
}

void Persistenta::incarcaRezervari(const std::string& fisier, Cinematograf& cinema) {
    std::ifstream in(fisier);
    if (!in) return;
    int loaded = 0;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        try {
            int filmId, salaId, rand, col, isOnline;
            std::string email;
            std::getline(ss, token, ';'); filmId   = std::stoi(token);
            std::getline(ss, token, ';'); salaId   = std::stoi(token);
            std::getline(ss, token, ';'); rand     = std::stoi(token);
            std::getline(ss, token, ';'); col      = std::stoi(token);
            std::getline(ss, token, ';'); isOnline = std::stoi(token);
            if (isOnline) std::getline(ss, email);

            std::cout.setstate(std::ios::failbit);
            if (isOnline && !email.empty())
                cinema.realizeazaRezervareOnline(filmId, salaId, rand, col, email);
            else
                cinema.realizeazaRezervare(filmId, salaId, rand, col);
            std::cout.clear();
            ++loaded;
        } catch (...) {
            std::cout.clear();
        }
    }
    if (loaded > 0)
        std::cout << "[PERSISTENTA] " << loaded << " rezervari incarcate.\n";
}

// ── SHORTCUT-URI ─────────────────────────────────────────────────────────────

void Persistenta::salveazaTot(Cinematograf& cinema) {
    salveazaFilme(F_FILME, cinema);
    salveazaSali(F_SALI, cinema);
    salveazaRezervari(F_REZERVARI, cinema);
    std::cout << "[PERSISTENTA] Date salvate.\n";
}

bool Persistenta::incarcaTot(Cinematograf& cinema) {
    if (!existaFisier(F_FILME)) return false;
    incarcaFilme(F_FILME, cinema);
    incarcaSali(F_SALI, cinema);
    incarcaRezervari(F_REZERVARI, cinema);
    return true;
}
