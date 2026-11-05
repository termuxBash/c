

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 20

int adjMatrix[MAX_VERTICES][MAX_VERTICES];
int visited[MAX_VERTICES];
int numVertices = 0;

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

void addEdge(int u, int v)
{
    if (u < 0 || u >= numVertices || v < 0 || v >= numVertices)
    {
        printf("[Error] Invalid edge between %d and %d!\n", u, v);
        return;
    }
    
    adjMatrix[u][v] = 1;
    adjMatrix[v][u] = 1;
}

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

void DFS(int startVertex)
{
    if (startVertex < 0 || startVertex >= numVertices)
    {
        printf("[Error] Invalid starting vertex %d!\n", startVertex);
        return;
    }

    
    for (int i = 0; i < numVertices; i++)
    {
        visited[i] = 0;
    }

    printf("DFS Traversal starting from %d: ", startVertex);
    dfsRec(startVertex);
    printf("\n");
}

void BFS(int startVertex)
{
    if (startVertex < 0 || startVertex >= numVertices)
    {
        printf("[Error] Invalid starting vertex %d!\n", startVertex);
        return;
    }

    
    for (int i = 0; i < numVertices; i++)
    {
        visited[i] = 0;
    }

    
    int queue[MAX_VERTICES];
    int front = 0;
    int rear = 0;

    
    visited[startVertex] = 1;
    queue[rear++] = startVertex;

    printf("BFS Traversal starting from %d: ", startVertex);

    
    while (front < rear)
    {
        int curr = queue[front++];
        printf("%d ", curr);

        
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

int main()
{
    printf("=== GRAPH DEMO (ADJACENCY MATRIX) ===\n");

    
    initGraph(5);

    addEdge(0, 1);
    addEdge(0, 2);
    addEdge(1, 2);
    addEdge(1, 3);
    addEdge(2, 3);
    addEdge(2, 4);

    displayMatrix();

    
    DFS(0);
    BFS(0);

    return 0;
}
