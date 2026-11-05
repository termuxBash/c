/**
 * ============================================================================
 * DATA STRUCTURE: FIBONACCI HEAP (Priority Queue)
 * ============================================================================
 * 
 * DESCRIPTION:
 * A Fibonacci Heap (invented by Fredman & Tarjan, 1987) is an advanced priority
 * queue data structure consisting of a collection of min-heap-ordered trees.
 * 
 * WHY IS IT FAMOUS (MUST-KNOW FOR EXAMS)?
 * While a standard Binary Heap takes O(log n) for Decrease-Key and Insert:
 * Fibonacci Heap achieves AMORTIZED O(1) CONSTANT TIME for:
 * - Insert:       O(1) amortized
 * - Find-Min:     O(1) worst-case
 * - Union:        O(1) amortized
 * - Decrease-Key: O(1) amortized
 * - Extract-Min:  O(log n) amortized
 * 
 * ALGORITHMIC IMPACT:
 * Allows Dijkstra's Shortest Path and Prim's Minimum Spanning Tree algorithms
 * to run in optimal O(E + V log V) time instead of O(E log V).
 * 
 * KEY STRUCTURAL CONCEPTS:
 * 1. Circular Doubly Linked Lists:
 *    - The root nodes of all trees are kept in a circular doubly linked "root list".
 *    - Children of every node are also linked in a circular doubly linked list.
 * 2. Lazy Operations:
 *    - Inserting or uniting simply concatenates root lists in O(1) time without
 *      restructuring trees immediately.
 *    - Trees are consolidated only during extractMin().
 * 3. Marking Rule & Cascading Cuts:
 *    - A node's mark is 0 (unmarked) initially.
 *    - When a node loses its FIRST child, its mark becomes 1.
 *    - When a marked node loses its SECOND child, it is cut from its parent,
 *      moved to the root list, and cascading cuts propagate up to ancestors.
 *    - This bound ensures tree degree is bounded by O(log n).
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Definition of a Fibonacci Heap Node
typedef struct FibNode {
    int key;                    // Priority key (smaller value = higher priority)
    int degree;                 // Number of children
    int mark;                   // 1 if node lost a child since being parented, else 0
    struct FibNode *parent;     // Pointer to parent node
    struct FibNode *child;      // Pointer to one of its children
    struct FibNode *left;       // Left sibling in circular list
    struct FibNode *right;      // Right sibling in circular list
} FibNode;

// Definition of the Fibonacci Heap
typedef struct FibHeap {
    FibNode *min;               // Pointer to root containing minimum key
    int n;                      // Total number of nodes in heap
} FibHeap;

// Function prototype
void consolidate(FibHeap *H);

/**
 * Initializes and returns an empty Fibonacci Heap.
 */
FibHeap *makeHeap(void)
{
    FibHeap *H = (FibHeap *)malloc(sizeof(FibHeap));
    if (H == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        exit(1);
    }
    H->min = NULL;
    H->n = 0;
    return H;
}

/**
 * Creates and allocates memory for a new Fibonacci Heap node with integer 'key'.
 */
FibNode *makeNode(int key)
{
    FibNode *x = (FibNode *)malloc(sizeof(FibNode));
    if (x == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        exit(1);
    }
    x->key = key;
    x->degree = 0;
    x->mark = 0;
    x->parent = NULL;
    x->child = NULL;
    // Single node forms a circular doubly linked list with itself
    x->left = x;
    x->right = x;
    return x;
}

/**
 * Inserts node 'x' immediately to the right of node 'a' in a circular list.
 */
static void insertAfter(FibNode *a, FibNode *x)
{
    x->right = a->right;
    x->left = a;
    a->right->left = x;
    a->right = x;
}

/**
 * Removes node 'x' from its circular doubly linked list.
 */
static void removeFromList(FibNode *x)
{
    x->left->right = x->right;
    x->right->left = x->left;
    x->left = x;
    x->right = x;
}

/**
 * Adds node 'x' to the root list of heap H.
 */
