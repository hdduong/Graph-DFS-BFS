#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	node* next;
};


// 0->1->2
// 1->4->5
//
//

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

void main() {
	graph* aGraph = createGraph(5);
	addEdge(aGraph,0,1);
	addEdge(aGraph,0,4);
	addEdge(aGraph, 1,4);
	addEdge(aGraph,1,2);
	addEdge(aGraph,1,3);
	addEdge(aGraph,2,3);
	addEdge(aGraph,3,4);

	printGraph(aGraph);
}