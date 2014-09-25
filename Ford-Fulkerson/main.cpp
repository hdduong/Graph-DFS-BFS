#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <list>

using namespace std;

// Graph is 2D matrix
struct Graph {
	int E, V;
	int **adjMatrix;
};


//prototype
void printPath(list <int> *path);

// also add edge here too
Graph* createGraph(int E, int V) {
	Graph *aGraph = (Graph*)malloc(sizeof(Graph));
	aGraph->E = E;
	aGraph->V  = V;

	aGraph->adjMatrix = (int**)malloc(V* sizeof(int*));

	for (int i = 0; i < V; i++) {
		aGraph->adjMatrix[i] = (int*)malloc(V*sizeof(int));
	}

	for (int i = 0; i < aGraph->V; i++) {
		for (int j = 0; j < aGraph->V; j ++) {
			aGraph->adjMatrix[i][j]  = 0;
		}
	}

	return aGraph;
}

void addDirectedEdge(Graph *aGraph, int srcV, int destV, int weight) {
	aGraph->adjMatrix[srcV][destV] = weight;
}

void printGraph(Graph *aGraph) {
	for (int i = 0; i < aGraph->V; i++) {
		for (int j = 0; j < aGraph->V; j ++) {
			cout << aGraph->adjMatrix[i][j] << " ";
		}
		cout << "\n";
	}
}

Graph* copyGraph(Graph* aGraph) {
	Graph* newGraph =  (Graph*)malloc(sizeof(Graph));

	newGraph->E = aGraph->E;
	newGraph->V  = aGraph->V;

	newGraph->adjMatrix = (int**)malloc(aGraph->V* sizeof(int*));
	for (int i = 0; i < aGraph->V; i++) {
		newGraph->adjMatrix[i] = (int*)malloc(aGraph->V*sizeof(int));
	}

	for (int i = 0; i < aGraph->V; i++) {
		for (int j = 0; j < aGraph->V; j ++) {
			newGraph->adjMatrix[i][j]  = 0;
		}
	}

	for (int i = 0; i < aGraph->V; i++) {
		for (int j = 0; j < aGraph->V; j ++) {
			newGraph->adjMatrix[i][j]  = aGraph->adjMatrix[i][j];
		}
	}

	return newGraph;
}

// return true of a path from source to destination available
bool DFS(Graph *aGraph, int srcV, int desV, list<int> *path, bool* visited, int size) {

	bool hasPath = false;
	visited[srcV] = true;

	for (int i = 0; i < aGraph->V; i++) {
		
		if ( (!visited[i]) && 
			(aGraph->adjMatrix[srcV][i] > 0)) {
				if (i == desV) { 
					path->push_back(i);
					hasPath = true; 
					break; 
				}
				else {
					hasPath = DFS(aGraph,i,desV, path,visited,size);
					if (hasPath) break;
				}
		}
	}
	path->push_back(srcV);
	return hasPath;
}

void cleanPath(list<int>* currentPath) {
	while(!currentPath->empty()) {
		currentPath->pop_back();
	}
}

void cleanVisit(bool* visited, int size) {
	for (int i = 0; i < size; i++) {
		visited[i]  = false;
	}
}

void DFSPath(Graph *aGraph, int srcV, int desV, bool* visited, int size, list<list<int>> *allPath, list<int> *currentPath) {

	
	currentPath->push_back(srcV);
	visited[srcV] = true;

	for (int i = 0; i < aGraph->V; i++) {
		
		if ( (!visited[i]) && 
			(aGraph->adjMatrix[srcV][i] > 0)) {
				
				if (i == desV) { 
					currentPath->push_back(i);
					allPath->push_back(*currentPath); //copy created when push_back
					// printPath(currentPath); 

					//back tracking steps
					//visited[desV] = false;
					//currentPath->pop_back();
				}
				else {
					DFSPath(aGraph, i, desV, visited, size, allPath,currentPath);
					
					//back tracking steps
					//visited[i] = false;
					//currentPath->pop_back();
				}
				//back tracking steps
				visited[i] = false;
				currentPath->pop_back();
				
		}
		
	}

	
	
}


int getMinFlowPath(Graph *aGraph, list<int> examinePath) {
	int min = INT_MAX;
	while(!examinePath.empty()) {
			int tmpSrc = examinePath.front();
			examinePath.pop_front();

			if (!examinePath.empty()) {
				int tmpDes = examinePath.front();
				if (min >aGraph->adjMatrix[tmpSrc][tmpDes])
					min = aGraph->adjMatrix[tmpSrc][tmpDes];
				
			}
		}
	return min;
}

