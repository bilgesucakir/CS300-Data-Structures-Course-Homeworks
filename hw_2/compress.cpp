#include <iostream>
#include <string>
#include <fstream>
#include "HashTable.h"
using namespace std;

//Bilgesu Cakir 27889 CS300 HW2 - compress

void initialize(HashTable<hashStruct> & hashTable){ //inserts the ascii characters (0-255) to the hashTable

	for(int i=0; i<256; i++){
		string theStr = "";
		char ch = i;
		theStr += ch;
		hashStruct hS(i, theStr);
		hashTable.insert(hS);
	}
}

void deleteFirstChar(string & s){ //deletes first char of the given string
	int  len = s.length();
	s = s.substr(1, len - 1);
}

void deleteCompressedPart(string & theString, string compressed){ //deletes the compressed part from the string that is read from the file
	int compressedLen = compressed.length();
	for(int i=0; i<compressedLen; i++){
		deleteFirstChar(theString);
	}
}

bool canWeCompressThis(HashTable<hashStruct> hashTable, hashStruct hashS){ //this function retuns true if that hashStruct object exists in the hash table
	return hashTable.find(hashS) == hashS;
}

void printCode(int code, ofstream & os){ //writes the corresponging number output to the related file
	string codeStr = to_string(code) + " ";
	os << codeStr;
}

void addNewPrefixToHashTable(string prefix, HashTable<hashStruct> & hashT, int code){ //this function adds a hashStruct to the hash table
	hashStruct hS(code, prefix);
	hashT.insert(hS);
}

void compressPrefix(string & mainStr, hashStruct prefixStruct, HashTable<hashStruct> hashTable, ofstream & os){ //does the related implementations for
	int code  = hashTable.findAndReturnIndex(prefixStruct);                                                     //compressing a string the deletes the
	printCode(code, os);                                                                                        //compressed part
	deleteCompressedPart(mainStr, prefixStruct.str);
}

int main(){
	string inputFile = "compin", outputFile = "compout", str="", notFoundStr = "notFound", copyStr;
	int notFoundInt = -1;

	hashStruct notFound(notFoundInt, notFoundStr);

	ifstream inputStream;
	ofstream outputStream;

	HashTable<hashStruct> hashTable(notFound, 4096); 

	inputStream.open(inputFile.c_str());
	outputStream.open(outputFile.c_str(), ios::app);

	char theChar;
	while(inputStream.get(theChar)){ //adding the output to a string for easing the implementations
		str = str + theChar;
	}

	copyStr = str;
	initialize(hashTable);
	cout << "The compressed version of the given string can be found in " << outputFile << endl;
	
	while(copyStr != ""){

		string current = "", next = ""; 
		bool isFound = false;
		int index = 0;
		hashStruct hashCurrent(0, current); //to avoid some mixups, number part of the hash struct is initialized to zero
		hashStruct hashNext(0, next);

		if(copyStr.length() > 1){//if length is 1, then this means that char must exist in the hash table
			if(!(canWeCompressThis(hashTable, hashCurrent) && (current == copyStr))){ //needed for the last cases to function

				while(!isFound){ //loop continues until we find a point that current string exists in the hash table but current string + one more char doesnt
					current += copyStr[index];
					next = current + copyStr[index+1];
					hashCurrent.str = current;
					hashNext.str = next;
				
					if(canWeCompressThis(hashTable, hashCurrent) && (!canWeCompressThis(hashTable, hashNext))){
						isFound = true;
					}

					index++;
				}

				int theCode = hashTable.returnFirstEmpty();
				addNewPrefixToHashTable(next, hashTable, theCode);
				compressPrefix(copyStr, hashCurrent, hashTable, outputStream);
			}
			else{
				current = copyStr;
				hashCurrent.str = current;
				compressPrefix(copyStr, hashCurrent, hashTable, outputStream);
			}
		}
		else{
			current = copyStr;
			hashCurrent.str = current;
			compressPrefix(copyStr, hashCurrent, hashTable, outputStream);
		}
	}

	outputStream.close();
	inputStream.close();
	return 0;
}