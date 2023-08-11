#include "B_plus_tree.h"
#include <queue>
#include "Node.h"
#include <iostream>
using namespace std;
// Declaración de una variable global llamada 'parent' de tipo puntero a 'Node'.
Node* parent = nullptr;

// Definición del constructor de la clase 'BPTree' que permite al usuario especificar los límites máximos.

B_plus_tree::B_plus_tree() {
	this->maxIntNodoLimite = 6; // Establece el límite máximo de hijos internos según el valor proporcionado por el usuario.
	this->maxHojasNodoLimite = 5; // Establece el límite máximo de nodos hoja según el valor proporcionado por el usuario.
	this->root = NULL; // Inicializa el puntero 'root' como NULL.

}

// Definición del constructor de la clase 'BPTree' que permite al usuario especificar los límites máximos.
B_plus_tree::B_plus_tree(int cantIntNode, int cantHojasNodo) {
	this->maxIntNodoLimite = cantIntNode; // Establece el límite máximo de hijos internos según el valor proporcionado por el usuario.
	this->maxHojasNodoLimite = cantHojasNodo; // Establece el límite máximo de nodos hoja según el valor proporcionado por el usuario.
	this->root = NULL; // Inicializa el puntero 'root' como NULL.

}


// Método que devuelve el límite máximo de hijos internos permitidos.
int B_plus_tree::getMaxIntNodoLimite() {
	return maxIntNodoLimite;
}

// Método que devuelve el límite máximo de nodos hoja permitidos.
int B_plus_tree::getMaxHojasNodoLimite() {
	return maxHojasNodoLimite;
}



// Método que devuelve un puntero al nodo raíz del árbol.
Node* B_plus_tree::getRoot() {
	return root;
}

// Método que establece el nodo pasado como nodo raíz del árbol.
void B_plus_tree::setRoot(Node* ptr) {
	this->root = ptr;
}

// Método que busca y devuelve el primer nodo hoja hacia la izquierda a partir del nodo 'cursor'.
Node* B_plus_tree::primerNodoIzq(Node* cursor) {
	if (cursor->isLeaf)
		return cursor; // Si el nodo es una hoja, devolvemos el mismo nodo.

	// Si el nodo no es una hoja, buscamos el primer nodo hoja entre sus hijos.
	for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
		if (cursor->ptr2TreeOrData.ptr2Tree[i] != NULL)
			return primerNodoIzq(cursor->ptr2TreeOrData.ptr2Tree[i]);
	}

	return NULL; // Si no se encuentra un nodo hoja, se devuelve NULL.
}

// Método que busca y devuelve un puntero al puntero del padre del nodo 'child' a partir del nodo 'cursor'.
// Utiliza un recorrido en profundidad para encontrar el padre.
Node** B_plus_tree::encontrarPadre(Node* cursor, Node* child) {
	if (cursor->isLeaf || cursor->ptr2TreeOrData.ptr2Tree[0]->isLeaf)
		return NULL; // Ignora los nodos hoja, ya que no pueden ser padres, y tampoco busca el padre de un nodo hoja.

	for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
		if (cursor->ptr2TreeOrData.ptr2Tree[i] == child) {
			parent = cursor; // Si encontramos el nodo 'child' entre los hijos del nodo 'cursor', almacenamos el puntero al padre en la variable global 'parent'.
		}
		else {
			// Comentando para evitar un posible error de 'vector out of bound'.
			// new (&cursor->ptr2TreeOrData.ptr2Tree) std::vector<Node*>; 

			Node* tmpCursor = cursor->ptr2TreeOrData.ptr2Tree[i];
			encontrarPadre(tmpCursor, child); // Llamada recursiva para continuar buscando en el subárbol correspondiente.
		}
	}

	return &parent; // Devuelve un puntero al puntero del padre del nodo 'child'.
}


////////////////////////////////////////////////////////////////////////////////////////////

void B_plus_tree::mostrar(Node* cursor) {
	auto inicio = std::chrono::high_resolution_clock::now();
	if (cursor == NULL) {
		std::cout << "El Arbol esta Vacio" << std::endl;
		auto fin = std::chrono::high_resolution_clock::now();
		auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
		std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
	}
	queue<Node*> q;
	q.push(cursor);

	while (!q.empty()) {
		int sz = q.size();
		for (int i = 0; i < sz; i++) {
			Node* u = q.front(); q.pop();

			// Imprimir las claves en el nodo actual
			for (int val : u->keys)
				cout << val << ",";

			cout << "||"; // Separador para separar los nodos adyacentes

			if (u->isLeaf != true) {
				// Agregar los hijos del nodo actual a la cola para recorrerlos en el siguiente nivel
				for (Node* v : u->ptr2TreeOrData.ptr2Tree) {
					q.push(v);
				}
			}
		}
		cout << endl;
	}
	auto fin = std::chrono::high_resolution_clock::now();
	auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
	std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}

