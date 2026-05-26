#include <cassert>
#include <iostream>
#include <stdexcept>
#include "../include/Film.h"
#include "../include/Sala.h"
#include "../include/Rezervare.h"
#include "../include/RezervareOnline.h"
#include "../include/Exceptii.h"

// ── Utilitare ────────────────────────────────────────────────────────────────
static int teste_trecute = 0;
static int teste_esuate  = 0;

#define RUN_TEST(fn) \
    do { \
        try { \
            fn(); \
            std::cout << "\033[32m[OK]\033[0m " << #fn << "\n"; \
            ++teste_trecute; \
        } catch (const std::exception& e) { \
            std::cout << "\033[31m[FAIL]\033[0m " << #fn \
                      << " — " << e.what() << "\n"; \
            ++teste_esuate; \
        } \
    } while(0)

// ── Teste pret ───────────────────────────────────────────────────────────────

// Film 2D, loc Standard, zi normala -> 30 lei
void test_pret_2D_standard() {
    Film film("Interstellar", "SF", 169, TipFilm::_2D);
    Sala sala("S1", 3, 3);
    Rezervare rez(&film, &sala, 0, 0);
    assert(rez.getPretFinal() == 30.0 || rez.getPretFinal() == 30.0 * 1.15);
    // acceptam ambele variante (depinde daca testul ruleaza sambata/duminica)
}

// Film 3D, loc Standard, zi normala -> 45 lei
void test_pret_3D_standard() {
    Film film("Inception", "SF", 148, TipFilm::_3D);
    Sala sala("S1", 3, 3);
    Rezervare rez(&film, &sala, 0, 0);
    assert(rez.getPretFinal() == 45.0 || rez.getPretFinal() == 45.0 * 1.15);
}

// Film 3D, loc VIP -> pret de baza * 1.5
void test_pret_3D_vip() {
    Film film("Dune", "SF", 155, TipFilm::_3D);
    Sala sala("S1", 3, 3);
    sala.setTipLoc(0, 0, TipLoc::VIP);
    Rezervare rez(&film, &sala, 0, 0);
    double pretAsteptat = 45.0 * 1.5; // 67.5 lei (fara weekend)
    double pretWeekend  = 45.0 * 1.5 * 1.15;
    assert(rez.getPretFinal() == pretAsteptat ||
           rez.getPretFinal() == pretWeekend);
}

// Film 2D, loc Student -> pret de baza * 0.8
void test_pret_2D_student() {
    Film film("Oppenheimer", "Drama", 180, TipFilm::_2D);
    Sala sala("S1", 3, 3);
    sala.setTipLoc(1, 1, TipLoc::STUDENT);
    Rezervare rez(&film, &sala, 1, 1);
    double pretAsteptat = 30.0 * 0.8; // 24 lei (fara weekend)
    double pretWeekend  = 30.0 * 0.8 * 1.15;
    assert(rez.getPretFinal() == pretAsteptat ||
           rez.getPretFinal() == pretWeekend);
}

// ── Teste exceptii ───────────────────────────────────────────────────────────

// Rezervare pe un loc deja ocupat -> LocOcupatException
void test_loc_ocupat_arunca_exceptie() {
    Film film("Inception", "SF", 148, TipFilm::_3D);
    Sala sala("S1", 3, 3);
    sala.ocupa(0, 0); // ocupam locul manual

    bool exceptieAruncata = false;
    try {
        sala.ocupa(0, 0); // incercam sa il ocupam din nou
    } catch (const LocOcupatException&) {
        exceptieAruncata = true;
    }
    assert(exceptieAruncata);
}

// Index in afara salii -> IndexInvalidException
void test_index_invalid_arunca_exceptie() {
    Sala sala("S1", 3, 3);

    bool exceptieAruncata = false;
    try {
        sala.ocupa(10, 10); // randul 10 nu exista intr-o sala 3x3
    } catch (const IndexInvalidException&) {
        exceptieAruncata = true;
    }
    assert(exceptieAruncata);
}

