#pragma once
#include "Page.h"
#include "StackBlocks.h"
#include <string>
#include <vector>
#include "B_plus_tree.h"
#include <filesystem>
#include <iostream>
#include <bitset>

class BufferPool {
private:
	std::vector<Page> frames;
	StackBlocks bloquesC;
	int numFrames, numPaginas, numBytes, typeofBuffer;
public:
	// Constructor por defecto
	BufferPool();
	// Constructor con parámetros
	int straInt(std::string cadena);
	bool isIndexFrames(int n);

	int getIndexFromFrame(int n);

	void LRU(int n);

	void MRU(int n);

	void CLOCK(int n);

	void InsertPagetoBuffer(int n);

	int getIndexFrames(int n);
	std::vector <int> getRegPart(int n) {
		return frames[n].getRegPart();
	}

	BufferPool(int numPaginas, int numFrames, int numBytes, StackBlocks bloques, int typeofBuffers);

	~BufferPool();

	void insertDataPage(char content, int nPage);

	// Getter para el atributo 'pagina'
	Page getPageAtIndex(int n);
	int getBytesbyIndex(int n);
	int getBytesOcupadosbyIndex(int n);

	void insertCabeceraReg(int nPage, int numReg, int pos);

	// Getter para el atributo 'paginas'
	const std::vector<Page>& getPages() const;

	std::vector<Page>& gettingPages();

	std::vector<std::pair<int, int>> getCabeceraN(int n);

	std::vector<std::pair<int, int>> getCabeceraC(int n) {
		return frames[n].getCabecera();
	}

	std::vector<int> getRegPartN(int n);
	std::vector<int> getSizeSectN(int n);
	std::vector <int> getDirtyBitSectN(int n);
	std::string getDataN(int n);

	int getNumRegTotal(int n);

	// Getter para el atributo 'numPaginas'
	int getNumPages() const;

	int getNumRegTotalY(int n);

	std::string getDataNY(int n);

	void showCabecera(int n);

	void showPage(int n);

	void ADDReg(int numReg, int pos, std::string registros_conca, int bytes_regis, int inde, int idPart);

	void ADDRegVar(int numReg, int pos, std::string registros_conca, int bytes_regis, int inde, int idPart);

	void ADDFREESPACE(int pos, int inde);

	int ValidateVariable();

	int freeSpaceMappingVar(int MAXREG);

	int freeSpaceMapping();

	int Page_complete();

	void FillBlankReg(std::string& cadena, int nPosicion);

	int buscarNumeroEnPrimerPar(const std::vector<std::pair<int, int>>& vectorPares, int numero);

	int ubicar_ultimo_registro(std::vector<std::pair<int, int>>& vectorPares);

	void ChangeCabecera(std::vector<std::pair<int, int>>& vectorPares, int valor, int Nvalor);

	void Add_Cabecera(std::vector<std::pair<int, int>>& vectorPares, int registro, int posicion_inicio);

	void Add_CabeceraVAR(std::vector<std::pair<int, int>>& vectorPares, int registro, int posicion_inicio, int tam);

	void Add_CabeceraVAR2(std::vector<std::pair<int, int>>& vectorPares, int registro, int posicion_inicio);

	void AddFreeSpace(std::vector<std::pair<int, int>>& vectorPares, int pos, int numReg, int pos2);

	bool EliminarReg(int numRegE);

	bool FindReginCabecera(const std::vector<std::pair<int, int>>& vectorPares, int n);

	int MaxNumReg();

	void InsertRegistro(std::string& data, std::string reg, int pos);

	void InsertarRegistroUltimo(std::string& data, std::string reg, int pos);


	void InsertarRegistroUltimoVAR(std::string& data, std::string reg);

	void InsertarRegistroUltimo2(std::string& data, std::string reg);

	int SelectlastPageFill();

	int obtenerUltimoIndiceNoCero(const std::vector<int>& vec);

	int obtnerBytesParte(int n, const std::string& data, const std::vector <int>& numRegPart, const std::vector<std::pair<int, int>>& cabecera);

	void Changestring(std::string& cadena, int nPosicion, std::string reg);

	void MoverRegs(int n1, int n2);

	void BuscarRegs(int n1);
	void AddNewRegVar(int MAXREG, int numRegN, std::string registro,bool check, B_plus_tree* Arbol);
	void AddNewRegFija(int numRegN, std::string registro);

	bool allDirtyKnow() {
		bool allDirty = false;
		if (frames.size() == numFrames) {
			allDirty = std::all_of(frames.begin(), frames.end(), [](const Page& page) {
				return page.getDirty() == 1;
				});
		}
		return allDirty;
	}
	void showReg(int idKey, int idPag, const std::vector<int>& values, const std::vector<std::string>& types);
	void MovPOSRegsCab(std::vector<std::pair<int, int>>& vectorPares,std::string uwu, int valorar, int posRegCab);
};