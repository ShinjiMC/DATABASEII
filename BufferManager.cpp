#include "BufferManager.h"
#include <string>



BufferManager::BufferManager() {
    Arbol = new B_plus_tree();
    diccionarios = Cache();
    diccionarios.Reload();
}
BufferManager::BufferManager(int numBytes, int numPages) {
    Arbol = new B_plus_tree();
    diccionarios = Cache();
    diccionarios.Reload();
}

BufferManager::~BufferManager() { delete Arbol; }

const BufferPool& BufferManager::getAllPages() const {
    return AllPages;
}

void BufferManager::MakeDisco() {
    auto inicio = std::chrono::high_resolution_clock::now();
    std::vector<std::string> carpetas = { "./Dir","./disk" };
    for (const auto& carpeta : carpetas) {
        if (fs::exists(carpeta)) {
            try {
                fs::remove_all(carpeta);
            }
            catch (const std::exception& e) {
                std::cerr << "Error al eliminar la carpeta: " << carpeta << std::endl;
                std::cerr << e.what() << std::endl;
            }
        }
    }
    int numPlatos = 0, numPistas = 0, numSectores = 0, numBytes = 0, numSectoresXBloque = 0, typeofBuffer = -1;
    std::cout << "Ingrese la cantidad de Platos: ";
    std::cin >> numPlatos;
    std::cout << "\nIngrese la cantidad de Pistas por Superficie: ";
    std::cin >> numPistas;
    std::cout << "\nIngrese la cantidad de Sectores por Pista: ";
    std::cin >> numSectores;
    std::cout << "\nIngrese la cantidad de Bytes por Sector: ";
    std::cin >> numBytes;
    std::cout << "\nIngrese la cantidad de Sectores por Bloque: ";
    std::cin >> numSectoresXBloque;
    std::cout << "\nEscoja la estrategia de reemplazo \n\t1. LRU\n\t2. MRU \n\t3. Clock \nIngrese el numero segun la eleccion:";
    std::cin >> typeofBuffer;

    if (typeofBuffer == 1) {
        std::cout << "Elegiste LRU" << std::endl;
    }
    else if (typeofBuffer == 2) {
        std::cout << "Elegiste MRU" << std::endl;
    }
    else if (typeofBuffer == 3) {
        std::cout << "Elegiste Clock" << std::endl;
    }
    else {
        std::cout << "Opción no válida. Se selecciona LRU por defecto." << std::endl;
        typeofBuffer = 1; // Establecer LRU como opción predeterminada
    }
    diccionarios = Cache();
    diccionarios.Reload();
    diccionarios.InsertDataDisk(numPlatos, numPistas, numSectores, numBytes, (numPlatos * numPistas * numSectores * 2) / numSectoresXBloque, numSectoresXBloque, typeofBuffer);
    Disco = HardDisk(numPlatos, numPistas, numSectores, numBytes, numSectoresXBloque);
    AllPages = BufferPool((numPlatos * numPistas * numSectores * 2) / numSectoresXBloque, (numPlatos * numPistas * numSectores * 2) / numSectoresXBloque/*6*/, numBytes * numSectoresXBloque, Disco.getStackBlock(), typeofBuffer);
    Arbol = new B_plus_tree(10,9);
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void BufferManager::LoadDisk() {
    auto inicio = std::chrono::high_resolution_clock::now();
    int numPlatos = 0, numPistas = 0, numSectores = 0, bytesSector = 0, numBloques = 0, numSectorXBloque = 0, typeofBuffer = -1;
    std::ifstream inputFile("./Cache/DISK");
    if (!inputFile) {
        std::cerr << "Error al abrir el archivo: " << "./Cache/DISK" << std::endl;
        return;
    }
    char valor[256];
    int contador = 0;
    while (inputFile.getline(valor, sizeof(valor), '#')) {
        switch (contador) {
        case 0:
            numPlatos = std::atoi(valor);
            break;
        case 1:
            numPistas = std::atoi(valor);
            break;
        case 2:
            numSectores = std::atoi(valor);
            break;
        case 3:
            bytesSector = std::atoi(valor);
            break;
        case 4:
            numBloques = std::atoi(valor);
            break;
        case 5:
            numSectorXBloque = std::atoi(valor);
            break;
        case 6:
            typeofBuffer = std::atoi(valor);
            break;
        default:
            break;
        }
        contador++;
    }

    inputFile.close();
    diccionarios = Cache();
    diccionarios.Reload();
    Disco = HardDisk(numPlatos, numPistas, numSectores, bytesSector, numSectorXBloque);
    AllPages = BufferPool(numBloques, 6, bytesSector * numSectorXBloque, Disco.getStackBlock(), typeofBuffer);
    Arbol = new B_plus_tree(10, 9);
    Arbol->cargarArbolDesdeArchivo();
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
    //Arbol = new B_plus_tree(9, 10);
}

void BufferManager::InsertStruct(const char* content, const char* filename) {
    auto inicio = std::chrono::high_resolution_clock::now();
    diccionarios.AddEstruct(content, filename);
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void BufferManager::CargarArchivo(const char* filename) {
    auto inicio = std::chrono::high_resolution_clock::now();
    diccionarios = Cache();
    diccionarios.Reload();
    const char extensionTXT[] = ".txt";
    std::size_t indicePunto = std::strcspn(filename, ".");
    std::size_t longitudMaxima = 256;
    char nombre2TXT[256];
    if (indicePunto < std::strlen(filename)) {
        std::strncpy(nombre2TXT, filename, indicePunto);
        nombre2TXT[indicePunto] = '\0';
    }
    else {
        std::strncpy(nombre2TXT, filename, longitudMaxima);
        nombre2TXT[longitudMaxima - 1] = '\0';
    }
    char nombreTXT[] = "new.txt";

    std::ifstream entrada(filename);
    if (!entrada) {
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return;
    }
    std::ofstream salida(nombreTXT);
    std::string linea;
    char c;

    // Ignorar la primera línea del archivo CSV
    entrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Procesar las líneas restantes
    bool dentroDeComillas = false;
    while (entrada.get(c)) {
        if (c == '\n') {
            // Escribir la línea procesada en el archivo de salida
            salida << linea << '\n';
            linea.clear();
        }
        else {
            size_t pos = linea.size();


            // Reemplazar comas por #
            if (c == ',') {
                // Verificar si estamos dentro de una cadena entre comillas
                if (dentroDeComillas) {
                    linea.push_back(c);
                }
                else {
                    linea.push_back('#');
                }
            }
            else if (c == '"') {
                // Cambiar el estado de dentroDeComillas
                dentroDeComillas = !dentroDeComillas;
                linea.push_back(c);
            }
            else {
                linea.push_back(c);
            }
        }
    }

    // Escribir la última línea procesada en el archivo de salida
    if (!linea.empty()) {
        salida << linea << '\n';
    }

    // Cerrar los archivos
    entrada.close();
    salida.close();

    //std::cout << "Archivo convertido exitosamente. Nombre del archivo TXT: " << nombreTXT << std::endl;

    const char* archivoAnterior2 = "new.txt";
    const char* archivoNuevo2 = strcat(strcpy(new char[strlen(nombre2TXT) + 5], nombre2TXT), ".txt");
    std::ifstream entrada2(archivoAnterior2);
    std::ofstream salida2(archivoNuevo2);
    char c2;
    int i2 = 0;
    int j2 = diccionarios.getEstructura(nombre2TXT).getTamAtIndex(i2);
    while (entrada2.get(c2)) {
        if (c2 == '\n') {
            while (j2 != 0) {
                salida2 << ' ';
                j2--;
            }
            salida2 << '\n';
            i2 = 0;
            j2 = diccionarios.getEstructura(nombre2TXT).getTamAtIndex(i2);
        }
        else if (c2 == '#') {
            if (j2 == 0) {
                i2++;
                j2 = diccionarios.getEstructura(nombre2TXT).getTamAtIndex(i2);
            }
            else {
                while (j2 != 0) {
                    salida2 << ' ';
                    j2--;
                }
                i2++;
                j2 = diccionarios.getEstructura(nombre2TXT).getTamAtIndex(i2);
            }
        }
        else {
            salida2 << c2;
            j2--;
        }
    }

    // Cerrar los archivos
    entrada2.close();
    salida2.close();
    int nSectores = Disco.getNumSectoresXBloque(); // N sectores por bloque o pagina 
    int bytes_por_sector = Disco.getNumBytes(); //bytes por sector
    std::ifstream pro(archivoNuevo2, std::ios::binary);
    pro.seekg(0, std::ios::end);
    std::streampos fileSize = pro.tellg();
    pro.seekg(0, std::ios::beg);
    int inde = -1;
    //UBICAR PAGINA PARA INGRESAR NUEVOS DATOS
    for (int jai = 0; jai < AllPages.getNumPages(); jai++) {
        if (AllPages.getBytesOcupadosbyIndex(jai) != AllPages.getBytesbyIndex(jai)) {
            inde = jai; break;
        }
    }

    if (inde != -1) {
        std::string car; //LINEA DE TEXTO
        std::string registros_conca; // Concatenar registros
        std::string cabecera_conca; // Concatenar cabeceras
        int bytes_regis = 2; // Recolectar suma de bytes_registros que pueden estar en un sector
        int tamReg = diccionarios.getEstructura(nombre2TXT).getTamTotal() + 1; // tamanio registro
        int indice = 1;//indice registro
        int posicion_regis = 1;
        int sect = 0;// primer sector por pagina o bloque
        if (fileSize < (AllPages.getBytesbyIndex(inde) * AllPages.getNumPages())) {
            //leer X caracteres y eso sera un registro luego le agregamos "|" al final y lo concatenamos en un string, la cabecera se determinara numReg y (numreg-1)*X =(posicion)
            //para agregarlo a la pagina debemos saber cuantos bytes serian el registro y la cabecera, segun eso consultamos si en nuestro tamaño sector bytes es 
            // optimo, sino pasar al otro sector agregando los datos anteriorres a la pagina
            //en caso de llenar los n sectores de pagina( ficticios) debemos cambiar pagina
            //std::cout << "PAGINA:" << inde << std::endl;
            while (std::getline(pro, car)) {
                car.erase(std::remove(car.begin(), car.end(), '\r'), car.end());
                car.erase(std::remove(car.begin(), car.end(), '\r\n'), car.end());
                car.erase(std::remove(car.begin(), car.end(), '\n'), car.end());
                car += '|';
                //std::cout << car << std::endl;
                std::string aux1 = std::to_string(indice);
                std::string valor1;
                for (int i = 0; i < (4 - aux1.length()); i++) {
                    valor1 += "0";
                }
                valor1 += aux1;
                std::string cabeceraTMP = valor1 + "," + std::to_string((posicion_regis - 1) * car.length()) + "#"; // insertar cabecera
                if ((bytes_regis + (car.length() + cabeceraTMP.length())) <= bytes_por_sector) { // VERIFICAR SI EL REGISTRO NO PASA LA CAPACIDAD DEL SECTOR
                    AllPages.ADDReg(indice, (posicion_regis - 1) * car.length(), car, car.length() + cabeceraTMP.length(), inde, sect);
                    bytes_regis += (car.length() + cabeceraTMP.length()); // aumenta el tamaño ocupado
                }
                else {
                    sect++; // PASA AL SIGUIENTE SECTOR
                    //Meter lo anterior registros_conca y cabecera_conca a Pagina
                    bytes_regis = 2 + (car.length() + cabeceraTMP.length());
                    if (sect == nSectores) {
                        savePage(inde);//vhj
                        inde++;

                        //std::cout << "PAGINA:" << inde << std::endl;
                        sect = 0;
                        posicion_regis = 1;
                    }
                    cabeceraTMP = std::to_string(indice) + "," + std::to_string((posicion_regis - 1) * tamReg) + "#";
                    AllPages.ADDReg(indice, (posicion_regis - 1) * car.length(), car, car.length() + cabeceraTMP.length(), inde, sect);
                }
                indice++;
                posicion_regis++;
            }
            savePage(inde);//vhj
        }
        else {
            std::cout << "El documento es muy grande para cargarlo" << std::endl;
        }
    }
    else {
        std::cout << "Las páginas ya están completas." << std::endl;
    }
    pro.close();


    try {
        std::filesystem::remove(nombreTXT);
    }
    catch (const std::filesystem::filesystem_error& error) {
        std::cout << "Error al eliminar el archivo: " << error.what() << std::endl;
    }
    try {
        std::filesystem::remove(archivoNuevo2);
    }
    catch (const std::filesystem::filesystem_error& error) {
        std::cout << "Error al eliminar el archivo: " << error.what() << std::endl;
    }
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
    LoadDisk();

}

std::vector<int> BufferManager::findIndicesOf(const std::vector<std::string>& strings, std::string n,std::string n2) {
    std::vector<int> indices;

    for (int i = 0; i < strings.size(); i++) {
        if (strings[i] == n|| strings[i] == n2) {
            indices.push_back(i);
        }
    }

    return indices;
}

std::vector<std::string> BufferManager::split(const std::string& linea, char separador) {
    std::vector<std::string> tokens;
    std::size_t inicio = 0;
    std::size_t fin = linea.find(separador);

    while (fin != std::string::npos) {
        std::string token = linea.substr(inicio, fin - inicio);
        tokens.push_back(token);
        inicio = fin + 1;
        fin = linea.find(separador, inicio);
    }

    std::string ultimoToken = linea.substr(inicio);
    tokens.push_back(ultimoToken);

    return tokens;
}

std::string BufferManager::convertToBinaryString(const std::vector<std::string>& valores) {
    std::string resultado;

    for (const std::string& valor : valores) {
        if (valor.empty()) {
            resultado += '0';
        }
        else {
            resultado += '1';
        }
    }

    return resultado;
}

int BufferManager::countOnes(const std::string& binaryString, const std::vector<int>& indices) {
    int count = 0;
    for (int index : indices) {
        if (index >= 0 && index < binaryString.size() && binaryString[index] == '1') {
            count++;
        }
    }
    return count;
}

std::string BufferManager::intTOstring(int number, int n) {
    std::string str = std::to_string(number);

    if (str.length() < n) {
        str = std::string(n - str.length(), '0') + str;
    }

    return str;
}

void BufferManager::CargarArchivoVariable(const char* filename) {
    auto inicio = std::chrono::high_resolution_clock::now();
    diccionarios = Cache(); // carga el esquema del disco 
    diccionarios.Reload();
    const char extensionTXT[] = ".txt";
    std::size_t indicePunto = std::strcspn(filename, ".");
    std::size_t longitudMaxima = 256;
    char nombre2TXT[256];
    if (indicePunto < std::strlen(filename)) {
        std::strncpy(nombre2TXT, filename, indicePunto);
        nombre2TXT[indicePunto] = '\0';
    }
    else {
        std::strncpy(nombre2TXT, filename, longitudMaxima);
        nombre2TXT[longitudMaxima - 1] = '\0';
    }
    char nombreTXT[] = "new.txt";

    std::ifstream entrada(filename);
    if (!entrada) {
        std::cout << "No se pudo abrir el archivo." << std::endl;
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
        std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
        return;
    }
    std::ofstream salida(nombreTXT);
    std::string linea;
    char c;

    // Ignorar la primera línea del archivo CSV
    entrada.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // esta contiene el esquema 

    // Procesar las líneas restantes
    bool dentroDeComillas = false;
    while (entrada.get(c)) {
        if (c == '\n') {
            // Escribir la línea procesada en el archivo de salida
            salida << linea << '\n';
            linea.clear();
        }
        else {
            size_t pos = linea.size();


            // Reemplazar comas por #
            if (c == ',') {
                // Verificar si estamos dentro de una cadena entre comillas
                if (dentroDeComillas) {
                    linea.push_back(c);
                }
                else {
                    linea.push_back('#');
                }
            }
            else if (c == '"') {
                // Cambiar el estado de dentroDeComillas
                dentroDeComillas = !dentroDeComillas;
                linea.push_back(c);
            }
            else {
                linea.push_back(c);
            }
        }
    }

    // Escribir la última línea procesada en el archivo de salida
    if (!linea.empty()) {
        salida << linea << '\n';
    }

    // Cerrar los archivos
    entrada.close();
    salida.close();



    //std::cout << "Archivo convertido exitosamente. Nombre del archivo TXT: " << nombreTXT << std::endl;
    ///posicion,longitud de los string, enteros , nullbitmap, string
    //21,5/26,10/36,10/#65000,#10101#palabra1palabra2
    std::vector<std::string> tipos_estructura = diccionarios.getEstructura(nombre2TXT).getTypesVect();
    /*std::cout << "Contenido del vector tipos_estructura:" << std::endl;
    for (const std::string& tipo : tipos_estructura) {
        std::cout << tipo << std::endl;
    }*/
    std::vector<int> indices_int = findIndicesOf(tipos_estructura, "INT","INT");
    /*for (const int& tipo : indices_int) {
        std::cout << tipo << std::endl;
    }*/
    std::vector<int> indices_str = findIndicesOf(tipos_estructura, "STR","STR");
    std::vector<int> indices_longint = findIndicesOf(tipos_estructura, "LONGINT", "LONGINT");
    /*for (const int& tipo : indices_str) {
        std::cout << tipo << std::endl;
    }*/
    int cantidaddeSTR = tipos_estructura.size() - indices_int.size() - indices_longint.size();
    //Hallar longitud de nullbitmap
    std::string ones(tipos_estructura.size(), '1');
    int decimalbitmap = std::stoi(ones, nullptr, 2);
    int lengthBitMap = std::to_string(decimalbitmap).length();
    ///
    int MAXREG = diccionarios.getEstructura(nombre2TXT).getTamTotalV();
    int LongBuff = std::to_string(MAXREG).length();

    //std::cout << "Tamano NULLBITMAP: " << lengthBitMap << std::endl;
    /*std::cout << " MAX reg : " << LongBuff << std::endl; */

    std::vector<std::string> valores;
    std::vector<int> IDKeys;

    std::ifstream entrada2(nombreTXT);

    if (!entrada2) {
        std::cout << "No se pudo abrir el archivo." << std::endl;
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
        std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
        return;
    }
    int numRegss = 1;
    std::string lineaU;
    while (std::getline(entrada2, lineaU)) {

        std::vector<std::string> valores_linea = split(lineaU, '#');
        /*for (const std::string& valor : valores_linea) {
            std::cout <<"Val: " << valor << std::endl;
        }*/
        std::string nullbitmap = convertToBinaryString(valores_linea);
        int decimal = std::stoi(nullbitmap, nullptr, 2);

        std::string registro;
        int pos_RVariable = (countOnes(nullbitmap, indices_str) * ((LongBuff * 2) + 2)) + 1 + (9 * indices_longint.size()) + (5 * indices_int.size()) + 1 + std::to_string(decimal).length() + 1;
        //std::cout << nullbitmap << " - " << std::to_string(decimal) << " - " << pos_RVariable << std::endl;
        std::string cabec;
        for (int i = 0; i < indices_str.size(); i++) {
            int j = indices_str[i];
            std::string palabra = valores_linea[j];
            if (palabra != "") {
                cabec += (intTOstring(pos_RVariable, LongBuff) + "," + intTOstring(valores_linea[j].length(), LongBuff) + "/");
                pos_RVariable += valores_linea[j].length();
            }
        }

        registro = cabec + "#";
        for (int i = 0; i < indices_longint.size(); i++) {
            int j = indices_longint[i];
            std::string num = valores_linea[j];
            if (num != "") {
                registro += (intTOstring(std::stoi(num), 8) + ",");
            }
        }
        for (int i = 0; i < indices_int.size(); i++) {
            int j = indices_int[i];
            std::string num = valores_linea[j];
            if (num != "") {
                registro += (intTOstring(std::stoi(num), 4) + ",");
            }
        }
        registro += ("#" + std::to_string(decimal) + "#");

        for (int i = 0; i < indices_str.size(); i++) {
            int j = indices_str[i];
            std::string palabra = valores_linea[j];
            if (palabra != "") {
                registro += palabra;
            }
        }
        try {
            int primerNumero = std::stoi(valores_linea[0]);
            IDKeys.push_back(primerNumero);
        }
        catch (const std::invalid_argument&) {
            IDKeys.push_back(numRegss);
            numRegss++;
        }

        registro += "|";
        valores.push_back(registro);
        //valores.push_back(valores_linea);
    }

    entrada2.close();
    //VECTOR VALORES -> TODOS LOS REGISTROS

    // Imprimir los valores extraídos
    /*for (const auto& linea_valores : valores) {
        std::cout << linea_valores << std::endl;
    }*/
    int fileSize = std::accumulate(valores.begin(), valores.end(), 0,
        [](int sum, const std::string& str) {
            return sum + str.length();
        }
    );
    for (size_t i = 0; i < valores.size();i++) {
        //std::cout << idKey << std::endl;
        AllPages.AddNewRegVar(MAXREG, IDKeys[i], valores[i], false, Arbol);
        
        if (AllPages.allDirtyKnow()) {
            //std::cout << "Ingresar Data------------------------------------------------------------------------------------------" << std::endl;
            allPages_to_bloqs();
        }
    }
    allPages_to_bloqs();
    try {
        std::filesystem::remove(nombreTXT);
    }
    catch (const std::filesystem::filesystem_error& error) {
        std::cout << "Error al eliminar el archivo: " << error.what() << std::endl;
    }
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void BufferManager::showCabecera(int n) {
    const std::vector<std::pair<int, int>> data = AllPages.getPages()[n].getCabecera();
    for (const auto& par : data) {
        std::cout << par.first << "," << par.second << "#";
    }
    std::cout << std::endl;
}

void BufferManager::showPage(int n) {
    auto inicio = std::chrono::high_resolution_clock::now();
    AllPages.showPage(n);
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void BufferManager::Page_to_Bloq(int n) {
    for (auto c : AllPages.getPages()) {
        if (c.getDirty() == 1) {
            std::string data = AllPages.getDataN(n);
            Disco.InsertBlock(data, AllPages.getRegPart(n), AllPages.getCabeceraC(n), AllPages.getSizeSectN(n), AllPages.getDirtyBitSectN(n), AllPages.getPages()[n].getIndex(), AllPages.getPages()[n].getBytes(), AllPages.getPages()[n].getBytesOcupados());
        }
    }
}

void BufferManager::allPages_to_bloqs() {
    auto inicio = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < AllPages.getPages().size(); i++) {
        Page_to_Bloq(i);
    }
    Arbol->guardarArbolEnArchivo();
    LoadDisk();
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void BufferManager::MetaDataBloq(int n) {
    auto inicio = std::chrono::high_resolution_clock::now();
    if (AllPages.getNumPages() <= n && 0 <= n) {
        std::cout << "Bloque " << n << " No existe." << std::endl;
    }
    else {
        std::cout << "Bloque " << n << ": " << std::endl;
        std::cout << "Bytes Total: " << Disco.getBytesBloq() << " bytes" << std::endl;
        std::cout << "Bytes Ocupados: " << Disco.getBytesBloqOcupados(n) << " bytes" << std::endl;
        std::cout << "Sectores: " << std::endl;
        Disco.ShowSectByBloq(n);
        std::cout << "Contenido: " << std::endl;
        Disco.ShowBloq(n);
    }
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void BufferManager::MetaDataSect(int n) {
    auto inicio = std::chrono::high_resolution_clock::now();
    if (AllPages.getNumPages() * Disco.getNumSectoresXBloque() <= n && 0 <= n) {
        std::cout << "Sector " << n - 1 << " No existe." << std::endl;
    }
    else {
        std::cout << "Sector " << n - 1 << ": " << std::endl;
        std::cout << "Bytes Total: " << Disco.getNumBytes() << " bytes" << std::endl;
        std::cout << "Bytes Ocupados: " << Disco.getBytesSectOcupados(n) << " bytes" << std::endl;
        std::cout << "Path: " << Disco.GetSectPATH(n) << std::endl;
        std::cout << "Contenido: " << std::endl;
        std::ifstream inputFile(Disco.GetSectPATH(n), std::ios::binary);
        if (!inputFile) {
            std::cerr << "Error al abrir el archivo Bloque: " << Disco.GetSectPATH(n) << std::endl;
            auto fin = std::chrono::high_resolution_clock::now();
            auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
            std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
            return;
        }
        char character;
        while (inputFile.get(character)) {
            std::cout << character;
        }
        inputFile.close();
    }
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void BufferManager::ShowFILE(const char* filename, int n) {
    auto inicio = std::chrono::high_resolution_clock::now();
    int reg = 0;
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return;
    }

    std::size_t indicePunto = std::strcspn(filename, ".");
    std::size_t longitudMaxima = 256;
    char nombre2TXT[256];
    if (indicePunto < std::strlen(filename)) {
        std::strncpy(nombre2TXT, filename, indicePunto);
        nombre2TXT[indicePunto] = '\0';
    }
    else {
        std::strncpy(nombre2TXT, filename, longitudMaxima);
        nombre2TXT[longitudMaxima - 1] = '\0';
    }
    std::cout << "Registro " << n << ": " << std::endl;
    char character;
    while (inputFile.get(character)) {
        if (character == '\n') {
            reg++;
        }
        if (reg == n) {
            std::cout << character;
        }
    }
    inputFile.close();
    std::cout << std::endl;
    std::cout << "# de bytes por Registro: " << diccionarios.getEstructura(nombre2TXT).getTamTotal() << " bytes" << std::endl;
    std::cout << "# de bits por Registro: " << diccionarios.getEstructura(nombre2TXT).getTamTotal() * 8 << " bits" << std::endl;
    std::cout << "# de bytes por FILE " << filename << ": " << diccionarios.getEstructura(nombre2TXT).getTamTotal() * reg << " bytes" << std::endl;
    std::cout << "# de bits por FILE " << filename << ": " << diccionarios.getEstructura(nombre2TXT).getTamTotal() * 8 * reg << " bits" << std::endl;
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void BufferManager::MoverRegistro(int n1, int n2) {
    auto inicio = std::chrono::high_resolution_clock::now();
    AllPages.MoverRegs(n1, n2);
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}
void BufferManager::ShowDataBloqO(int n) {
    Disco.ShowBloqO(n);
}
void BufferManager::BuscarRegistro(int n) {
    auto inicio = std::chrono::high_resolution_clock::now();
    int B=Arbol->getIDBloq(n);
    if (B == -1) {
        std::cout << "El registro "<<n << " no se encontro." << std::endl;
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
        std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
    }
    else {
        Disco.ShowBloqO(B);
        AllPages.BuscarRegs(n);
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
        std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
    }
}

void BufferManager::EliminarRegistro(int n) {
    auto inicio = std::chrono::high_resolution_clock::now();
    if (AllPages.EliminarReg(n)) {
        Arbol->eliminar(n);
    }
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void BufferManager::AddRegistro(const char* tabla, int numReg, std::string registro) {
    auto inicio = std::chrono::high_resolution_clock::now();
    if (int j = AllPages.ValidateVariable(); j == 0) {
        std::string registroCompleto;
        std::vector<std::string> tipos_estructura = diccionarios.getEstructura(tabla).getTypesVect();
        std::vector<int> indices_int = findIndicesOf(tipos_estructura, "INT","int");
        std::vector<int> indices_longint = findIndicesOf(tipos_estructura, "LONGINT", "longint");
        std::vector<int> indices_str = findIndicesOf(tipos_estructura, "STR","str");
        int cantidaddeSTR = tipos_estructura.size() - indices_int.size() - indices_longint.size();
        std::string ones(tipos_estructura.size(), '1');
        int decimalbitmap = std::stoi(ones, nullptr, 2);
        int lengthBitMap = std::to_string(decimalbitmap).length();
        int maxLongReg = diccionarios.getEstructura(tabla).getTamTotal();//Tamaño maximo de Registro
        int MAXREG = diccionarios.getEstructura(tabla).getTamTotalV();
        int LongBuff = std::to_string(MAXREG).length();

        std::vector<std::string> valores_linea = split(registro, '#');
        std::string nullbitmap = convertToBinaryString(valores_linea);
        int decimal = std::stoi(nullbitmap, nullptr, 2);

        std::string registro;
        int pos_RVariable = (countOnes(nullbitmap, indices_str) * ((static_cast<unsigned long long>(LongBuff) * 2) + 2)) + 1 + (9 * indices_longint.size()) + (5 * indices_int.size()) + 1 + std::to_string(decimal).length() + 1;
        std::string cabec;
        for (int i = 0; i < indices_str.size(); i++) {
            int j = indices_str[i];
            std::string palabra = valores_linea[j];
            if (palabra != "") {
                cabec += (intTOstring(pos_RVariable, LongBuff) + "," + intTOstring(valores_linea[j].length(), LongBuff) + "/");
                pos_RVariable += valores_linea[j].length();
            }
        }

        registro = cabec + "#";
        for (int i = 0; i < indices_longint.size(); i++) {
            int j = indices_longint[i];
            std::string num = valores_linea[j];
            if (num != "") {
                registro += (intTOstring(std::stoi(num), 8) + ",");
            }
        }
        for (int i = 0; i < indices_int.size(); i++) {
            int j = indices_int[i];
            std::string num = valores_linea[j];
            if (num != "") {
                registro += (intTOstring(std::stoi(num), 4) + ",");
            }
        }
        registro += ("#" + std::to_string(decimal) + "#");

        for (int i = 0; i < indices_str.size(); i++) {
            int j = indices_str[i];
            std::string palabra = valores_linea[j];
            if (palabra != "") {
                registro += palabra;
            }
        }
        int IDKey = 0;
        try {
            int primerNumero = std::stoi(valores_linea[0]);
            IDKey=primerNumero;
        }
        catch (const std::invalid_argument&) {
            std::cout << "Registro no Tiene ID, Error" << std::endl;
            auto fin = std::chrono::high_resolution_clock::now();
            auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
            std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
            return;
        }

        registro += "|";
        AllPages.AddNewRegVar(MAXREG, numReg, registro,true,Arbol);
        BuscarRegistro(numReg);
        std::cout << "Registro Agregado" << std::endl;
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
        std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
    }
    else {
        std::string registroCompleto;
        int i2 = 0;
        int j2 = diccionarios.getEstructura(tabla).getTamAtIndex(i2);
        for (char c : registro) {
            if (c == '#') {
                if (j2 == 0) {
                    i2++;
                    j2 = diccionarios.getEstructura(tabla).getTamAtIndex(i2);
                }
                else {
                    while (j2 != 0) {
                        registroCompleto += ' ';
                        j2--;
                    }
                    i2++;
                    j2 = diccionarios.getEstructura(tabla).getTamAtIndex(i2);
                }
            }
            else {
                registroCompleto += c;
                j2--;
            }
        }
        while (j2 != 0) {
            registroCompleto += ' ';
            j2--;
        }
        AllPages.AddNewRegFija(numReg, registroCompleto);
        std::cout << "Registro Agregado" << std::endl;
    }
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;

}

void BufferManager::getValFreeSpaceMapping() {
    auto inicio = std::chrono::high_resolution_clock::now();
    std::cout << "La página con mayor Registros eliminados es la PAGINA " << AllPages.freeSpaceMapping() << std::endl;
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void BufferManager::ShowDataTree() {
    bool flag = true;
    int option = 0;
    do {
        std::cout << "\nPor favor, ingrese una opción: " << std::endl;
        std::cout << "\t1. Buscar \n\t2. Mostrar Arbol Jerarquicamente\n\t3. Mostrar Arbol Secuencialmente\n\t4. Regresar al Menu Principal" << std::endl;
        std::cin >> option;

        switch (option) {
        case 1:
            buscar_metodo(); // Llamada al método de búsqueda pasando el árbol B+.
            break;
        case 2:
            mostrar_metodo(1); // Llamada al método de impresión pasando el árbol B+.
            break;
        case 3:
            mostrar_metodo(0); // Llamada al método de eliminación pasando el árbol B+.
            break;
        default:
            flag = false; // Finalizar el bucle do-while cuando se selecciona otra opción.
            break;
        }
    } while (flag);
}

void BufferManager::buscar_metodo() {
    int rollNo;
    std::cout << "Ingrese el numero de registro que desea buscar en el Arbol: ";
    std::cin >> rollNo;
    std::cout << std::endl;
    Arbol->buscar(rollNo);
}

void BufferManager::mostrar_metodo(int opt) {
    std::cout << "\nEsta es su estructura de archivos" << std::endl;
    if (opt == 1)
        // Se muestra la estructura del árbol B+ de manera jerárquica.
        Arbol->mostrar(Arbol->getRoot());
    else
        // Se muestra la estructura del árbol B+ de manera secuencial.
        Arbol->mostrar_secuencial(Arbol->getRoot());
}

void BufferManager::mostrar_data(const char* name, bool n) {
    if (n) {
        auto inicio = std::chrono::high_resolution_clock::now();
        std::vector<std::string> tipos_estructura = diccionarios.getEstructura(name).getTypesVect();
        std::vector<std::string> names = diccionarios.getEstructura(name).getNames();
        std::vector<int> values = diccionarios.getEstructura(name).getValues();
        for (int i = 0; i != names.size(); i++) {
            std::cout << names[i];
            if (values[i] < names[i].size()) {
                values[i] = names[i].size();
                std::cout << "|";
            }
            else {
                for (int j = 0; j != values[i] - names[i].size(); j++) {
                    std::cout << " ";
                }
                std::cout << "|";
            }

        }
        std::cout << std::endl;
        std::vector<std::pair< int, int >> valuesTree = Arbol->getRangeal(0, 0);
        for (const auto& p : valuesTree) {
            AllPages.showReg(p.first, p.second, values, tipos_estructura);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
        std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
    }
    else {
        int min=0, max=0;
        std::cout << "Ingrese el valor menor de registros (ingrese 0 para el valor minimo posible): ";
        std::cin >> min;
        std::cout << "Ingrese el valor mayor de registros (ingrese 0 para el valor maximo posible): ";
        std::cin >> max;
        if (max < 0) {
            max = 0;
        }
        auto inicio = std::chrono::high_resolution_clock::now();
        std::vector<std::string> tipos_estructura = diccionarios.getEstructura(name).getTypesVect();
        std::vector<std::string> names = diccionarios.getEstructura(name).getNames();
        std::vector<int> values = diccionarios.getEstructura(name).getValues();
        for (int i = 0; i != names.size(); i++) {
            std::cout << names[i];
            if (values[i] < names[i].size()) {
                values[i] = names[i].size();
                std::cout << "|";
            }
            else {
                for (int j = 0; j != values[i] - names[i].size(); j++) {
                    std::cout << " ";
                }
                std::cout << "|";
            }

        }
        std::cout << std::endl;
        std::vector<std::pair< int, int >> valuesTree = Arbol->getRangeal(min, max);
        for (const auto& p : valuesTree) {
            AllPages.showReg(p.first, p.second, values, tipos_estructura);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
        std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
    }
}
