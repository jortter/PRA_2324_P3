bin/testTableEntry: testTableEntry.cpp TableEntry.h
		mkdir -p bin
		g++ -o bin/testTableEntry testTableEntry.cpp

bin/testHashTable: Dict.h HashTable.h TableEntry.h testHashTable.cpp
		mkdir -p bin
		g++ -o bin/testHashTable testHashTable.cpp

clean:
		rm -rf *.o *.gch bin