void B_plus_tree::mostrar_secuencial(Node* cursor) {
	// Obtener el primer nodo hoja (izquierdo) del árbol
	auto inicio = std::chrono::high_resolution_clock::now();
	Node* firstLeft = primerNodoIzq(cursor);

	if (firstLeft == NULL) {
		cout << "El Arbol esta Vacio" << endl;
		auto fin = std::chrono::high_resolution_clock::now();
		auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
		std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
		return;
	}
	while (firstLeft != NULL) {
		// Imprimir las claves del nodo hoja actual
		for (int i = 0; i < firstLeft->keys.size(); i++) {
			cout << firstLeft->keys[i] << " ";
			//<< " :  "<<firstLeft->ptr2TreeOrData.idPag[i]<<std::endl;
		}

		firstLeft = firstLeft->ptr2siguiente; // Mover al siguiente nodo hoja
	}
	cout << endl;
	auto fin = std::chrono::high_resolution_clock::now();
	auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
	std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
}
int B_plus_tree::getIDBloq(int key) {
	// Verificar si el árbol está vacío (ninguna tupla ha sido insertada aún)
	if (root == NULL) {
		cout << "No hay Datos" << endl;
		return -1;
	}
	else {
		Node* cursor = root; // Inicializar el cursor en la raíz del árbol B+
		// Recorrer desde la raíz hasta un nodo hoja (donde se almacenan los datos reales)
		while (cursor->isLeaf == false) {
			// Utilizar upper_bound para encontrar el índice del nodo hijo donde la clave podría estar presente
			int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
			cursor = cursor->ptr2TreeOrData.ptr2Tree[idx]; // Actualizar el cursor al nodo hijo
		}

		// Ahora el cursor apunta a un nodo hoja que contiene la clave o la clave más cercana más grande

		// Utilizar lower_bound para encontrar el índice de la clave en el nodo hoja
		int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin(); // Búsqueda binaria

		// Verificar si la clave no se encuentra en el nodo hoja
		if (idx == cursor->keys.size() || cursor->keys[idx] != key) {
			cout << "No existe en el Arbol" << endl;
			return -1;
		}
		return cursor->ptr2TreeOrData.idPag[idx];
	}
}

// Función para buscar una clave en el árbol B+ y recuperar sus datos correspondientes
void B_plus_tree::buscar(int key) {
	auto inicio = std::chrono::high_resolution_clock::now();
	// Verificar si el árbol está vacío (ninguna tupla ha sido insertada aún)
	if (root == NULL) {
		cout << "No hay Datos" << endl;
		auto fin = std::chrono::high_resolution_clock::now();
		auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
		std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
		return;
	}
	else {
		Node* cursor = root; // Inicializar el cursor en la raíz del árbol B+
		cout << "||";
		for (const auto& elemento : cursor->keys) {
			std::cout << elemento << ",";
		}
		cout << "||" << std::endl;
		// Recorrer desde la raíz hasta un nodo hoja (donde se almacenan los datos reales)
		while (cursor->isLeaf == false) {
			// Utilizar upper_bound para encontrar el índice del nodo hijo donde la clave podría estar presente
			int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
			cursor = cursor->ptr2TreeOrData.ptr2Tree[idx]; // Actualizar el cursor al nodo hijo
			cout << "|" << endl;
			cout << "v" << endl;
			cout << "||";
			for (const auto& elemento : cursor->keys) {
				std::cout << elemento << ",";
			}
			cout << "||" << std::endl;
		}

		// Ahora el cursor apunta a un nodo hoja que contiene la clave o la clave más cercana más grande

		// Utilizar lower_bound para encontrar el índice de la clave en el nodo hoja
		int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin(); // Búsqueda binaria

		// Verificar si la clave no se encuentra en el nodo hoja
		if (idx == cursor->keys.size() || cursor->keys[idx] != key) {
			cout << "No existe en el Arbol" << endl;
			auto fin = std::chrono::high_resolution_clock::now();
			auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
			std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
			return;
		}

		// Si se encuentra la clave en el nodo hoja, recuperar los datos correspondientes

		// Preparar el nombre del archivo usando el valor de la clave para abrir el archivo
		//Node* firstLeft = primerNodoIzq(cursor);
		cout << "|" << endl;
		cout << "v" << endl;
		cout << "||";
		for (const auto& elemento : cursor->keys) {
			std::cout << elemento << ",";
		}
		cout << "||" << std::endl;
		cout << "|" << endl;
		cout << "v" << endl;
		auto fin = std::chrono::high_resolution_clock::now();
		auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
		cout << "ID Registro " << cursor->keys[idx] << " : ID Bloque " << cursor->ptr2TreeOrData.idPag[idx] << std::endl;
		std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
		cout << endl;
		return;
	}
}
////////////////////////////////////////////////////////////////////////////

