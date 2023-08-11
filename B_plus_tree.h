#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <chrono>
#include "Node.h"



class B_plus_tree
{
private:
    int maxIntNodoLimite;                                   // Límite de #de hijos para nodos internos
    int maxHojasNodoLimite;                                   // Límite de #de nodos para nodos hoja
    Node* root;                                             // Puntero a la raíz del Árbol B+
    void insertInternal(int x, Node** cursor, Node** hijo);  // Inserta x desde child en cursor (padre)
    Node** encontrarPadre(Node* cursor, Node* hijo);
    Node* primerNodoIzq(Node* cursor);
    void guardarNodoEnArchivo(Node* nodo, std::ofstream& archivo);

public:
    B_plus_tree();
    B_plus_tree(int cantIntNode, int cantHojasNodo);

    Node* getRoot();
    int getMaxIntNodoLimite();
    int getMaxHojasNodoLimite();
    int getIDBloq(int Key);
    void guardarArbolEnArchivo();
    void cargarArbolDesdeArchivo();
    void setRoot(Node*);
    void mostrar(Node* cursor);
    void mostrar_secuencial(Node* cursor);
    void buscar(int Key);
    void insertar(int key, int idPag);
    void eliminar(int key);
    void eliminarInterno(int x, Node* cursor, Node* hijo);
    std::vector<std::pair<int, int>> getRangeal(int min, int max);
};