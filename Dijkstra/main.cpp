#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>

using namespace std;
struct Edge {
	int src, dest, weight;
};

struct Graph {
	int E, V;
	///list<Edge> *arrEdge;
	list<Edge*> *arrEdge;
};

Edge* createEdge(int srcV, int destV, int weightV) {
	Edge* aEdge = (Edge*)malloc(sizeof(Edge));
	aEdge->src = srcV;
	aEdge->dest = destV;
	aEdge->weight = weightV;

	return aEdge;
}

Graph* createGraph(int nuE, int nuV) {
	Graph* aGraph = (Graph*)malloc(sizeof(Graph));
	aGraph->E = nuE;
	aGraph->V = nuV;

	// NEVER USE MALLOC FOR list
	//aGraph->arrEdge = (list<Edge>*)malloc(sizeof(list<Edge>) * nuV);
	//for (int i = 0; i < nuV; i++) {
	//	aGraph->arrEdge[i] = list<Edge>();
	//}

	///aGraph->arrEdge = new list<Edge>[nuV];
	aGraph->arrEdge = new list<Edge*>[nuV];
	return aGraph;
}


void addEdge(Graph* aGraph, int srcV, int desV, int weightV) {
	Edge *aEdge = createEdge(srcV,desV, weightV);
	aGraph->arrEdge[srcV].push_back(aEdge);

	Edge *rEdge = createEdge(desV,srcV,weightV);
	aGraph->arrEdge[desV].push_back(rEdge);
}

void printGraph(Graph *aGraph) {
	for (int i = 0; i < aGraph->V;i++) {
		list<Edge*>::iterator aEdge;
		for(aEdge = aGraph->arrEdge[i].begin(); aEdge != aGraph->arrEdge[i].end(); aEdge++) {
			cout << (*aEdge)->src << "-->" << (*aEdge)->dest << " w:" << (*aEdge)->weight << "; " ;
		}
		cout << "\n";
	}
}

bool isSPTFull(bool* sptSet, int size) {
	bool containAll = true;
	for (int i = 0; i < size; i++) {
		if (sptSet[i] == false) {
			containAll = false;
			break;
		}
	}
	return containAll;
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

int* Dijkstra(Graph *aGraph, int src) {
	bool* sptSet = (bool*) malloc(sizeof(bool) * aGraph->V);
	int* distance = (int*) malloc(sizeof(int)* aGraph->V);
	for (int i = 0; i < aGraph->V; i++) {
		sptSet[i] = false;
		distance[i] = INT_MAX;
	}

	sptSet[src] = true;
	distance[src] = 0;
	int u = src;

	while(!isSPTFull(sptSet, aGraph->V)) {
		
		list<Edge*>::iterator aEdge;
		for(aEdge = aGraph->arrEdge[u].begin(); aEdge != aGraph->arrEdge[u].end(); aEdge++) {
			 int v = (*aEdge)->dest; // adjacent vertex
			 if (distance[v] > distance[u] + (*aEdge)->weight)
				 distance[v] = distance[u] + (*aEdge)->weight;
		}

		u = postMinList(distance,sptSet,aGraph->V);
		sptSet[u] = true;
	}
	return distance;
}

void main() {
	Graph* aGraph = createGraph(14, 9);
	addEdge(aGraph,0,1,4);
	addEdge(aGraph,0,7,8);
	addEdge(aGraph,1,7,11);
	addEdge(aGraph,1,2,8);
	addEdge(aGraph,7,8,7);
	addEdge(aGraph,7,6,1);
	addEdge(aGraph,2,8,2);
	addEdge(aGraph,8,6,6);
	addEdge(aGraph,2,3,7);
	addEdge(aGraph,2,5,4);
	addEdge(aGraph,6,5,2);
	addEdge(aGraph,3,4,9);
	addEdge(aGraph,3,5,14);
	addEdge(aGraph,4,5,10);

	printGraph(aGraph);

	int* distance = Dijkstra(aGraph,0);
	for (int i = 0; i < aGraph->V; i++) {
		cout << distance[i] << " ";
	}
	cout << "\n";
}