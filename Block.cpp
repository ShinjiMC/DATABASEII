#include "Block.h"
#include "Block.h"
#include <iostream>
#include <fstream>
#include <numeric>

Block::Block() : idBloque(0), numSectorXBloque(0), capacidadBytes(0), BytesOcupados(0) {}
std::string Block::GetSectPATH(int indexBloque, int numPlatos, int numSectores, int numPistas) {
	int i = indexBloque;
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
	std::string pathSect = "./Disk/Plato_" + std::to_string(Pn) + "/Superficie_" + std::to_string(Sn) + "/Pista_" + std::to_string(Pin) + "/Sector_" + std::to_string(Sectn) + ".txt";
	return pathSect;
}
Block::Block(int idBloque, int numSectoresXBloque, int numPlatos, int numSectores, int numPistas, int BytesXSector)
	: capacidadBytes(BytesXSector* numSectoresXBloque), idBloque(idBloque), numSectorXBloque(numSectoresXBloque) {
	int index = idBloque * numSectorXBloque;
	BytesOcupados = 0;
	std::vector<int> numRegPart2(numSectoresXBloque, 0);
	numRegistosPart = numRegPart2;
	DirtyBitSect = numRegPart2;
	int IndicadorFoV = 0;
	for (int i = 1; i <= numSectoresXBloque; i++) {
		//OBTENER PATH DE SECTOR
		//numRegistosPart
		//sizeSect
		//DirtyBitSect
		int n = 0;
		std::string path = GetSectPATH(index + i, numPlatos, numSectores, numPistas);
		pathSectores.push_back(path);
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		if (file.is_open()) {
			std::string firstLine, restOfFile;
			std::streamsize fileSize = file.tellg(); // TAMANIO DEL FILE
			sizeSect.push_back(fileSize);
			file.seekg(0, std::ios::beg); //UBICARSE AL INICIO DEL TXT
			if (fileSize != 0) {
				std::getline(file, firstLine);//LEE LA PRIMERA LINEA DEL TXT (CABECERA) 
				//cabeceraBlock += firstLine;
				if (i == 1 && firstLine[0] == '0' && firstLine[1] == ',') {
					IndicadorFoV = 1;
					std::string pairStr;
					//std::cout << "Variable 1" << std::endl;
					size_t start = 0;
					size_t end = firstLine.find('#');
					n -= 1;
					while (end != std::string::npos) {
						pairStr = firstLine.substr(start, end - start);
						size_t commaPos = pairStr.find(',');

						if (commaPos != std::string::npos) {
							int secondNum = std::stoi(pairStr.substr(commaPos + 1));
							int firstNum = 0;
							if (pairStr.substr(0, commaPos) == "00-1") {
								firstNum = -1;
							}
							else {
								firstNum = std::stoi(pairStr.substr(0, commaPos));
							}

							cabeceraBlock.push_back(std::make_pair(firstNum, secondNum));
							n++;
						}

						start = end + 1;
						end = firstLine.find('#', start);
					}

					restOfFile.resize(fileSize - firstLine.size());
					file.read(&restOfFile[0], restOfFile.size());// GUARDA EL CONTENIDO EN EL BLOQUE SIN CONSIDERAR LA PRIMERA LINEA
					BytesOcupados += fileSize; // BYTES OCUPADOS EN EL BLOQUE
					size_t pos = restOfFile.find('\0');

					// Eliminar el carácter nulo si se encuentra
					if (pos != std::string::npos) {
						restOfFile.erase(pos);
					}
					std::string registroV;
					size_t startPos = 0;
					size_t pipePos;

					while ((pipePos = restOfFile.find("|", startPos)) != std::string::npos) {
						std::string extractedString = restOfFile.substr(startPos, pipePos - startPos + 1);
						dataBlock = extractedString + dataBlock;
						startPos = pipePos + 1;
					}
					std::string lastString = restOfFile.substr(startPos);
					dataBlock = lastString + dataBlock;

				}
				else if (IndicadorFoV == 1) {
					std::string pairStr;
					//std::cout << "Variable 2" << std::endl;
					size_t start = 0;
					size_t end = firstLine.find('#');

					while (end != std::string::npos) {
						pairStr = firstLine.substr(start, end - start);
						size_t commaPos = pairStr.find(',');

						if (commaPos != std::string::npos) {
							int secondNum = std::stoi(pairStr.substr(commaPos + 1));
							int firstNum = 0;
							if (pairStr.substr(0, commaPos) == "00-1") {
								firstNum = -1;
							}
							else {
								firstNum = std::stoi(pairStr.substr(0, commaPos));
							}

							cabeceraBlock.push_back(std::make_pair(firstNum, secondNum));
							n++;
						}

						start = end + 1;
						end = firstLine.find('#', start);
					}

					restOfFile.resize(fileSize - firstLine.size());
					file.read(&restOfFile[0], restOfFile.size());// GUARDA EL CONTENIDO EN EL BLOQUE SIN CONSIDERAR LA PRIMERA LINEA
					BytesOcupados += fileSize; // BYTES OCUPADOS EN EL BLOQUE
					size_t pos = restOfFile.find('\0');

					// Eliminar el carácter nulo si se encuentra
					if (pos != std::string::npos) {
						restOfFile.erase(pos);
					}
					std::string registroV;
					size_t startPos = 0;
					size_t pipePos;

					while ((pipePos = restOfFile.find("|", startPos)) != std::string::npos) {
						std::string extractedString = restOfFile.substr(startPos, pipePos - startPos + 1);
						dataBlock = extractedString + dataBlock;
						startPos = pipePos + 1;
					}
					std::string lastString = restOfFile.substr(startPos);
					dataBlock = lastString + dataBlock;
				}
				if (IndicadorFoV == 0) {
					//std::cout <<"Linea cabecera : "<< firstLine<<std::endl;
					std::string pairStr;
					//std::cout << "Fija " << std::endl;
					size_t start = 0;
					size_t end = firstLine.find('#');

					while (end != std::string::npos) {
						pairStr = firstLine.substr(start, end - start);
						size_t commaPos = pairStr.find(',');

						if (commaPos != std::string::npos) {
							int secondNum = std::stoi(pairStr.substr(commaPos + 1));
							int firstNum = 0;
							if (pairStr.substr(0, commaPos) == "00-1") {
								firstNum = -1;
							}
							else {
								firstNum = std::stoi(pairStr.substr(0, commaPos));
							}

							cabeceraBlock.push_back(std::make_pair(firstNum, secondNum));
							n++;
						}

						start = end + 1;
						end = firstLine.find('#', start);
					}

					restOfFile.resize(fileSize - firstLine.size());
					file.read(&restOfFile[0], restOfFile.size());// GUARDA EL CONTENIDO EN EL BLOQUE SIN CONSIDERAR LA PRIMERA LINEA
					BytesOcupados += fileSize; // BYTES OCUPADOS EN EL BLOQUE
					size_t pos = restOfFile.find('\0');

					// Eliminar el carácter nulo si se encuentra
					if (pos != std::string::npos) {
						restOfFile.erase(pos);
					}
					dataBlock += restOfFile;
				}


			}
			file.close();
		}

		else {
			std::cerr << "Error al abrir el archivo: " << path << std::endl;
		}
		numRegistosPart[i - 1] = n;
	}
}

