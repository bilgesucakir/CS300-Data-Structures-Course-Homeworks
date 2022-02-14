#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

//Bilgesu Cakir 27889 CS300 HW2

int returnFirstEmpty(string arr[]){ //returns the first empty index on the array
	int i=0;
	while((arr[i] != "")&&(i<4096)){
		i++;
	}
	return i;
}

void initialize(string * arr){//inserts ascii characters form 0 to 255 to initialize the array

	for(int i=0; i<256; i++){
		string theStr = "";
		char ch = i;
		theStr  = theStr + ch;
		arr[i] = theStr;
	}
}

void printString(string text, ofstream & os){ //writes the output to the related txt file
	os << text;
}

bool isExistsInDictionary(string arr[], int code){ //returns true if that code exists in the array
	return arr[code] != "";
}

string returnCorresponding(string arr[], int code){ //returns the element at the given position
	return arr[code];
}

string stringToAdd(string prev, string prefix, string arr[], int code){ //this function decides which string should be added as a new element to the array according to given instructions in the hw file
	string str;
	if(isExistsInDictionary(arr, code)){
		str = prev + prefix.substr(0, 1);
	}
	else{
		str = prev + prev.substr(0, 1);
	}
	return str;
}

void addToDictionary(string * arrayStr, int & code, string prefix){ //this function inserts an element to the array
	*(arrayStr + returnFirstEmpty(arrayStr)) = prefix;
}

int main(){

	string inputFile = "compout.txt", outputFile = "decompout.txt", codeStr, prevText, currentText, toAdd;
	string arrayString[4096]; //initializing the array
	int code, count = 0;

	initialize(arrayString);
	
	ifstream inputStream;
	inputStream.open(inputFile.c_str());
	ofstream outputStream;
	outputStream.open(outputFile.c_str(), ios::app);

	while(inputStream >> code){
		if(count == 0){ //if code is the first code we read forom the file, we initialize previous text here and decompress it
			count++;
			printString(returnCorresponding(arrayString, code), outputStream);
			prevText = returnCorresponding(arrayString, code);
		}
		else{
			if(isExistsInDictionary(arrayString, code)){ //if given code is not the first code and that code exists in the array (meaning that that index
				currentText = returnCorresponding(arrayString, code); //of the array is not empty)
				toAdd = stringToAdd(prevText, currentText, arrayString, code);
				addToDictionary(arrayString, code, toAdd);
			}
			else{ //if given code is not the first code and that index (code) of the array is empty
				toAdd = stringToAdd(prevText, currentText, arrayString, code);
				addToDictionary(arrayString, code, toAdd);
				currentText = toAdd;
			}
			printString(currentText, outputStream); //related string is written to the related file 

			prevText = currentText; //to keep the previous text, we make current text equal to previous before we start reading the next code (number)
		}	
	}
	inputStream.close();
	outputStream.close();
	return 0;
}