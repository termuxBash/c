

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int vertex;
    struct Node *next;
} Node;

typedef struct Graph {
    int numVertices;
    Node **adjLists;
} Graph;

Node *createNode(int vertex)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        exit(1);
    }
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int vertices)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (graph == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        exit(1);
    }

    graph->numVertices = vertices;
    graph->adjLists = (Node **)malloc(vertices * sizeof(Node *));
    if (graph->adjLists == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        free(graph);
        exit(1);
    }

    
    for (int i = 0; i < vertices; i++)
    {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

int isValidVertex(Graph *graph, int vertex)
{
    return (graph != NULL && vertex >= 0 && vertex < graph->numVertices);
}

void addEdge(Graph *graph, int src, int dest)
{
    if (!isValidVertex(graph, src) || !isValidVertex(graph, dest))
    {
        printf("[Error] Invalid vertices (%d, %d)!\n", src, dest);
        return;
    }

    
    Node *newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void displayGraph(Graph *graph)
{
    if (graph == NULL)
    {
        printf("Graph is empty.\n");
        return;
    }

    printf("\nAdjacency List (%d vertices):\n", graph->numVertices);
    for (int i = 0; i < graph->numVertices; i++)
    {
        Node *temp = graph->adjLists[i];
        printf("Vertex %2d: [head]", i);
        while (temp != NULL)
        {
            printf(" -> %d", temp->vertex);
            temp = temp->next;
        }
        printf(" -> NULL\n");
    }
    printf("\n");
}

void dfsRec(Graph *graph, int vertex, int visited[])
{
    visited[vertex] = 1;
    printf("%d ", vertex);

    Node *temp = graph->adjLists[vertex];
    while (temp != NULL)
    {
        int neighbor = temp->vertex;
        if (!visited[neighbor])
        {
            dfsRec(graph, neighbor, visited);
        }
        temp = temp->next;
    }
}

void DFS(Graph *graph, int startVertex)
{
    if (!isValidVertex(graph, startVertex))
    {
        printf("[Error] Invalid starting vertex %d!\n", startVertex);
        return;
    }

    int *visited = (int *)calloc(graph->numVertices, sizeof(int));
    if (visited == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        return;
    }

    printf("DFS Traversal starting from %d: ", startVertex);
    dfsRec(graph, startVertex, visited);
    printf("\n");

    free(visited);
}

void BFS(Graph *graph, int startVertex)
{
    if (!isValidVertex(graph, startVertex))
    {
        printf("[Error] Invalid starting vertex %d!\n", startVertex);
        return;
    }

    int *visited = (int *)calloc(graph->numVertices, sizeof(int));
    int *queue = (int *)malloc(graph->numVertices * sizeof(int));
    if (visited == NULL || queue == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        free(visited);
        free(queue);
        return;
    }

    int front = 0;
    int rear = 0;

    
    visited[startVertex] = 1;
    queue[rear++] = startVertex;

    printf("BFS Traversal starting from %d: ", startVertex);

    while (front < rear)
    {
        int curr = queue[front++];
        printf("%d ", curr);

        
        Node *temp = graph->adjLists[curr];
        while (temp != NULL)
        {
            int neighbor = temp->vertex;
            if (!visited[neighbor])
            {
                visited[neighbor] = 1;
                queue[rear++] = neighbor;
            }
            temp = temp->next;
        }
    }
    printf("\n");

    free(visited);
    free(queue);
}

void freeGraph(Graph *graph)
{
    if (graph == NULL)
        return;

    for (int i = 0; i < graph->numVertices; i++)
    {
        Node *temp = graph->adjLists[i];
        while (temp != NULL)
        {
            Node *toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    free(graph->adjLists);
    free(graph);
}

int main()
{
    printf("=== GRAPH DEMO (ADJACENCY LIST) ===\n");

    
    Graph *graph = createGraph(5);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);
    addEdge(graph, 2, 4);

    displayGraph(graph);

    
    DFS(graph, 0);
    BFS(graph, 0);

    freeGraph(graph);
    return 0;
}