void B_plus_tree::insertar(int key, int id_pagina) {

	if (root == NULL) {
		//NODO RAIZ
		root = new Node;
		root->isLeaf = true;
		root->keys.push_back(key);
		//almacenar un entero en el nodo raíz evita la llamada innecesaria al constructor del entero,no es necesario inicializar el entero 
		new (&root->ptr2TreeOrData.idPag) std::vector<int>; // almacenar un entero sin llamar al constructor entero 
		root->ptr2TreeOrData.idPag.push_back(id_pagina);
		return;
	}
	else {
		//std::cout << "muere 1" << std::endl;
		Node* cursor = root;
		Node* parent = NULL;
		// Buscando la posición posible para la clave dada, siguiendo el mismo procedimiento que en la búsqueda
		while (!cursor->isLeaf) {
			parent = cursor;
			int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
			cursor = cursor->ptr2TreeOrData.ptr2Tree[idx]; // Actualizar el cursor al siguiente nodo
		}

		// Ahora, el cursor es el nodo hoja en el que insertaremos la nueva clave
		if (cursor->keys.size() < maxHojasNodoLimite) {

			//std::cout << "muere 3" << std::endl;
			/*
				Si el nodo hoja actual NO está LLENO, ¡encontramos la posición correcta para la nueva clave e insertamos!
			*/
			int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
			cursor->keys.push_back(key);
			cursor->ptr2TreeOrData.idPag.push_back(id_pagina);
			//std::cout << "sds" << cursor->keys.size() << std::endl;
			if (i != cursor->keys.size() - 1) {

				//std::cout << "muere 4" << std::endl;
				for (int j = cursor->keys.size() - 1; j > i; j--) {  // desplazando la posición de las claves y los punteros de datos
					cursor->keys[j] = cursor->keys[j - 1];
					cursor->ptr2TreeOrData.idPag[j] = cursor->ptr2TreeOrData.idPag[j - 1];
					//cursor->ptr2TreeOrData.idPag=
				}

				// ya que el paso anterior fue solo para aumentar el tamaño de los vectores y crear espacio, ahora simplemente insertamos
				cursor->keys[i] = key;
				cursor->ptr2TreeOrData.idPag[i] = id_pagina;
			}
			//cout << "Insertado exitosamente: " << key << endl;
		}
		else {
			//std::cout << "muere 5" << std::endl;
			/*
				¡Demonios! ¡El nodo está desbordado! :(
				¡HAIYYA! Dividir el nodo.
			*/
			vector<int> virtualNode(cursor->keys);
			vector<int> virtualDataNode(cursor->ptr2TreeOrData.idPag);


			// buscando el lugar probable para insertar la clave
			int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
			//std::cout << "i" << i<<std::endl;
			virtualNode.push_back(key);          // para crear espacio
			virtualDataNode.push_back(id_pagina);
			//std::cout << "virtualsdsd" << virtualNode.size() << std::endl;
			if (i != virtualNode.size() - 1) {
				for (int j = virtualNode.size() - 1; j > i; j--) {  // desplazando la posición de las claves y los punteros de datos
					virtualNode[j] = virtualNode[j - 1];
					virtualDataNode[j] = virtualDataNode[j - 1];
					//std::cout << "nuevo nodo" << virtualNode[j] << std::endl;
				}

				// insertando
				virtualNode[i] = key;
				virtualDataNode[i] = id_pagina;
				//std::cout << "i" << i << std::endl;
			}
			/*
				¡BAZINGA! Tengo el poder de crear un nuevo nodo hoja :)
			*/

			auto* newLeaf = new Node;
			newLeaf->isLeaf = true;
			new (&newLeaf->ptr2TreeOrData.idPag) std::vector<int>;

			// intercambiando el puntero siguiente
			Node* temp = cursor->ptr2siguiente;
			cursor->ptr2siguiente = newLeaf;
			newLeaf->ptr2siguiente = temp;


			// redimensionando y copiando las claves y los punteros de datos al Nodo Antiguo
			cursor->keys.resize((maxHojasNodoLimite) / 2 + 1); // revisar +1 o no al hacer la partición
			cursor->ptr2TreeOrData.idPag.resize((maxHojasNodoLimite) / 2 + 1);
			for (int i = 0; i <= (maxHojasNodoLimite) / 2; i++) {
				//std::cout << "numero" << i;

				cursor->keys[i] = virtualNode[i];
				cursor->ptr2TreeOrData.idPag[i] = virtualDataNode[i];
				//std::cout << "cursor" << cursor->keys[i];
			}

			// insertando las nuevas claves y punteros de datos en el Nuevo Nodo
			for (int i = (maxHojasNodoLimite) / 2 + 1; i < virtualNode.size(); i++) {
				//std::cout << "sdsdsdggg" <<i;
				newLeaf->keys.push_back(virtualNode[i]);
				newLeaf->ptr2TreeOrData.idPag.push_back(virtualDataNode[i]);
				//std::cout << "cursor" ;

			}

			if (cursor == root) {
				/*
					Si el cursor es el nodo raíz, creamos un nuevo nodo raíz
				*/

				auto* newRoot = new Node;
				newRoot->keys.push_back(newLeaf->keys[0]);
				new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
				newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
				newRoot->ptr2TreeOrData.ptr2Tree.push_back(newLeaf);
				root = newRoot;
				//cout << "¡Creada nueva raíz!" << endl;
			}
			else {
				// Insertar nueva clave en el padre
				insertInternal(newLeaf->keys[0], &parent, &newLeaf);
			}
		}
	}
}


