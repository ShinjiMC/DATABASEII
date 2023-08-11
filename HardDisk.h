#pragma once
#include "StackBlocks.h"
#include <string>
#include <vector>
#include <iostream>
class HardDisk {
private:
	StackBlocks bloques;
	int numBloques, numSectoresXBloque, numPlatos, numPistas, numSectores, numBytes;
public:
	HardDisk();
	HardDisk(int numPlatos, int numPistas, int numSectores, int numBytes, int numSectoresXBloque);
	~HardDisk() {}

	const char* GetSectPATH(int indexSector);

	void ShowSectPATH(int n);

	void ShowSectXBloq(int indBloq);

	void InsertBlock(std::string data, std::vector <int> numRegPart, std::vector<std::pair<int, int>> cabecera, std::vector <int> sizeSect, std::vector <int> DirtyBitSect, int index, int bytes, int bytesOcupados);

	int getBytesBloq();
	int getBytesBloqOcupados(int n);
	int getBytesSectOcupados(int n);
	void ShowBloq(int n);
	void ShowBloqO(int n);
	void ShowSectByBloq(int n);

	int getNumSectoresXBloque() const;

	// Getter para el atributo 'numPlatos'
	int getNumPlatos() const;

	// Getter para el atributo 'numPistas'
	int getNumPistas() const;
	// Getter para el atributo 'numSectores'
	int getNumSectores() const;

	// Getter para el atributo 'numBloques'
	int getNumBloques() const;

	// Getter para el atributo 'numBytes'
	int getNumBytes() const;

	StackBlocks getStackBlock();
};

