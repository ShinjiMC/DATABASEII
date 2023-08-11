#pragma once
#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
namespace fs = std::filesystem;

class structuras {
private:
    char Path[256];
    char name[256];
    int tamTotalStatic, cantidadAtributos, tamTotalMAXVariable;
    std::vector<int> tams;
    std::vector<std::string> names;
    std::vector<std::string> types;

public:
    structuras();
    structuras(const char* path, const char* nombre);

    const char* getPath() const;
    const char* getName() const;
    int getTamTotal() const;
    int getTamTotalV() const;
    int getTamAtIndex(int index) const;
    void setPath(const char* path);
    std::vector<std::string> getTypesVect();
    void setTamAtIndex(int index, int size);
    void showData();
    std::vector<std::string>& getNames();
    std::vector<std::string>& getTypes();
    std::vector<int>& getValues();
};