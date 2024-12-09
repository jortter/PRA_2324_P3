#ifndef TABLEENTRY_H
#define TABLEENTRY_H

#include <string>
#include <ostream>

template <typename V> 
class TableEntry {
    public:
        // miembros públicos
		// Atributos
		std::string key;		// Elemento clave del par
		V value;				// Elemento valor del par
    	
		// Métodos
		// Método constructor con el par clave->valor.
		TableEntry(std::string key, V value){
			this->key = key;
			this->value = value;
		}
		
		// Crea una entrada solo con clave (sin valor).
		TableEntry(std::string key){
			this->key = key;
		}

		// Crea una entrada con la clave "" (cadena vacía) y sin valor.
		TableEntry(){
			key = "";
		}
		// Sobrecarga global del operador == para determinar que 2 instancias de TableEntry son iguales solo si comparten
		friend bool operator==(const TableEntry<V> &te1, const TableEntry<V> &te2){
			return(te1.key == te2.key);
		}
		
		// Sobrecarga global del operador != para determinar que dos instancias de TableEntry son diferentes
		friend bool operator!=(const TableEntry<V> &te1, const TableEntry<V> &te2){
			return(te1.key != te2.key);
		}

		// Sobrecarga global del operador << para imprimir el contenido de la entrada
		friend std::ostream& operator<<(std::ostream &out, const TableEntry<V> &te){
			out << "('" << te.key << "' -> " << te.value << ")";
			return out;
		}

		// Para BSTreeDict
		// Sobrecarga global del operador < para determinar que una instancia de TableEntry es menor que otra si su key es menor
		friend bool operator<(const TableEntry<V> &te1, const TableEntry<V> &te2){
			return (te1.key < te2.key);
		}

		// Sobrecarga global del operador > para determinar que una instancia de TableEntry es mayor que otra si su key es mayor
		friend bool operator>(const TableEntry<V> &te1, const TableEntry<V> &te2){
			// Negamos el resultado del operador <
			return (te1.key > te2.key);
		}

};

#endif
