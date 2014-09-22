#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class Edge {
private:
	int src, dest;
	int weight;
public:
	Edge(int srcV, int desV, int weightW);
	void setSrc(int srcV);
	void setDes(int destV);
	void setWeight(int weightW);
	int getSrc();
	int getDes();
	int getWeight();
};

Edge::Edge(int srcV, int desV, int weightW) {
		setSrc(srcV);
		setDes(desV);
		setWeight(weightW);
}

void Edge::setSrc(int srcV) { src = srcV; }
void Edge::setDes(int desV) { dest = desV; }
void Edge::setWeight(int weightW) { weight = weightW; }
int Edge::getSrc() { return src;};
int Edge::getDes() { return dest;};
int Edge::getWeight() { return weight;};


class Graph {
public:
	int E, V; // number of Edges and Vertex
	vector<Edge>* arrEdge; // all 
	
	Graph(int nuE, int nuV);
	void addEdge(int srcV, int desV, int weight);
	void printGraph();
};

Graph::Graph(int nuE, int nuV) {
	E = nuE;
	V = nuV;
	arrEdge = new vector<Edge>[V];
}

void Graph::addEdge(int srcV,int desV, int weight) {
	Edge aEdge = Edge(srcV, desV, weight);
	arrEdge[srcV].push_back(aEdge);

	Edge aEdgeD = Edge(desV,srcV,weight);
	arrEdge[desV].push_back(aEdgeD);
}

void Graph::printGraph() {
	for (int i = 0; i < V;i++) {
		cout << i << ": ";
		vector<Edge>::iterator aEdge;
		for(aEdge = arrEdge[i].begin(); aEdge != arrEdge[i].end(); aEdge++) {
			cout << (*aEdge).getSrc() << "-->" << (*aEdge).getDes() << " w:" << (*aEdge).getWeight() << "; " ;
		}
		cout << "\n";
	}
}

int postMinList(int* listInt, bool* listBool, int size) {
	int smallIndex = INT_MIN;
	int smallValue = INT_MAX;
	for (int i = 0; i < size; i++) {
		if(!listBool[i]) {
			if(listInt[i] < smallValue) {
				smallIndex = i;
				smallValue = listInt[i];
			}
		}
	}
	return smallIndex;
}

Edge indexOfOtherForest(Graph *aGraph, int src, bool* listBool, int size) {
	int index = INT_MIN;
	int minWeight = INT_MAX;
	

	vector<Edge>::iterator aEdge;
	for(aEdge = aGraph->arrEdge[src].begin(); aEdge != aGraph->arrEdge[src].end(); aEdge++) {
		if(listBool[aEdge->getDes()] && (minWeight > aEdge->getWeight()) ) {
			index = aEdge->getDes();
			minWeight = aEdge->getWeight();
		}
		
	}
	Edge returnEdge = Edge(src,index,minWeight);
	return returnEdge;
}

vector<Edge> PrimMST(Graph  *aGraph) {
	vector<Edge> result; 
	int *MSTValue = (int*) malloc(aGraph->V * sizeof(int));
	bool* MSTSet = (bool*) malloc(aGraph->V * sizeof(bool));
	int *MSTParent = (int*) malloc(aGraph->V * sizeof(int));

	for (int i = 0; i < aGraph->V; i++) {
		MSTSet[i] = false;
		MSTParent[i] = -1;
		MSTValue[i] = INT_MAX;
	}

	int currentIndex = 0;
	int currentMaxValue = INT_MAX;
	int nextIndex = 0;
	int srcIndex = currentIndex;

	MSTValue[currentIndex] = 0; // first vertex in MST;
	MSTSet[currentIndex] = true;

	while(result.size() < aGraph->V - 1) {
		//get min from MST
		vector<Edge>::iterator aEdge;
		for(aEdge = aGraph->arrEdge[currentIndex].begin(); aEdge != aGraph->arrEdge[currentIndex].end(); aEdge++) {
			if (!MSTSet[(*aEdge).getDes()] ) {
				if ((MSTValue[(*aEdge).getDes()] == INT_MAX) || (MSTValue[(*aEdge).getDes()] > (*aEdge).getWeight()) ) {
					MSTValue[(*aEdge).getDes()] = (*aEdge).getWeight();	
					if ( (*aEdge).getWeight() < currentMaxValue) {
						currentMaxValue = (*aEdge).getWeight();
						nextIndex = aEdge->getDes();
					}

				}
			}	
		}

		currentIndex = postMinList(MSTValue,MSTSet,aGraph->V);
		Edge connectEdge = indexOfOtherForest(aGraph,currentIndex,MSTSet,aGraph->V);
		MSTSet[connectEdge.getDes()] = true;
		MSTSet[connectEdge.getSrc()] = true;
		result.push_back(connectEdge);	
		/*
		if (currentMaxValue != INT_MAX) { //actually found potential Index. Else mean no update
			Edge MSTEdge = Edge(currentIndex,nextIndex,currentMaxValue);
			result.push_back(MSTEdge);	
			MSTSet[currentIndex] = true;

			currentIndex = postMinList(MSTValue,MSTSet,aGraph->V);
			MSTSet[currentIndex] = true;
		}
		else {
			MSTValue[currentIndex] = INT_MAX;
			currentIndex = postMinList(MSTValue,MSTSet,aGraph->V);

			//connect two forests
			Edge connectEdge = indexOfOtherForest(aGraph,currentIndex,MSTSet,aGraph->V);
			result.push_back(connectEdge);	
			MSTSet[connectEdge.getDes()] = true;
			MSTSet[connectEdge.getSrc()] = true;
		}
		*/
				
		currentMaxValue = INT_MAX;
	}
	return result;
}

void printPRIM(vector<Edge> *lstEdge) {
	vector<Edge>::iterator aEdge;
	cout << "MST: \n";
	for(aEdge = lstEdge->begin(); aEdge!= lstEdge->end(); aEdge++) {
		cout << aEdge->getSrc() << "-->" << aEdge->getDes() << "\n";
	}
}

void main () {
	Graph aGraph = Graph(14, 9);
	aGraph.addEdge(0,1,4);
	aGraph.addEdge(0,7,8);
	aGraph.addEdge(1,7,11);
	aGraph.addEdge(1,2,8);
	aGraph.addEdge(7,8,7);
	aGraph.addEdge(7,6,1);
	aGraph.addEdge(2,8,2);
	aGraph.addEdge(8,6,6);
	aGraph.addEdge(2,3,7);
	aGraph.addEdge(2,5,4);
	aGraph.addEdge(6,5,2);
	aGraph.addEdge(3,4,9);
	aGraph.addEdge(3,5,14);
	aGraph.addEdge(4,5,10);

	aGraph.printGraph();

	vector<Edge> mst = PrimMST(&aGraph);
	printPRIM(&mst);
}


