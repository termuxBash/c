/**
 * ============================================================================
 * DATA STRUCTURE: B-TREE (Multi-Way Search Tree)
 * ============================================================================
 * 
 * DESCRIPTION:
 * A B-Tree is a self-balancing, multi-way search tree commonly used in disk 
 * storage systems, file systems, and databases where read/write operations
 * are performed in large blocks.
 * 
 * In this implementation (Order 4 / degree t = 2):
 * - MAX_KEYS     = 3  (2t - 1)
 * - MIN_KEYS     = 1  (t - 1 for non-root nodes)
 * - MAX_CHILDREN = 4  (MAX_KEYS + 1)
 * 
 * KEY B-TREE PROPERTIES (MUST KNOW FOR EXAMS):
 * 1. All leaf nodes must reside at the EXACT SAME LEVEL (depth).
 * 2. Every node (except root) contains at least MIN_KEYS.
 * 3. Every node contains at most MAX_KEYS.
 * 4. A node with k keys has exactly (k + 1) children.
 * 5. Keys inside every node are stored in sorted ascending order.
 * 6. Subtree children[i] stores keys strictly between keys[i-1] and keys[i].
 * 
 * INSERTION LOGIC (SPLITTING FULL NODES):
 * - If root is full (contains MAX_KEYS), create a new root, split old root,
 *   and promote the middle (median) key to the new root.
 * - During traversal down to a leaf, split full children before descending
 *   (proactive splitting prevents cascading splits).
 * 
 * TIME COMPLEXITIES:
 * - Search:    O(log n)
 * - Insertion: O(log n)
 * - Traversal: O(n)
 * SPACE COMPLEXITY: O(n)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYS 3
#define MIN_KEYS 1
#define MAX_CHILDREN (MAX_KEYS + 1)

// Definition of a B-Tree Node
typedef struct BTreeNode {
    int keys[MAX_KEYS];                       // Array of keys stored in node
    struct BTreeNode *children[MAX_CHILDREN]; // Array of child pointers
    int numKeys;                              // Current number of keys
    int isLeaf;                               // 1 if leaf node, 0 otherwise
} BTreeNode;

/**
 * Creates and allocates memory for a new B-Tree node.
 */
