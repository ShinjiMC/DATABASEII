#include <iostream>
#include "BufferManager.h"

int main() {

    int opcion = 0;
    BufferManager* PC = new BufferManager();
    while (true) {
        // Mostrar opciones disponibles
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        std::cout << "Opciones disponibles:" << std::endl;
        std::cout << "1. Crear Disco" << std::endl;
        std::cout << "2. Cargar Disco" << std::endl;
        std::cout << "3. Crear Tabla" << std::endl;
        std::cout << "4. Insertar Documento de Longitud Fija" << std::endl;
        std::cout << "5. Mostrar Datos del FILE" << std::endl;
        std::cout << "6. Mostrar Bloque" << std::endl;
        std::cout << "7. Mostrar Sector" << std::endl;
        std::cout << "8. Mostrar Pagina" << std::endl;
        std::cout << "9. Guardar Cambios de Pagina" << std::endl;
        std::cout << "10. Guardar Cambios de Todas las Paginas" << std::endl;
        std::cout << "11. Mover Registros" << std::endl;
        std::cout << "12. Buscar un registro" << std::endl;
        std::cout << "13. Eliminar Registro" << std::endl;
        std::cout << "14. Free Space Mapping" << std::endl;
        std::cout << "15. Agregar Nuevo Registro" << std::endl;
        std::cout << "16. Insertar Documento de Longitud Variable" << std::endl;
        std::cout << "20. Apagar" << std::endl;

        // Solicitar opción al usuario
        std::cout << "Ingrese el numero de opcion: ";
        std::cin >> opcion;
        // Evaluar la opción ingresada
        if (opcion == 1) {
            std::cout << "Selecciono la opcion: Crear Disco" << std::endl;
            PC->MakeDisco();
        }
        else if (opcion == 2) {
            std::cout << "Selecciono la opcion: Cargar Disco" << std::endl;
            PC->LoadDisk();
        }
        else if (opcion == 3) {
            std::cout << "Selecciono la opcion: Crear Tabla" << std::endl;
            char nombreArchivo[256];
            std::string values;
            std::cout << "Ingrese el nombre de la Estructura: ";
            std::cin >> nombreArchivo;
            std::cout << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ingrese la composicion (tipo#valor#nombre|tipo2#valor2#nombre2|...): ";
            std::getline(std::cin, values);
            std::cout << std::endl;
            PC->InsertStruct(values.c_str(), nombreArchivo);
        }
        else if (opcion == 4) {
            std::cout << "Selecciono la opcion: Insertar Documento de Longitud Fija" << std::endl;
            char nombreArchivo[256];
            std::cout << "Ingrese el nombre del Documento con extension: ";
            std::cin >> nombreArchivo;
            std::cout << std::endl;
            PC->CargarArchivo(nombreArchivo);
        }
        else if (opcion == 5) {
            std::cout << "Selecciono la opcion: Mostrar Datos del FILE" << std::endl;
            char nombreArchivo[256];
            int opt = 0;
            std::cout << "Ingrese el nombre del Documento con extension: ";
            std::cin >> nombreArchivo;
            std::cout << "Ingrese el numero de Registro: ";
            std::cin >> opt;
            std::cout << std::endl;
            PC->ShowFILE(nombreArchivo, opt);
        }
        else if (opcion == 6) {
            std::cout << "Selecciono la opcion: Mostrar Bloque" << std::endl;
            int opt = 0;
            std::cout << "Ingrese el numero de Bloque: ";
            std::cin >> opt;
            std::cout << std::endl;
            PC->MetaDataBloq(opt);
        }
        else if (opcion == 7) {
            std::cout << "Selecciono la opcion: Mostrar Sector" << std::endl;
            int opt = 0;
            std::cout << "Ingrese el numero de Sector: ";
            std::cin >> opt;
            std::cout << std::endl;
            PC->MetaDataSect(opt + 1);
        }
        else if (opcion == 8) {
            std::cout << "Selecciono la opcion: Mostrar Pagina" << std::endl;
            int opt = 0;
            std::cout << "Ingrese el numero de Pagina: ";
            std::cin >> opt;
            std::cout << std::endl;

            PC->showPage(opt);
        }
        else if (opcion == 9) {
            std::cout << "Selecciono la opcion: Guardar Cambios de Pagina" << std::endl;
            int opt = 0;
            std::cout << "Ingrese el numero de Pagina a Guardar: ";
            std::cin >> opt;
            std::cout << std::endl;
            PC->Page_to_Bloq(opt);
        }
        else if (opcion == 10) {
            std::cout << "Selecciono la opcion: Guardar Cambios de Todas las Paginas" << std::endl;
            PC->allPages_to_bloqs();
        }
        else if (opcion == 11) {
            std::cout << "Selecciono la opcion: Mover Registros" << std::endl;
            int opt = 0, opt2 = 0;
            std::cout << "Ingrese el numero del Primer Registro: ";
            std::cin >> opt;
            std::cout << "Ingrese el numero del Segundo Registro: ";
            std::cin >> opt2;
            std::cout << std::endl;
            PC->MoverRegistro(opt, opt2);
        }
        else if (opcion == 12) {
            std::cout << "Selecciono la opcion: Buscar Registro" << std::endl;
            int opt = 0;
            std::cout << "Ingrese el numero del Registro: ";
            std::cin >> opt;
            std::cout << std::endl;
            PC->BuscarRegistro(opt);
        }
        else if (opcion == 13) {
            std::cout << "Selecciono la opcion: Eliminar Registro" << std::endl;
            int opt = 0;
            std::cout << "Ingrese el numero del Registro: ";
            std::cin >> opt;
            std::cout << std::endl;
            PC->EliminarRegistro(opt);
        }
        else if (opcion == 14) {
            std::cout << "Selecciono la opcion: Free Space Mapping" << std::endl;
            std::cout << std::endl;
            PC->getValFreeSpaceMapping();
        }
        else if (opcion == 15) {
            std::cout << "Selecciono la opcion: Agregar Nuevo Registro" << std::endl;
            char nombreTabla[256];
            std::string Registro;
            int numReg = 0;
            std::cout << "Ingrese el nombre de la Tabla: ";
            std::cin >> nombreTabla;
            std::cout << "Ingrese el numero de Registro: ";
            std::cin >> numReg;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ingrese el Registro: ";
            std::getline(std::cin, Registro);
            std::cout << std::endl;
            int i = 1;
            while (i != 0) {
                PC->AddRegistro(nombreTabla, numReg, Registro);
                i--;
            }

        }
        else if (opcion == 16) {
            std::cout << "Selecciono la opcion: Insertar Documento de Longitud Variable" << std::endl;
            char nombreArchivo[256];
            std::cout << "Ingrese el nombre del Documento con extension: ";
            std::cin >> nombreArchivo;
            std::cout << std::endl;
            PC->CargarArchivoVariable(nombreArchivo);
        }
        else if (opcion == 20) {
            std::cout << "Selecciono la opcion: Apagar" << std::endl;
            std::cout << "Apagando Sistema..." << std::endl;
            break;
        }
        else {
            std::cout << "Opcion invalida. Por favor, ingrese un numero de opcion valido." << std::endl;
        }
    }

    return 0;
}



//5-3-5-3280-3
//titanic.csv
//INT#PassengerId|STR#2#Survived|STR#2#PClass|STR#84#Name|STR#7#Sex|STR#5#Age|STR#2#SibSp|STR#2#Parch|STR#20#Ticket|STR#12#Fare|STR#20#Cabin|STR#2#Embarked|
//2#1#1#"Cumings, Mrs. John Bradley (Florence Briggs Thayer)"#female#38#1#0#PC 17599#71.2833#C85#C
//Movie_Ratings.csv
//STR#23#Pelicula|INT#Patrick C|INT#Heather|INT#Bryan|INT#Patrick T|INT#Thomas|INT#aaron|INT#vanessa|INT#greg|INT#brian|INT#ben|INT#Katherine|INT#Jonathan|INT#Zwe|INT#Erin|INT#Chris|INT#Zak|INT#Matt|INT#Chris|INT#Josh|INT#Amy|INT#Valerie|INT#Gary|INT#Stephen|INT#Jessica|INT#Jeff|
//"Avatar 2"#1#1#2######################2#
//_CRT_SECURE_NO_WARNINGS