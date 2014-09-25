#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>


using namespace std;
struct Edge {
	int dest;
};

struct Graph {
	int E, V;
	///list<Edge> *arrEdge;
	list<Edge*> *arrEdge;
};

Edge* createEdge(int srcV, int destV) {
	Edge* aEdge = (Edge*)malloc(sizeof(Edge));
	aEdge->dest = destV;
	//aEdge->weight = weightV;

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


void addEdge(Graph* aGraph, int srcV, int desV) {
	Edge *aEdge = createEdge(srcV,desV);
	aGraph->arrEdge[srcV].push_back(aEdge);

	Edge *rEdge = createEdge(desV,srcV);
	aGraph->arrEdge[desV].push_back(rEdge);
}

void addEdge1Way(Graph* aGraph, int srcV, int desV) {
	Edge *aEdge = createEdge(srcV,desV);
	aGraph->arrEdge[srcV].push_back(aEdge);
}

void printGraph(Graph *aGraph) {
	for (int i = 0; i < aGraph->V;i++) {
		list<Edge*>::iterator aEdge;
		for(aEdge = aGraph->arrEdge[i].begin(); aEdge != aGraph->arrEdge[i].end(); aEdge++) {
			cout << i << "-->" << (*aEdge)->dest << ";";//" w:" << (*aEdge)->weight << "; " ;
		}
		cout << "\n";
	}
}


Graph* tranposeGraph(Graph *aGraph) {
	Graph* newGraph = createGraph(aGraph->E, aGraph->V);
	for (int i = 0; i < aGraph->V;i++) {
		list<Edge*>::iterator aEdge;
		for(aEdge = aGraph->arrEdge[i].begin(); aEdge != aGraph->arrEdge[i].end(); aEdge++) {
			int v = (*aEdge)->dest;
			addEdge1Way(newGraph,v,i);
		}
	}
	return newGraph;
}

void DFS(Graph *aGraph,bool* visited, list<int> *travel, int srcVertex) {
	visited[srcVertex]= true;

	list<Edge*>::iterator aEdge;
	for(aEdge = aGraph->arrEdge[srcVertex].begin(); aEdge != aGraph->arrEdge[srcVertex].end(); aEdge++) {
		int v = (*aEdge)->dest;
		if (!visited[v])
			DFS(aGraph,visited, travel, v);
	}
	travel->push_back(srcVertex);
}


void DFSUntil(Graph *aGraph,int* reverseVisited, int srcVertex, int mark) {
	reverseVisited[srcVertex]= mark;

	list<Edge*>::iterator aEdge;
	for(aEdge = aGraph->arrEdge[srcVertex].begin(); aEdge != aGraph->arrEdge[srcVertex].end(); aEdge++) {
		int v = (*aEdge)->dest;
		if (reverseVisited[v] == 0)
			DFSUntil(aGraph,reverseVisited, v, mark);
	}
}

// result = 0: Not connected
//		  = 1: Connected	
int printStronglyConnectedComponent(Graph* aGraph) {
	int result = 0; 

	list<int> *travel = new list<int>[aGraph->V];

	int srcVertex = 0; // choose source Vertex
	bool* visited = (bool*)malloc(aGraph->V * sizeof(bool));
	int* reverseVisited = (int*)malloc(aGraph->V * sizeof(int));
	for (int i = 0l; i < aGraph->V;i++) {
		visited[i] = false;
		reverseVisited[i] = 0;
	}

	DFS(aGraph,visited,travel,srcVertex);
	
	//check connected or not
	for (int i = 0; i < aGraph->V;i++) {
		if(!visited[i]) {
			result = 0;
			return result;
		}
	}

	Graph* newGraph = tranposeGraph(aGraph);

	result = 1; 
	int mark = 1;
	while(!travel->empty()) {
		int u = travel->back();
		if(reverseVisited[u] == 0) {
			travel->pop_back();
					
			reverseVisited[u] = mark;
			DFSUntil(newGraph,reverseVisited, u, mark);
		
			mark++;
		} else { 
			travel->pop_back();
		}
	}
	for (int k = 1; k <= mark; k++) {
		for (int i = 0; i < aGraph->V; i++) {
			if (reverseVisited[i] == k)
				cout << i << " ";
		}
		cout << "\n";
	}
}


void main() {
	Graph* aGraph = createGraph(5, 5);
	addEdge1Way(aGraph,0,2);
	addEdge1Way(aGraph,0,3);
	addEdge1Way(aGraph,1,0);
	addEdge1Way(aGraph,2,1);
	addEdge1Way(aGraph,3,4);
	
	printGraph(aGraph);
	cout << "\n";
	
	Graph* newGraph = tranposeGraph(aGraph);
	printGraph(newGraph);
	cout << "\n";

	int result = printStronglyConnectedComponent(aGraph);

	for (int i = 0; i < aGraph->V; i++) {
	
	}
	cout << "\n";
}