// Index negativ -> IndexInvalidException
void test_index_negativ_arunca_exceptie() {
    Sala sala("S1", 3, 3);

    bool exceptieAruncata = false;
    try {
        sala.ocupa(-1, 0);
    } catch (const IndexInvalidException&) {
        exceptieAruncata = true;
    }
    assert(exceptieAruncata);
}

// ── Teste RezervareOnline ────────────────────────────────────────────────────

// RezervareOnline salveaza emailul corect
void test_rezervare_online_email() {
    Film film("Dune", "SF", 155, TipFilm::_3D);
    Sala sala("S1", 3, 3);
    RezervareOnline rez(&film, &sala, 0, 0, "test@gmail.com");
    assert(rez.getEmailClient() == "test@gmail.com");
    assert(rez.getTip() == "online");
}

// RezervareOnline trimite confirmarea
void test_rezervare_online_confirmare() {
    Film film("Dune", "SF", 155, TipFilm::_3D);
    Sala sala("S1", 3, 3);
    RezervareOnline rez(&film, &sala, 1, 1, "client@yahoo.com");
    assert(!rez.isConfirmareTrimisa());
    rez.trimitConfirmare();
    assert(rez.isConfirmareTrimisa());
}

// ── Teste Sala ───────────────────────────────────────────────────────────────

// Sala nou creata are toate locurile libere
void test_sala_initial_libera() {
    Sala sala("S1", 4, 5);
    assert(sala.areLocuriLibere());
    assert(sala.getNumarLocuriLibere() == 20); // 4 * 5
}

// Dupa ocupare, numarul de locuri libere scade
void test_sala_locuri_libere_scad() {
    Sala sala("S1", 3, 3);
    assert(sala.getNumarLocuriLibere() == 9);
    sala.ocupa(0, 0);
    sala.ocupa(1, 1);
    assert(sala.getNumarLocuriLibere() == 7);
}

// Dupa eliberare, locul devine din nou disponibil
void test_sala_eliberare_loc() {
    Sala sala("S1", 3, 3);
    sala.ocupa(0, 0);
    assert(!sala.esteLiber(0, 0));
    sala.elibereaza(0, 0);
    assert(sala.esteLiber(0, 0));
}

// ── Main ─────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "\033[1m\n  === TESTE UNITARE — Cinema Sistem ===\n\n\033[0m";

    std::cout << "\033[33m[Preturi]\033[0m\n";
    RUN_TEST(test_pret_2D_standard);
    RUN_TEST(test_pret_3D_standard);
    RUN_TEST(test_pret_3D_vip);
    RUN_TEST(test_pret_2D_student);

    std::cout << "\n\033[33m[Exceptii]\033[0m\n";
    RUN_TEST(test_loc_ocupat_arunca_exceptie);
    RUN_TEST(test_index_invalid_arunca_exceptie);
    RUN_TEST(test_index_negativ_arunca_exceptie);

    std::cout << "\n\033[33m[Rezervare online]\033[0m\n";
    RUN_TEST(test_rezervare_online_email);
    RUN_TEST(test_rezervare_online_confirmare);

    std::cout << "\n\033[33m[Sala]\033[0m\n";
    RUN_TEST(test_sala_initial_libera);
    RUN_TEST(test_sala_locuri_libere_scad);
    RUN_TEST(test_sala_eliberare_loc);

    std::cout << "\n" << std::string(40, '-') << "\n";
    if (teste_esuate == 0) {
        std::cout << "\033[32m\033[1m  Toate " << teste_trecute
                  << " teste au trecut!\033[0m\n\n";
        return 0;
    } else {
        std::cout << "\033[31m  " << teste_esuate << " teste esuate, "
                  << teste_trecute << " trecute.\033[0m\n\n";
        return 1;
    }
}
