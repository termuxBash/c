/**
 * ============================================================================
 * DATA STRUCTURE: GRAPH (Adjacency List Representation)
 * ============================================================================
 * 
 * DESCRIPTION:
 * An Adjacency List represents a graph as an array of linked lists.
 * The size of the array is equal to the number of vertices V.
 * - adjList[i] is the head of a linked list containing all neighbor vertices 
 *   directly connected to vertex i.
 * 
 * EXAM POINTERS & TRADE-OFFS:
 * Advantages:
 * - Highly memory-efficient for SPARSE graphs (where E << V^2):
 *   Space complexity is O(V + E) instead of O(V^2) for adjacency matrix.
 * - Iterating over adjacent vertices takes O(degree(v)) time.
 * - Adding an edge takes O(1) time (inserting at head of linked list).
 * 
 * Disadvantages:
 * - Checking if an edge (u, v) exists takes O(degree(u)) in worst case.
 * 
 * TIME COMPLEXITIES:
 * - Add Edge:              O(1)
 * - Check Edge:            O(degree(u))
 * - DFS Traversal:         O(V + E)
 * - BFS Traversal:         O(V + E)
 * SPACE COMPLEXITY:        O(V + E)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Node in the adjacency list of a vertex
typedef struct Node {
    int vertex;
    struct Node *next;
} Node;

// Graph structure containing vertex count and array of adjacency lists
typedef struct Graph {
    int numVertices;
    Node **adjLists;
} Graph;

/**
 * Creates and allocates memory for a new adjacency list node.
 */
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

/**
 * Creates and initializes a graph with 'vertices' number of vertices.
 */
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

    // Initialize all adjacency list heads to NULL
    for (int i = 0; i < vertices; i++)
    {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

/**
 * Checks if vertex index is within valid range [0, numVertices - 1].
 */
int isValidVertex(Graph *graph, int vertex)
{
    return (graph != NULL && vertex >= 0 && vertex < graph->numVertices);
}

/**
 * Adds an undirected edge between 'src' and 'dest'.
 * Inserts at the beginning (head) of each vertex's list in O(1) time.
 */
void addEdge(Graph *graph, int src, int dest)
{
    if (!isValidVertex(graph, src) || !isValidVertex(graph, dest))
    {
        printf("[Error] Invalid vertices (%d, %d)!\n", src, dest);
        return;
    }

    // Add edge from src to dest
    Node *newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // Since the graph is undirected, add edge from dest to src as well
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

/**
 * Displays the Adjacency List representation of the graph.
 */
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

/**
 * Recursive helper function for Depth-First Search (DFS).
 */
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

/**
 * Depth-First Search (DFS) Traversal starting from startVertex.
 */
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

/**
 * Breadth-First Search (BFS) Traversal starting from startVertex using a Queue.
 */
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

    // Enqueue start vertex and mark visited
    visited[startVertex] = 1;
    queue[rear++] = startVertex;

    printf("BFS Traversal starting from %d: ", startVertex);

    while (front < rear)
    {
        int curr = queue[front++];
        printf("%d ", curr);

        // Traverse all adjacent nodes in linked list
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

/**
 * Frees all allocated memory for nodes and graph.
 */
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

/* ============================================================================
 * DEMO MAIN FUNCTION FOR TESTING & REVISION
 * ============================================================================ */
int main()
{
    printf("=== GRAPH DEMO (ADJACENCY LIST) ===\n");

    /*
     * Graph structure:
     *      0 ----- 1
     *      |     / |
     *      |   /   |
     *      | /     |
     *      2 ----- 3
     *      |
     *      4
     */
    Graph *graph = createGraph(5);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);
    addEdge(graph, 2, 4);

    displayGraph(graph);

    // Traversals
    DFS(graph, 0);
    BFS(graph, 0);

    freeGraph(graph);
    return 0;
}
