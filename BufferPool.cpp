#include "BufferPool.h"
#include <string>

// Constructor por defecto
BufferPool::BufferPool() : numFrames(0), numPaginas(0), numBytes(0), typeofBuffer(0) {}
// Constructor con parámetros
int BufferPool::straInt(std::string cadena) {
	int num = 0;
	if (cadena == "00-1") {
		num = -1;
	}
	else {
		num = stoi(cadena);
	}
	return num;
}

bool BufferPool::isIndexFrames(int n) {
	for (const Page& page : frames) {
		if (page.getIndex() == n) {
			return false; // El índice n ya existe en frames
		}
	}
	return true; // El índice n no se encontró en frames
}

int BufferPool::getIndexFromFrame(int n) {
	try {
		auto it = std::find_if(frames.begin(), frames.end(), [n](const Page& page) {
			return page.getIndex() == n;
			});

		if (it != frames.end()) {
			// El elemento con getIndex() igual a n fue encontrado en Cache
			int index = std::distance(frames.begin(), it);
			return index;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return -1; // Si no se encuentra, retornar -1 o un valor que indique que no se encontró
}


void BufferPool::LRU(int n) {//LRU
	if (n >= 0 && n < numPaginas) {
		if (isIndexFrames(n)) { // No está en vector
			if (frames.size() == numFrames) {
				frames.erase(frames.begin());
			}
			std::vector<std::pair<int, int>> CabeceraVector = bloquesC.getCabeceraN(n);
			Page tmp(n, numBytes, bloquesC.getBytesOcupadosN(n), bloquesC.getDataN(n), CabeceraVector, bloquesC.getVectNumReg(n), bloquesC.getSizeSect(n), bloquesC.getDirtyBitSect(n));
			frames.push_back(tmp);
		}
		else { // Está en vector
			auto it = std::find_if(frames.begin(), frames.end(), [&](const Page& page) {
				return page.getIndex() == n;
				});

			if (it != frames.end()) {
				std::rotate(it, it + 1, frames.end());
			}
		}
	}
	else {
		std::cout << "Página no encontrada" << std::endl;
		return;
	}
	/*std::cout << "Contenido del buffer de páginas:" << std::endl;
	for (const auto& page : frames) {
		std::cout << "Página ID: " << page.getIndex() << std::endl;
		//std::cout << "Página ID: " << page.getData() << std::endl;
		// Imprimir otros atributos de la página según sea necesario
	}
	std::cout << "Contenido del buffer de cache:" << std::endl;
	for (const auto& page : Cache) {
		std::cout << "Página ID: " << page.getIndex() << std::endl;
		//std::cout << "Página ID: " << page.getData() << std::endl;
		// Imprimir otros atributos de la página según sea necesario
	}*/
}

void BufferPool::MRU(int n) {
	if (n >= 0 && n < numPaginas) {
		if (isIndexFrames(n)) {
			if (frames.size() == numFrames) {
				frames.pop_back();
			}
			std::vector<std::pair<int, int>> CabeceraVector = bloquesC.getCabeceraN(n);
			Page tmp(n, numBytes, bloquesC.getBytesOcupadosN(n), bloquesC.getDataN(n), CabeceraVector, bloquesC.getVectNumReg(n), bloquesC.getSizeSect(n), bloquesC.getDirtyBitSect(n));
			frames.push_back(tmp);
		}
		else {
			size_t it = 0;
			while (it < frames.size()) {
				if (frames[it].getIndex() == n) {
					std::rotate(frames.begin() + it, frames.begin() + it + 1, frames.end());
					break;
				}
				++it;
			}
		}
	}
	else {
		std::cout << "Página no encontrada" << std::endl;
		return;
	}
	/*std::cout << "Contenido del buffer de páginas:" << std::endl;
	for (const auto& page : frames) {
		std::cout << "Página ID: " << page.getIndex() << std::endl;
		//std::cout << "Página ID: " << page.getData() << std::endl;
		// Imprimir otros atributos de la página según sea necesario
	}
	std::cout << "Contenido del buffer de cache:" << std::endl;
	for (const auto& page : Cache) {
		std::cout << "Página ID: " << page.getIndex() << std::endl;
		//std::cout << "Página ID: " << page.getData() << std::endl;
		// Imprimir otros atributos de la página según sea necesario
	}*/
}

void BufferPool::CLOCK(int n) {
	int hand = 0;

	if (n >= 0 && n < numPaginas) {
		if (isIndexFrames(n)) { // no está en el frame
			/*std::cout << "Contenido del buffer de páginas:" << std::endl;
			for (const auto& page : frames) {
				if (page.getIndex() != -1) {
					std::cout << "Página ID: " << page.getIndex() << " Bit de referencia: " << page.getReferenceBit() << " Dirty Bit: " << page.getDirty() << " Pin Count: " << page.getPinCount() << std::endl;
				}
			}*/
			if (frames.size() == numFrames) { // se llenó frames
				bool allReferencesTrue = true;
				for (const auto& page : frames) {
					if (!page.getReferenceBit()) {
						allReferencesTrue = false;
						break;
					}
				}
				while (true) {
					if (!frames[hand].getReferenceBit()) { // buscar el primero que es 0
						if (frames[hand].getDirty() != 1) {
							int i = frames[hand].getIndex();
							std::vector<std::pair<int, int>> CabeceraVector = bloquesC.getCabeceraN(n);
							frames[hand] = Page(n, numBytes, bloquesC.getBytesOcupadosN(n), bloquesC.getDataN(n), CabeceraVector, bloquesC.getVectNumReg(n), bloquesC.getSizeSect(n), bloquesC.getDirtyBitSect(n));
							frames[hand].setReferenceBit(true);
							//std::cout << "Se Ingresa pagina: " << n << " quitando la Pagina " << i << std::endl;
							hand = (hand + 1) % frames.size();
							break;
						}
						else {
							//std::cout << "Le doy 1 a: " << frames[hand].getIndex() << std::endl;
							frames[hand].setReferenceBit(true);
							hand = (hand + 1) % frames.size();
						}
					}
					else {
						if (!allReferencesTrue) {
							hand = (hand + 1) % frames.size();
						}
						else {
							frames[hand].setReferenceBit(false);
							hand = (hand + 1) % frames.size();
						}
					}
				}
			}
			else {
				std::vector<std::pair<int, int>> CabeceraVector = bloquesC.getCabeceraN(n);
				Page tmp(n, numBytes, bloquesC.getBytesOcupadosN(n), bloquesC.getDataN(n), CabeceraVector, bloquesC.getVectNumReg(n), bloquesC.getSizeSect(n), bloquesC.getDirtyBitSect(n));
				tmp.setReferenceBit(true);
				frames.push_back(tmp); // Agregar la nueva página al final de los frames
				//std::cout << "Se Ingresa pagina: " << n << " sin quitar nada " << std::endl;
			}
		}
		else { // está en frame
			auto it = std::find_if(frames.begin(), frames.end(), [&](const Page& page) {
				return page.getIndex() == n;
				});
			if (it != frames.end()) {
				it->setPinCount();
				it->setReferenceBit(true);
			}
		}
	}
	/*
	std::cout << "Contenido del buffer de páginas:" << std::endl;
	for (const auto& page : frames) {
		if (page.getIndex() != -1) {
			std::cout << "Página ID: " << page.getIndex() << " Bit de referencia: " << page.getReferenceBit() << " Dirty Bit: " << page.getDirty() << " Pin Count: " << page.getPinCount() << std::endl;
		}
	}*/
}



void BufferPool::InsertPagetoBuffer(int n) {
	if (typeofBuffer == 1) {
		LRU(n);
	}
	else if (typeofBuffer == 2) {
		MRU(n);
	}
	else if (typeofBuffer == 3) {
		CLOCK(n);
	}
}

int BufferPool::getIndexFrames(int n) {
	int index = -1;
	for (int i = 0; i < frames.size(); i++) {
		if (frames[i].getIndex() == n) {
			index = i;
		}
	}
	return index;
}

BufferPool::BufferPool(int numPaginas, int numFrames, int numBytes, StackBlocks bloques, int typeofBuffers) : numFrames(numFrames), numPaginas(numPaginas), numBytes(numBytes) {
	bloquesC = bloques;
	typeofBuffer = typeofBuffers;
	/*for (int i = 0; i < numPaginas; i++) {
		std::vector<std::pair<int, int>> CabeceraVector = bloques.getCabeceraN(i);
		Page tmp = Page(i, numBytes, bloques.getBytesOcupadosN(i), bloques.getDataN(i), CabeceraVector, bloques.getVectNumReg(i));
		frames.push_back(tmp);
	}*/
}

BufferPool::~BufferPool() {}

void BufferPool::insertDataPage(char content, int nPage) {
	if (nPage >= 0 && nPage < numPaginas) {
		InsertPagetoBuffer(nPage);
		//int id = getIndexFromFrame(nPage);
		frames[getIndexFrames(nPage)].InsertData(content);
	}
	else {
		std::cerr << "Error: Pagina no encontrada." << nPage << std::endl;
	}
}

// Getter para el atributo 'pagina'
Page BufferPool::getPageAtIndex(int n) {
	if (n >= 0 && n < numPaginas) {
		InsertPagetoBuffer(n);
		return frames[getIndexFrames(n)];
	}
	else {
		std::cerr << "Error: Índice de página no válido: " << n << std::endl;
		// Devuelve una página inválida o lanza una excepción según tus necesidades
		return Page();
	}
}

int BufferPool::getBytesbyIndex(int n) {
	if (n >= 0 && n < numPaginas) {
		InsertPagetoBuffer(n);
		return frames[getIndexFrames(n)].getBytes();
	}
	else {
		std::cerr << "Error: Índice de página no válido: " << n << std::endl;
		// Devuelve una página inválida o lanza una excepción según tus necesidades
		return -1;
	}
}

int BufferPool::getBytesOcupadosbyIndex(int n) {
	if (n >= 0 && n < numPaginas) {
		InsertPagetoBuffer(n);
		return frames[getIndexFrames(n)].getBytesOcupados();
	}
	else {
		std::cerr << "Error: Índice de página no válido: " << n << std::endl;
		// Devuelve una página inválida o lanza una excepción según tus necesidades
		return -1;
	}
}

void BufferPool::insertCabeceraReg(int nPage, int numReg, int pos) {
	if (nPage >= 0 && nPage < numPaginas) {
		InsertPagetoBuffer(nPage);
		frames[getIndexFrames(nPage)].InsertCabecera2(numReg, pos);
	}
	else {
		std::cerr << "Error: Pagina no encontrada." << nPage << std::endl;
	}


}

// Getter para el atributo 'paginas'
const std::vector<Page>& BufferPool::getPages() const {
	return frames;
}

std::vector<Page>& BufferPool::gettingPages() {
	return frames;
}

std::vector<std::pair<int, int>> BufferPool::getCabeceraN(int n) {
	InsertPagetoBuffer(n);
	return frames[getIndexFrames(n)].getCabecera();
}

std::vector<int> BufferPool::getRegPartN(int n) {
	InsertPagetoBuffer(n);
	return frames[getIndexFrames(n)].getRegPart();
}

std::vector<int> BufferPool::getSizeSectN(int n) {
	InsertPagetoBuffer(n);
	return frames[getIndexFrames(n)].getSizeSect();
}
std::vector <int> BufferPool::getDirtyBitSectN(int n) {
	InsertPagetoBuffer(n);
	return frames[getIndexFrames(n)].getDirtySect();
}

std::string BufferPool::getDataN(int n) {
	frames[n].unsetDirty();
	frames[n].setPinCount();
	return frames[n].getData();
}

int BufferPool::getNumRegTotal(int n) {
	InsertPagetoBuffer(n);
	return frames[getIndexFrames(n)].getNumregT();
}

// Getter para el atributo 'numPaginas'
int BufferPool::getNumPages() const {
	return numPaginas;
}

int BufferPool::getNumRegTotalY(int n) {//Ya cargado n en frames
	return frames[getIndexFrames(n)].getNumregT();
}

std::string BufferPool::getDataNY(int n) {//Ya cargado n en frames
	return frames[getIndexFrames(n)].getData();
}

void BufferPool::showCabecera(int n) {
	const std::vector<std::pair<int, int>> data = frames[getIndexFrames(n)].getCabecera();
	for (const auto& par : data) {
		std::cout << par.first << "," << par.second << "#";
	}
	std::cout << std::endl;
}

void BufferPool::showPage(int n) {
	InsertPagetoBuffer(n);
	std::cout << "Pagina " << n << ": " << std::endl;
	std::cout << "Bytes Total: " << frames[getIndexFrames(n)].getBytes() << std::endl;
	std::cout << "Bytes Ocupados: " << frames[getIndexFrames(n)].getBytesOcupados() << std::endl;
	std::cout << "Bytes Libres: " << frames[getIndexFrames(n)].getBytes() - frames[getIndexFrames(n)].getBytesOcupados() << std::endl;
	std::cout << "Dirty Bit: " << frames[getIndexFrames(n)].getDirty() << std::endl;
	std::cout << "PinCount: " << frames[getIndexFrames(n)].getPinCount() << std::endl;
	std::cout << "Cabecera: " << std::endl;
	showCabecera(n);
	std::cout << "Cantidad de Registros: " << std::endl;
	std::cout << getNumRegTotalY(n) << std::endl;
	std::cout << "Registros: " << std::endl;
	std::cout << getDataNY(n) << std::endl;
}

void BufferPool::ADDReg(int numReg, int pos, std::string registros_conca, int bytes_regis, int inde, int idPart) {
	InsertPagetoBuffer(inde);
	int a = getIndexFrames(inde);
	frames[a].setDirty();
	frames[a].Insert(numReg, pos, registros_conca, bytes_regis, idPart);
}

void BufferPool::ADDRegVar(int numReg, int pos, std::string registros_conca, int bytes_regis, int inde, int idPart) {
	InsertPagetoBuffer(inde);
	int a = getIndexFrames(inde);
	frames[a].setDirty();
	frames[a].InsertVar(numReg, pos, registros_conca, bytes_regis, idPart);
}

void BufferPool::ADDFREESPACE(int pos, int inde) {
	InsertPagetoBuffer(inde);
	int a = getIndexFrames(inde);
	frames[a].setDirty();
	frames[a].InsertVarFE(pos);
}

int BufferPool::ValidateVariable() {
	int numPag = 1;
	InsertPagetoBuffer(0);
	int j = frames[getIndexFrames(0)].getFirstRegs();
	if (j == 0) {
		numPag = 0;
	}
	return numPag;
}

int BufferPool::freeSpaceMappingVar(int MAXREG) {
	int numPag = -1;
	for (int i = 0; i < numPaginas; i++) {
		//std::cout << "Evaluando Página: " << i <<" - "<<MAXREG << std::endl;
		InsertPagetoBuffer(i);
		int j = frames[getIndexFrames(i)].getSpacePageVar(MAXREG);
		//std::cout << "  Espacio Libre en la Página " << i << ": " << j << std::endl;
		if (j > 0) {
			numPag = i;
			break;
		}
	}
	if (numPag == -1) {

	}
	return numPag;
}


int BufferPool::freeSpaceMapping() {
	int numPag = -1;
	int val = 0;
	for (int i = 0; i < numPaginas; i++) {
		InsertPagetoBuffer(i);
		int j = frames[getIndexFrames(i)].getSpacePage();
		if (j > val) {
			numPag = i;
		}
	}
	return numPag;
}

int BufferPool::Page_complete() {
	int numPag = -1;
	int val = 0;
	for (int i = 0; i < numPaginas; i++) {
		InsertPagetoBuffer(i);
		int j = frames[getIndexFrames(i)].getSpacePage();
		if (j > val) {
			numPag = i;
		}
	}
	return numPag;
}

void BufferPool::FillBlankReg(std::string& cadena, int nPosicion) {//eliminar registro de cadena llenandolo por espacio vacio
	size_t inicio = nPosicion;
	size_t fin = cadena.find("|", inicio + 1);
	//std::cout << "reemplazar pos: " << inicio << "-" << fin << std::endl;

	if (inicio != std::string::npos && fin != std::string::npos) {
		cadena.replace(inicio, fin - inicio, fin - inicio, ' ');
	}
}

int BufferPool::buscarNumeroEnPrimerPar(const std::vector<std::pair<int, int>>& vectorPares, int numero) {//retorna pos del n registro
	auto it = std::find_if(vectorPares.begin(), vectorPares.end(), [numero](const std::pair<int, int>& par) {
		return par.first == numero;
		});

	if (it != vectorPares.end()) {
		return it->second;
	}
	return -1;
}

int BufferPool::ubicar_ultimo_registro(std::vector<std::pair<int, int>>& vectorPares) {
	if (!vectorPares.empty()) {
		int ultimo_registro = vectorPares.back().first;
		return ultimo_registro;
	}
	else {
		std::cout << "El vector está vacío." << std::endl;
	}
}

void BufferPool::ChangeCabecera(std::vector<std::pair<int, int>>& vectorPares, int valor, int Nvalor) {//se cambia a nValor
	auto it = std::find_if(vectorPares.begin(), vectorPares.end(), [valor](std::pair<int, int>& par) {
		return par.second == valor;
		});

	if (it != vectorPares.end()) {
		it->first = Nvalor;
	}
}

void BufferPool::Add_Cabecera(std::vector<std::pair<int, int>>& vectorPares, int registro, int posicion_inicio) {
	vectorPares.push_back(std::make_pair(registro, posicion_inicio));
}

void BufferPool::Add_CabeceraVAR(std::vector<std::pair<int, int>>& vectorPares, int registro, int posicion_inicio, int tam) {
	vectorPares.push_back(std::make_pair(registro, posicion_inicio));
	vectorPares[0].second -= tam;
}

void BufferPool::Add_CabeceraVAR2(std::vector<std::pair<int, int>>& vectorPares, int registro, int posicion_inicio) {
	vectorPares.push_back(std::make_pair(registro, posicion_inicio));
}

void BufferPool::AddFreeSpace(std::vector<std::pair<int, int>>& vectorPares, int pos, int numReg, int pos2) {
	vectorPares.push_back(std::make_pair(0, pos));
	vectorPares.push_back(std::make_pair(numReg, pos2));
}

void BufferPool::MovPOSRegsCab(std::vector<std::pair<int, int>>& vectorPares, std::string cadena2, int MAX, int posRegCab) {//se cambia a nValor
	std::string cadena = cadena2;
	if (posRegCab >= 0 && posRegCab < vectorPares.size()) {
		vectorPares.erase(vectorPares.begin() + posRegCab); // Eliminar elemento en la posición M
	}
	int i = 1;
	while (!cadena.empty()) {
		// Encontrar la última aparición de '|'
		size_t ultimaBarra = cadena.find_last_of('|');

		// Extraer la subcadena desde la última '|' hasta el final de la cadena
		std::string subcadena = cadena.substr(ultimaBarra + 1);
		if (subcadena.size() != 0) {        // Imprimir la subcadena
			subcadena += "|";
			MAX -= subcadena.size();
			vectorPares[i].second = MAX;
			i++;
			if (i == vectorPares.size())break;
		}
		// Borrar la subcadena procesada de la cadena original
		cadena.erase(ultimaBarra);
	}
	vectorPares[0].second = (MAX - 1);
}
int obtenerLongitudYBorrar(std::string& cadena, int N) {
	// Buscar la posición del próximo '|'
	size_t posBarra = cadena.find('|', N-1);

	// Verificar si se encontró una '|'
	if (posBarra != std::string::npos) {
		// Obtener la subcadena desde N hasta la posición de la '|'
		std::string subcadena = cadena.substr(N-1, posBarra - N+1);
		std::cout << subcadena << std::endl;
		// Calcular la longitud de la subcadena
		int longitud = subcadena.length()+1;

		// Borrar la subcadena del string original
		cadena.erase(N-1, posBarra - N + 2);

		return longitud;
	}

	// Si no se encuentra una '|', retornar -1 para indicar error
	return -1;
}

bool BufferPool::EliminarReg(int numRegE) {
	for (int i = 0; i < numPaginas; i++) {
		InsertPagetoBuffer(i);
		int b = getIndexFrames(i);
		if (frames[b].getCabecera()[0].first == 0) {
			int val = buscarNumeroEnPrimerPar(frames[b].getCabecera(), numRegE);
			if (val != -1) {
				int sumd = frames[b].getCabecera()[0].second;
				int u = obtenerLongitudYBorrar(frames[b].getDataR(), val- sumd );
				int posReg = -1;
				int yu = -1;
				int ik;
				for (ik = 0; ik < frames[b].getCabecera().size(); ++ik) {
					if (frames[b].getCabecera()[ik].first == numRegE) {
						posReg = ik;
						yu = ik;
						ik = frames[b].getCabecera().size();
					}
				}
				MovPOSRegsCab(frames[b].getCabeceraR(), frames[b].getData(), frames[b].getBytes(), posReg);
				frames[b].setDirty();
				frames[b].DirtyBitSects();
				frames[b].Eliminar(ik);
				//frames[i].UpdateBytesOcupados();
				return true;
			}
		}
		else {
			int val = buscarNumeroEnPrimerPar(frames[b].getCabecera(), numRegE);
			//std::cout << "Elminar: " << val << std::endl;
			if (val != -1) {
				ChangeCabecera(frames[b].getCabeceraR(), val, -1);
				FillBlankReg(frames[b].getDataR(), val);
				frames[b].setDirty();
				//frames[i].UpdateBytesOcupados();
				return false;
			}
		}
	}
	std::cout << "El registro " << numRegE << " no existe " << std::endl;
}

bool BufferPool::FindReginCabecera(const std::vector<std::pair<int, int>>& vectorPares, int n) {
	return std::any_of(vectorPares.begin(), vectorPares.end(), [n](const std::pair<int, int>& par) {
		return par.first == n;
		});
}

int BufferPool::MaxNumReg() {
	int maxValue = std::numeric_limits<int>::min();
	for (int i = 0; i < numPaginas; i++) {
		InsertPagetoBuffer(i);
		for (const auto& par : frames[getIndexFrames(i)].getCabecera()) {
			maxValue = std::max(maxValue, par.first);
		}
	}
	return maxValue;
}

void BufferPool::InsertRegistro(std::string& data, std::string reg, int pos) {
	data.replace(pos, reg.length(), reg);

}

void BufferPool::InsertarRegistroUltimo(std::string& data, std::string reg, int pos) {
	//std::cout << "POSICION: " << pos << std::endl;
	data.replace(pos, reg.length(), reg);
	data += "|";

}

void BufferPool::InsertarRegistroUltimoVAR(std::string& data, std::string reg) {
	//std::cout << "POSICION: " << pos << std::endl;
	data = reg + data;

}

void BufferPool::InsertarRegistroUltimo2(std::string& data, std::string reg) {
	size_t lastPipePos = data.find_last_of("|");
	if (lastPipePos != std::string::npos) {
		data = data.substr(0, lastPipePos + 1) + reg;
	}
	else {
		data += reg;
	}

}

int BufferPool::SelectlastPageFill() {
	int id = -1; //
	for (int i = 0; i < numPaginas; i++) {
		InsertPagetoBuffer(i);
		if (frames[getIndexFrames(i)].getData().empty()) {
			id = i - 1;
			break;
		}
		else if (i == numPaginas - 1) {
			id = i;
		}
	}
	return id;
}

int BufferPool::obtenerUltimoIndiceNoCero(const std::vector<int>& vec) {
	for (int i = vec.size() - 1; i >= 0; --i) {
		if (vec[i] != 0) {
			return i;
		}
	}
	return 0; // Si todos los elementos son cero, retorna -1
}

int BufferPool::obtnerBytesParte(int n, const std::string& data, const std::vector <int>& numRegPart, const std::vector<std::pair<int, int>>& cabecera) {
	// Verificar si el índice n es válido
	int longitud = data.length();
	if (data.empty()) {
		return 0;
	}
	else {
		if (n < 0 || n >= numRegPart.size()) {
			std::cout << "El índice n es inválido." << std::endl;
			return -1;
		}

		// Obtener la posición inicial (I) y final (F) de la cabecera
		int posInicial = 0;
		int posFinal = 0;

		for (int i = 0; i <= n; ++i) {
			if (i == n) {
				posInicial = posFinal;
			}
			posFinal += numRegPart[i];
		}

		// Verificar si las posiciones son válidas
		if (posInicial < 0 || posInicial >= cabecera.size() || posFinal < 0 || posFinal > cabecera.size()) {
			std::cout << "Las posiciones de la cabecera son inválidas." << std::endl;
			return -1;
		}

		// Obtener el rango en data y obtener el tamaño
		int inicio = cabecera[posInicial].second;
		int fin = cabecera[posFinal - 1].second;

		while (fin < longitud && data[fin] != '|') {
			fin++;
		}
		fin++;
		//std::cout << "Las posiciones son: " << inicio << " - " << fin << std::endl;
		std::string parteData = data.substr(inicio, fin - inicio);
		int tamano = parteData.length();
		int tamanoCabecera = 0;
		for (int i = posInicial; i < posFinal; ++i) {
			std::string segundoPar = std::to_string(cabecera[i].second);
			tamanoCabecera += segundoPar.length() + 6;  // Sumar 6 por la coma los primeros 4 bytes del primer par y 2 por # y ,
		}

		tamano += tamanoCabecera + 2;
		return tamano;
	}

}

void BufferPool::Changestring(std::string& cadena, int nPosicion, std::string reg) {//Poner un reg dentro de la cadena segun pos
	size_t inicio = nPosicion;
	size_t fin = cadena.find("|", inicio + 1);
	//std::cout << "reemplazar pos: " << inicio << "-" << fin << std::endl;

	if (inicio != std::string::npos && fin != std::string::npos) {
		cadena.replace(inicio, fin - inicio, reg);
	}
}

void BufferPool::MoverRegs(int n1, int n2) {
	int idPag1 = -1, idPag2 = -1;
	for (int i = 0; i < numPaginas; i++) {
		InsertPagetoBuffer(i);
		if (FindReginCabecera(frames[getIndexFrames(i)].getCabecera(), n1)) {
			idPag1 = i;
		}
		if (FindReginCabecera(frames[getIndexFrames(i)].getCabecera(), n2)) {
			idPag2 = i;
		}
	}
	if (idPag1 != -1 && idPag2 != -1) {
		std::string extracted1, extracted2;
		InsertPagetoBuffer(idPag1);
		int a = getIndexFrames(idPag1);
		frames[a].setDirty();
		int pos1 = buscarNumeroEnPrimerPar(frames[a].getCabecera(), n1);
		std::string data1 = frames[getIndexFrames(idPag1)].getData();
		std::size_t startPos1 = pos1; // Posición inicial para la búsqueda
		std::size_t endPos1 = data1.find('|', startPos1); // Posición donde se encuentra el carácter '|' después de startPos

		if (endPos1 != std::string::npos) {
			extracted1 = data1.substr(startPos1, endPos1 - startPos1);
		}
		InsertPagetoBuffer(idPag2);
		int b = getIndexFrames(idPag2);
		frames[b].setDirty();
		int pos2 = buscarNumeroEnPrimerPar(frames[getIndexFrames(idPag2)].getCabecera(), n2);
		std::string data2 = frames[getIndexFrames(idPag2)].getData();
		std::size_t startPos2 = pos2; // Posición inicial para la búsqueda
		std::size_t endPos2 = data1.find('|', startPos2); // Posición donde se encuentra el carácter '|' después de startPos
		if (endPos2 != std::string::npos) {
			extracted2 = data2.substr(startPos2, endPos2 - startPos2);
		}
		Changestring(frames[getIndexFrames(idPag2)].getDataR(), pos2, extracted1);
		ChangeCabecera(frames[getIndexFrames(idPag2)].getCabeceraR(), pos2, n1);
		InsertPagetoBuffer(idPag1);
		Changestring(frames[getIndexFrames(idPag1)].getDataR(), pos1, extracted2);
		ChangeCabecera(frames[getIndexFrames(idPag1)].getCabeceraR(), pos1, n2);
	}
	else {
		if (idPag1 == -1) {
			std::cout << "Registro " << n1 << " no se encuentra en la tabla." << std::endl;
		}
		if (idPag2 == -2) {
			std::cout << "Registro " << n2 << " no se encuentra en la tabla." << std::endl;
		}
	}

}

void BufferPool::BuscarRegs(int n1) {
	int idPag1 = -1;
	for (int i = 0; i < numPaginas; i++) {
		InsertPagetoBuffer(i);
		if (FindReginCabecera(frames[getIndexFrames(i)].getCabecera(), n1)) {
			idPag1 = i;
		}


	}
	//std::cout << idPag1;
	if (idPag1 != -1) {
		std::string extracted1;
		InsertPagetoBuffer(idPag1);
		int pos1 = buscarNumeroEnPrimerPar(frames[getIndexFrames(idPag1)].getCabecera(), n1);
		if (frames[getIndexFrames(idPag1)].getCabecera()[0].first == 0) {
			pos1 = pos1 - frames[getIndexFrames(idPag1)].getCabecera()[0].second - 1;
		}

		std::string data1 = frames[getIndexFrames(idPag1)].getData();
		std::size_t startPos1 = pos1; // Posición inicial para la búsqueda
		std::size_t endPos1 = data1.find('|', startPos1); // Posición donde se encuentra el carácter '|' después de startPos
		if (endPos1 != std::string::npos) {
			extracted1 = data1.substr(startPos1, endPos1 - startPos1);
		}
		//std::cout << "Posicion Inicio: " << startPos1 << std::endl;
		//std::cout << "Posicion Final: " << endPos1 << std::endl;
		std::cout << "Registro se encuentra en la pagina: " << idPag1 << std::endl;

		std::cout << extracted1 << std::endl;
		//std::cout << "Registro se encuentra en el sector: " << frames[idPag1]. << std::endl;
	}
	else {
		if (idPag1 == -1) {
			std::cout << "Registro " << n1 << " no se encuentra en la tabla." << std::endl;
		}
	}/* if (frames.back().getCabecera()[0].first==0) {
			pos1 = pos1 - frames.back().getCabecera()[0].second-1;
		}
		*/
}

void BufferPool::AddNewRegVar(int MAXREG, int numRegN, std::string registro, bool check, B_plus_tree* Arbol) {
	int idPag = freeSpaceMappingVar(MAXREG); //ENCONTRAR LA PAGINA QUE TIENE ESPACIO PARA INSERTAR
	if (check) {
		for (int i = 0; i < numPaginas; i++) {
			InsertPagetoBuffer(i);
			if (FindReginCabecera(frames[getIndexFrames(i)].getCabecera(), numRegN)) {
				numRegN = MaxNumReg() + 1;
				break;
			}
		}
	}
	//std::cout << "ID Pagina: " << idPag << std::endl;
	/*std::cout << "ID Registro: " << numRegN << std::endl;//idkey
	std::cout << "Registro: " << registro << std::endl;//registro|*/
	if (idPag == -1) {
		std::cout << "El Disco ya no acepta más registros" << std::endl;
		return;
	}
	else {
		int idSect = 0;
		InsertPagetoBuffer(idPag);
		for (int i = 0; i < frames[getIndexFrames(idPag)].getRegPart().size(); i++) {
			if (frames[getIndexFrames(idPag)].getRegPart()[i] != 0) {
				idSect = i;
			}
		}
		//std::cout << "Parte Sect " << idSect << std::endl;
		if (idSect == 0) {
			int cantRegs = frames[getIndexFrames(idPag)].getRegPart()[idSect];
			//std::cout << "-1 " << idSect << std::endl;
			if (cantRegs == 0) {//Pagina Vacia
				//std::cout << "--1 " << idSect << std::endl;
				int spaceMAX = frames[getIndexFrames(idPag)].getPartSize();//bytes de un sector maximo
				int bytes = frames[getIndexFrames(idPag)].getBytes();//bytes totales de pagian maximo

				frames[getIndexFrames(idPag)].getDataR() = registro + frames[getIndexFrames(idPag)].getDataR();
				frames[getIndexFrames(idPag)].getSizeSectR()[idSect] += (registro.size() + 2 + (8 + std::to_string(bytes).size()));

				int newpos = bytes - registro.size();
				int newposFree = newpos - 1;
				frames[getIndexFrames(idPag)].getCabeceraR().push_back(std::make_pair(0, newposFree));//Insertamos o creamos free space cab
				frames[getIndexFrames(idPag)].getCabeceraR().push_back(std::make_pair(numRegN, newpos));//Insertamos registro cab
				frames[getIndexFrames(idPag)].getCabeceraR()[0].second = newposFree;
				frames[getIndexFrames(idPag)].changeDirtyBitSect(idSect);
				frames[getIndexFrames(idPag)].setDirty();
				frames[getIndexFrames(idPag)].getBytesOcupadosR() += (registro.size() + (8 + std::to_string(bytes).size()) + 2 /*Del salto de linea*/);//espacio de reg
				frames[getIndexFrames(idPag)].getBytesOcupadosR() += (3 + (std::to_string(bytes).size()));//espacio de cab de free space 0,posicion#
				frames[getIndexFrames(idPag)].getRegPartR()[idSect] += 1;//aumentamos uno al sector de cantidad de registros por sector
				Arbol->insertar(numRegN, idPag);
				//showPage(idPag);
			}
			else {//primer sector pero con unos cuantos datos
				//std::cout << "--2 " << idSect << std::endl;
				const std::vector<std::pair<int, int>>& cabecera = frames[getIndexFrames(idPag)].getCabecera();
				int spaceMAX = frames[getIndexFrames(idPag)].getPartSize();
				int bytes = frames[getIndexFrames(idPag)].getBytes();
				std::vector<int> spaceSect= frames[getIndexFrames(idPag)].getSizeSect();
				if ((spaceSect[idSect] + registro.size() + (8 + std::to_string(bytes).size())/*000000,0000#*/) <= spaceMAX) {//entra en el primer sector
					//std::cout << "---1 " << idSect << std::endl;
					frames[getIndexFrames(idPag)].getDataR() = registro + frames[getIndexFrames(idPag)].getDataR();
					frames[getIndexFrames(idPag)].getSizeSectR()[idSect] += (registro.size() + (8 + std::to_string(bytes).size()));
					int ultimoValor = cabecera.back().second;
					int newpos = ultimoValor - registro.size();
					int newposFree = newpos - 1;
					frames[getIndexFrames(idPag)].getCabeceraR().push_back(std::make_pair(numRegN, newpos));
					frames[getIndexFrames(idPag)].getCabeceraR()[0].second = newposFree;
					frames[getIndexFrames(idPag)].changeDirtyBitSect(idSect);
					frames[getIndexFrames(idPag)].setDirty();
					frames[getIndexFrames(idPag)].getBytesOcupadosR() += (registro.size() + (8 + std::to_string(bytes).size()));
					frames[getIndexFrames(idPag)].getRegPartR()[idSect] += 1;
					Arbol->insertar(numRegN, idPag);
				}
				else {//no entra en el primer sectr
					//std::cout << "---2 " << idSect << std::endl;
					if ((idSect + 1) < spaceSect.size()) {//Si el sector siguiente supera el limite de sectores por pag
						//std::cout << "----1 " << idSect << std::endl;
						idSect++;
						frames[getIndexFrames(idPag)].getDataR() = registro + frames[getIndexFrames(idPag)].getDataR();
						frames[getIndexFrames(idPag)].getSizeSectR()[idSect] += (registro.size() + 2 + (8 + std::to_string(bytes).size()));//salto de linea
						int ultimoValor = cabecera.back().second;
						int newpos = ultimoValor - registro.size();
						int newposFree = newpos - 1;
						frames[getIndexFrames(idPag)].getCabeceraR().push_back(std::make_pair(numRegN, newpos));
						frames[getIndexFrames(idPag)].getCabeceraR()[0].second = newposFree;
						frames[getIndexFrames(idPag)].changeDirtyBitSect(idSect);
						frames[getIndexFrames(idPag)].setDirty();
						frames[getIndexFrames(idPag)].getBytesOcupadosR() += (registro.size() + 2 + (8 + std::to_string(bytes).size()));//salto de linea
						frames[getIndexFrames(idPag)].getRegPartR()[idSect] += 1;
						Arbol->insertar(numRegN, idPag);
					}
				}


			}
		}
		else {//segundo a mas sectores
			//std::cout << "-2 " << idSect << std::endl;
			const std::vector<std::pair<int, int>>& cabecera = frames[getIndexFrames(idPag)].getCabecera();
			int spaceMAX = frames[getIndexFrames(idPag)].getPartSize();//bytes de un sector maximo
			int bytes = frames[getIndexFrames(idPag)].getBytes();//bytes totales de pagian maximo

			std::vector<int> spaceSect = frames[getIndexFrames(idPag)].getSizeSect();
			int indn = 0;
			int plus=0;
			while (true) {
				if ((spaceSect[idSect] + registro.size() + (8 + std::to_string(bytes).size())/*0000,0000#*/) <= spaceMAX) {//si el sector X tiene espacio
					//std::cout << "--1 " << idSect << std::endl;
					frames[getIndexFrames(idPag)].getDataR() = registro + frames[getIndexFrames(idPag)].getDataR();
					frames[getIndexFrames(idPag)].getSizeSectR()[idSect] += (registro.size() + plus + (8 + std::to_string(bytes).size()));
					int ultimoValor = cabecera.back().second;
					int newpos = ultimoValor - registro.size();
					int newposFree = newpos - 1;
					frames[getIndexFrames(idPag)].getCabeceraR().push_back(std::make_pair(numRegN, newpos));
					frames[getIndexFrames(idPag)].getCabeceraR()[0].second = newposFree;
					frames[getIndexFrames(idPag)].changeDirtyBitSect(idSect);
					frames[getIndexFrames(idPag)].setDirty();
					frames[getIndexFrames(idPag)].getBytesOcupadosR() += (registro.size() + plus + (8 + std::to_string(bytes).size()));
					frames[getIndexFrames(idPag)].getRegPartR()[idSect] += 1;
					Arbol->insertar(numRegN, idPag);
					break;
				}
				else {
					if ((idSect + 1) < spaceSect.size()) {
						idSect++;
						plus = 2;
					}
					else {
						plus = (static_cast<unsigned long long>(3) + 2 + (std::to_string(bytes).size()));
					}
				}
			}

		}
	}
}

void BufferPool::AddNewRegFija(int numRegN, std::string registro) {
	int idPag = freeSpaceMapping(); //ENCONTRAR LA PAGINA QUE TIENE ESPACIO PARA INSERTAR
	for (int i = 0; i < numPaginas; i++) {
		InsertPagetoBuffer(i);
		if (FindReginCabecera(frames[getIndexFrames(i)].getCabecera(), numRegN)) {
			numRegN = MaxNumReg() + 1;
			break;
		}
	}
	if (idPag != -1) { // SI HAY ESPACIO LIBRE DENTRO DE LA TABLA
		InsertPagetoBuffer(idPag);
		int val = buscarNumeroEnPrimerPar(frames[getIndexFrames(idPag)].getCabecera(), -1); // RETORNAR LA POSICION DE  REGISTRO LIBRE
		ChangeCabecera(frames[getIndexFrames(idPag)].getCabeceraR(), val, numRegN); // MODIFICAR CABECERA
		InsertRegistro(frames[getIndexFrames(idPag)].getDataR(), registro, val); //INSERTA EL REGISTRO
		//frames[idPag].UpdateBytesOcupados(); // ACTUALIZA LA CAPACIDAD DE BYTES DE LA PAGINA
	}
	else { // NO HAY ESPACIO LIBRE
		idPag = SelectlastPageFill(); // Ultima p[agina llena o completada  // 16
		if (idPag == -1) {
			std::cout << "El Disco ya no puede aceptar más registros con esa longitud" << std::endl;
		}
		else {
			int BytesOcupadostmp = 0;
			InsertPagetoBuffer(idPag);
			const std::vector<std::pair<int, int>>& cabecera = frames[getIndexFrames(idPag)].getCabecera();



			//int num_new_regis = ubicar_ultimo_registro(frames[idPag].getCabeceraR()) + 1;
			int num_new_regis = numRegN;
			int posNEW = 0;
			size_t lastPipePos = frames[getIndexFrames(idPag)].getData().find_last_of("|");
			if (lastPipePos != std::string::npos) {
				posNEW = static_cast<int>(lastPipePos) + 1;
			}
			else {
				std::cerr << "----Error----" << std::endl;
			}
			std::string CabezaTMP = std::to_string(num_new_regis) + "," + std::to_string(posNEW) + "#";
			//std::cout << "CabezaTMP:" << CabezaTMP << std::endl;
			BytesOcupadostmp = CabezaTMP.length() + registro.length();
			//std::cout << "Bytes Ocupados: " << BytesOcupadostmp;
			//std::cout << "PAGINA :" << idPag << std::endl;
			int bytesxPart = frames[getIndexFrames(idPag)].getPartSize();
			//Saber cual es el ultimo valor agregado en el vector int partes, es decir si hay 23,0,0 debe decir que es el parte 1. si hay 23,12,3 debe ser la parte 3
			//despues calcular cuantos bytesocupados tiene esa parte, luego saber que bytesxPart es el limite de un sector y saber si entra en el sector, en caso 
			//contraro pasarlo a otro sector o sino a la siguiente pagina. al agregar a uno debe sumar en el vector con esa parte +1 del registro.

			/*int contador = std::count_if(firstLine.begin(), firstLine.end(), std::bind(std::equal_to<char>(), std::placeholders::_1, '#'));
			numRegistosPart[i - 1] = contador;*/
			int idParte = obtenerUltimoIndiceNoCero(frames[getIndexFrames(idPag)].getRegPart());
			int bytesOcupadosxPart = obtnerBytesParte(idParte, frames[getIndexFrames(idPag)].getData(), frames[getIndexFrames(idPag)].getRegPart(), frames[getIndexFrames(idPag)].getCabecera());
			//std::cout << "Parte Sect de Pagina:" << idParte << std::endl;
			//std::cout << "bytesocupados:" << bytesOcupadosxPart << std::endl;

			if ((bytesOcupadosxPart + BytesOcupadostmp) <= bytesxPart) {
				int b = getIndexFrames(idPag);
				frames[b].setDirty();
				Add_Cabecera(frames[b].getCabeceraR(), num_new_regis, posNEW);
				InsertarRegistroUltimo(frames[b].getDataR(), registro, posNEW);
				frames[b].getRegPartR()[idParte] = frames[b].getRegPartR()[idParte] + 1;
			}
			else {
				if (idParte + 1 == frames[getIndexFrames(idPag)].getRegPart().size()) {
					idPag++;
					if (idPag == numPaginas) {
						std::cout << "El Disco ya no acepta más registros" << std::endl;
						return;
					}
					InsertPagetoBuffer(idPag);
					idParte = 0;
					int b = getIndexFrames(idPag);
					frames[b].setDirty();
					Add_Cabecera(frames[b].getCabeceraR(), num_new_regis, 0);
					InsertarRegistroUltimo(frames[b].getDataR(), registro, 0);
					frames[b].getRegPartR()[idParte] = frames[b].getRegPartR()[idParte] + 1;
				}
				else {
					idParte++;
					int b = getIndexFrames(idPag);
					frames[b].setDirty();
					Add_Cabecera(frames[b].getCabeceraR(), num_new_regis, posNEW);
					InsertarRegistroUltimo(frames[b].getDataR(), registro, posNEW);
					frames[b].getRegPartR()[idParte] = frames[getIndexFrames(idPag)].getRegPartR()[idParte] + 1;
				}
			}
		}
	}
}

void BufferPool::showReg(int idKey, int idPag, const std::vector<int>& values, const std::vector<std::string>& types) {
	InsertPagetoBuffer(idPag);
	int idFrames = getIndexFrames(idPag);
	const std::vector<std::pair<int, int>>& cabecera = frames[idFrames].getCabecera();
	const std::string& cadena = frames[idFrames].getData();
	int freespace = -1;
	int posReg = -1;
	for (const auto& p : cabecera) {
		if (p.first == 0) {
			freespace = p.second;
		}
		if (p.first == idKey) {
			posReg = p.second;
			break;
		}
	}
	std::string reg;
	char caracter = '|';
	size_t posicion = posReg - freespace - 1;
	if (size_t siguiente_posicion = cadena.find(caracter, posicion); siguiente_posicion != std::string::npos) {
		reg = cadena.substr(posicion, siguiente_posicion - posicion);
	}
	else {
		std::cout << "No se pudo extraer el Registro" << std::endl;
	}
	//std::cout << reg << std::endl;
	// Extraer valores hasta el primer '#'
	size_t posicion_hashtag = reg.find('#');
	std::string subcadena1 = reg.substr(0, posicion_hashtag);

	// Crear vector de pares int a partir de la subcadena1
	std::vector<std::pair<int, int>> valores_pair;
	size_t posicion_coma = 0;
	while (posicion_coma != std::string::npos) {
		size_t posicion_slash = subcadena1.find('/', posicion_coma);
		if (posicion_slash != std::string::npos) {
			std::string valor = subcadena1.substr(posicion_coma, posicion_slash - posicion_coma);
			size_t posicion_coma_valor = valor.find(',');
			int primero = std::stoi(valor.substr(0, posicion_coma_valor));
			int segundo = std::stoi(valor.substr(posicion_coma_valor + 1));
			valores_pair.push_back(std::make_pair(primero, segundo));
			posicion_coma = posicion_slash + 1;
		}
		else {
			break;
		}
	}

	// Extraer valores después del primer '#' hasta el segundo '#'
	size_t posicion_hashtag2 = reg.find('#', posicion_hashtag + 1);
	std::string subcadena2 = reg.substr(posicion_hashtag + 1, posicion_hashtag2 - posicion_hashtag - 1);

	// Crear vector de int a partir de la subcadena2
	std::vector<int> valores_int;
	size_t posicion_coma2 = 0;
	while (posicion_coma2 != std::string::npos) {
		size_t posicion_coma_valor = subcadena2.find(',', posicion_coma2);
		if (posicion_coma_valor != std::string::npos) {
			std::string valor = subcadena2.substr(posicion_coma2, posicion_coma_valor - posicion_coma2);
			valores_int.push_back(std::stoi(valor));
			posicion_coma2 = posicion_coma_valor + 1;
		}
		else {
			break;
		}
	}
	// Extraer valores después del primer '#' hasta el segundo '#'
	size_t posicion_hashtag3 = reg.find('#', posicion_hashtag2 + 1);
	std::string subcadena3 = reg.substr(posicion_hashtag2 + 1, posicion_hashtag3 - posicion_hashtag2 - 1);
	int freemap = std::stoi(subcadena3);
	std::bitset<sizeof(int) * 8> bits(freemap);
	std::string binario = bits.to_string().substr(sizeof(int) * 8 - values.size());
	size_t posicion_hashtag4 = reg.find('#', posicion_hashtag3 + 1);
	std::string subcadena5 = reg.substr(posicion_hashtag3 + 1, posicion_hashtag4 - posicion_hashtag3 - 1);
	//std::cout << subcadena5 << std::endl;
	int resta = 0;
	if (reg.size() - subcadena5.size() != valores_pair[0].first) {
		//std::cout << "SI" << std::endl;
		if (valores_pair[0].first < (reg.size() - subcadena5.size())) {
			resta = (-1) * ((reg.size() - subcadena5.size()) - valores_pair[0].first);
		}
		else {
			resta = (valores_pair[0].first - (reg.size() - subcadena5.size()));
		}
		
	}
	int indiceTypes = 0;
	int indiceSTR = 0;
	int indiceINT = 0;
	for (char c : binario) {
		if(c== '1'){
			if (types[indiceTypes] == "STR") {
				int pos = valores_pair[indiceSTR].first;
				int tam = valores_pair[indiceSTR].second;
				std::string subcadena4 = reg.substr(pos - resta, tam);
				std::cout << subcadena4 ;
				if (subcadena4.size() < values[indiceTypes]) {
					for (int j = 0; j != values[indiceTypes] - subcadena4.size(); j++) {
						std::cout << " ";
					}
				}
				std::cout << "|";
				indiceTypes++;
				indiceSTR++;
			}
			else {
				std::cout << valores_int[indiceINT];
				if (std::to_string(valores_int[indiceINT]).size() < values[indiceTypes]) {
					for (int j = 0; j != values[indiceTypes] - std::to_string(valores_int[indiceINT]).size(); j++) {
						std::cout << " ";
					}
				}
				std::cout << "|";
				indiceTypes++;
				indiceINT++;
			}
		}
		else {
			if (types[indiceTypes] == "STR") {
				for (int j = 0; j != values[indiceTypes]; j++) {
					std::cout << " ";
				}
				std::cout << "|";
				indiceTypes++;
			}
			else {
				for (int j = 0; j != values[indiceTypes]; j++) {
					std::cout << " ";
				}
				std::cout << "|";
				indiceTypes++;
			}
		}
		
	}
	std::cout << std::endl;
}
