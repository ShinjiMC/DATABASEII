#pragma once
#include "structuras.h"
#include <vector>
namespace fs = std::filesystem;

class Cache {
private:
    char Path[8] = "./Cache";
    std::vector <structuras> estructuras;
    int cantidadEstruct;

public:
    Cache();

    structuras& getEstructura(const char* val);

    int getCantidadEstruct();

    void InsertDataDisk(int numPlatos, int numPistas, int numSectores, int bytesSector, int numBloques, int numSectorXBloque, int typeBuffer);

    void Reload();

    void AddEstruct(const char* cadena, const char* fileName);

    void showDataIndex(int n);
};