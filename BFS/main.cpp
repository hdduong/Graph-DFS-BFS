#include <stdio.h>
#include <stdlib.h>
#include <queue>

using namespace std;

struct node {
	int data;
	node* next;
};

struct graph {
	int size; //number of vertex in graph
	node** head; // number of head vertex
};

node* newNode(int data) {
	node* aNewNode = (node*) malloc(sizeof(node));
	aNewNode->data = data;
	aNewNode->next = NULL;

	return aNewNode;
}

void addEdge(graph* graph, int i, int j) {
	// add both way graph
	node* nodei = newNode(i);
	node* nodej = newNode(j);

	nodej->next= graph->head[i]->next;
	graph->head[i]->next = nodej;
	
	nodei->next = graph->head[j]->next;
	graph->head[j]->next = nodei;
}
void addEdge1Way(graph* graph, int i, int j) {
	// add both way graph
	node* nodei = newNode(i);
	node* nodej = newNode(j);

	nodej->next= graph->head[i]->next;
	graph->head[i]->next = nodej;
	
}


graph* createGraph(int size) {
	graph* aNewGraph = (graph*) malloc(sizeof(graph));
	aNewGraph->size = size;

	aNewGraph->head = (node**) malloc(aNewGraph->size * sizeof(node*));
	for (int i = 0; i < aNewGraph->size; i++) {
		aNewGraph->head[i] = newNode(i);
	}
	return aNewGraph;
}

void printGraph(graph *aGraph) {
	for (int i = 0; i < aGraph->size; i++) {
		node* tmp = aGraph->head[i];
		while(tmp!= NULL) {
			printf("%d ", tmp->data);
			tmp=tmp->next;
		}
		printf("\n");
	}
}

void BFS(graph *aGraph, int startVertex) {
	bool* visited = (bool*)malloc(aGraph->size * sizeof(bool));
	for (int i = 0; i < aGraph->size;i++) {
		visited[i] = false;
	}

	queue<node*> queueBFS;

	queueBFS.push(aGraph->head[startVertex]);

	while(!queueBFS.empty()) {
		node* tmp = queueBFS.front();
		visited[tmp->data] = true; // already visited
		printf("%d ", tmp->data);

		node* travel = aGraph->head[tmp->data];
		while(travel != NULL) {
			travel = travel->next;
			if( (travel!=NULL) && (!visited[travel->data]) ) {
				queueBFS.push(travel);
			}
		}
		queueBFS.pop();
	}
	
}


void main() {
	graph* aGraph = createGraph(4);
	addEdge1Way(aGraph,0,1);
	addEdge1Way(aGraph,0,2);
	addEdge1Way(aGraph,1,2);
	addEdge1Way(aGraph,2,0);
	addEdge1Way(aGraph,2,3);
	addEdge1Way(aGraph,3,3);

	printGraph(aGraph);

	BFS(aGraph,2);
}