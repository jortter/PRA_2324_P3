#ifndef BSNODE_H
#define BSNODE_H

#include <ostream>

template <typename T> 
class BSNode {
    public:
    // miembros públicos
    	// atributos
		T elem;					// elemento almacenado en el nodo
		BSNode<T>* left;		// puntero al nodo sucesor izquierdo
		BSNode<T>* right;		// puntero al nodo sucesor derecho

		// métodos
		// Constructor que crea un BSNode con elemento y los punteros sucesores
		BSNode(T elem, BSNode<T>* left=nullptr, BSNode<T>* right=nullptr){
			this->elem = elem;
			this->left = left;
			this->right = right;
		}

		// Sobrecarga global del operador << para imprimir el nodo por pantalla
		friend std::ostream& operator<<(std::ostream &out, const BSNode<T> &bsn){
			out << bsn.elem;
			return out;
		}
};

#endif