static void addToRootList(FibHeap *H, FibNode *x)
{
    x->parent = NULL;
    x->mark = 0;
    if (H->min == NULL)
    {
        x->left = x;
        x->right = x;
        H->min = x;
    }
    else
    {
        insertAfter(H->min, x);
        if (x->key < H->min->key)
        {
            H->min = x;
        }
    }
}

/**
 * Inserts a new integer key into the Fibonacci Heap in O(1) amortized time.
 * Returns pointer to the newly created FibNode.
 */
FibNode *fibInsert(FibHeap *H, int key)
{
    FibNode *x = makeNode(key);
    addToRootList(H, x);
    H->n++;
    return x;
}

/**
 * Returns pointer to node with minimum key in O(1) time.
 */
FibNode *fibMinimum(FibHeap *H)
{
    return (H == NULL) ? NULL : H->min;
}

/**
 * Extracts and returns the node with minimum key from heap H.
 * Running time: O(log n) amortized.
 */
FibNode *fibExtractMin(FibHeap *H)
{
    if (H == NULL || H->min == NULL)
        return NULL;

    FibNode *z = H->min;

    // Step 1: Add all children of z to the root list of H
    if (z->child != NULL)
    {
        FibNode *child = z->child;
        FibNode *curr = child;
        do
        {
            FibNode *next = curr->right;
            curr->parent = NULL;
            curr->mark = 0;
            removeFromList(curr);
            addToRootList(H, curr);
            curr = next;
        } while (curr != child);
        z->child = NULL;
        z->degree = 0;
    }

    // Step 2: Remove z from the root list
    if (z->right == z)
    {
        H->min = NULL; // z was the only node left in heap
    }
    else
    {
        H->min = z->right;
        removeFromList(z);
    }
    H->n--;

    // Step 3: Consolidate root list so no two roots share the same degree
    if (H->min != NULL)
    {
        consolidate(H);
    }

    z->left = z;
    z->right = z;
    z->parent = NULL;
    return z;
}

/**
 * Consolidates the root list of H by joining trees of equal degree
 * until at most one tree of each degree exists.
 */
void consolidate(FibHeap *H)
{
    enum { DEGREE_TABLE_SIZE = 64 };
    FibNode *table[DEGREE_TABLE_SIZE] = {NULL};
    FibNode **roots = NULL;
    int rootCount = 0;

    if (H->min != NULL)
    {
        FibNode *curr = H->min;
        do
        {
            rootCount++;
            curr = curr->right;
        } while (curr != H->min);

        roots = (FibNode **)malloc((size_t)rootCount * sizeof(*roots));
        if (roots == NULL)
            return;

        curr = H->min;
        for (int i = 0; i < rootCount; i++)
        {
            roots[i] = curr;
            curr = curr->right;
        }
    }

    // Process each root in the root list
    for (int i = 0; i < rootCount; i++)
    {
        FibNode *x = roots[i];
        int degree = x->degree;

        // If another tree of the same degree exists, merge them
        while (table[degree] != NULL)
        {
            FibNode *y = table[degree];
            // Make sure x has smaller key than y
            if (y->key < x->key)
            {
                FibNode *temp = x;
                x = y;
                y = temp;
            }
            table[degree] = NULL;

            // Link y as child of x (FibHeap-Link)
            removeFromList(y);
            y->parent = x;
            y->mark = 0;
            if (x->child == NULL)
            {
                x->child = y;
                y->left = y;
                y->right = y;
            }
            else
            {
                insertAfter(x->child, y);
            }
            x->degree++;
            degree = x->degree;
        }
        table[degree] = x;
    }

    // Reconstruct root list from table entries and update min pointer
    H->min = NULL;
    for (int i = 0; i < DEGREE_TABLE_SIZE; i++)
    {
        if (table[i] != NULL)
        {
            table[i]->left = table[i];
            table[i]->right = table[i];
            addToRootList(H, table[i]);
        }
    }
    free(roots);
}

/**
 * Cuts child link between x and its parent y, moving x to root list.
 */
