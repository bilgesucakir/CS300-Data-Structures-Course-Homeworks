#ifndef HASHTABLE_H_
#define HASHTABLE_H_

//modified hash table (modification is made for linear probing and some functions are added)

#include <iostream>
#include <string>
#include <vector>

struct hashStruct{
	int number;
	std::string str;
	hashStruct::hashStruct(){}
	hashStruct::hashStruct(int i, std::string s):number(i), str(s){}
};

template <class HashedObj>
class HashTable
{
public:
	explicit HashTable( const HashedObj & notFound, int size) :  ITEM_NOT_FOUND(notFound), theArray(size), currentSize(0) {}
	HashTable( const HashTable & rhs ): ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),theArray( rhs.theArray ), currentSize( rhs.currentSize ) { }
	const HashedObj & find( const HashedObj & x ) const;
	void makeEmpty();
	void insert(const HashedObj & x);
	void remove(const HashedObj & x);
	const HashTable & operator=(const HashTable & rhs);
	enum EntryType {ACTIVE, EMPTY, DELETED};
	int findAndReturnIndex(const HashedObj & obj);
	int returnFirstEmpty();

private:
	struct HashEntry
	{
		HashedObj element;
		EntryType info;
		HashEntry(const HashedObj & e = HashedObj(), EntryType i = EMPTY): element(e), info(i){}
	};

	std::vector<HashEntry> theArray;
	int currentSize;
	const HashedObj ITEM_NOT_FOUND;
	bool isActive( int currentPos ) const;
	int findPos( const HashedObj & x ) const;
	void rehash();
};

int hashFunction(const hashStruct & theStruct, int tableSize);
int hashFunction(const std::string & theStr, int tableSize);
int hashFunction(const int & theInt, int tableSize);

bool operator == (const hashStruct & lhs, const hashStruct & rhs);
bool operator != (const hashStruct & lhs, const hashStruct & rhs);

#include "hashTable.cpp"
#endif