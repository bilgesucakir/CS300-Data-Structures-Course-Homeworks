#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ostream>
#include <queue>        //I used the built-in queue class as mentioned in the sent email.
using namespace std;

//Bilgesu Cakir 27889 CS300 HW1 (Multi-Level Feedback Queue Assignment)

struct process{ //struct containing slices ("0", "1" or "-") and process' id.  
	vector<string> slices;
	int processID;
};

struct queueWInfo{ //struct containing 1 queue made of process structs and that queue's id.
	queue<process> theQueue;
	int queueID;
};

bool isAllTerminated(int & queuesNum, vector<queueWInfo> & vectorOfQueues){  //This function checks whether all of the processes terminated or not.
	for(int i=0; i < queuesNum; i++){
		if(! vectorOfQueues[i].theQueue.empty()){
			return false;
		}
	}
	return true;
}

bool isTheCurrentLastSlice(process & pr){ //This function checks whether there is one slice left before the "-". This is needed to distinguish "E" cases
	bool isMinus = false;                 //from others.
	int untilMinusSignCount = 0, vecLen = pr.slices.size();
	for(int i=0; i<vecLen; i++){
		if((pr.slices[i] != "-")&&(!isMinus)){
			untilMinusSignCount++;
		}
		else{
			isMinus = true;
		}
	}
	return (untilMinusSignCount == 1);
}

void deleteFirstElemOfVectorProcess(vector<string> & slices){ //This function deletes one slice info ffom the process' vector of strings.
	int Size = slices.size();
	for(int i=0; i<Size - 1; i++){
		slices[i] = slices[i+1];
	}
	slices[slices.size() - 1].erase();
}

void deleteForBCasesAndEnqueueToTopMost(queue<process> & currentQ, queue<process> & topMost){ //This function deletes one process form a given queue and 
	process processToCarryToTopMost = currentQ.front();                                       //enqueus the same process to the back of the top most queue.
	currentQ.pop();
	topMost.push(processToCarryToTopMost);
}

void dropOneQueue(int processId, queueWInfo & currentQWInfo, vector<queueWInfo> & theVec){ //This function makes a process pop form its current queue and
	int queueId = currentQWInfo.queueID, newQueueID;                                       //enqueues the same process to the queue that is one below 
	if(queueId > 1){                                                                       //than the popped queue.
		newQueueID = queueId - 1;
	}
	else{
		newQueueID = queueId;
	}
	process container;
	container = currentQWInfo.theQueue.front();
	currentQWInfo.theQueue.pop();
	theVec[newQueueID - 1].theQueue.push(container);
}

void writeOutputOneLine(ofstream & os, string operation, int processNum, string queueID, int terminatedOnes, int processes){ //This function takes some parameters and the ofstream variable 
	string processName = "PC" + to_string(processNum);                                                                       //and writes one line to output.txt. 
	if((terminatedOnes == processes)&&(operation == "E")){
		os << operation << ", " << processName << ", " << queueID; //this is for not leaving an endline character at the end of the last line of the output.txt
	}
	else{
		os << operation << ", " << processName << ", " << queueID << endl;
	}

}

void handleBCases(vector<queueWInfo> & queues, ofstream & os, int & terminated, int & processN){ //This function is used for enqueueing the processes that are not at top most queue when
	int queuesSize = queues.size();                                                              //S is reached and to write the lines that starts with "B" to the output file.
	string topMostName = "Q" + to_string(queues[queuesSize - 1].queueID), operationName = "B";

	if(queuesSize >= 2){
		for(int i=0; i<queuesSize - 1; i++){

			if(! queues[queues.size() - 2 - i].theQueue.empty()){
				int processes = queues[queues.size() - 2 - i].theQueue.size();
				for(int j=0; j<processes; j++){

					writeOutputOneLine(os, operationName, queues[queues.size() - 2 - i].theQueue.front().processID, topMostName, terminated, processN);
					deleteForBCasesAndEnqueueToTopMost(queues[queues.size() - 2 - i].theQueue, queues[queuesSize - 1].theQueue);
				}
			}
		}
	}
}

void getOperationNameQDestination(int currentQ, process & thePr, ofstream & os, vector<queueWInfo> & allQueues, int & terminatedProcesses, int & processes){ //This function decides on what to do
	string sliceInfo, qDest;                                                                                                                                 //according to slice infos ("0", "1") and is
	int procID = thePr.processID;                                                                                                                            //the slice the last slice of that process 
	if(isTheCurrentLastSlice(thePr)){                                                                                                                        //or not. Process is deleted form the queues
		sliceInfo = "E";                                                                                                                                     //if there is one slice left, if a slice is
		qDest = "QX";                                                                                                                                        //"0" and it is not the last one, than it is
		terminatedProcesses++;                                                                                                                               //popped from that queue and pushed to the 
		writeOutputOneLine(os, sliceInfo, procID, qDest, terminatedProcesses, processes);                                                                    //back of the same queue, and if it is "1" 
		allQueues[currentQ - 1].theQueue.pop();                                                                                                              //and not the last slcie, than that process
	}                                                                                                                                                        //is dropped one queue to the below. This 
	else{                                                                                                                                                    //function also calls the funciton for 
		if(thePr.slices[0] == "1"){                                                                                                                          //writing necessary line to the output.txt.
			sliceInfo = "1";
			qDest = "Q" + to_string(currentQ - 1);
			deleteFirstElemOfVectorProcess(thePr.slices);
			dropOneQueue(procID, allQueues[currentQ - 1], allQueues);
		}
		else{
			sliceInfo = "0";
			qDest = "Q" + to_string(currentQ);
			deleteFirstElemOfVectorProcess(thePr.slices);
			process dummy = allQueues[currentQ - 1].theQueue.front();
			allQueues[currentQ - 1].theQueue.pop();
			allQueues[currentQ - 1].theQueue.push(dummy);
		}
		writeOutputOneLine(os, sliceInfo, procID, qDest, terminatedProcesses, processes);
	}
}

