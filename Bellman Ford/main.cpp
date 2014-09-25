#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>

using namespace std;

struct Edge {
	int dest;
	int weight;
	Edge* next; //point to next Edge in list
};

Edge* createEdge(int destV, int weightV) {
	Edge *aEdge = (Edge*) malloc(sizeof(Edge));
	aEdge->dest = destV;
	aEdge->weight = weightV;
	aEdge->next = NULL;

	return aEdge;
}

struct Graph {
	int E,V;
	Edge** edgeList; 
};

Graph* createGraph(int nuEdge, int nuVertex) {
	Graph* aGraph = (Graph*)malloc(sizeof(Graph));
	aGraph->E = nuEdge;
	aGraph->V = nuVertex;
	aGraph->edgeList = (Edge**)malloc(aGraph->V * sizeof(Edge*));
	for (int i = 0; i < aGraph->V; i++) {
		aGraph->edgeList[i] = NULL;
	}
	return aGraph;
}


void insertInList(Edge** edgeList, Edge* insertEdge) {
	Edge* tmp = *edgeList;
	if (tmp == NULL) {
		*edgeList = insertEdge;
		return;
	}

	while(tmp->next != NULL) {
		tmp = tmp->next;
	}

	// tmp is last node
	tmp->next = insertEdge;
}

void addEdge1Way(Graph* aGraph, int i, int j, int weightV) {
	// add both way graph
	Edge* edgei = createEdge(j,weightV); // i-j
	//Edge* edgej = createEdge(i,weightV); //j-i // for 2 ways

	insertInList(&aGraph->edgeList[i],edgei);
	//insertInList(&aGraph->edgeList[j],edgej); //for 2 ways
}


void printGraph(Graph *aGraph) {
	for (int i = 0; i < aGraph->V; i++) {
		cout << i << ": ";
		Edge* tmp = aGraph->edgeList[i];
		while(tmp!= NULL) {
			cout << "-->" << tmp->dest << "&w:"<< tmp->weight << "; ";
			tmp = tmp->next;
		}
		printf("\n");
	}
}

int* BellmanFord(Graph* aGraph, bool* negativeCycle, int srcV) {
	*negativeCycle = false;
	int size = aGraph->V;
	int* distance = (int*)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++) {
		distance[i]= INT_MAX;
	}
	distance[0] = 0;

	int loop = 1;
	while (loop <= (size-1) ) { //loop = size -1
		for (int i = 0; i < size;i ++) {
			Edge* tmp = aGraph->edgeList[i]; //edgeList[i] actually head of list
			while(tmp!= NULL) {
				int v= tmp->dest;
				
				if (distance[v] > distance[i] + tmp->weight)
					distance[v] = distance[i] + tmp->weight;
				
				tmp = tmp->next;
			}
		}
		loop++;
	}

	//run 1 moret time to report negative weight
	for (int i = 0; i < size;i ++) {
		Edge* tmp = aGraph->edgeList[i]; //edgeList[i] actually head of list
		while(tmp!= NULL) {
			int v= tmp->dest;
				
			if (distance[v] > distance[i] + tmp->weight) {
				*negativeCycle = true;
				break;
			}				
			tmp = tmp->next;
		}
	}

	return distance;
	
}


void main() {
	Graph* aGraph = createGraph(8,5);
	addEdge1Way(aGraph,0,1,-1);
	addEdge1Way(aGraph,0,2,4);
	addEdge1Way(aGraph,1,2,3);
	addEdge1Way(aGraph,1,3,2);
	addEdge1Way(aGraph,1,4,2);
	addEdge1Way(aGraph,3,2,5);
	addEdge1Way(aGraph,3,1,2);
	addEdge1Way(aGraph,4,3,-3);

	printGraph(aGraph);

	bool negativeCycle;
	int sourceVertex = 0;
	int* distance = BellmanFord(aGraph,&negativeCycle,sourceVertex);
	for (int i = 0; i < aGraph->V;i ++) {
		cout << distance[i] << " ";
	}
	printf("\n");

	cout << ( (negativeCycle)?"Yes":"No") << "\n";

}