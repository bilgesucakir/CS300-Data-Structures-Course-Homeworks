#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
using namespace std;

#ifndef GRAPH_H
#define GRAPH_H

struct ShortestPath{
	bool known;
	int dist;
	vector<string> path;
	ShortestPath(){}
	ShortestPath(bool k, int d, vector<string> p){
		known = k;
		dist = d;
		path = p;
	}
};

struct graphNode{
	string word;
	string explanation;
	graphNode * next;
	graphNode(){}
	graphNode(string w, string e, graphNode * n){
		word = w;
		explanation = e;
		next = n;
	}
};

struct wordNode{
	string word;
	wordNode * nextWord;
	graphNode * nextEdge;
	ShortestPath shP;
	wordNode(){}
	wordNode(string theWord, wordNode * nxWord, graphNode * nxEdge, ShortestPath shrPth){
		word = theWord;
		nextWord = nxWord;
		nextEdge = nxEdge;
		shP = shrPth;
	}
};

struct edge{
	string from;
	string to;
	edge(){}
	edge(string f, string t){
		from = f;
		to = t;
	}
};

class Graph{
public:
	Graph();
	~Graph();
	void addWord(string word);
	bool isExists(string theWord);
	void findShortestPathOrNot(string start, string destination);
private:
	wordNode * headWord;
	unordered_map<string, wordNode *> map;

	void deleteGraphNodesList(graphNode * headGraphNode);

	wordNode * addWordToEndWordNode(string newWord);
	void addWordToEndGraphNode(wordNode * node, string newWord, string theExpl);
	void connectOrNot(string newWord, wordNode * existingWordNode, wordNode * newlyAdded);

	string removeOneChar(string str, int index);
	wordNode * findWord(string startingWord);
	graphNode * findWordGivenPrev(string prev, string toFind);

	bool canInsert(string newWord, string existingWord, int & place);
	bool canChange(string newWord, string existingWord, int & place);
	bool canDelete(string newWord, string existingWord, int & place);

	void markNodesUnknown();
	void giveDistancesToNodes(string start, string end);
	void shortestPathGivenDistances(wordNode * startingNode, string end);
	void coutNecessaryInfo(string explanation, string word);

	void makePathEmpty();
};
#endif