void printPath(list <int> *path) {
	list<int>::iterator aEdge;
		for (aEdge = path->begin(); aEdge != path->end(); aEdge++) {
			cout << *aEdge << " ";
		}
		cout << "\n";
}

void printAllPath(list<list<int>> *allPath) {
	list<list<int>>::iterator aPath;
	for (aPath = allPath->begin(); aPath != allPath->end(); aPath++) {
		printPath(&*aPath);
	}
}


int FolkFulkerson(Graph* aGraph, int src, int dest) {
	list<int> path;
	int maxFlow = 0;
	
	Graph* residualGraph = copyGraph(aGraph);
	
	int size = residualGraph->V;
	bool* visited = (bool*)malloc(sizeof(bool) * size);
	for (int i = 0; i < aGraph->V; i++) {
		visited[i] = false;
	}
	
	list<list<int>> allPath;
	DFSPath(aGraph, src, dest, visited, size, &allPath, &path);

	//loop allPath
	//list<list<int>>::iterator aFullPath;
	//for (aFullPath = allPath.begin(); aFullPath != allPath.end(); aFullPath++) {
	while(!allPath.empty()) {
		//list<int> aFullPath = allPath.front();
		list<int> examinePath = allPath.front();
		
		allPath.pop_front();

		int minFlow = getMinFlowPath(residualGraph, examinePath);
		if (minFlow == 0) continue; // another path
		
		// minFlow > 0
		while(!examinePath.empty()) {
			int tmpSrc = examinePath.front();
			examinePath.pop_front();

			if (!examinePath.empty()) {
				int tmpDes = examinePath.front();
				residualGraph->adjMatrix[tmpSrc][tmpDes] -= minFlow;			
				residualGraph->adjMatrix[tmpDes][tmpSrc] += minFlow;
			}
		}

		//printPath(&*aFullPath);
		
		//getchar();
		for (int i = 0; i < aGraph->V; i++) {
			visited[i] = false;
		}	
		cleanPath(&path);
		DFSPath(residualGraph, src, dest, visited, size, &allPath, &path);
	}

	cout << "Matrix: \n";
	printGraph(residualGraph);

	for (int i = 0; i < aGraph->V; i++) {
		maxFlow +=	residualGraph->adjMatrix[i][0];
	}
	


	return maxFlow;
}

void main() {
	Graph* aGraph = createGraph(10,6);
	addDirectedEdge(aGraph,0,1,16);
	addDirectedEdge(aGraph,0,2,13);
	addDirectedEdge(aGraph,2,1,4);
	addDirectedEdge(aGraph,1,2,10);
	addDirectedEdge(aGraph,1,3,12);
	addDirectedEdge(aGraph,2,4,14);
	addDirectedEdge(aGraph,3,2,9);
	addDirectedEdge(aGraph,3,2,9);
	addDirectedEdge(aGraph,4,3,7);
	addDirectedEdge(aGraph,3,5,20);
	addDirectedEdge(aGraph,4,5,4);

	printGraph(aGraph);
	cout << "\n";

	
	Graph* residualGraph = copyGraph(aGraph);
	
	printGraph(residualGraph);
	cout << "\n";
	
	list<int> path;
	int src = 0;
	int des = 5;
	
	//int maxFlow = FolkFulkerson(aGraph,src,des);

	
	int size = aGraph->V;
	bool* visited = (bool*)malloc(sizeof(bool) * size);
	for (int i = 0; i < aGraph->V; i++) {
		visited[i] = false;
	}
	
	//list<list<int>> allPath;
	//DFSPath(aGraph, src, des, visited, size, &allPath, &path);
	//printAllPath(&allPath);

	/* 
	if (hasPath) {
		list<int>::iterator aEdge;
		for (aEdge = path.begin(); aEdge != path.end(); aEdge++) {
			cout << *aEdge << " ";
		}
	} else {
		list<int>::iterator aEdge;
		for (aEdge = path.begin(); aEdge != path.end(); aEdge++) {
			cout << *aEdge << " ";
		}
	}
	*/

	int maxFlow = FolkFulkerson(aGraph,src,des);
	cout << maxFlow;
	cout << "\n";



}