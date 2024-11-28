#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <ostream>
#include <stdexcept>
#include "Dict.h"
#include "TableEntry.h"

#include "/home/jorter/practPRA/pract1/PRA_2324_P1/ListLinked.h"  
//#include "\\wsl.localhost\Ubuntu\home\jorter\practPRA\pract1\PRA_2324_P1\ListLinked.h"
// En este ficheros se deben definir e implementar tanto los métodos públicos de esta clase como los de Dict<V>

template <typename V>
class HashTable: public Dict<V> {

    private:
    // Atributos
		int n;								// Número de elementos almacenados actualmente en la tabla hash
		int max;							// Tamaño de la tabla hash (número total de cubetas)
		ListLinked<TableEntry<V>>* table;	// Tabla de cubetas, representada por un array de punteros a listas enlazadas 
											// (tipo ListLinked<T>) que almacenan pares clave→valor (tipo TableEntry<V>)
		
		/* Función hash que devuelve la posición (cubeta) en la tabla hash de key (i.e. devuelve la posición de la clave en la tabla hash)
		** Se calcula como el resto de la división entre la suma de los valores ASCII numéricos de los caractéres de la clave, y el tamaño de la tabla hash
		** El método at(i) de std::string permite obtener el carácter situado en la posición i de un string.
		** La función int(c) permite obtener el valor ASCII numérico del carácter c	*/
		int h(std::string key){
			int resultado = 0;
			for(char c : key){
				resultado += static_cast<int>(c);
			}

			return resultado % max;
		}

    public:
    // Métodos heredados de Dict.h
		// Inserta el par key->value en el diccionario. Lanza una excepción std::runtime_error si key ya existe en el diccionario
		void insert(std::string key, V value) override{
			// Recordar que la función hash (h()) se usa para determinar en qué cubeta del array table debe almacenarse el par key->value
			int cubeta = h(key);
			
			// Antes de insertar un nuevo par key->value debo comprobar si la clave ya está presente en la lista enlazada correspondiente a esa cubeta
			// Para ello, tenemos que recorrer la lista enlazada en table[cubeta] y verificar si algún nodo tiene la clave key
			
			// Traducir a este contexto: Node<T>* prev = nullptr;
			Node<TableEntry<V>>* prev = nullptr;
			// Traducir a este contexto: Node<T>* aux = first;
			Node<TableEntry<V>>* aux = table[cubeta].getFirst();
	
			// Recorrido de la secuencia enlazada
			while(aux != nullptr){
				// Ver si key ya existe en el diccionario
				if(aux->data.key == key)
					throw std::runtime_error("Esta clave ya existe en el diccionario: " + key);
				
				// Si no existe, avanzamos en la lista enlazada
				prev = aux;
				aux = aux->next;
			}

			// Tras el bucle, aux se va actualizando, luego si queremos insertar en la primera posición NO podemos usar aux
			// (porque ya no se cumpliría que aux = table[cubeta]), luego tenemos que usar table[cubeta].first

			// Inserción en la tabla hash -> Se suele ser siempre al principio de la secuencia enlazada para conseguir coste O(1)
			// Traducir a este contexto: first = new Node<T>(e, first); siendo e el par key->value
			table[cubeta].getFirstRef() = new Node<TableEntry<V>>(TableEntry<V>(key, value), table[cubeta].getFirst());
			// Incrementar en 1 el contador de elementos
			n++;
		}

		// Busca el valor correspondiente a key. Devuelve el valor value asociado si key está en el diccionario. 
		// Si no se encuentra, lanza una excepción std::runtime_error
		V search(std::string key) override{
			int cubeta = h(key);
			// Traducir a este contexto: Node<T>* aux = first;
			Node<TableEntry<V>>* aux = table[cubeta].getFirst();

			// Recorrido de la secuencia enlazada
			while(aux != nullptr){
				// Si encontramos key devolvemos su valor
				if(aux->data.key == key)
					return aux->data.value;
				// Si no encontramos key, avanzamos en el bucle hasta encontrarlo
				aux = aux->next;
			}
			
			// Si no hemos encontrado key lanzamos la excepción
			throw std::runtime_error("Esta clave NO existe en el diccionario: " + key);	
		}

		// Elimina el par key->value si se encuentra en el diccionario. 
		// Si no se encuentra, lanza una excepción std::runtime_error
		V remove(std::string key) override{
			int cubeta = h(key);
			V valor_eliminado;
			
			Node<TableEntry<V>>* prev = nullptr;
			// Traducir a este contexto: Node<T>* aux = first;
			Node<TableEntry<V>>* aux = table[cubeta].getFirst();

			while(aux != nullptr){
				// Si encontramos la clave, la eliminas
				if(aux->data.key == key){
					// Eliminamos el par key->value
					if(prev == nullptr)
						table[cubeta].getFirstRef() = aux->next;
					else
						prev->next = aux->next;
					// Al encontrar el elemento nos salimos del bucle
					break;
				}
					
				// Si no encontramos key, avanzamos en el bucle hasta encontrarlo
				prev = aux;
				aux = aux->next;
			}
		
			// Si no hemos encontrado key (hemos llegado al último nodo de la secuencia) lanzamos la excepción
			if(aux == nullptr)
				throw std::runtime_error("Esta clave NO existe en el diccionario: " + key);
		
			valor_eliminado = aux->data.value;
			delete aux;
			n--;				
			return valor_eliminado;
		}

		// Devuelve el número de elementos que tiene el Diccionario.
		int entries() override{
			return n;
		}

	// Resto de atributos propios de la clase
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
				// Si no hay elementos en la cubeta, i.e. el único nodo que hay (el primero) apunta a nullptr porque el par key->value es nulo, imprimimos la lista vacía
				if(th.table[i].getFirst() == nullptr)
					out << "List => []" << "\n\n";
				else{
					out << "List => [";
					out << "\n";
					// Ahora hay que imprimir los pares key->value (aux->data.key y aux->data.value)
					Node<TableEntry<V>>* aux = th.table[i].getFirst();
					// Tenemos que ir recorriendo la lista enlazada de la cubeta
					while(aux != nullptr){
						out << "  ('" << aux->data.key << "' => " << aux->data.value << ")";
						aux = aux->next;
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
