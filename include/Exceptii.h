#pragma once
#include <stdexcept>
#include <string>

class CinemaException : public std::exception {
protected:
    std::string mesaj;
public:
    explicit CinemaException(const std::string& mesaj) : mesaj(mesaj) {}
    const char* what() const noexcept override { return mesaj.c_str(); }
};

class LocOcupatException : public CinemaException {
public:
    LocOcupatException(int rand, int col)
        : CinemaException("Locul [" + std::to_string(rand) + "]["
                          + std::to_string(col) + "] este deja ocupat!") {}
};

class IndexInvalidException : public CinemaException {
public:
    IndexInvalidException(int rand, int col)
        : CinemaException("Index invalid: rand=" + std::to_string(rand)
                          + ", col=" + std::to_string(col)) {}
};

class FilmNotFoundException : public CinemaException {
public:
    explicit FilmNotFoundException(int id)
        : CinemaException("Filmul cu id-ul " + std::to_string(id) + " nu a fost gasit!") {}
};

class SalaNotFoundException : public CinemaException {
public:
    explicit SalaNotFoundException(int id)
        : CinemaException("Sala cu id-ul " + std::to_string(id) + " nu a fost gasita!") {}
};

class RezervareNotFoundException : public CinemaException {
public:
    explicit RezervareNotFoundException(int id)
        : CinemaException("Rezervarea cu id-ul " + std::to_string(id) + " nu a fost gasita!") {}
};

class AnulareImpossibilaException : public CinemaException {
public:
    AnulareImpossibilaException()
        : CinemaException("Anularea nu este posibila! Filmul incepe in mai putin de 2 ore.") {}
};
