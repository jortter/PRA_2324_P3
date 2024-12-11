#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <ostream>
#include <stdexcept>
#include "Dict.h"
#include "TableEntry.h"

#include "/home/jorter/practPRA/pract1/PRA_2324_P1/ListLinked.h"  
// En este ficheros se deben definir e implementar tanto los métodos públicos de esta clase como los de Dict<V>

template <typename V> class HashTable : public Dict<V> {

    private:

        int n;			// Nº de elementos almacenados actualmente en la tabla hash
		int max; 		// Tamaño de la tabla hash (nº total de cubetas)
		// Tabla de cubetas representada por un array de punteros a listas enlazadas (tipo ListLinked<T>) que almacenan pares clave -> valor (tipo TableEntry<V>) 
		ListLinked<TableEntry<V>>* table;	
		
		// Función hash que devuelve la posición (cubeta) en la tabla hash de key (i.e. devuelve la posición de la clave en la tabla hash)
		// Se calcula como el resto de la división entre la suma de los valores ASCII numéricos de los caractéres de la clave, y el tamaño de la tabla hash
		// El método at(i) de std::string permite obtener el carácter situado en la posición i de un string.
		// La función int(c) permite obtener el valor ASCII numérico del carácter c
		int h(std::string key){
			int resultado = 0;
			for(char c : key){
				resultado += static_cast<int>(c);
			}

			return resultado % max;
		}

	public:

        // Constructor -> Reserva memoria dinámica para crear una tabla (array en nuestro caso) table de tamaño size, e inicializará los atributos n y max de la clase
		HashTable(int size){
			// Inicializamos atributos
			table = new ListLinked<TableEntry<V>>[size];
			n = 0;
			max = size;
		}
		
		// Destructor -> Liberar la memoria dinámica reservada al crear la tabla table
		~HashTable(){
			delete[]table;
		}
		
		// Devuelve el nº total de cubetas de la tabla
		int capacity() const{
			return max;
		}
	

		/***Implementación de los métodos de Dict<V> + sobrecarga de operadores de HashTable<V>***/
		
		// Inserta el par key->value en el diccionario y lanza un std::runtime_error si key ya existe en el diccionario (key es mi posición en el array)
		void insert(std::string key, V value) override{
			// Recordar que la función hash (h()) se usa para determinar en qué cubeta del array table debe almacenarse el par key->value
			int cubeta = h(key);
			// aux va a ser el elemento que me va a apuntar a las funciones implementadas en ListLinked.h
			// Usamos &aux para trabajar directamente sobre la referencia directa a la lista enlazada dentro de table[cubeta]
			ListLinked<TableEntry<V>> &aux = table[cubeta];
			// Antes de insertar un nuevo par key->value debo comprobar si la clave ya está presente en la lista enlazada correspondiente a esa cubeta

			// Para ello, tenemos que recorrer la tabla hash (table[cubeta]) y verificar si aux apunta a la clave key
			for(int i = 0; i < aux.size(); i++){
				// Si la clave ya existe en la tabla, lanzamos la exceoción
				if(aux.get(i).key == key){
					throw std::runtime_error("Esta clave ya existe en el diccionario: " + key);	
				}
			}
			// La funcion append espera que le pasemos una instancia de TableEntry
			TableEntry<V> entrada(key, value);
			// Para conseguir coste O(1) insertamos al principio de la lista (prepend)
			aux.prepend(entrada);
			n++;
		}
		
		// Busca el valor correspondiente a key, devolviendo el valor value asociado si key está en el diccionario
		// Si no se encuentra key en el diccionario, lanza un std::runtime_error()
		V search(std::string key) override{
			int cubeta = h(key);
			ListLinked<TableEntry<V>> &aux = table[cubeta];
			for(int i = 0; i < aux.size(); i++){
				// Si encontramos la clave en la tabla, devolvemos el valor de dicha clave
				if(aux.get(i).key == key){
					return aux.get(i).value;
				}
			}
			// Si tras recorrer la tabla no se ha encontrado la clave (no se ha cumplido el if del bucle), lanzamos la excepción
			throw std::runtime_error("Esta clave NO existe en el diccionario: " + key);	
		}		
		// Elimina el par key->value si se encuentra en el diccionario. Si no se encuentra el par, lanza un std::runtime_error()
		V remove(std::string key) override{
			int cubeta = h(key);
			ListLinked<TableEntry<V>> &aux = table[cubeta];
			for(int i = 0; i < aux.size(); i++){
				// Si encontramos la clave la eliminamos
				if(aux.get(i).key == key){
					V valor_eliminado = aux.get(i).value;
					aux.remove(i);
					n--;
					return valor_eliminado;
				}
			}
			// Si tras recorrer la tabla NO se ha encontrado el key (no se ha cumplido el if del bucle), lanzamos la excepción
			throw std::runtime_error("Esta clave NO existe en el diccionario: " + key);
		}
	
		// Devuelve el nº de elementos que tiene el diccionario (justo al llamar a la función)
		int entries() const override{
			return n;
		}


		// Sobrecarga del operador [] -> Devuelve el valor correspondiente a key. Si no existe (i.e. si no lo encuentras), lanza std::runtime_error
		V operator[](std::string key){
			V resultado = search(key);
			return resultado;
  		 } 

		// Sobrecarga global del operador << para imprimir el contenido de la tabla hash por pantalla
		friend std::ostream& operator<<(std::ostream &out, const HashTable<V> &th){
			out << "HashTable [entries: " << th.entries() << ", capacity: " << th.capacity() << "]" << "\n";
			out << "==============" << "\n\n";
			
			for(int i = 0; i < th.max; i++){
				out << "== Cubeta " << i << " ==\n\n";
				ListLinked<TableEntry<V>> &aux = th.table[i];
				// Si NO hay elementos en la cubeta, la lista está vacía
				if(aux.empty())
					out << "List => []" << "\n\n";
				else{
					out << "List => [";
					out << "\n";	
					// Ahora hay que imprimir los pares key->value (aux->data.key y aux->data.value)
					// Tenemos que ir recorriendo la lista enlazada de la cubeta
					for(int j = 0; j < aux.size(); j++){
						out << "  ('" << aux[j].key << "' => " << aux[j].value << ")";
						out << "\n";
					}
					out << "]" << "\n\n";

				}
				
			}

			out << "==============" << "\n";
			return out;

		}

};

#endif