void cut(FibHeap *H, FibNode *x, FibNode *y)
{
    if (x->right == x)
    {
        y->child = NULL;
    }
    else
    {
        if (y->child == x)
            y->child = x->right;
        removeFromList(x);
    }
    y->degree--;
    addToRootList(H, x);
}

/**
 * Cascading Cut: Recursively cuts marked ancestor nodes.
 */
void cascadingCut(FibHeap *H, FibNode *y)
{
    FibNode *z = y->parent;
    if (z != NULL)
    {
        if (y->mark == 0)
        {
            // First child loss: mark the node
            y->mark = 1;
        }
        else
        {
            // Second child loss: cut y from z and propagate upward
            cut(H, y, z);
            cascadingCut(H, z);
        }
    }
}

/**
 * Decreases the key of node 'x' to 'newKey'.
 * Running time: O(1) amortized.
 */
int fibDecreaseKey(FibHeap *H, FibNode *x, int newKey)
{
    if (H == NULL || x == NULL || newKey > x->key)
    {
        printf("[Error] New key cannot be greater than current key!\n");
        return 0;
    }

    x->key = newKey;
    FibNode *y = x->parent;

    // If heap order is violated (child key < parent key), cut x from y
    if (y != NULL && x->key < y->key)
    {
        cut(H, x, y);
        cascadingCut(H, y);
    }

    // Update heap minimum if necessary
    if (H->min == NULL || x->key < H->min->key)
    {
        H->min = x;
    }
    return 1;
}

/**
 * Merges (unites) two Fibonacci Heaps H1 and H2 into one in O(1) time.
 */
FibHeap *fibUnion(FibHeap *H1, FibHeap *H2)
{
    if (H1 == NULL) return H2;
    if (H2 == NULL) return H1;

    if (H1->min == NULL)
    {
        H2->n += H1->n;
        free(H1);
        return H2;
    }
    if (H2->min == NULL)
    {
        H1->n += H2->n;
        free(H2);
        return H1;
    }

    FibHeap *result = makeHeap();
    result->min = (H1->min->key < H2->min->key) ? H1->min : H2->min;

    // Concatenate circular root lists
    FibNode *h1 = H1->min;
    FibNode *h2 = H2->min;
    FibNode *h1Right = h1->right;
    FibNode *h2Left = h2->left;

    h1->right = h2;
    h2->left = h1;
    h1Right->left = h2Left;
    h2Left->right = h1Right;

    result->n = H1->n + H2->n;
    free(H1);
    free(H2);
    return result;
}

/* ============================================================================
 * DEMO MAIN FUNCTION FOR TESTING & REVISION
 * ============================================================================ */
int main()
{
    printf("=== FIBONACCI HEAP DEMO (PRIORITY QUEUE) ===\n\n");

    FibHeap *heap = makeHeap();

    // Insert keys
    printf("Inserting keys: 30, 10, 20, 5, 40...\n");
    fibInsert(heap, 30);
    fibInsert(heap, 10);
    FibNode *node20 = fibInsert(heap, 20);
    fibInsert(heap, 5);
    FibNode *node40 = fibInsert(heap, 40);

    printf("Total nodes: %d\n", heap->n);
    printf("Minimum key: %d\n\n", fibMinimum(heap)->key);

    // Extract Min
    FibNode *minNode = fibExtractMin(heap);
    printf("Extracted Min: %d\n", minNode->key);
    free(minNode);

    printf("New Minimum key after extraction: %d\n\n", fibMinimum(heap)->key);

    // Decrease Key (Amortized O(1))
    printf("Decreasing key of node (40 -> 2)...\n");
    fibDecreaseKey(heap, node40, 2);
    printf("New Minimum key: %d\n\n", fibMinimum(heap)->key);

    // Extract all remaining elements in sorted order
    printf("Extracting all remaining elements:\n");
    while (heap->min != NULL)
    {
        FibNode *curr = fibExtractMin(heap);
        printf("%d ", curr->key);
        free(curr);
    }
    printf("\n");

    free(heap);
    (void)node20; // Silence unused variable warning
    return 0;
}
