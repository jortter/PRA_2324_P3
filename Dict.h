#ifndef DICT_H
#define DICT_H
#include <string>

template <typename V> 
class Dict {
    public:
        // ... aquí los métodos virtuales puros
		virtual void insert(std::string key, V value) = 0;
		virtual V search(std::string key) = 0;
		virtual V remove(std::string key) = 0;
		virtual int entries() = 0;
};

#endif
