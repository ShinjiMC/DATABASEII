#include "HardDisk.h"
#include "DiskCreator.h"
#include <filesystem>
#include <fstream>
#include <numeric>
namespace fs = std::filesystem;

HardDisk::HardDisk() : numSectoresXBloque(0), numPlatos(0), numPistas(0), numSectores(0) {}
// Constructor por parametros
HardDisk::HardDisk(int numPlatos, int numPistas, int numSectores, int numBytes, int numSectoresXBloque)
	: numSectoresXBloque(numSectoresXBloque), numPlatos(numPlatos), numPistas(numPistas), numSectores(numSectores), numBytes(numBytes) {
	numBloques = ((numPlatos * 2 * numPistas * numSectores) / numSectoresXBloque);
	StackPlatos::getInstance(numPlatos, numPistas, numSectores);
	bloques = StackBlocks(numBloques, numSectoresXBloque, numPlatos, numSectores, numPistas, numBytes);
}

const char* HardDisk::GetSectPATH(int indexSector) {
	int i = indexSector;
	int Pn = 0, Sn = 0, Pin = 0, Sectn = 0;
	while (i != 0) {
		if (Sn == 2) { Pn++; Sn = 0; }
		if (Pn == numPlatos) { Sectn++; Pn = 0; }
		if (Sectn == numSectores) { Pin++; Sectn = 0; }
		if (Pin == numPistas) { std::cout << "No existe en el Disco " << std::endl; }
		i--;
		if (i == 0) { break; }
		else { Sn++; }
	}
	char Path[7] = "./Disk";
	static char filePath[256];
	snprintf(filePath, sizeof(filePath), "%s/Plato_%d/Superficie_%d/Pista_%d/Sector_%d.txt", Path, Pn, Sn, Pin, Sectn);
	return filePath;
}

void HardDisk::ShowSectPATH(int n) {
	int i = n;
	int Pn = 0, Sn = 0, Pin = 0, Sectn = 0;
	while (i != 0) {
		if (Sn == 2) { Pn++; Sn = 0; }
		if (Pn == numPlatos) { Sectn++; Pn = 0; }
		if (Sectn == numSectores) { Pin++; Sectn = 0; }
		if (Pin == numPistas) { std::cout << "No existe en el Disco " << std::endl; }
		i--;
		if (i == 0) { break; }
		else { Sn++; }
	}
	char Path[7] = "./Disk";
	static char filePath[256];
	snprintf(filePath, sizeof(filePath), "%s/Plato_%d/Superficie_%d/Pista_%d/Sector_%d.txt", Path, Pn, Sn, Pin, Sectn);
	std::cout << "\t\tPlato:" << Pn << "\t" << "Superficie: " << Sn << "\t" << "Pista: " << Pin << "\t" << "Sector: " << Sectn << std::endl;
	std::cout << "\t\tPath: " << filePath << std::endl;
}

void HardDisk::ShowSectXBloq(int indBloq) {
	bloques.getBloques()[indBloq].ShowSectores();
}

void HardDisk::InsertBlock(std::string data, std::vector <int> numRegPart, std::vector<std::pair<int, int>> cabecera, std::vector <int> sizeSect, std::vector <int> DirtyBitSect, int index,int bytes, int bytesOcupados) {
	bloques.InsertData(data, numRegPart, cabecera, sizeSect, DirtyBitSect,index,bytes, bytesOcupados);
}

int HardDisk::getBytesBloq() {
	return (numBytes * numSectoresXBloque);
}

int HardDisk::getBytesBloqOcupados(int n) {
	return bloques.getBytesOcupadosN(n);
}

int HardDisk::getBytesSectOcupados(int n) {
	const char* path = GetSectPATH(n);
	std::ifstream inputFile(path, std::ios::binary | std::ios::ate);
	if (!inputFile) {
		std::cerr << "Error al abrir el archivo Bloque: " << path << std::endl;
		return -1;
	}
	std::streampos fileSize = inputFile.tellg();
	inputFile.close();
	return static_cast<int>(fileSize);
}

void HardDisk::ShowBloq(int n) {
	bloques.ImprimirBloque(n);
}

void HardDisk::ShowSectByBloq(int n) {
	ShowSectXBloq(n);
}

// Getter para el atributo 'numSectoresXBloque'
int HardDisk::getNumSectoresXBloque() const { return numSectoresXBloque; }

// Getter para el atributo 'numPlatos'
int HardDisk::getNumPlatos() const {
	return numPlatos;
}

// Getter para el atributo 'numPistas'
int HardDisk::getNumPistas() const {
	return numPistas;
}

// Getter para el atributo 'numSectores'
int HardDisk::getNumSectores() const {
	return numSectores;
}

// Getter para el atributo 'numBloques'
int HardDisk::getNumBloques() const {
	return numBloques;
}

// Getter para el atributo 'numBytes'
int HardDisk::getNumBytes() const {
	return numBytes;
}


StackBlocks HardDisk::getStackBlock() {
	return bloques;
}