void B_plus_tree::insertInternal(int x, Node** cursor, Node** child) {  //en Nodos internos
	if ((*cursor)->keys.size() < maxIntNodoLimite - 1) {
		/*
			Si el nodo no está lleno, encontramos la posición para la nueva clave.
		*/
		int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
		(*cursor)->keys.push_back(x);
		(*cursor)->ptr2TreeOrData.ptr2Tree.push_back(*child);

		if (i != (*cursor)->keys.size() - 1) {
			// Diferentes ciclos porque el tamaño es diferente para ambas (es decir, diferencia de uno)
			for (int j = (*cursor)->keys.size() - 1; j > i; j--) {  // desplazamos las posiciones de las claves y punteros
				(*cursor)->keys[j] = (*cursor)->keys[j - 1];
			}
			for (int j = (*cursor)->ptr2TreeOrData.ptr2Tree.size() - 1; j > (i + 1); j--) {
				(*cursor)->ptr2TreeOrData.ptr2Tree[j] = (*cursor)->ptr2TreeOrData.ptr2Tree[j - 1];
			}
			(*cursor)->keys[i] = x;
			(*cursor)->ptr2TreeOrData.ptr2Tree[i + 1] = *child;
		}
		//cout << "Clave insertada en el nodo interno :)" << endl;
	}
	else {  //dividir
		//cout << "Nodo insertado en el nodo interno correctamente" << endl;
		//cout << "Desbordamiento en el nodo interno: (¡¡HAIYAA!! dividiendo nodos internos)" << endl;

		vector<int> virtualKeyNode((*cursor)->keys);
		vector<Node*> virtualTreePtrNode((*cursor)->ptr2TreeOrData.ptr2Tree);
		// Crear copias temporales de las claves y punteros del nodo interno para realizar la división.

		int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();  //encontrar la posición para x
		virtualKeyNode.push_back(x);                                                                   // crear espacio
		virtualTreePtrNode.push_back(*child);                                                           // crear espacio

		if (i != virtualKeyNode.size() - 1) {
			for (int j = virtualKeyNode.size() - 1; j > i; j--) {  // desplazamos las posiciones de las claves y punteros
				virtualKeyNode[j] = virtualKeyNode[j - 1];
			}
			for (int j = virtualTreePtrNode.size() - 1; j > (i + 1); j--) {
				virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
			}
			virtualKeyNode[i] = x;
			virtualTreePtrNode[i + 1] = *child;
		}

		int partitionKey;                                            // excluir el elemento del medio durante la división
		partitionKey = virtualKeyNode[(virtualKeyNode.size() / 2)];  // derecho sesgado
		int partitionIdx = (virtualKeyNode.size() / 2);
		//std::cout << "valor de particoin"<<partitionIdx << std::endl;

		// Redimensionar y copiar las claves y punteros al NodoViejo
		(*cursor)->keys.resize(partitionIdx);
		(*cursor)->ptr2TreeOrData.ptr2Tree.resize(partitionIdx + 1);
		(*cursor)->ptr2TreeOrData.ptr2Tree.resize(partitionIdx + 1);
		for (int i = 0; i < partitionIdx; i++) {
			(*cursor)->keys[i] = virtualKeyNode[i];
		}
		for (int i = 0; i < partitionIdx + 1; i++) {
			(*cursor)->ptr2TreeOrData.ptr2Tree[i] = virtualTreePtrNode[i];
		}

		Node* newInternalNode = new Node;
		new (&newInternalNode->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
		// Insertar las nuevas claves y punteros en el NuevoNodo

		for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
			newInternalNode->keys.push_back(virtualKeyNode[i]);
		}
		for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) {  // porque solo se excluye la clave, no el puntero
			newInternalNode->ptr2TreeOrData.ptr2Tree.push_back(virtualTreePtrNode[i]);
		}

		if ((*cursor) == root) {
			/*
				Si el cursor es la raíz, creamos un nuevo NodoRaíz
			*/
			Node* newRoot = new Node;
			newRoot->keys.push_back(partitionKey);
			new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
			newRoot->ptr2TreeOrData.ptr2Tree.push_back(*cursor);
			newRoot->ptr2TreeOrData.ptr2Tree.push_back(newInternalNode);
			root = newRoot;
		}
		else {
			/*
				::Recursión::
			*/
			insertInternal(partitionKey, encontrarPadre(root, *cursor), &newInternalNode);
		}
	}
}