void Block::Bloque_Sector() {
	std::vector<std::pair<int, int>> cabecera = cabeceraBlock;
	int bytesSector = capacidadBytes / numSectorXBloque; // OBTENER LA CAPACIDAD DE BYTES POR SECTOR 
	size_t pos = 0;
	int bytes_registro_sector = 2;
	std::string cabeceraSect, dataSect;
	int nSect = 0;
	std::string data = dataBlock;
	if (!cabecera.empty()) {
		if (cabecera[0].first == 0) {
			int spaceLibre = 0;
			int n = 0;
			//std::cout <<"Nums: "<< numSectorXBloque << std::endl;
			for (int j = 0; j < numSectorXBloque; j++) {
				int cantidadRegs = numRegistosPart[j] + n;
				std::string cabeceraIng = "", dataIng = "";
				if (j == 0) {
					std::pair<int, int> cabReg = cabecera[0];
					std::string aux1 = std::to_string(cabReg.first);
					std::string valor2 = std::to_string(cabReg.second);
					std::string cabTMP = aux1 + "," + valor2 + "#";
					cabeceraIng += cabTMP;
					spaceLibre = (cabReg.second + 1);
					cabecera.erase(cabecera.begin());
				}
				//if (DirtyBitSect[j] == 1) {
					for (int i = n; i < cantidadRegs; i++) {
						std::pair<int, int> cabReg = cabecera[i];
						//NUM REGISTRO
						std::string aux1 = std::to_string(cabReg.first);
						std::string valor1;
						for (int i = 0; i < (6 - aux1.length()); i++) {
							valor1 += "0";
						}
						valor1 += aux1;
						// POSICION 
						std::string valor2 = std::to_string(cabReg.second);
						//FORMAR CABECERA QUE SE INSERTAR[A EN EL TXT
						std::string cabTMP = valor1 + "," + valor2 + "#";
						cabeceraIng += cabTMP;

						int positionK = cabReg.second - spaceLibre;
						//std::cout << "Posicion de Reg " << valor1 << " : " << positionK << std::endl;
						std::string substring = "";
						size_t pipePos = data.find("|", positionK);
						if (pipePos != std::string::npos) {
							substring = data.substr(positionK, pipePos - positionK + 1);
						}
						dataIng += substring;

					}
					/*std::cout << "Path: " << pathSectores[j] << std::endl;
					std::cout << "Informacion: " << std::endl;
					std::cout << cabeceraIng << std::endl;
					std::cout << dataIng << std::endl;*/
					std::fstream file(pathSectores[j], std::ios::out);
					if (file.is_open()) {
						file.clear();
						if (!cabeceraIng.empty()) {
							file << cabeceraIng;//ESCRIBE LA CABECERA EN EL SECTOR
							file << '\n';
							file << dataIng;// ESCRIBE EL CONTENIDO DE REGISTROS EN EL SECTOR SEGUN CORRESPONDA 
						}
						file.close();
					}
					else {
						std::cerr << "Error al abrir el archivo: " << pathSectores[j] << std::endl;
					//}
				}
				n = numRegistosPart[j] + n;
			}
		}
		else {
			//--------------------        
			//std::string cabecera = cabeceraBlock;
			//std::cout < cabecera<<std::endl;
			//std::cout << dataBlock;

			int n = 0;
			//std::cout <<"Nums: "<< numSectorXBloque << std::endl;
			for (int j = 0; j < numSectorXBloque; j++) {
				int cantidadRegs = numRegistosPart[j] + n;
				std::string cabeceraIng = "", dataIng = "";
				if (DirtyBitSect[j] == 1) {
					for (int i = n; i < cantidadRegs; i++) {
						std::pair<int, int> cabReg = cabecera[i];
						//NUM REGISTRO
						std::string aux1 = std::to_string(cabReg.first);
						std::string valor1;
						for (int i = 0; i < (4 - aux1.length()); i++) {
							valor1 += "0";
						}
						valor1 += aux1;
						// POSICION 
						std::string valor2 = std::to_string(cabReg.second);
						//FORMAR CABECERA QUE SE INSERTAR[A EN EL TXT
						std::string cabTMP = valor1 + "," + valor2 + "#";
						cabeceraIng += cabTMP;


						std::string substring = "";
						size_t pipePos = data.find("|", cabReg.second);
						if (pipePos != std::string::npos) {
							substring = data.substr(cabReg.second, pipePos - cabReg.second + 1);
						}
						dataIng += substring;

					}
					/*std::cout <<"Path: "<< pathSectores[j] << std::endl;
					std::cout << "Informacion: " << std::endl;
					std::cout << cabeceraIng << std::endl;
					std::cout << dataIng << std::endl;*/
					std::fstream file(pathSectores[j], std::ios::out);
					if (file.is_open()) {
						file.clear();
						if (!cabeceraIng.empty()) {
							file << cabeceraIng;//ESCRIBE LA CABECERA EN EL SECTOR
							file << '\n';
							file << dataIng;// ESCRIBE EL CONTENIDO DE REGISTROS EN EL SECTOR SEGUN CORRESPONDA 
						}
						file.close();
					}
					else {
						std::cerr << "Error al abrir el archivo: " << pathSectores[j] << std::endl;
					}
				}
				n = numRegistosPart[j] + n;
			}
		}
	}
}

