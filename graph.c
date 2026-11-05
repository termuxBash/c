/**
 * ============================================================================
 * DATA STRUCTURE: GRAPH (Adjacency Matrix Representation)
 * ============================================================================
 * 
 * DESCRIPTION:
 * An Adjacency Matrix represents a graph with V vertices as a 2D array of
 * dimensions V x V.
 * - adjMatrix[i][j] = 1 if an edge exists between vertex i and vertex j.
 * - adjMatrix[i][j] = 0 if no edge exists.
 * - For an undirected graph, the adjacency matrix is SYMMETRIC:
 *   adjMatrix[i][j] == adjMatrix[j][i].
 * 
 * EXAM POINTERS & TRADE-OFFS:
 * Advantages:
 * - Edge lookup is O(1) constant time (check if adjMatrix[u][v] == 1).
 * - Simple to implement.
 * - Excellent for DENSE graphs (where E is close to V^2).
 * 
 * Disadvantages:
 * - High space requirement: Always O(V^2), even for sparse graphs.
 * - Iterating over adjacent vertices takes O(V) time.
 * 
 * TIME COMPLEXITIES:
 * - Add Edge:              O(1)
 * - Remove Edge:           O(1)
 * - Check Edge:            O(1)
 * - BFS / DFS Traversal:   O(V^2)
 * SPACE COMPLEXITY:        O(V^2)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 20

// Graph representation using Adjacency Matrix
int adjMatrix[MAX_VERTICES][MAX_VERTICES];
int visited[MAX_VERTICES];
int numVertices = 0;

/**
 * Initializes the adjacency matrix with 0s.
 */
void initGraph(int vertices)
{
    if (vertices > MAX_VERTICES)
    {
        printf("[Error] Exceeded maximum supported vertices (%d).\n", MAX_VERTICES);
        vertices = MAX_VERTICES;
    }
    numVertices = vertices;
    for (int i = 0; i < numVertices; i++)
    {
        for (int j = 0; j < numVertices; j++)
        {
            adjMatrix[i][j] = 0;
        }
    }
}

/**
 * Adds an undirected edge between vertex u and vertex v.
 */
void addEdge(int u, int v)
{
    if (u < 0 || u >= numVertices || v < 0 || v >= numVertices)
    {
        printf("[Error] Invalid edge between %d and %d!\n", u, v);
        return;
    }
    // Set both directions since graph is undirected
    adjMatrix[u][v] = 1;
    adjMatrix[v][u] = 1;
}

/**
 * Displays the 2D Adjacency Matrix.
 */
void displayMatrix()
{
    printf("\nAdjacency Matrix (%d vertices):\n     ", numVertices);
    for (int j = 0; j < numVertices; j++)
    {
        printf("%2d ", j);
    }
    printf("\n    +");
    for (int j = 0; j < numVertices; j++)
    {
        printf("---");
    }
    printf("\n");

    for (int i = 0; i < numVertices; i++)
    {
        printf("%2d  |", i);
        for (int j = 0; j < numVertices; j++)
        {
            printf("%2d ", adjMatrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Recursive helper function for Depth-First Search (DFS).
 * 
 * Logic:
 * 1. Mark current vertex as visited.
 * 2. Process / print vertex.
 * 3. Recursively visit all unvisited neighbors.
 */
void dfsRec(int vertex)
{
    visited[vertex] = 1;
    printf("%d ", vertex);

    for (int neighbor = 0; neighbor < numVertices; neighbor++)
    {
        if (adjMatrix[vertex][neighbor] == 1 && !visited[neighbor])
        {
            dfsRec(neighbor);
        }
    }
}

/**
 * Depth-First Search (DFS) Traversal starting from startVertex.
 * Uses system recursion stack (LIFO behavior).
 */
void DFS(int startVertex)
{
    if (startVertex < 0 || startVertex >= numVertices)
    {
        printf("[Error] Invalid starting vertex %d!\n", startVertex);
        return;
    }

    // Reset visited array
    for (int i = 0; i < numVertices; i++)
    {
        visited[i] = 0;
    }

    printf("DFS Traversal starting from %d: ", startVertex);
    dfsRec(startVertex);
    printf("\n");
}

/**
 * Breadth-First Search (BFS) Traversal starting from startVertex.
 * Uses a Queue (FIFO behavior) to visit neighbors level-by-level.
 */
void BFS(int startVertex)
{
    if (startVertex < 0 || startVertex >= numVertices)
    {
        printf("[Error] Invalid starting vertex %d!\n", startVertex);
        return;
    }

    // Reset visited array
    for (int i = 0; i < numVertices; i++)
    {
        visited[i] = 0;
    }

    // Local Queue for BFS
    int queue[MAX_VERTICES];
    int front = 0;
    int rear = 0;

    // Step 1: Enqueue startVertex and mark visited
    visited[startVertex] = 1;
    queue[rear++] = startVertex;

    printf("BFS Traversal starting from %d: ", startVertex);

    // Step 2: Process queue until empty
    while (front < rear)
    {
        int curr = queue[front++];
        printf("%d ", curr);

        // Step 3: Enqueue all unvisited adjacent vertices
        for (int neighbor = 0; neighbor < numVertices; neighbor++)
        {
            if (adjMatrix[curr][neighbor] == 1 && !visited[neighbor])
            {
                visited[neighbor] = 1;
                queue[rear++] = neighbor;
            }
        }
    }
    printf("\n");
}

/* ============================================================================
 * DEMO MAIN FUNCTION FOR TESTING & REVISION
 * ============================================================================ */
int main()
{
    printf("=== GRAPH DEMO (ADJACENCY MATRIX) ===\n");

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
    initGraph(5);

    addEdge(0, 1);
    addEdge(0, 2);
    addEdge(1, 2);
    addEdge(1, 3);
    addEdge(2, 3);
    addEdge(2, 4);

    displayMatrix();

    // Traversals
    DFS(0);
    BFS(0);

    return 0;
}
