#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class Node {
public:
    bool isLeaf;
    std::vector<int> keys;
    Node* ptr2siguiente;              // Puntero para conectar al siguiente nodo para nodos hoja
    union ptr {                  // Para hacer el nodo eficiente en memoria
        std::vector<Node*> ptr2Tree;  // Matriz de punteros a sub-árboles hijos para nodos intermedios
        std::vector<int> idPag; // almacena el numero de pagina
        ptr();   // ¿Para eliminar el error
        ~ptr();  // ¿Para eliminar el error
    } ptr2TreeOrData;

    friend class B_plus_tree;  // Para acceder a los miembros privados de Node y mantener el concepto de encapsulación
    Node();
    ~Node();
};