///////////////////////////////////////////////////////

void B_plus_tree::eliminar(int x) {
	Node* root = getRoot();

	// Si el árbol está vacío
	if (root == NULL) {
		cout << "El Arbol esta Vacio" << endl;
		return;
	}

	Node* cursor = root;
	Node* parent = root;
	int leftSibling, rightSibling;



	// Llegar al nodo hoja que puede contener la clave
	// TO-DO: Utilizar una búsqueda binaria para encontrar la clave
	//(!cursor->isLeaf = false)
	while (!cursor->isLeaf) {
		for (int i = 0; i < cursor->keys.size(); i++) {
			parent = cursor;
			leftSibling = i - 1;
			rightSibling = i + 1;

			if (x < cursor->keys[i]) {
				cursor = cursor->ptr2TreeOrData.ptr2Tree[i];
				break;
			}
			if (i == cursor->keys.size() - 1) {
				leftSibling = i;
				rightSibling = i + 2;
				cursor = cursor->ptr2TreeOrData.ptr2Tree[i + 1];
				break;
			}
		}
	}

	// Verificar si el valor existe en este nodo hoja
	int pos = 0;
	bool found = false;
	for (pos = 0; pos < cursor->keys.size(); pos++) {
		if (cursor->keys[pos] == x) {
			found = true;
			break;
		}
	}

	auto itr = lower_bound(cursor->keys.begin(), cursor->keys.end(), x);

	if (found == false) {
		cout << "La clave no fue encontrada en el Arbol" << endl;
		return;
	}

	// Desplazar las claves y ptr2Tree para el nodo hoja
	for (int i = pos; i < cursor->keys.size() - 1; i++) {
		cursor->keys[i] = cursor->keys[i + 1];
		cursor->ptr2TreeOrData.idPag[i] = cursor->ptr2TreeOrData.idPag[i + 1];
	}
	int prev_size = cursor->keys.size();
	cursor->keys.resize(prev_size - 1);
	cursor->ptr2TreeOrData.idPag.resize(prev_size - 1);
	// Si es una hoja y también el nodo raíz
	if (cursor == root) {
		if (cursor->keys.size() == 0) {
			// El árbol queda vacío
			setRoot(NULL);
		}
	}

	//cout << "Clave " << x << " eliminada del nodo hoja exitosamente" << endl;
	if ((cursor->keys.size() >= (getMaxHojasNodoLimite() + 1) / 2) || (cursor == root)) {
		// Suficiente nodo disponible para mantener el invariante
		return;
	}

	//cout << "UnderFlow en el nodo hoja ¡Ha ocurrido!" << endl;
	//cout << "Comenzando redistribución..." << endl;

	// 1. Intentar tomar una clave del hermano izquierdo
	if (leftSibling >= 0) {
		//std::cout << "muere" << std::endl;
		Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

		// Verificar si el hermano izquierdo tiene una clave extra para transferir
		if (leftNode->keys.size() >= (getMaxHojasNodoLimite() + 1) / 2 + 1) {

			// Transferir la clave máxima del hermano izquierdo
			int maxIdx = leftNode->keys.size() - 1;
			cursor->keys.insert(cursor->keys.begin(), leftNode->keys[maxIdx]);
			cursor->ptr2TreeOrData.idPag
				.insert(cursor->ptr2TreeOrData.idPag.begin(), leftNode->ptr2TreeOrData.idPag[maxIdx]);

			// Redimensionar el nodo hermano izquierdo después de la transferencia
			leftNode->keys.resize(maxIdx);
			leftNode->ptr2TreeOrData.idPag.resize(maxIdx);
			// Actualizar el padre
			parent->keys[leftSibling] = cursor->keys[0];
			//printf("Transferido desde el hermano izquierdo del nodo hoja\n");
			return;
		}
	}

	// 2. Intentar tomar una clave del hermano derecho
	if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
		//std::cout << "muere 2" << std::endl;
		Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];

		// Verificar si el hermano derecho tiene una clave extra para transferir
		if (rightNode->keys.size() >= (getMaxHojasNodoLimite() + 1) / 2 + 1) {

			// Transferir la clave mínima del hermano derecho
			int minIdx = 0;
			cursor->keys.push_back(rightNode->keys[minIdx]);
			cursor->ptr2TreeOrData.idPag
				.push_back(rightNode->ptr2TreeOrData.idPag[minIdx]);

			// Redimensionar el nodo hermano derecho después de la transferencia
			rightNode->keys.erase(rightNode->keys.begin());
			rightNode->ptr2TreeOrData.idPag.erase(rightNode->ptr2TreeOrData.idPag.begin());

			// Actualizar el padre
			parent->keys[rightSibling - 1] = rightNode->keys[0];
			//printf("Transferido desde el hermano derecho del nodo hoja\n");
			return;
		}
	}

	// Combinar y eliminar el nodo
	if (leftSibling >= 0) { // Si existe un hermano izquierdo
		//std::cout << "muere 3" << std::endl;
		Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
		// Transferir clave y ptr2Tree al hermano izquierdo y conectar ptr2siguiente
		for (int i = 0; i < cursor->keys.size(); i++) {
			leftNode->keys.push_back(cursor->keys[i]);
			leftNode->ptr2TreeOrData.idPag
				.push_back(cursor->ptr2TreeOrData.idPag[i]);
		}
		leftNode->ptr2siguiente = cursor->ptr2siguiente;
		//cout << "Combinando dos nodos hoja" << endl;
		eliminarInterno(parent->keys[leftSibling], parent, cursor); // eliminar clave del nodo padre
		// eliminar cursor;
	}
	else if (rightSibling <= parent->keys.size()) {

		//std::cout << "muere 4" << std::endl;
		Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
		// Transferir clave y ptr2Tree al hermano derecho y conectar ptr2siguiente
		for (int i = 0; i < rightNode->keys.size(); i++) {
			cursor->keys.push_back(rightNode->keys[i]);
			cursor->ptr2TreeOrData.idPag
				.push_back(rightNode->ptr2TreeOrData.idPag[i]);
		}
		cursor->ptr2siguiente = rightNode->ptr2siguiente;
		//cout << "Combinando dos nodos hoja" << endl;
		eliminarInterno(parent->keys[rightSibling - 1], parent, rightNode); // eliminar clave del nodo padre
		// eliminar rightNode;
	}
}

