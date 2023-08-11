#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class Sector {
public:
    static void getInstance(int idPlato, int idSuperficie, int idPista, int idSector);
private:
    Sector() = default;
};

class Pista {
public:
    static void getInstance(int idPlato, int idSuperficie, int idPista, int numSectores);

private:
    Pista() = default;
};

class Superficie {
public:
    static void getInstance(int idPlato, int idSuperficie, int numPistas, int numSectores);

private:
    Superficie() = default;
};

class Plato {
public:
    static void getInstance(int idPlato, int numPistas, int numSectores);

private:
    Plato() = default;
};

class StackPlatos {
public:
    static void getInstance(int numPlatos, int numPistas, int numSectores);

private:
    StackPlatos() = default;
};
