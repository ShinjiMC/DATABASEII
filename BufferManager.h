#include "BufferPool.h"
#include "Cache.h"
#include "HardDisk.h"
#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <iostream>

class BufferManager {
private:
    Cache diccionarios;
    BufferPool AllPages;
    B_plus_tree* Arbol;
    HardDisk Disco;
public:
    BufferManager();
    BufferManager(int numBytes, int numPages);
    ~BufferManager();
    const BufferPool& getAllPages() const;
    void MakeDisco();
    void LoadDisk();
    void InsertStruct(const char* content, const char* filename);
    void CargarArchivo(const char* filename);
    std::vector<int> findIndicesOf(const std::vector<std::string>& strings, std::string n, std::string n2);
    std::vector<std::string> split(const std::string& linea, char separador);
    std::string convertToBinaryString(const std::vector<std::string>& valores);
    int countOnes(const std::string& binaryString, const std::vector<int>& indices);
    std::string intTOstring(int number, int n);
    void CargarArchivoVariable(const char* filename);
    void showCabecera(int n);
    void showPage(int n);
    void Page_to_Bloq(int n);
    void allPages_to_bloqs();
    void MetaDataBloq(int n);
    void MetaDataSect(int n);
    void ShowFILE(const char* filename, int n);
    void MoverRegistro(int n1, int n2);
    void BuscarRegistro(int n);
    void EliminarRegistro(int n);
    void AddRegistro(const char* tabla, int numReg, std::string registro);
    void getValFreeSpaceMapping();
    void savePage(int n) {
        int a = AllPages.getIndexFrames(n);
        Page_to_Bloq(a);
    }
    void ShowDataTree();
    void buscar_metodo();
    void mostrar_metodo(int opt);
    void mostrar_data(const char* name,bool n);
    void ShowDataBloqO(int n);
};

