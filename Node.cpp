#include "Node.h"
// Definici�n del constructor por defecto de la clase 'Node::ptr'.
Node::ptr::ptr() {
}

// Definici�n del destructor de la clase 'Node::ptr'.
Node::ptr::~ptr() {
}

// Definici�n del constructor de la clase 'Node'.
Node::Node() {
    this->isLeaf = false; // Inicializa 'isLeaf' como falso, indicando que este nodo no es una hoja.
    this->ptr2siguiente = NULL; // Inicializa el puntero 'ptr2next' como NULL.
}

Node::~Node() = default;