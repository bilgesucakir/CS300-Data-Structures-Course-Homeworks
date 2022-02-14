#include "hashTable.h"
#include <iostream>

template <class HashedObj>
int HashTable<HashedObj>::findPos( const HashedObj & x ) const
{
	int i = 0; // number of collusions
	int p = hashFunction(x, theArray.size()); // current position
	while( theArray[p].info != EMPTY && theArray[p].element != x){
		p += 1; // add the difference
		if(p >= theArray.size()){ // perform the mod
			p = p - theArray.size(); // if necessary
		}
	}
	return p;
}

template <class HashedObj>
const HashTable<HashedObj> & HashTable<HashedObj>::operator=( const HashTable & rhs )
{
	this.currentSize = rhs.currentSize;
	this.ITEM_NOT_FOUND = rhs.ITEM_NOT_FOUND;
	std::vector<hashedObj> vec(currentSize);

	for(int i=0; i<theArray.size(); i++){
		vec[i] = this.theArray[i];
	}
	rhs.theArray = vec;
}

template <class HashedObj>
void HashTable<HashedObj>::makeEmpty()
{
	for(int i=0; i<theArray.size(); i++){
		theArray[i].info = EMPTY;
	}
}

template <class HashedObj>
bool HashTable<HashedObj>::isActive( int currentPos ) const
{
return theArray[ currentPos ].info == ACTIVE;
}

template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x )
{
	int currentPos = findPos( x );
	if( isActive( currentPos ) ){
		theArray[currentPos].info = DELETED;
	}
}

template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find( const HashedObj & x ) const
{
	int currentPos = findPos( x );
	if (isActive( currentPos )) {
		return theArray[ currentPos ].element; 
	}
	return ITEM_NOT_FOUND;
}

template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj & x)
{
// Insert x as active
	int currentPos = findPos( x );
	if( isActive(currentPos)){
		return;
	}
	theArray[currentPos] = HashEntry( x, ACTIVE );
// enlarge the hash table if necessary
	if( ++currentSize >= theArray.size() / 2 ){
		rehash();
	}
}

template <class HashedObj>
int HashTable<HashedObj>::findAndReturnIndex(const HashedObj & obj)
{
	for(int i=0; i<theArray.size(); i++){
		if(theArray[i].info == ACTIVE){
			if(theArray[i].element == obj){
				return i;
			}
		}
	}
	return -1;
}

template <class HashedObj>
int HashTable<HashedObj>::returnFirstEmpty(){
	for(int i=0; i<theArray.size(); i++){
		if(theArray[i].info == EMPTY){
			return i;
		}
	}
	return -1;
}

template <class HashedObj>
void HashTable<HashedObj>:: rehash(){

	std::vector<HashEntry> oldArray = theArray;
	theArray.resize(2 * oldArray.size());

	for( int j = 0; j < theArray.size(); j++ ){
		theArray[j].info = EMPTY;
	}

	currentSize = 0;
	for( int i = 0; i < oldArray.size( ); i++ ){
		if( oldArray[ i ].info == ACTIVE ){
			insert( oldArray[ i ].element );
		}
	}

}

int hashFunction(const std::string & theStr, int tableSize)
{
	return 0;
}

int hashFunction(const int & theInt, int tableSize)
{
	return 0;
}

int hashFunction(const hashStruct & theStruct, int tableSize){

	return 0;
}

bool operator!= (const hashStruct & lhs, const hashStruct & rhs)
{
	return (lhs.str != rhs.str);
}

bool operator== (const hashStruct & lhs, const hashStruct & rhs)
{
	return (lhs.str == rhs.str);
}

