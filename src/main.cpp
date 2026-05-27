#include <iostream>
#include <string>
#include "../include/Cinematograf.h"
#include "../ui_terminal/UIAdmin.h"
#include "../ui_terminal/UIAngajat.h"

#ifdef BUILD_QT_UI
#include <QApplication>
#include "../ui_qt/MainWindow.h"
#endif

// Populeaza cinematograful cu date demo pentru prezentare
static void seedDemoData(Cinematograf& cinema) {
    cinema.adaugaFilm(Film("Inception",       "SF",      148, TipFilm::_3D, "Un hot de vise"));
    cinema.adaugaFilm(Film("Interstellar",    "SF",      169, TipFilm::_2D, "Calatorie in spatiu"));
    cinema.adaugaFilm(Film("Dune: Part Two",  "SF",      167, TipFilm::_3D, "Continuarea epicei"));
    cinema.adaugaFilm(Film("Oppenheimer",     "Drama",   180, TipFilm::_2D, "Parintele bombei atomice"));
    cinema.adaugaFilm(Film("The Dark Knight", "Actiune", 152, TipFilm::_2D, "Batman vs Joker"));

    Sala* s1 = new Sala("Sala 1", 6, 8);
    // Cateva locuri VIP in primul rand
    for (int c = 0; c < 8; ++c) s1->setTipLoc(0, c, TipLoc::VIP);
    // Cateva locuri Student in ultimul rand
    for (int c = 0; c < 8; ++c) s1->setTipLoc(5, c, TipLoc::STUDENT);
    cinema.adaugaSala(s1);

    Sala* s2 = new Sala("Sala 2", 5, 6);
    cinema.adaugaSala(s2);
}

static void printUsage(const std::string& prog) {
    std::cout << "Utilizare:\n"
              << "  " << prog << " --staff    # UI terminal (admin / angajat)\n"
              << "  " << prog << " --client   # UI grafic Qt (client)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string mod = argv[1];
    Cinematograf cinema("Cinema Central");

    if (mod == "--staff") {
        std::cout << Color::CYAN << Color::BOLD
                  << "\n  Selectati rolul:\n" << Color::RESET
                  << "  [1] Administrator\n"
                  << "  [2] Angajat\n"
                  << "  > ";
        int alegere;
        std::cin >> alegere;
        std::cin.ignore();

        if (alegere == 1) {
            UIAdmin ui(cinema);
            ui.run();
        } else if (alegere == 2) {
            UIAngajat ui(cinema);
            ui.run();
        } else {
            std::cerr << "Rol invalid!\n";
            return 1;
        }

    } else if (mod == "--client") {
#ifdef BUILD_QT_UI
        seedDemoData(cinema);
        QApplication app(argc, argv);
        MainWindow window(cinema);
        window.show();
        return app.exec();
#else
        std::cerr << Color::RED
                  << "UI-ul Qt nu este disponibil in aceasta compilare.\n"
                  << "Recompilati cu: cmake -DBUILD_QT_UI=ON ..\n"
                  << Color::RESET;
        return 1;
#endif
    } else {
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