void B_plus_tree::eliminarInterno(int x, Node* cursor, Node* child) {
	Node* root = getRoot();

	// Verificar si la clave del nodo raíz debe ser eliminada
	if (cursor == root) {
		if (cursor->keys.size() == 1) {
			// Si queda solo una clave y coincide con uno de los punteros hijos
			if (cursor->ptr2TreeOrData.ptr2Tree[1] == child) {
				setRoot(cursor->ptr2TreeOrData.ptr2Tree[0]);
				// eliminar cursor;
				//cout << "¡Wow! Nuevo nodo raíz cambiado" << endl;
				return;
			}
			else if (cursor->ptr2TreeOrData.ptr2Tree[0] == child) {
				setRoot(cursor->ptr2TreeOrData.ptr2Tree[1]);
				// eliminar cursor;
				//cout << "¡Wow! Nuevo nodo raíz cambiado" << endl;
				return;
			}
		}
	}

	// Eliminando la clave x del nodo padre
	int pos;
	for (pos = 0; pos < cursor->keys.size(); pos++) {
		if (cursor->keys[pos] == x) {
			break;
		}
	}
	for (int i = pos; i < cursor->keys.size() - 1; i++) {
		cursor->keys[i] = cursor->keys[i + 1];
	}
	cursor->keys.resize(cursor->keys.size() - 1);

	// Ahora eliminando el ptr2Tree
	for (pos = 0; pos < cursor->ptr2TreeOrData.ptr2Tree.size(); pos++) {
		if (cursor->ptr2TreeOrData.ptr2Tree[pos] == child) {
			break;
		}
	}

	for (int i = pos; i < cursor->ptr2TreeOrData.ptr2Tree.size() - 1; i++) {
		cursor->ptr2TreeOrData.ptr2Tree[i] = cursor->ptr2TreeOrData.ptr2Tree[i + 1];
	}
	cursor->ptr2TreeOrData.ptr2Tree.resize(cursor->ptr2TreeOrData.ptr2Tree.size() - 1);

	// Si no hay underflow. ¡Uf!
	if (cursor->keys.size() >= (getMaxHojasNodoLimite() + 1) / 2 - 1) {
		//cout << "Clave " << x << " eliminada del nodo interno exitosamente\n";
		return;
	}

	//cout << "¡UnderFlow en el nodo interno! ¿Qué has hecho? :/" << endl;

	if (cursor == root) {
		return;
	}

	Node** p1 = encontrarPadre(root, cursor);
	Node* parent = *p1;

	int leftSibling, rightSibling;


	// Encontrar hermanos izquierdo y derecho como se hizo anteriormente
	for (pos = 0; pos < parent->ptr2TreeOrData.ptr2Tree.size(); pos++) {
		if (parent->ptr2TreeOrData.ptr2Tree[pos] == cursor) {
			leftSibling = pos - 1;
			rightSibling = pos + 1;
			break;
		}
	}
	//cout << "sssss :/" << endl;
	// Si es posible transferir al hermano izquierdo
	if (leftSibling >= 0) {
		//std::cout << "muerte interna 1" << std::endl;
		Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

		// Verificar si el hermano izquierdo tiene una clave extra para transferir
		if (leftNode->keys.size() >= (getMaxIntNodoLimite() + 1) / 2) {

			// Transferir clave del hermano izquierdo a través del padre
			int maxIdxKey = leftNode->keys.size() - 1;
			cursor->keys.insert(cursor->keys.begin(), parent->keys[leftSibling]);
			parent->keys[leftSibling] = leftNode->keys[maxIdxKey];

			int maxIdxPtr = leftNode->ptr2TreeOrData.ptr2Tree.size() - 1;
			cursor->ptr2TreeOrData.ptr2Tree
				.insert(cursor->ptr2TreeOrData.ptr2Tree.begin(), leftNode->ptr2TreeOrData.ptr2Tree[maxIdxPtr]);

			// Redimensionar el nodo hermano izquierdo después de la transferencia
			leftNode->keys.resize(maxIdxKey);

			return;
		}
	}

	// Si es posible transferir al hermano derecho
	if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
		//std::cout << "muerte interna 2" << std::endl;
		Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];

		// Verificar si el hermano derecho tiene una clave extra para transferir
		if (rightNode->keys.size() >= (static_cast<unsigned long long>(getMaxIntNodoLimite()) + 1) / 2) {

			// Transferir clave del hermano derecho a través del padre
			int maxIdxKey = rightNode->keys.size() - 1;
			cursor->keys.push_back(parent->keys[pos]);
			parent->keys[pos] = rightNode->keys[0];
			rightNode->keys.erase(rightNode->keys.begin());

			// Transferir el puntero desde el hermano derecho a cursor
			cursor->ptr2TreeOrData.ptr2Tree
				.push_back(rightNode->ptr2TreeOrData.ptr2Tree[0]);
			cursor->ptr2TreeOrData.ptr2Tree
				.erase(cursor->ptr2TreeOrData.ptr2Tree.begin());

			return;
		}
	}

	// Comenzar a fusionar ahora, si ninguno de los casos anteriores se aplica
	if (leftSibling >= 0) {
		//std::cout << "muerte interna 3" << std::endl;
		//leftNode + clave del padre + cursor
		Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
		leftNode->keys.push_back(parent->keys[leftSibling]);

		for (int val : cursor->keys) {
			leftNode->keys.push_back(val);
		}

		for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
			leftNode->ptr2TreeOrData.ptr2Tree
				.push_back(cursor->ptr2TreeOrData.ptr2Tree[i]);
			cursor->ptr2TreeOrData.ptr2Tree[i] = NULL;
		}

		cursor->ptr2TreeOrData.ptr2Tree.resize(0);
		cursor->keys.resize(0);

		eliminarInterno(parent->keys[leftSibling], parent, cursor);
		//cout << "Combinado con el hermano izquierdo" << endl;
	}
	else if (rightSibling < parent->keys.size()) {
		//std::cout << "muerte interna 4" << std::endl;
		// cursor + clave del padre + rightNode
		Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
		cursor->keys.push_back(parent->keys[rightSibling - 1]);

		for (int val : rightNode->keys) {
			cursor->keys.push_back(val);
		}

		for (int i = 0; i < rightNode->ptr2TreeOrData.ptr2Tree.size(); i++) {
			cursor->ptr2TreeOrData.ptr2Tree
				.push_back(rightNode->ptr2TreeOrData.ptr2Tree[i]);
			rightNode->ptr2TreeOrData.ptr2Tree[i] = NULL;
		}

		rightNode->ptr2TreeOrData.ptr2Tree.resize(0);
		rightNode->keys.resize(0);

		eliminarInterno(parent->keys[rightSibling - 1], parent, rightNode);
		//cout << "Combinado con el hermano derecho" << endl;
	}
}