void Block::InsertarTodo(std::string data, std::vector <int> numRegPart, std::vector<std::pair<int, int>> cabecera, std::vector <int> sizeSect2, std::vector <int> DirtyBitSect2, int bytes, int bytesOcupados) {
	dataBlock = data;
	cabeceraBlock = cabecera;
	numRegistosPart = numRegPart;
	sizeSect = sizeSect2;
	DirtyBitSect = DirtyBitSect2;
	capacidadBytes = bytes;
	BytesOcupados = bytesOcupados;
	Bloque_Sector(); // GUARDAR DATOS DEL BLOQUE A LOS SECTORES 
}

void Block::ShowSectores() {
	std::cout << "Sectores: " << std::endl;
	// IMPRIME LAS RUTAS DE LOS SECTORES CORRESPONDIENTES AL BLOQUE
	for (const std::string& path : pathSectores) {
		std::cout << "- " << path << std::endl;
	}
}

void Block::showBloque() {
	//IMPRIME CABECERA
	std::cout << "Bloque: " << idBloque << std::endl;
	std::cout << "Cantidad de Regitros: " << std::accumulate(numRegistosPart.begin(), numRegistosPart.end(), 0) << std::endl;
	std::cout << "Cabecera: " << std::endl;
	for (const auto& par : cabeceraBlock) {
		std::cout << par.first << "," << par.second << "#";
	}
	std::cout << std::endl;
	std::cout << "Sectores: " << std::endl; 
	
	for (int i = 0; i < pathSectores.size(); i++) {
		std::cout << " - " << pathSectores[i] << " Bytes Ocupados: " << sizeSect[i] /* << " - " << DirtyBitSect[i]*/ << std::endl;
	}
	//std::cout << cabeceraBlock << std::endl;
	//IMPRIME DATOS DEL BLOQUE
	std::cout << "Registros: " << std::endl;
	std::cout << dataBlock << std::endl;
	std::cout << std::endl;
}

void Block::showBloqueO() {
	//IMPRIME CABECERA
	std::cout << "Bloque: " << idBloque << std::endl;
	std::cout << "Cantidad de Regitros: " << std::accumulate(numRegistosPart.begin(), numRegistosPart.end(), 0) << std::endl;
	std::cout << std::endl;
	std::cout << "Sectores: " << std::endl;

	for (int i = 0; i < pathSectores.size(); i++) {
		std::cout << " - " << pathSectores[i] << " Bytes Ocupados: " << sizeSect[i] /* << " - " << DirtyBitSect[i]*/ << std::endl;
	}
	//std::cout << cabeceraBlock << std::endl;
	//IMPRIME DATOS DEL BLOQUE
}

int Block::getBytesOcupados() {
	return BytesOcupados;
}

std::string Block::getData() {
	return dataBlock;
}

std::vector<std::pair<int, int>> Block::getCabecera() {
	return cabeceraBlock;
}

std::vector<int> Block::getNumRegvect() {
	return numRegistosPart;
}
std::vector<int> Block::getSizeSect() {
	return sizeSect;
}
std::vector<int> Block::getDirtyBitSect() {
	return DirtyBitSect;
}

// Destructor
Block::~Block() {}
