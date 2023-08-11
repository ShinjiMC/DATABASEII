#pragma once
#include "StackBlocks.h"
#include <string>
#include <vector>
#include <numeric>
#include <iostream>

class Page {
private:
	std::string data;
	std::vector <int> numRegPart;
	std::vector<std::pair<int, int>> cabecera;
	std::vector <int> sizeSect;
	std::vector <int> DirtyBitSect;
	int index;
	int bytes;
	int bytesOcupados;
	int DirtyBit;
	int PinCount;
	bool referenceBit;
public:
	Page() : index(0), bytes(0), bytesOcupados(0), DirtyBit(0), PinCount(1), referenceBit(0) {}
	Page(int idx, int numBytes, int BytesOcup, std::string dat, std::vector<std::pair<int, int>> cabec, std::vector <int> tmp, std::vector <int> SizeSector, std::vector <int> DirtySector);

	void InsertCabecera2(int numReg, int pos);
	void InsertData(char content);
	void Eliminar(int n);
	bool getReferenceBit() const;
	void setReferenceBit(bool bit);
	void DirtyBitSects();
	void setDirty();
	void unsetDirty();
	int getDirty() const;

	void setPinCount() { PinCount++; }
	int getPinCount() const;
	// Constructor con parámetros


	~Page() {}

	std::string& getDataR();
	// Getter para el atributo 'data'
	const std::string& getData() const;

	//
	std::vector<std::pair<int, int>>& getCabeceraR();

	const std::vector<std::pair<int, int>> getCabecera() const;
	// Getter para el atributo 'index'
	int getIndex() const;

	// Getter para el atributo 'bytes'
	int getBytes() const;

	// Getter para el atributo 'bytesOcupados'
	int getBytesOcupados() const;
	int getNumregT() const;

	void replaceData(std::string n);
	std::vector <int> getRegPart();
	std::vector <int>& getRegPartR();

	void Insert(int numReg, int pos, std::string registros_conca, int bytes_regis, int idPart);
	void InsertVar(int numReg, int pos, std::string registros_conca, int bytes_regis, int idPart);

	void InsertVarFE(int pos);

	int getSpacePageVar(int MAXREG);

	int getSpacePage();

	int getFirstRegs();

	int getFirstReg();

	int getLastReg();

	int getPart(int n);

	int numBytesforPart(int n);
	int getPartSize();
	std::vector <int> getSizeSect();
	std::vector <int>& getSizeSectR();
	std::vector <int> getDirtySect();
	void changeDirtyBitSect(int n);
	int& getBytesOcupadosR();
};