// Implementación de guardarArbolEnArchivo
void B_plus_tree::guardarArbolEnArchivo() {
	char PATH[13] = "./Cache/Tree";
	ofstream archivo(PATH);

	if (!archivo.is_open()) {
		cout << "No se pudo abrir el archivo para escritura." << endl;
		return;
	}

	guardarNodoEnArchivo(root, archivo);

	archivo.close();
}

void B_plus_tree::guardarNodoEnArchivo(Node* nodo, ofstream& archivo) {

	// Obtener el primer nodo hoja (izquierdo) del árbol
	Node* firstLeft = primerNodoIzq(nodo);

	if (firstLeft == NULL) {
		cout << "¡No hay datos en la base de datos todavía!" << endl;
		return;
	}
	while (firstLeft != NULL) {
		// Imprimir las claves del nodo hoja actual
		for (int i = 0; i < firstLeft->keys.size(); i++) {
			archivo << firstLeft->keys[i] << "-->" << firstLeft->ptr2TreeOrData.idPag[i] << endl;
		}

		firstLeft = firstLeft->ptr2siguiente; // Mover al siguiente nodo hoja
	}
	cout << endl;

}

// Implementación de cargarArbolDesdeArchivo
void B_plus_tree::cargarArbolDesdeArchivo() {
	char PATH[13] = "./Cache/Tree";
	ifstream archivo(PATH);
	if (!archivo.is_open()) {
		cout << "No se pudo abrir el archivo para lectura." << endl;
		return;
	}
	std::string linea;
	while (std::getline(archivo, linea)) {
		size_t pos = linea.find("-->");
		if (pos == std::string::npos) {
			std::cerr << "Error al encontrar el delimitador en la línea." << std::endl;
			continue;
		}

		std::string valor1 = linea.substr(0, pos);
		std::string valor2 = linea.substr(pos + 3); // 3 es la longitud de "-->"

		// Procesar los valores leídos
		try {
			insertar(std::stoi(valor1), std::stoi(valor2));
		}
		catch (const std::exception& e) {
			std::cerr << "Error al convertir y/o insertar valores: " << e.what() << std::endl;
		}
	}
	archivo.close();
}


