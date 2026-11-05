/**
 * ============================================================================
 * DATA STRUCTURE: DISJOINT SET UNION (DSU) / UNION-FIND
 * ============================================================================
 * 
 * DESCRIPTION:
 * Disjoint Set Union (DSU) keeps track of a set of elements partitioned into
 * a number of non-overlapping (disjoint) subsets.
 * 
 * CORE OPERATIONS:
 * 1. makeSet(n):  Initializes n separate sets, each element being its own parent.
 * 2. find(x):     Determines which subset x belongs to (returns root/representative).
 * 3. unionSets(a, b): Merges the two subsets containing a and b.
 * 
 * ESSENTIAL EXAM CONCEPTS:
 * - NAIVE APPROACH:
 *   Trees can degenerate into linear chains (like linked lists), resulting in
 *   O(n) time per find/union.
 * 
 * - OPTIMIZATION 1: PATH COMPRESSION (in find)
 *   While traversing up to find the root, redirect each visited node's parent
 *   pointer directly to the root. This flattens the tree structure!
 * 
 * - OPTIMIZATION 2: UNION BY RANK / SIZE
 *   Always attach the root of the smaller tree under the root of the larger tree.
 *   This prevents deep trees from forming.
 * 
 * TIME COMPLEXITY:
 * - With Path Compression + Union by Rank/Size:
 *   Amortized O(alpha(n)) per operation, where alpha is the Inverse Ackermann
 *   function. For any practical input (n <= 10^80), alpha(n) <= 4.
 *   This is effectively O(1) CONSTANT TIME!
 * 
 * APPLICATIONS:
 * - Kruskal's Minimum Spanning Tree (MST) algorithm.
 * - Cycle detection in undirected graphs.
 * - Finding connected components in networks/social graphs.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Global arrays for DSU
int *parent;
int *rankArr;  // Upper bound on height of tree rooted at this node
int *sz;       // Size of connected component
int numSets;   // Total number of disjoint sets

/**
 * Initializes 'n' elements (labeled 0 to n-1).
 */
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
        parent[i] = i;  // Each element is its own parent initially
        rankArr[i] = 0; // Initial rank is 0
        sz[i] = 1;      // Initial component size is 1
    }
    numSets = n;
}

/**
 * Finds the representative (root) of the set containing element 'x'.
 * Uses PATH COMPRESSION: flattens the tree by pointing parent directly to root.
 */
int find(int x)
{
    if (parent[x] != x)
    {
        // Recursive call with path compression
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

/**
 * Merges the set containing 'a' with the set containing 'b' using UNION BY RANK.
 * Returns 1 if sets were merged, or 0 if they were already in the same set.
 */
int unionByRank(int a, int b)
{
    int rootA = find(a);
    int rootB = find(b);

    // If both elements already share the same root, they are in the same set
    if (rootA == rootB)
    {
        return 0; // No union performed (cycle detected if adding an edge!)
    }

    // Attach tree with smaller rank under tree with larger rank
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
        // Equal ranks: attach one under the other and increment rank of new root
        parent[rootB] = rootA;
        sz[rootA] += sz[rootB];
        rankArr[rootA]++;
    }

    numSets--;
    return 1;
}

/**
 * Checks whether elements 'a' and 'b' belong to the same disjoint set.
 */
int isConnected(int a, int b)
{
    return find(a) == find(b);
}

/**
 * Returns the size of the component containing element 'x'.
 */
int getComponentSize(int x)
{
    int root = find(x);
    return sz[root];
}

/**
 * Frees allocated memory.
 */
void freeSet()
{
    free(parent);
    free(rankArr);
    free(sz);
}

/* ============================================================================
 * DEMO MAIN FUNCTION FOR TESTING & REVISION
 * ============================================================================ */
int main()
{
    int n = 7; // Elements 0, 1, 2, 3, 4, 5, 6

    printf("=== DISJOINT SET UNION (DSU) DEMO ===\n\n");
    makeSet(n);
    printf("Created %d disjoint sets (0 to %d).\n\n", n, n - 1);

    // Perform union operations
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

    // Merge {0, 1, 2} with {3, 4}
    printf("Uniting 2 and 3...\n");
    unionByRank(2, 3);
    printf("Are 0 and 4 connected now? %s\n", isConnected(0, 4) ? "YES" : "NO");
    printf("New component size containing 0: %d\n", getComponentSize(0));
    printf("Total disjoint sets remaining: %d\n", numSets);

    freeSet();
    return 0;
}
