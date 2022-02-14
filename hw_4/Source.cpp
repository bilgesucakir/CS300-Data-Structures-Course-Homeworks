#include <iostream>
#include <string>
#include <fstream>
#include "Graph.h";
using namespace std;

bool hasStar(string word){
	return (word.substr(0,1) == "*");
}

int main(){
	string fileName = "words.txt", word;
	ifstream fileStream;
	fileStream.open(fileName.c_str());

	Graph graph;
	while(fileStream >> word){
		if(! graph.isExists(word)){
			graph.addWord(word);
		}
	}
	fileStream.close();

	bool stopLoop = false;
	string firstWord, secondWord;

	while(! stopLoop){
		cout << "Please enter two words: " << endl;
		cin >> firstWord >> secondWord;

		stopLoop = hasStar(firstWord);
		if(! stopLoop){

			if(graph.isExists(firstWord)&&graph.isExists(secondWord)){

				if(firstWord != secondWord){
					graph.findShortestPathOrNot(firstWord, secondWord);
				}
				else{
					cout << "You entered the same word (" << firstWord << ") twice. There is no need for calculation." << endl;
				}
			}
			else{
				cout << "Either one of the given words or both words does not exist in the list." << endl;
			}
		}
		cout << endl;
	}
	return 0;
}