std::vector<std::pair<int, int>> B_plus_tree::getRangeal(int min, int max) {
	auto inicio = std::chrono::high_resolution_clock::now();
	std::vector<std::pair<int, int>> valoresArbol;
	// Verificar si el árbol está vacío (ninguna tupla ha sido insertada aún)
	if (root == NULL) {
		cout << "No hay Datos" << endl;
		return valoresArbol;
	}
	else {
		Node* cursor = root; // Inicializar el cursor en la raíz del árbol B+
		// Recorrer desde la raíz hasta un nodo hoja (donde se almacenan los datos reales)
		while (cursor->isLeaf == false) {
			// Utilizar upper_bound para encontrar el índice del nodo hijo donde la clave podría estar presente
			int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), min) - cursor->keys.begin();
			cursor = cursor->ptr2TreeOrData.ptr2Tree[idx]; // Actualizar el cursor al nodo hijo
		}

		// Ahora el cursor apunta a un nodo hoja que contiene la clave o la clave más cercana más grande

		// Utilizar lower_bound para encontrar el índice de la clave en el nodo hoja
		int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), min) - cursor->keys.begin(); // Búsqueda binaria

		// Verificar si la clave no se encuentra en el nodo hoja
		if (idx == cursor->keys.size() || cursor->keys[idx] != min) {//No existe
			//NODO ENCONTRADO
			for (int f = 0; f != cursor->keys.size(); f++) {
				if ((max == 0 || max > cursor->keys[f]) && cursor->keys[f] > min) {
					valoresArbol.push_back(std::make_pair(cursor->keys[f], cursor->ptr2TreeOrData.idPag[f]));
				}
			}

			//NODOS RESTANTE HASTA MAX
			cursor=cursor->ptr2siguiente;
			//Node* nodo = cursor;
			while (cursor != nullptr) {
				for (int i = 0; i < cursor->keys.size(); i++) {
					if (max == 0 || max >= cursor->keys[i]) {
						valoresArbol.push_back(std::make_pair(cursor->keys[i], cursor->ptr2TreeOrData.idPag[i]));
					}
					else {
						break;
					}
				}

				cursor = cursor->ptr2siguiente;
				
			}
		}

		else {
			// Nodo actual
			for (int f = idx; f != cursor->keys.size(); f++) {
				valoresArbol.push_back(std::make_pair(cursor->keys[f], cursor->ptr2TreeOrData.idPag[f]));
			}
			cursor = cursor->ptr2siguiente;

			while (cursor != nullptr) {
				for (int i = 0; i < cursor->keys.size(); i++) {
					if (max == 0 || max > cursor->keys[i]) {
						valoresArbol.push_back(std::make_pair(cursor->keys[i], cursor->ptr2TreeOrData.idPag[i]));
					}
					else {
						break;
					}
				}

				cursor = cursor->ptr2siguiente;

			}
		}
	}
	//std::cout << "Tiempo de Demora: " << duracion << " microsegundos" << std::endl;
	return valoresArbol;
}
