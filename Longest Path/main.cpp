// C++  style

#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <iostream>

using namespace std;

class edge {
public:
	int src, dest;
	int weight;
	edge(int src, int dest, int weight);

};

edge::edge(int srcV, int destV, int weightE) {
	src = srcV;
	dest = destV;
	weight = weightE;
}

class Graph {
public:
	int V; //number of Edge and Vertex
	list<edge> *adjacentList;
	
	Graph(int V);
	void addEdge(int src, int dest, int weight);
	void printGraph();
};

Graph::Graph(int nuVertex) {
	V = nuVertex;
	adjacentList = new list<edge>[V];
}

void Graph::addEdge(int srcV, int destV, int weightE) {
	edge newEdge = edge(srcV, destV, weightE);
	adjacentList[srcV].push_back(newEdge);
}

void Graph::printGraph() {
	for (int i = 0; i < V;i++) {
		printf("%d: ", i);	
		list<edge>::iterator aEdge;
		for (aEdge = adjacentList[i].begin(); aEdge!=adjacentList[i].end(); aEdge++) {
			printf("%d-->%d-w:%d ",aEdge->src, aEdge->dest, aEdge->weight);
		}
		printf("\n");
	}
}

void DFSUntil(Graph *aGraph, bool* visited, list<int> &topoList, int v) {
	visited[v] = true;

	list<edge>::iterator aEdge;
	for (aEdge = aGraph->adjacentList[v].begin(); aEdge!= aGraph->adjacentList[v].end(); aEdge++) {
		if (!visited[aEdge->dest]) {
			DFSUntil(aGraph, visited, topoList, aEdge->dest);
		}
	}
	topoList.push_back(v);
}

list<int> DFSTopo(Graph *aGraph) {
	bool* visited = (bool*)malloc(sizeof(bool) * aGraph->V);
	for (int i = 0; i < aGraph->V;i++)
		visited[i] = false;

	list<int> topo; // store tooplogical sort

	for (int v = 0; v < aGraph->V; v++) {
		if (!visited[v]) {
			DFSUntil(aGraph, visited, topo, v);				
		}
	}

	topo.reverse();
	return topo;
}

int* longestPath(list<int> &topoList, Graph* aGraph, int s) { // longest path from s node
	int* distance = (int*)malloc(sizeof(int) * topoList.size());
	for (int i = 0; i < topoList.size(); i++) {
		distance[i] = INT_MIN;
	}
	distance[s] = 0;

	list<int>::iterator topo;
	for (topo = topoList.begin(); topo != topoList.end(); topo++) {
		if(distance[*topo] != INT_MIN) {  // Important since no need to calculate before s in topo since s cannot reach those
			list<edge>::iterator aEdge;
		
			for (aEdge = aGraph->adjacentList[*topo].begin(); aEdge!= aGraph->adjacentList[*topo].end(); aEdge++) {
				if(distance[aEdge->dest] < distance[aEdge->src] + aEdge->weight) {
					distance[aEdge->dest] = distance[aEdge->src] + aEdge->weight;
				}
			}
		}
	}

	return distance;
}


void main() {
	Graph aGraph = Graph(4);
	aGraph.addEdge(0,2,1);
	aGraph.addEdge(0,3,3);
	aGraph.addEdge(2,3,1);
	aGraph.addEdge(3,1,2);

	aGraph.printGraph();
	printf("\n");

	list<int> topo = DFSTopo(&aGraph);
	list<int>::iterator i;
	for(i = topo.begin(); i != topo.end(); i++) {
		cout << *i << " "; // iterator with * to get value
	}
	cout << "\n";

	int* distance = longestPath(topo,&aGraph,2); 
	for (int i = 0; i < topo.size(); i++) {
		cout << distance[i] << " ";
	}
	cout << "\n";
}