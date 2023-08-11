#include "Page.h"

// Constructor con parámetros
Page::Page(int idx, int numBytes, int BytesOcup, std::string dat, std::vector<std::pair<int, int>> cabec, std::vector <int> tmp, std::vector <int> SizeSector, std::vector <int> DirtySector)
	: index(idx), bytes(numBytes), bytesOcupados(BytesOcup), DirtyBit(0), PinCount(1), referenceBit(false) {
	numRegPart = tmp;
	cabecera = cabec;
	data = dat;
	sizeSect = SizeSector;
	DirtyBitSect = DirtySector;
}

void Page::InsertCabecera2(int numReg, int pos) {
	std::pair<int, int> tmp = std::make_pair(numReg, pos);
	cabecera.push_back(tmp);
}

void Page::InsertData(char content) {
	data += content;
	bytesOcupados++;
}

bool Page::getReferenceBit() const {
	return referenceBit;
}

void Page::setReferenceBit(bool bit) {
	referenceBit = bit;
}

void Page::setDirty() {
	DirtyBit = 1;
}
void Page::DirtyBitSects() {
	for (auto c : DirtyBitSect) {
		c = 1;
	}
}
void Page::unsetDirty() {
	DirtyBit = 0;
}

int Page::getDirty() const {
	return DirtyBit;
}

int Page::getPinCount() const {
	return PinCount;
}



std::string& Page::getDataR() {
	return data;
}
// Getter para el atributo 'data'
const std::string& Page::getData() const {
	return data;
}

//
std::vector<std::pair<int, int>>& Page::getCabeceraR() {
	return cabecera;
}

const std::vector<std::pair<int, int>> Page::getCabecera() const {
	return cabecera;
}
// Getter para el atributo 'index'
int Page::getIndex() const {
	return index;
}

// Getter para el atributo 'bytes'
int Page::getBytes() const {
	return bytes;
}

// Getter para el atributo 'bytesOcupados'
int Page::getBytesOcupados() const {
	return bytesOcupados;
}

int& Page::getBytesOcupadosR() {
	return bytesOcupados;
}

int Page::getNumregT() const {
	return std::accumulate(numRegPart.begin(), numRegPart.end(), 0);
}

void Page::replaceData(std::string n) {
	data = n;
}
std::vector <int> Page::getRegPart() {
	return numRegPart;
}
std::vector <int>& Page::getRegPartR() {//para sobreescribir datos
	return numRegPart;
}

std::vector <int> Page::getSizeSect() {
	return sizeSect;
}
std::vector <int>& Page::getSizeSectR() {
	return sizeSect;
}
std::vector <int> Page::getDirtySect() {
	return DirtyBitSect;
}
void Page::Eliminar(int id) {
	id--;
	int sum=0;
	for (int i = 0; i != numRegPart.size(); i++) {
		sum += numRegPart[i];
		if (sum <= id) {
			numRegPart[i] = numRegPart[i] - 1;
		}
	}
}
void Page::Insert(int numReg, int pos, std::string registros_conca, int bytes_regis, int idPart) {
	std::pair<int, int> tmp = std::make_pair(numReg, pos);
	cabecera.push_back(tmp);
	data += registros_conca;
	bytesOcupados += bytes_regis;
	numRegPart[idPart] = numRegPart[idPart] + 1;
}
void Page::InsertVar(int numReg, int pos, std::string registros_conca, int bytes_regis, int idPart) {
	std::pair<int, int> tmp = std::make_pair(numReg, pos);
	cabecera.push_back(tmp);
	data = registros_conca + data;
	bytesOcupados += bytes_regis;
	numRegPart[idPart] = numRegPart[idPart] + 1;
}
void Page::InsertVarFE(int pos) {
	std::pair<int, int> tmp = std::make_pair(0, pos);
	cabecera.insert(cabecera.begin(), tmp);
	int bytesTotal = bytes;
	std::string numStr = std::to_string(pos);
	if (std::to_string(pos).length() < std::to_string(bytesTotal).length()) {
		int zerosToAdd = std::to_string(bytesTotal).length() - std::to_string(pos).length();
		std::string zeros(zerosToAdd, '0');
		numStr = zeros + numStr;
	}
	std::string cabFreeSpace = ("0," + numStr + "#");
	bytesOcupados += cabFreeSpace.length();
	sizeSect[0] += cabFreeSpace.length();
}

int Page::getSpacePageVar(int MAXREG) {
	int totalMaxReg = 0;
	/*std::cout << "sizeSect: ";
	for (int i = 0; i < sizeSect.size(); ++i) {
		std::cout << sizeSect[i];
		if (i < sizeSect.size() - 1) {
			std::cout << ", ";
		}
	}
	std::cout << std::endl;*/
	for (int i = 0; i < sizeSect.size(); ++i) {
		if (sizeSect[i] == 0) {
			totalMaxReg += (bytes / sizeSect.size()) / MAXREG;
		}
		else {
			totalMaxReg += ((bytes / sizeSect.size()) - sizeSect[i]) / MAXREG;
		}
	}
	return totalMaxReg;
}

int Page::getSpacePage() {
	int numFillRegs = 0;
	for (const auto& par : cabecera) {
		if (par.first == -1) {
			numFillRegs++;
		}
	}
	return numFillRegs;
}

int Page::getFirstRegs() {
	int numFillRegs = -1;
	if (cabecera.size() > 0) {
		numFillRegs = cabecera[0].first;
		//Aca falta aplicar diccionarios, extrayendo desde el nombre de tabla ubicado en esa pagina, 
	}
	return numFillRegs;
}

int Page::getFirstReg() {
	if (!cabecera.empty()) {
		return cabecera.front().first;
	}
	return -1;
}

int Page::getLastReg() {
	if (!cabecera.empty()) {
		return cabecera.back().first;
	}
	return -1;
}


int Page::getPart(int n) {
	int suma = 0;
	int parte = 0;

	for (int i = 0; i < numRegPart.size(); i++) {
		suma += numRegPart[i];
		if (n <= cabecera[suma - 1].first) {
			parte = i;
			break;
		}
	}
	return parte;
}

int Page::numBytesforPart(int n) {//Segun el numero de registro te retorna la capacidadOcupada de esta parte de pagina (sector)
	int parte = getPart(n);
	std::vector<std::pair<int, int>> tmp;
	for (int i = numRegPart[parte - 1]; i < numRegPart[parte] + numRegPart[parte - 1]; i++) {
		tmp.push_back(cabecera[i]);
	}
	int bytesIndice = 4, bytesdd = 0;
	for (int i = 0; i < tmp.size(); i++) {
		bytesdd += (2 + std::to_string(tmp[i].second).length() + bytesIndice);
	}
	bytesdd += 2;
	int inicio = tmp[0].second;
	int fin = tmp.back().second + 1;
	size_t siguienteBarra = data.find("|", fin);
	if (siguienteBarra != std::string::npos) {
		fin = static_cast<int>(siguienteBarra);
	}
	int cantidadBytes = fin - inicio;
	return cantidadBytes + bytesdd;
}
int Page::getPartSize() { return bytes / numRegPart.size(); }

void Page::changeDirtyBitSect(int n) {
	DirtyBitSect[n] = 1;
}
