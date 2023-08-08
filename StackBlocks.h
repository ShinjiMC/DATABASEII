#pragma once
#include "Block.h"
#include <string>
#include <vector>

class StackBlocks {
private:
	std::vector<Block> bloques;
	int numBloques;
public:
	StackBlocks();
	StackBlocks(int numBloques, int numSectoresXBloque, int numPlatos, int numSectores, int numPistas, int BytesXSector);
	void InsertData(std::string data, std::vector <int> numRegPart, std::vector<std::pair<int, int>> cabecera, std::vector <int> sizeSect, std::vector <int> DirtyBitSect, int index, int bytes, int bytesOcupados);
	std::vector<Block>& getBloques();
	int getNumBloques() const;
	void ImprimirBloque(int N);
	std::vector<int> getVectNumReg(int n);
	std::string getDataN(int n);
	std::vector<std::pair<int, int>> getCabeceraN(int n);
	int getBytesOcupadosN(int n);
	std::vector<int> getSizeSect(int n);
	std::vector<int> getDirtyBitSect(int n);
	~StackBlocks() {}
};