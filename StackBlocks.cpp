#include "StackBlocks.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <numeric>
namespace fs = std::filesystem;

StackBlocks::StackBlocks() : numBloques(0) {}

StackBlocks::StackBlocks(int numBloques, int numSectoresXBloque, int numPlatos, int numSectores, int numPistas, int BytesXSector) : numBloques(numBloques) {
	if (fs::exists("./Disk")) {
		for (int i = 0; i < numBloques; i++) {
			Block tmp = Block(i, numSectoresXBloque, numPlatos, numSectores, numPistas, BytesXSector);
			bloques.push_back(tmp);
		}
	}
	else {
		std::cerr << "Error: No existe Disco creado previamente." << std::endl;
	}
}

void StackBlocks::InsertData(std::string data, std::vector <int> numRegPart, std::vector<std::pair<int, int>> cabecera, std::vector <int> sizeSect, std::vector <int> DirtyBitSect, int index, int bytes, int bytesOcupados) {
	bloques[index].InsertarTodo(data, numRegPart, cabecera, sizeSect, DirtyBitSect, bytes, bytesOcupados);
}

std::vector<Block>& StackBlocks::getBloques() {
	return bloques;
}

int StackBlocks::getNumBloques() const {
	return numBloques;
}

void StackBlocks::ImprimirBloque(int N) {
	std::cout << "Bloque " << N << ":" << std::endl;
	bloques[N].showBloque();
}

void StackBlocks::ImprimirBloqueO(int n)
{
	std::cout << "Bloque " << n << ":" << std::endl;
	bloques[n].showBloqueO();
}

std::vector<int> StackBlocks::getVectNumReg(int n) {
	return bloques[n].getNumRegvect();
}

std::vector<int> StackBlocks::getSizeSect(int n) {
	return bloques[n].getSizeSect();
}
std::vector<int> StackBlocks::getDirtyBitSect(int n) {
	return bloques[n].getDirtyBitSect();
}

std::string StackBlocks::getDataN(int n) {
	return bloques[n].getData();
}

std::vector<std::pair<int, int>> StackBlocks::getCabeceraN(int n) {
	return bloques[n].getCabecera();
}

int StackBlocks::getBytesOcupadosN(int n) {
	return bloques[n].getBytesOcupados();
}