BTreeNode *createNode(int isLeaf)
{
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    if (node == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        exit(1);
    }
    node->isLeaf = isLeaf;
    node->numKeys = 0;
    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

/**
 * Searches for a key in the B-Tree starting from root.
 * Returns pointer to the BTreeNode containing key, or NULL if not found.
 */
BTreeNode *search(BTreeNode *root, int key)
{
    if (root == NULL)
        return NULL;

    int i = 0;
    // Find the first key greater than or equal to key
    while (i < root->numKeys && key > root->keys[i])
    {
        i++;
    }

    // Key found in this node
    if (i < root->numKeys && key == root->keys[i])
    {
        return root;
    }

    // Key not found and this node is a leaf -> key doesn't exist
    if (root->isLeaf)
    {
        return NULL;
    }

    // Recur to the appropriate child subtree
    return search(root->children[i], key);
}

/**
 * Splits a full child node (having MAX_KEYS) of a parent node.
 * 
 * Diagram of Splitting:
 * Child before split: [10 | 20 | 30] (Full with 3 keys)
 * Middle key (20) is promoted to parent.
 * Left child retains [10], New right child gets [30].
 */
void splitChild(BTreeNode *parent, int index, BTreeNode *child)
{
    // Create new node which will store (MIN_KEYS) keys of child
    BTreeNode *newChild = createNode(child->isLeaf);
    newChild->numKeys = MIN_KEYS;

    // Copy the last MIN_KEYS keys from child to newChild
    for (int i = 0; i < MIN_KEYS; i++)
    {
        newChild->keys[i] = child->keys[i + MIN_KEYS + 1];
    }

    // Copy child pointers if child is not a leaf
    if (!child->isLeaf)
    {
        for (int i = 0; i <= MIN_KEYS; i++)
        {
            newChild->children[i] = child->children[i + MIN_KEYS + 1];
        }
    }

    // Reduce number of keys in original child
    child->numKeys = MIN_KEYS;

    // Make space in parent for new child pointer
    for (int i = parent->numKeys; i >= index + 1; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newChild;

    // Shift keys in parent to make room for promoted middle key
    for (int i = parent->numKeys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }

    // Copy the middle key of child up to parent
    parent->keys[index] = child->keys[MIN_KEYS];
    parent->numKeys++;
}

/**
 * Inserts a key into a non-full B-Tree node.
 */
void insertNonFull(BTreeNode *node, int key)
{
    int i = node->numKeys - 1;

    // Case 1: Node is a Leaf -> Insert key into sorted position
    if (node->isLeaf)
    {
        while (i >= 0 && node->keys[i] > key)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->numKeys++;
    }
    // Case 2: Node is an Internal Node
    else
    {
        // Find which child will receive the new key
        while (i >= 0 && node->keys[i] > key)
        {
            i--;
        }
        i++;

        // If the chosen child is full, split it first!
        if (node->children[i]->numKeys == MAX_KEYS)
        {
            splitChild(node, i, node->children[i]);

            // After split, the middle key went up to node->keys[i].
            // See if key belongs to left or right child of split.
            if (key > node->keys[i])
            {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

/**
 * Inserts a key into the B-Tree.
 * Handles root splitting if root is full.
 */
void insert(BTreeNode **root, int key)
{
    BTreeNode *r = *root;

    // If tree is completely empty
    if (r == NULL)
    {
        *root = createNode(1);
        (*root)->keys[0] = key;
        (*root)->numKeys = 1;
        return;
    }

    // If root node is full, the tree grows in height!
    if (r->numKeys == MAX_KEYS)
    {
        BTreeNode *newRoot = createNode(0);
        newRoot->children[0] = r;

        // Split the old root
        splitChild(newRoot, 0, r);

        // Determine which of the two children gets the new key
        int i = 0;
        if (key > newRoot->keys[0])
        {
            i++;
        }
        insertNonFull(newRoot->children[i], key);

        *root = newRoot;
    }
    else
    {
        insertNonFull(r, key);
    }
}

/**
 * In-Order Traversal: Visits keys in sorted ascending order.
 */
void traverse(BTreeNode *root)
{
    if (root == NULL)
        return;

    int i;
    for (i = 0; i < root->numKeys; i++)
    {
        // Traverse child subtree before printing current key
        if (!root->isLeaf)
        {
            traverse(root->children[i]);
        }
        printf("%d ", root->keys[i]);
    }

    // Traverse the last child
    if (!root->isLeaf)
    {
        traverse(root->children[i]);
    }
}

/**
 * Utility function to print search status.
 */
void searchKey(BTreeNode *root, int key)
{
    BTreeNode *result = search(root, key);
    if (result != NULL)
    {
        printf("Key %d: Found in B-Tree.\n", key);
    }
    else
    {
        printf("Key %d: NOT Found in B-Tree.\n", key);
    }
}

/* ============================================================================
 * DEMO MAIN FUNCTION FOR TESTING & REVISION
 * ============================================================================ */
int main()
{
    BTreeNode *root = NULL;

    printf("=== B-TREE DEMO (ORDER 4 / MAX 3 KEYS PER NODE) ===\n\n");

    /*
     * Inserting keys: 10, 20, 5, 6, 12, 30, 7, 17
     */
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++)
    {
        printf("Inserting %d...\n", keys[i]);
        insert(&root, keys[i]);
    }

    printf("\nIn-Order Traversal (Sorted): ");
    traverse(root);
    printf("\n\n");

    // Search tests
    searchKey(root, 6);
    searchKey(root, 15);
    searchKey(root, 30);

    return 0;
}
