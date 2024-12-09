#ifndef BSTREEDICT_H
#define BSTREEDICT_H

#include <ostream>
#include <stdexcept>
#include "Dict.h"
#include "BSTree.h"
#include "TableEntry.h"

template <typename V>
class BSTreeDict: public Dict<V> {

    private:
        // atributos
		BSTree<TableEntry<V>>* tree;		// ABB con elementos de tipo TableEntry<V> para gestionar los elementos de un diccionario

    public:
        // metodos
		// Constructor. Crea un ABB vacío con memoria dinámica
		BSTreeDict(){
			tree = new BSTree<TableEntry<V>>();
		}
		
		// Destructor. Se encargará de liberar la memoria dinámica ocupada por el ABB tree
		~BSTreeDict(){
			delete tree;
		}

		// Sobrecarga global del operador << para imprimir el contenido del Diccionario 
		friend std::ostream& operator<<(std::ostream &out, const BSTreeDict<V> &bs){
			// Desreferenciamos el puntero para que se pase como valor y no como referencia (así conseguimos que nos imprima el valor y No la dirección de memoria 
			out << *(bs.tree);
			out << "\n";
			return out;
		}
        
		// Sobrecarga del operador []. Actúa como interfaz al método de interfaz heredado search(std::string key)
		V operator[](std::string key){
			return search(key);
		}

		// Implementación de los métodos abstractos entries(),insert(), search() y remove() heredados de Dict<V>
		void insert(std::string key, V value) override{
			// Crearme un objeto de tipo TableEntry<V> con el que poder acceder a su constructor usando los atributos key y value: 
			TableEntry<V> entrada(key, value);
			// Hago que tree apunte a la función insert() que llama a la función privada recursiva insert() de BSTree.h, y la excepción se lanza en dicha función
			// Gracias al override, diferenciamos la función insert() de Dict<V> de insert() de BSTree<T>
			tree->insert(entrada);
		}

		// Busca el valor correspondiente a key, devolviendo el valor value asociado si key está en el diccionario
		// Si no se encuentra key en el diccionario, lanza un std::runtime_error()
		V search(std::string key) override{
			TableEntry<V> busqueda(key);
			return tree->search(busqueda).value;
		}

		// Elimina el par key->value si se encuentra en el diccionario. Si no se encuentra el par, lanza un std::runtime_error()
		V remove(std::string key) override{
			TableEntry<V> eliminacion(key);
			// Buscas el valor asociado con la clave antes de eliminarla
			V valor_eliminado = tree->search(eliminacion).value;
			// Eliminas la clave
			tree->remove(eliminacion);
			// Devuelves el valor eliminado
			return valor_eliminado;
		}

		int entries() const override{
			// Como la función lanzadera size() no requiere de parámetros, la llamamos directamente con tree
			return tree->size();
		}
};

#endif
