#include "structuras.h"
#include <string>

structuras::structuras() : tamTotalStatic(0), cantidadAtributos(0), tamTotalMAXVariable(0) {}

structuras::structuras(const char* path, const char* nombre) {
    strncpy(Path, path, sizeof(Path) - 1);
    strncpy(name, nombre, sizeof(name) - 1);
    // Contar lineas
    std::ifstream inputFile0(Path);
    if (!inputFile0) {
        std::cerr << "Error al abrir el archivo: " << Path << std::endl;
        return;
    }

    int lineCount = 0;
    char d;
    while (inputFile0.get(d)) {
        if (d == '\n') {
            lineCount++;
        }
    }
    inputFile0.clear();  // Restaurar el estado del flujo
    inputFile0.seekg(0, std::ios::beg);  // Volver al principio del archivo
    tams.resize(lineCount);
    names.resize(lineCount);
    types.resize(lineCount);
    inputFile0.close();
    cantidadAtributos = lineCount;
    ///posicion,longitud de los string, enteros , nullbitmap, string
    //21,5/26,10/36,10/#65000,#10101#palabra1palabra2
    // Extraer valores
    std::ifstream inputFile(Path);

    if (!inputFile) {
        std::cerr << "Error al abrir el archivo: " << Path << std::endl;
        return;
    }
    int lengthBitMap = lineCount;
    int espacioTotal = 0;
    char word[256];
    int countnointeger = 0;
    char c;
    int index = 0;
    int line = 0;
    int nW = 0;
    int indicesINT = 0;
    int indicesSTR = 0;
    while (inputFile.get(c)) {
        if (c == '#' || c == '\n') {
            word[index] = '\0';  // Agrega el carácter nulo al final de la palabra
            if (nW == 0) {
                if (strcmp(word, "INT") == 0) {
                    espacioTotal += 4;
                    indicesINT ++;
                    tams[line] = 4;
                    types[line] = word;
                    nW = 2;
                }
                if (strcmp(word, "LONGINT") == 0) {
                    espacioTotal += 8;
                    indicesINT++;
                    tams[line] = 8;
                    types[line] = word;
                    nW = 2;
                }
                else { types[line] = word; nW++; indicesSTR++; }
            }
            else if (nW == 1) { tams[line] = std::atoi(word); espacioTotal += std::atoi(word); countnointeger++; nW++; }
            else if (nW == 2) { names[line] = word; }
            index = 0;  // Reinicia el índice para la siguiente palabra
            if (c == '\n') { line++; nW = 0; }
        }
        else {
            word[index] = c;
            index++;
        }
    }
    int maxspace = espacioTotal + 3 + indicesINT + lengthBitMap + ((std::to_string(espacioTotal).length() * 2 + 2) * indicesSTR);
    if (std::to_string(espacioTotal).length() != std::to_string(maxspace).length()) {
        maxspace = espacioTotal + 3 + indicesINT + lengthBitMap + (((std::to_string(espacioTotal).length() + 1) * 2 + 2) * indicesSTR);
    }
    tamTotalStatic = espacioTotal;
    tamTotalMAXVariable = maxspace;
    inputFile.close();
}


const char* structuras::getPath() const {
    return Path;
}

const char* structuras::getName() const {
    return name;
}

int structuras::getTamTotal() const {
    return tamTotalStatic;
}

int structuras::getTamTotalV() const {
    return tamTotalMAXVariable;
}

int structuras::getTamAtIndex(int index) const {
    return tams[index];
}

void structuras::setPath(const char* path) {
    std::strcpy(Path, path);
}

std::vector<std::string> structuras::getTypesVect() {
    return types;
}

void structuras::setTamAtIndex(int index, int size) {
    tams[index] = size;
}

void structuras::showData() {
    for (int i = 0; i < cantidadAtributos; i++) {
        std::cout << "Tipo " << types[i] << " Tamano: " << tams[i] << " Nombre: " << names[i] << std::endl;
    }
}

std::vector<std::string>& structuras::getNames() {
    return names;
}

std::vector<std::string>& structuras::getTypes() {
    return types;
}

std::vector<int>& structuras::getValues() {
    return tams;
}