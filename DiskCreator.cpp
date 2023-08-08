#include "DiskCreator.h"
void Sector::getInstance(int idPlato, int idSuperficie, int idPista, int idSector) {
    char Path[256];
    snprintf(Path, sizeof(Path), "./Disk/Plato_%d/Superficie_%d/Pista_%d/Sector_%d.txt", idPlato, idSuperficie, idPista, idSector);
    Path[sizeof(Path) - 1] = '\0';
    if (!fs::exists(Path)) {
        std::ofstream file(Path);
        if (!file.is_open()) {
            std::cerr << "Error al crear el archivo del Sector " << idSector << ": " << Path << std::endl;
        }
    }
}

void Pista::getInstance(int idPlato, int idSuperficie, int idPista, int numSectores) {
    char Path[256];
    snprintf(Path, sizeof(Path), "./Disk/Plato_%d/Superficie_%d/Pista_%d", idPlato, idSuperficie, idPista);
    Path[sizeof(Path) - 1] = '\0';
    if (!fs::exists(Path)) {
        if (fs::create_directory(Path)) {
            for (int i = 0; i < numSectores; i++) {
                Sector::getInstance(idPlato, idSuperficie, idPista, i);
            }
        }
        else {
            std::cerr << "Error al crear el directorio de Pista " << idPista << ": " << Path << std::endl;
        }
    }
}

void Superficie::getInstance(int idPlato, int idSuperficie, int numPistas, int numSectores) {
    char Path[256];
    snprintf(Path, sizeof(Path), "./Disk/Plato_%d/Superficie_%d", idPlato, idSuperficie);
    Path[sizeof(Path) - 1] = '\0';
    if (!fs::exists(Path)) {
        if (fs::create_directory(Path)) {
            for (int i = 0; i < numPistas; i++) {
                Pista::getInstance(idPlato, idSuperficie, i, numSectores);
            }
        }
        else {
            std::cerr << "Error al crear el directorio de Superficie " << idSuperficie << ": " << Path << std::endl;
        }
    }
}

void Plato::getInstance(int idPlato, int numPistas, int numSectores) {
    char Path[256];
    snprintf(Path, sizeof(Path), "./Disk/Plato_%d", idPlato);
    Path[sizeof(Path) - 1] = '\0';
    if (!fs::exists(Path)) {
        if (fs::create_directory(Path)) {
            for (int i = 0; i < 2; i++) {
                Superficie::getInstance(idPlato, i, numPistas, numSectores);
            }
        }
        else {
            std::cerr << "Error al crear el directorio de Plato " << idPlato << ": " << Path << std::endl;
        }
    }
}

void StackPlatos::getInstance(int numPlatos, int numPistas, int numSectores) {
    if (!fs::exists("./Disk")) {
        if (fs::create_directory("./Disk")) {
            for (int i = 0; i < numPlatos; i++) {
                Plato::getInstance(i, numPistas, numSectores);
            }
        }
        else {
            std::cerr << "Error al crear el directorio principal del Disco: ./Disk" << std::endl;
        }
    }
}