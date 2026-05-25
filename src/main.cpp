#include <iostream>
#include <string>
#include "../include/Cinematograf.h"
#include "../ui_terminal/UIAdmin.h"
#include "../ui_terminal/UIAngajat.h"

#ifdef BUILD_QT_UI
#include <QApplication>
#include "../ui_qt/MainWindow.h"
#endif

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
