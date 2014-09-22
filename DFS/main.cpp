#include <stdio.h>
#include <stdlib.h>
#include <stack>

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

void DFS(graph *aGraph) {
	bool* visited = (bool*)malloc(aGraph->size * sizeof(bool));
	for (int i = 0; i < aGraph->size;i++) {
		visited[i] = false;
	}
	stack<node*> stackDFS;

	for (int i = 0; i < aGraph->size;i++) { //this loop covers case many forest in graph
		if (!visited[i]) {
			stackDFS.push(aGraph->head[i]);
			
			node* firstChain = stackDFS.top();
			printf("%d " , firstChain->data);
			visited[firstChain->data] = true;
			stackDFS.pop();

			while( (firstChain->next != NULL) && (!visited[firstChain->next->data])) {
				stackDFS.push(firstChain->next);
				visited[firstChain->next->data] = true;
				firstChain = firstChain->next;
			}
			
			while (!stackDFS.empty()) {
				node* tmp = stackDFS.top();
				stackDFS.pop();

				printf("%d " , tmp->data);
				
				while((tmp->next != NULL) && (!visited[tmp->next->data])) {
					stackDFS.push(tmp->next);
					visited[tmp->next->data] = true;
					tmp = tmp->next;
				}
			}
		}
	}
}

// DFS to detect connected graph
bool DFSCycle(graph *aGraph) {
	bool* visited = (bool*)malloc(aGraph->size * sizeof(bool));
	for (int i = 0; i < aGraph->size;i++) {
		visited[i] = false;
	}
	stack<node*> stackDFS;

	for (int i = 0; i < aGraph->size;i++) { //this loop covers case many forest in graph
		if (!visited[i]) {
			stackDFS.push(aGraph->head[i]);
			
			node* firstChain = stackDFS.top();
			//printf("%d " , firstChain->data);
			visited[firstChain->data] = true;
			stackDFS.pop();

			while (firstChain->next != NULL) {
				if  (!visited[firstChain->next->data]) {
					stackDFS.push(aGraph->head[firstChain->next->data]); //very important since we 
					visited[firstChain->next->data] = true;
					firstChain = firstChain->next;
				}
				else {
					return true;
				}
			}
			
			while (!stackDFS.empty()) {
				node* tmp = stackDFS.top();
				stackDFS.pop();

				//printf("%d " , tmp->data);
				
				while(tmp->next != NULL) {
					if (!visited[tmp->next->data]) {
						stackDFS.push(tmp->next);
						visited[tmp->next->data] = true;
						tmp = tmp->next;
					}
					else {
						return true;
					}
				}
			}
		}
	}
	return false;
}


void main() {
	graph* aGraph = createGraph(4);
	addEdge1Way(aGraph,0,1);
	addEdge1Way(aGraph,0,2);
	addEdge1Way(aGraph,1,2);
	addEdge1Way(aGraph,2,0);
	addEdge1Way(aGraph,2,3);
	addEdge1Way(aGraph,3,3);

	//addEdge1Way(aGraph,0,1);
	//addEdge1Way(aGraph,0,2);
	//addEdge1Way(aGraph,2,3);

	printf("Graph: \n");
	printGraph(aGraph);

	printf("dfs travel: \n");
	DFS(aGraph);
	printf("\n");

	bool cycle = DFSCycle(aGraph);
	printf((cycle) ? "true" : "false");
	printf("\n");
}