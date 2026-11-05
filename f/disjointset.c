

#include <stdio.h>
#include <stdlib.h>

int *parent;
int *rankArr;  
int *sz;       
int numSets;   

void makeSet(int n)
{
    parent = (int *)malloc(n * sizeof(int));
    rankArr = (int *)malloc(n * sizeof(int));
    sz = (int *)malloc(n * sizeof(int));

    if (parent == NULL || rankArr == NULL || sz == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        parent[i] = i;  
        rankArr[i] = 0; 
        sz[i] = 1;      
    }
    numSets = n;
}

int find(int x)
{
    if (parent[x] != x)
    {
        
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

int unionByRank(int a, int b)
{
    int rootA = find(a);
    int rootB = find(b);

    
    if (rootA == rootB)
    {
        return 0; 
    }

    
    if (rankArr[rootA] < rankArr[rootB])
    {
        parent[rootA] = rootB;
        sz[rootB] += sz[rootA];
    }
    else if (rankArr[rootA] > rankArr[rootB])
    {
        parent[rootB] = rootA;
        sz[rootA] += sz[rootB];
    }
    else
    {
        
        parent[rootB] = rootA;
        sz[rootA] += sz[rootB];
        rankArr[rootA]++;
    }

    numSets--;
    return 1;
}

int isConnected(int a, int b)
{
    return find(a) == find(b);
}

int getComponentSize(int x)
{
    int root = find(x);
    return sz[root];
}

void freeSet()
{
    free(parent);
    free(rankArr);
    free(sz);
}

int main()
{
    int n = 7; 

    printf("=== DISJOINT SET UNION (DSU) DEMO ===\n\n");
    makeSet(n);
    printf("Created %d disjoint sets (0 to %d).\n\n", n, n - 1);

    
    printf("Uniting 0 and 1...\n");
    unionByRank(0, 1);

    printf("Uniting 1 and 2...\n");
    unionByRank(1, 2);

    printf("Uniting 3 and 4...\n");
    unionByRank(3, 4);

    printf("Uniting 5 and 6...\n");
    unionByRank(5, 6);

    printf("\n--- Connectivity Checks ---\n");
    printf("Are 0 and 2 connected? %s\n", isConnected(0, 2) ? "YES" : "NO");
    printf("Are 0 and 3 connected? %s\n", isConnected(0, 3) ? "YES" : "NO");
    printf("Are 5 and 6 connected? %s\n", isConnected(5, 6) ? "YES" : "NO");

    printf("\nComponent size containing 0: %d\n", getComponentSize(0));
    printf("Component size containing 3: %d\n", getComponentSize(3));
    printf("Total disjoint sets remaining: %d\n\n", numSets);

    
    printf("Uniting 2 and 3...\n");
    unionByRank(2, 3);
    printf("Are 0 and 4 connected now? %s\n", isConnected(0, 4) ? "YES" : "NO");
    printf("New component size containing 0: %d\n", getComponentSize(0));
    printf("Total disjoint sets remaining: %d\n", numSets);

    freeSet();
    return 0;
}
