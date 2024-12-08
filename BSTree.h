#ifndef BSTREE_H
#define BSTREE_H

#include <ostream>
#include <stdexcept>
#include "BSNode.h"

template <typename T> 
class BSTree {
    private:
    // miembros privados
		// atributos

		int nelem;			// n de elementos almacenados en el ABB
		BSNode<T> *root;	// nodo raíz del ABB

		// métodos

		// Método recursivo para la inserción ordenada de elementos. Inserta el elemento e de tipo T de manera ordenada en el (sub-)árbol cuya raíz es n. Devuelve el nodo que encabeza dicho (sub-)árbol modificado. Si el elemento e ya existe, lanza un std::runtime_error.
		BSNode<T>* search(BSNode<T>* n, T e) const{
			if(n == nullptr){
				throw std::runtime_error("Elemento no encontrado");
			}else if(n->elem < e){
				return search(n->right, e);
			}else if(n->elem > e){
				return search(n->left, e);
			}else{
				return n;
			}
		}
		
		// Método recursivo para la inserción ordenada de elementos. Inserta el elemento e de tipo T de manera ordenada en el (sub-)árbol cuya raíz es n. Devuelve el nodo que encabeza dicho (sub-)árbol modificado. Si el elemento e ya existe, lanza un std::runtime_error
		BSNode<T>* insert(BSNode<T>* n, T e){
			if(n == nullptr){
				n = new BSNode<T>(e);
				// Incrementamos el contador de elementos del ABB justo cuando nos creamos el nodo
				n_elem++;
			}else if(n->elem == 0){
				throw std::runtime_error("Elemento duplicado");
			}else if(n->elem < e){
				n->right = insert(n->right, e);
			}else{
				n->left = insert(n->left, e);
			}
			return n;
		}

		// Recorrido inorden o simétrico del (sub-)árbol cuya raíz es n para mostrar a través de out los elementos ordenados de menor a mayor
		void print_inorder(std::ostream &out, BSNode<T>* n) const{
			if(n != 0){
				print_inorder(out, n->left);
				out << n->elem << " ";
				print_inorder(out, n->right);
			}
		}

		//Elimina el elemento e del (sub-)árbol cuya raíz es n. Devuelve el nodo que encabeza dicho (sub-)árbol modificado. En caso de eliminar un nodo con dos sucesores (izquierdo y derecho), aplicará la politica de reemplazo por el elemento máximo del subárbol izquierdo.
		BSNode<T>* remove(BSNode<T>* n, T e){
			if(n == nullptr){
				throw std::runtime_error("Elemento no encontrado");
			}else if(n->elem < e){
				n->right = remove(n->right, e);
			}else if(n->elem > e){
				n->left = remove(n->left, e);
			}else{
				nelem--;
				if((n->right != nullptr) && (n->left != nullptr)){		// 2 descendientes
					n->elem = max(n->left);
					n->left = remove_max(n->left);
				}else{													// 1 o 0 descendiente
					n = n->right;
				}
			}
			return n;
		}

		// Método recursivo que devuelve el elemento de máximo valor contenido en el (sub-)árbol cuya raíz es n
		T max(BSNode<T>* n) const{
			if(n == nullptr){
				throw std::runtime_error("Elemento no encontrado");
			}else if(n->right != nullptr){
				return max(n->right);				
			}else{
				return n->elem;
			}
		}

		// Método recursivo que elimina el elemento de máximo valor contenido en el (sub-)árbol cuya raíz es n
		BSNode<T>* remove_max(BSNode<T>* n){
			// Caso base
			if(n->right == nullptr){
				return n->left;
			// Caso recursivo
			}else{
				n->right = remove_max(n->right);
				return n;
			}
		}

		// Método recursivo para liberación de la memoria dinámica ocupada por los objetos de tipo BSNode<T> que conforman el (sub-)árbol cuya raíz es n
		void delete_cascade(BSNode<T>* n){
			// Caso base -> Si no hay más nodos que procesar
			if(n == nullptr){
				return;
			}
			// Casos recursivos
			// Antes de liberar el nodo actual n usando delete, primero debemos liberar los subárboles izquierdo y derecho, para que así los nodos descendientes sean
			// liberados antes que el actual
			delete_cascade(n->left);
			delete_cascade(n->right);
			// Liberamos la memoria dinámica del nodo actual usando delete
			delete n;
		}


    public:
    // miembros públicos
		// métodos
		
		// constructor crea ABB vacío
		BSTree(){
			nelem = 0;
			root = nullptr;
		}

		// Devuelve el número de elementos nelem del ABB
		int size() const{
			return nelem;
		}
		
		// Busca y devuelve el elemento  e de tipo T en el ABB.  Actúa como método lanzadera del método privado recursivo search(BSNode<T>* n, T e).  Notar que deberá devolver el elemento contenido por el nodo devuelto por el método privado
		T search(T e) const{
		return search(root, e)->elem;
		}
		
		// Sobrecarga del operador[]. Actúa como interfaz al método search(T e)
		T operator[](T e) const{
			return search(e);
		}

		// Inserta el elemento e de tipo T de manera ordenada en el  ABB. Actúa como método lanzadera del método privado recursivo insert(BSNode<T>* n, T e)
		void insert(T e){
			root = insert(root, e);
		}

    	// Sobrecarga del operador << para mostrar a través de out los contenidos del ABB bst, realizando un recorrido inorden o simétrico del árbol para mostrar los elementos ordenados de menor a mayor. Delega en el método privado recursivo print_inorder()
		friend std::ostream& operator<<(std::ostream &out, const BSTree<T> &bst){
			bst.print_inorder(out, bst.root);
			return out;
		}

		// Elimina el elemento e de tipo T del ABB. Actúa como método lanzadera del método privado recursivo remove(BSNode<T>* n, T e)
		void remove(T e){
			root = remove(root, e);
		}

		// Método destructor. Delega en el método privado recursivo delete_cascade()
		~BSTree(){
			delete_cascade(root);
		}
};

#endif
