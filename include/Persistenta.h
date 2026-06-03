#pragma once
#include <string>
#include "Cinematograf.h"

class Persistenta {
public:
    // Salveaza toate rezervarile curente intr-un fisier CSV
    static void salveaza(const std::string& fisier, const Cinematograf& cinema);

    // Incarca rezervarile dintr-un fisier CSV in cinematograf
    // Daca fisierul nu exista (prima rulare), nu face nimic
    static void incarca(const std::string& fisier, Cinematograf& cinema);
};
