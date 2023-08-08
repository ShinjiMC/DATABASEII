#pragma once
#include <string>
#include <vector>

class Block {
private:
	std::string dataBlock;
	std::vector<std::pair<int, int>> cabeceraBlock;
	std::vector<std::string> pathSectores;
	std::vector <int> sizeSect;//NEW
	std::vector<int> numRegistosPart;
	std::vector <int> DirtyBitSect;//NEW
	int idBloque, numSectorXBloque, capacidadBytes, BytesOcupados;

public:
	Block();
	Block(int idBloque, int numSectoresXBloque, int numPlatos, int numSectores, int numPistas, int BytesXSector);
	std::string GetSectPATH(int indexBloque, int numPlatos, int numSectores, int numPistas);
	void Bloque_Sector();
	void InsertarTodo(std::string data, std::vector <int> numRegPart, std::vector<std::pair<int, int>> cabecera, std::vector <int> sizeSect, std::vector <int> DirtyBitSect, int bytes, int bytesOcupados);
	void ShowSectores();
	void showBloque();
	int getBytesOcupados();
	std::string getData();
	std::vector<std::pair<int, int>> getCabecera();
	std::vector<int> getNumRegvect();
	std::vector<int> getSizeSect();
	std::vector<int> getDirtyBitSect();
	~Block();
};