int main(){
	string folder, fileWDirectory, outFWDirectory;
	int queueNum, processNum, S;
	ifstream firstInput;

	cout << "Please enter the process folder name: ";
	cin >> folder;
	cout << "When all processes are completed, you can find execution sequence in" << endl << "\"" << folder << "/" << "output.txt" << "\"" << "." << endl;

	fileWDirectory = folder + "/" + "configuration.txt";
	outFWDirectory = folder + "/" + "output.txt";

	firstInput.open(fileWDirectory.c_str());
	firstInput >> queueNum >> processNum >> S;  //getting number of queues, number of processes, and S.
	firstInput.close();

	vector<queueWInfo> queuesVector(queueNum); //a vector containing structs called queueWInfo which contains queue id and the process queue itself.
	for(int j=0; j<queueNum; j++){             //In each processes, there is process id and avector of string containing slices.
		queuesVector[j].queueID = j + 1;
	}

	for(int i=1; i <= processNum; i++){  //slices, ids are added to the main vector (inside its top most queue).
		ifstream processInput;
		string sliceInfo, fileNameToBeOpen = folder + "/p" + to_string(i) + ".txt";

		processInput.open(fileNameToBeOpen.c_str());

		process theProcess;
		theProcess.processID = i;
		while(processInput >> sliceInfo){
			theProcess.slices.push_back(sliceInfo);
		}
		queuesVector[queueNum - 1].theQueue.push(theProcess); //queuesVector[queueNum - 1] refers to the top most queue.

		processInput.close();
	}

	ofstream outputStream;
	outputStream.open(outFWDirectory.c_str(), ios::app);
	
	bool timeOfS = false;
	int sCount = 0, currentQID, currentPID, k=0, terminatedOnes = 0;

	queue<process> * currentQ;  //some pointers to pass current queues and processes to the functions where necessary.
	currentQ = &queuesVector[queuesVector.size() - 1].theQueue;
	currentQID = queuesVector.size();
	process * currentP;
	currentP = &queuesVector[queuesVector.size() -1].theQueue.front();
	currentPID = queuesVector[queuesVector.size() -1].theQueue.front().processID;

	int sizeCQ = (*currentQ).size();
	while(! isAllTerminated(queueNum, queuesVector)){ //loop stops if there are no processes to handle anymore.
		getOperationNameQDestination(currentQID, *currentP, outputStream, queuesVector, terminatedOnes, processNum);
		sCount++; //this is needed for B cases when S is reached.

		if(! isAllTerminated(queueNum, queuesVector)){
			if(sCount == S){
				timeOfS = true;
			}
			if(timeOfS){ //if S is reached, then processes that are not in the top most queue will be carried to the top most queue
				handleBCases(queuesVector, outputStream, terminatedOnes, processNum);

				timeOfS = false;
				sCount = 0;
				k = 0;

				currentQ = &queuesVector[queuesVector.size() - 1].theQueue; //currentQ becomes the top most queue since all of the processes are there now
				currentQID = queuesVector.size();

				currentP = &queuesVector[queuesVector.size() -1].theQueue.front();  //currentP becomes front of the top most queue
				currentPID = queuesVector[queuesVector.size() -1].theQueue.front().processID;
			}
			else{
				if((* currentQ).empty()){ //if the current queue is empty, we should work on one level below queue starting from its front process.
					k++;
					if(currentQ != &queuesVector[0].theQueue){ //if the current queue is not the Q1, then we can go one level down.
						currentQ = &queuesVector[queuesVector.size() - 1 - k].theQueue;
						currentQID = queuesVector.size() - k;

						currentP = &queuesVector[queuesVector.size() -1 -k].theQueue.front();
						currentPID = queuesVector[queuesVector.size() -1 -k].theQueue.front().processID;
					}
					else{ //if the current q is Q1, we cannot go down anymore so we should work on the same queue, starting form its front.
						currentQ = &queuesVector[queuesVector.size() - 1].theQueue;
						currentQID = queuesVector.size();

						currentP = &queuesVector[queuesVector.size() -1].theQueue.front();
						currentPID = queuesVector[queuesVector.size() -1].theQueue.front().processID;
						k = 0;
					}
				}
				else{ //if the current q is not empty, we will continue to work on the same queue, but we will make current process its front.
					currentP = &(* currentQ).front();
					currentPID = (* currentQ).front().processID;
				}
			}
		}
	}
	outputStream.close();
	return 0;
}