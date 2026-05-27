#include "../include/Film.h"
#include "../include/Sala.h"
#include "../include/Rezervare.h"
#include "../include/RezervareOnline.h"
#include "../include/Exceptii.h"
#include <iostream>
#include <cassert>
#include <stdexcept>

static int passed = 0, failed = 0;

#define RUN_TEST(name) \
    try { name(); \
        std::cout << "\033[32m[OK]\033[0m " #name "\n"; ++passed; \
    } catch (const std::exception& e) { \
        std::cout << "\033[31m[FAIL]\033[0m " #name ": " << e.what() << "\n"; ++failed; \
    }

void test_pret_2D_standard() {
    Film f("Test", "Gen", 100, TipFilm::_2D);
    Sala s("S", 2, 2);
    Rezervare r(&f, &s, 0, 0);
    assert(r.getPret() >= 30.0 && r.getPret() <= 34.6);
}

void test_pret_3D_standard() {
    Film f("Test", "Gen", 100, TipFilm::_3D);
    Sala s("S", 2, 2);
    Rezervare r(&f, &s, 0, 0);
    assert(r.getPret() >= 45.0 && r.getPret() <= 51.8);
}

void test_pret_3D_vip() {
    Film f("Test", "Gen", 100, TipFilm::_3D);
    Sala s("S", 2, 2);
    s.setTipLoc(0, 0, TipLoc::VIP);
    Rezervare r(&f, &s, 0, 0);
    assert(r.getPret() >= 67.5 && r.getPret() <= 77.6);
}

void test_pret_2D_student() {
    Film f("Test", "Gen", 100, TipFilm::_2D);
    Sala s("S", 2, 2);
    s.setTipLoc(0, 0, TipLoc::STUDENT);
    Rezervare r(&f, &s, 0, 0);
    assert(r.getPret() >= 24.0 && r.getPret() <= 27.7);
}

void test_loc_ocupat_arunca_exceptie() {
    Sala s("S", 3, 3);
    s.ocupa(1, 1);
    bool threw = false;
    try { s.ocupa(1, 1); } catch (const LocOcupatException&) { threw = true; }
    assert(threw);
}

void test_index_invalid_arunca_exceptie() {
    Sala s("S", 3, 3);
    bool threw = false;
    try { s.ocupa(5, 0); } catch (const IndexInvalidException&) { threw = true; }
    assert(threw);
}

void test_index_negativ_arunca_exceptie() {
    Sala s("S", 3, 3);
    bool threw = false;
    try { s.ocupa(-1, 0); } catch (const IndexInvalidException&) { threw = true; }
    assert(threw);
}

void test_rezervare_online_email() {
    Film f("Test", "Gen", 100, TipFilm::_2D);
    Sala s("S", 2, 2);
    RezervareOnline ro(&f, &s, 0, 0, "test@test.com");
    assert(ro.getEmail() == "test@test.com");
}

void test_rezervare_online_confirmare() {
    Film f("Test", "Gen", 100, TipFilm::_2D);
    Sala s("S", 2, 2);
    RezervareOnline ro(&f, &s, 0, 1, "client@yahoo.com");
    assert(!ro.isConfirmareTrimisa());
    ro.trimitConfirmare();
    assert(ro.isConfirmareTrimisa());
}

void test_sala_initial_libera() {
    Sala s("S", 4, 4);
    assert(s.getNumarLocuriLibere() == 16);
}

void test_sala_locuri_libere_scad() {
    Sala s("S", 3, 3);
    s.ocupa(0, 0);
    s.ocupa(1, 1);
    assert(s.getNumarLocuriLibere() == 7);
}

void test_sala_eliberare_loc() {
    Sala s("S", 3, 3);
    s.ocupa(0, 0);
    s.elibereaza(0, 0);
    assert(s.esteLiber(0, 0));
}

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
    if (failed == 0)
        std::cout << "\033[32m\033[1m  Toate " << passed << " teste au trecut!\033[0m\n\n";
    else
        std::cout << "\033[31m  " << failed << " teste au esuat din " << (passed+failed) << "\033[0m\n\n";

    return failed;
}
