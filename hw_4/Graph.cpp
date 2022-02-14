#include "Graph.h";

Graph::Graph(){

	headWord = NULL;
}

Graph::~Graph(){

	if(headWord != NULL){
		wordNode * temp;
		while(headWord != NULL){
			temp = headWord;
			headWord = temp->nextWord;

			deleteGraphNodesList(temp->nextEdge);

			delete temp;
			temp = NULL;
		}
	}
	map.erase(map.begin(), map.end());
}

void Graph::addWord(string word){

	wordNode * temp = headWord;
	wordNode * newlyAdded = addWordToEndWordNode(word);
	if(temp != NULL){
		while(temp->nextWord != NULL){

			connectOrNot(word, temp, newlyAdded);
			temp = temp->nextWord;
		}
	}
}

void Graph::deleteGraphNodesList(graphNode * headGraphNode){

	while(headGraphNode != NULL){
		graphNode * temp = headGraphNode;
		headGraphNode = headGraphNode->next;
		delete temp;
		temp = NULL;
	}
}

void Graph::connectOrNot(string newWord, wordNode * existingWordNode, wordNode * newlyAdded){

	string existingWord = existingWordNode->word, expl, expl2;
	int place = -1;
	if(canInsert(newWord, existingWord, place)){

		expl = "insert " + newWord.substr(place, 1) + " after position " + to_string(place);
		expl2 = "delete " + newWord.substr(place, 1) + " at position " + to_string(place+ 1);
		addWordToEndGraphNode(existingWordNode, newWord, expl);
		addWordToEndGraphNode(newlyAdded, existingWordNode->word, expl2);
	}
	else if(canDelete(newWord, existingWord, place)){

		expl = "delete " + existingWord.substr(place, 1) + " at position " + to_string(place + 1);
		expl2 = "insert " + existingWord.substr(place, 1) + " after position " + to_string(place);
		addWordToEndGraphNode(existingWordNode, newWord, expl);
		addWordToEndGraphNode(newlyAdded, existingWordNode->word, expl2);
	}
	else if(canChange(newWord, existingWord, place)){

		expl = "change " + existingWord.substr(place, 1) + " at position " + to_string(place + 1) + " to " + newWord.substr(place, 1);
		expl2 = "change " + newWord.substr(place, 1) + " at position " + to_string(place + 1) + " to " + existingWord.substr(place, 1);
		addWordToEndGraphNode(existingWordNode, newWord, expl);
		addWordToEndGraphNode(newlyAdded, existingWord, expl2);
	}
	else{
		return;
	}
}

bool Graph::canInsert(string newWord, string existingWord, int & place){//insert to old to get the new

	if(newWord.length() == existingWord.length() + 1){
		for(int i=0; i<newWord.length(); i++){
			string newWordMinusOne = removeOneChar(newWord, i);
			if(newWordMinusOne == existingWord){
				place = i;
				return true;
			}
		}
		return false;
	}
	else{
		return false;
	}
}

