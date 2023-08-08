#include "Cache.h"
#include <string>

Cache::Cache() {
    int count = 0;
    if (!fs::exists(Path)) {
        if (fs::create_directory(Path)) {
            std::cout << "Directory created: " << Path << std::endl;
        }
        else {
            std::cerr << "Error creating directory: " << Path << std::endl;
        }
    }
    else {
        for (const auto& entry : fs::directory_iterator(Path)) {
            if (fs::is_regular_file(entry)) {
                count++;
            }
        }
    }
    cantidadEstruct = count;
}

structuras& Cache::getEstructura(const char* val) {
    for (int n = 0; cantidadEstruct > n; n++) {
        if (std::strcmp(estructuras[n].getName(), val) == 0) {
            return estructuras[n];
        }
    }
    throw std::runtime_error("No structure found with the specified name");
}

int Cache::getCantidadEstruct() {
    return cantidadEstruct;
}

void Cache::InsertDataDisk(int numPlatos, int numPistas, int numSectores, int bytesSector, int numBloques, int numSectorXBloque, int typeBuffer) {
    char DataDisk[256];
    snprintf(DataDisk, sizeof(DataDisk), "%d#%d#%d#%d#%d#%d#%d", numPlatos, numPistas, numSectores, bytesSector, numBloques, numSectorXBloque, typeBuffer);
    char PATH[13] = "./Cache/DISK";
    std::ofstream file(PATH, std::ios::trunc);
    if (file.is_open()) {
        file << DataDisk;
        file.close();
    }
    else {
        std::cerr << "Error creating or resetting file: " << PATH << std::endl;
    }
}

void Cache::Reload() {
    if (cantidadEstruct > 0) {
        std::vector<structuras> newEstructuras; // Creamos un vector temporal para almacenar las nuevas estructuras
        for (int n = 0; n < cantidadEstruct; n++) { // Usamos < en lugar de > en el bucle
            int currentIndex = 0;
            for (const auto& entry : fs::directory_iterator(Path)) {
                if (entry.is_regular_file()) {
                    if (currentIndex == n) {
                        const std::string& filePath = entry.path().string();
                        const std::string& fileName = entry.path().filename().string();

                        // Evitamos las fugas de memoria utilizando directamente std::string en lugar de char* y std::strcpy
                        if (fileName != "DISK") {
                            newEstructuras.push_back(structuras(filePath.c_str(), fileName.c_str()));
                        }
                    }
                    currentIndex++;
                }
            }
        }
        estructuras = std::move(newEstructuras); // Movemos las nuevas estructuras al vector miembro "estructuras"
        cantidadEstruct = estructuras.size(); // Actualizamos la cantidad de estructuras en el vector miembro
    }
}


void Cache::AddEstruct(const char* cadena, const char* fileName) {
    std::ofstream outputFile;
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/%s", Path, fileName);
    outputFile.open(filePath);

    if (outputFile.is_open()) {
        int currentIndex = 0;
        int charIndex = 0;
        char currentChar;

        while ((currentChar = cadena[charIndex]) != '\0') {
            if (currentChar == '|') {
                outputFile << std::endl;
                currentIndex++;
            }
            else {
                outputFile << currentChar;
            }
            charIndex++;
        }

        outputFile.close();
        cantidadEstruct++;
        std::cout << "File \"" << fileName << "\" saved successfully in the folder \"" << Path << "\"" << std::endl;
    }
    else {
        std::cout << "Error opening the output file." << std::endl;
    }
    Reload();
}

void Cache::showDataIndex(int n) {
    if (n < cantidadEstruct) {
        estructuras[n].showData();
    }
}