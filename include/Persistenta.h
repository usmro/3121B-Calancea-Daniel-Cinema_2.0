#pragma once
#include <string>
#include "Cinematograf.h"

class Persistenta {
public:
    // Verifica daca un fisier exista
    static bool existaFisier(const std::string& fisier);

    // ── Filme ────────────────────────────────────────────────────
    static void salveazaFilme(const std::string& fisier, const Cinematograf& cinema);
    static void incarcaFilme(const std::string& fisier, Cinematograf& cinema);

    // ── Sali ─────────────────────────────────────────────────────
    static void salveazaSali(const std::string& fisier, const Cinematograf& cinema);
    static void incarcaSali(const std::string& fisier, Cinematograf& cinema);

    // ── Rezervari ────────────────────────────────────────────────
    static void salveazaRezervari(const std::string& fisier, const Cinematograf& cinema);
    static void incarcaRezervari(const std::string& fisier, Cinematograf& cinema);

    // ── Salveaza tot dintr-o data ────────────────────────────────
    static void salveazaTot(Cinematograf& cinema);

    // ── Incarca tot dintr-o data ─────────────────────────────────
    // Returneaza true daca datele au fost incarcate din fisiere
    // Returneaza false daca fisierele nu existau (prima rulare)
    static bool incarcaTot(Cinematograf& cinema);
};