bool Graph::canChange(string newWord, string existingWord, int & place){

	int copyPlace = -1;
	if(newWord.length() == existingWord.length()){
		int count = 0;
		for(int i=0; i<newWord.length(); i++){
			if(newWord[i] == existingWord[i]){
				count++;
			}
			else{
				copyPlace = i;
			}
		}
		if(count + 1 == newWord.length()){
			place = copyPlace;
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

bool Graph::canDelete(string newWord, string existingWord, int & place){

	if(newWord.length() + 1 == existingWord.length()){
		for(int i=0; i<existingWord.length(); i++){
			string existingWordMinusOne = removeOneChar(existingWord, i);
			if(existingWordMinusOne == newWord){
				place = i;
				return true;
			}
		}
		return false;
	}
	else{
		return false;
	}
}

wordNode * Graph::addWordToEndWordNode(string newWord){

	wordNode * temp = headWord;
	ShortestPath sp;
	wordNode * last = new wordNode(newWord, NULL, NULL, sp);
	map[newWord] = last;

	if(temp != NULL){
		while(temp->nextWord != NULL){
			temp = temp->nextWord;
		}
		temp->nextWord = last;
	}
	else{
		headWord = last;
	}
	return last;
}


void Graph::addWordToEndGraphNode(wordNode * node, string newWord, string theExpl){

	graphNode * last = new graphNode(newWord, theExpl, NULL);
	graphNode * temp = node->nextEdge;

	if(temp != NULL){
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = last;
	}
	else{
		node->nextEdge = last;
	}
}

string Graph::removeOneChar(string str, int index){

	if(index == 0){
		return str.substr(1, str.length() - 1);
	}
	else if(index == str.length() - 1){
		return str.substr(0, str.length() - 1);
	}
	else{
		return str.substr(0, index) + str.substr(index + 1, str.length() - index - 1);
	}
}

bool Graph::isExists(string theWord){//checks the word if it exists in the graph

	if(headWord == NULL){
		return false;
	}
	else{
		wordNode * temp = headWord;
		while(temp != NULL){
			if(temp->word == theWord){
				return true;
			}
			temp = temp->nextWord;
		}
		return false;
	}
}

void Graph::coutNecessaryInfo(string explanation, string word){
	cout << word << " " << "(" << explanation << ")" << endl;
}

void Graph::findShortestPathOrNot(string start, string destination){//public function to calculate shortest path from start to destination word

	wordNode * startNode = findWord(start);
	if(startNode == NULL){
		cout << "There is no such word in the graph." << endl; 
	}
	else{ 
		markNodesUnknown();
		int d = -1;
		vector<string> path;
		giveDistancesToNodes(start, destination);

		shortestPathGivenDistances(startNode, destination);
	}
}

void Graph::markNodesUnknown(){

	makePathEmpty();

	wordNode * copy = headWord;
	while(copy != NULL){

		copy->shP.known = false;
		copy = copy->nextWord;
	}
}

wordNode * Graph::findWord(string startingWord){ //retruns the node of the given word
	
	if(map.find(startingWord) != map.end()){
		return map.find(startingWord)->second;
	}
	else{
		return NULL;
	}
}

void Graph::giveDistancesToNodes(string start, string end){

	deque<wordNode *> queue;

	wordNode * startNode = findWord(start);

	startNode->shP.dist = 0;
	vector<string> empty;
	startNode->shP.path = empty;//empty vector
	queue.push_front(startNode);

	bool found = false;
	while(queue.size() > 0 || found == false){

		wordNode * currentNode = queue.back();
		queue.pop_back();
		currentNode->shP.known = true;
		
		graphNode * temp = currentNode->nextEdge;
		findWord(temp->word)->shP.path = currentNode->shP.path;
		findWord(temp->word)->shP.path.push_back(currentNode->word);

		while(temp != NULL){
			findWord(temp->word)->shP.path = currentNode->shP.path;
			findWord(temp->word)->shP.path.push_back(currentNode->word);

			if(findWord(temp->word)->shP.known == false){
				if(temp->word == end){

					found == true;
					return;
				}
				else{
					wordNode * node = findWord(temp->word);

					node->shP.dist = 1 + currentNode->shP.dist;

					queue.push_front(node);
				}
			}
			temp = temp->next;
		}
	}
}

void Graph::shortestPathGivenDistances(wordNode * startingNode, string end){

	wordNode * destNode = findWord(end);
	vector<string> shortestPath = destNode->shP.path;
	graphNode * gNode;
	string expl;

	if(shortestPath.size() > 0){//if beginning word can turn into the ending word

		cout << "Shortest path from first word (" << startingNode->word << ") to second word (" << end << ") can be found below: " << endl;

		for(int i=0; i<shortestPath.size(); i++){
			if(i == 0){
				cout << shortestPath[i] << endl;
			}
			else{
				gNode = findWordGivenPrev(shortestPath[i-1], shortestPath[i]);
				expl = gNode->explanation;
				coutNecessaryInfo(expl, shortestPath[i]);
			}
		}
		gNode = findWordGivenPrev(shortestPath[shortestPath.size() - 1], end);
		expl = gNode->explanation;
		coutNecessaryInfo(expl, end);
	}
	else{

		cout << "First word (" << startingNode->word << ") cannot transfrom into second word (" << end << ")." << endl;
	}
}

graphNode * Graph::findWordGivenPrev(string prev, string toFind){//returns graph node of the given word

	wordNode * node = findWord(prev);
	graphNode * temp = node->nextEdge;
	while(temp != NULL){
		if(temp->word == toFind){
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void Graph::makePathEmpty(){

	wordNode * temp = headWord;
	ShortestPath sp;
	while(temp != NULL){
		temp->shP = sp;
		temp = temp->nextWord;
	}
}