/**
 * ============================================================================
 * DATA STRUCTURE: BINARY SEARCH TREE (BST)
 * ============================================================================
 * 
 * DESCRIPTION:
 * A Binary Search Tree is a hierarchical node-based binary tree data structure
 * with the following invariant (BST Property):
 * 1. The left subtree of a node contains only keys LESS than the node's key.
 * 2. The right subtree of a node contains only keys GREATER than the node's key.
 * 3. Both the left and right subtrees must also be binary search trees.
 * 4. Duplicate keys are typically not allowed.
 * 
 * KEY EXAM POINTERS:
 * - IN-ORDER TRAVERSAL (Left -> Root -> Right) ALWAYS yields keys in 
 *   ASCENDING SORTED ORDER.
 * - PRE-ORDER:  Root -> Left -> Right (used for copying or serializing trees).
 * - POST-ORDER: Left -> Right -> Root (used for deleting trees from bottom up).
 * - MINIMUM: Follow left pointers until node->left == NULL.
 * - MAXIMUM: Follow right pointers until node->right == NULL.
 * 
 * DELETION CASES:
 * - Case 1: Leaf node (0 children) -> simply free the node and return NULL.
 * - Case 2: Single child (left OR right) -> replace node with its child.
 * - Case 3: Two children -> find the In-order Successor (smallest node in the
 *           right subtree), copy its key into the current node, then delete 
 *           the successor from the right subtree.
 * 
 * TIME COMPLEXITIES:
 * - Search:    Average O(log n), Worst O(n) [when tree is skewed]
 * - Insertion: Average O(log n), Worst O(n)
 * - Deletion:  Average O(log n), Worst O(n)
 * SPACE COMPLEXITY: O(h) where h is tree height (recursion call stack)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Definition of a BST Node
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

/**
 * Creates and allocates memory for a new BST node.
 */
Node *createNode(int key)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        exit(1);
    }
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/**
 * Inserts a new key into the BST recursively.
 * Returns the updated root pointer.
 */
Node *insertNode(Node *root, int key)
{
    // Base Case: If current spot is empty, create the node here
    if (root == NULL)
    {
        return createNode(key);
    }

    // Recur down the left subtree if key is smaller
    if (key < root->key)
    {
        root->left = insertNode(root->left, key);
    }
    // Recur down the right subtree if key is larger
    else if (key > root->key)
    {
        root->right = insertNode(root->right, key);
    }
    // Duplicate keys are ignored in standard BST
    else
    {
        printf("[Notice] Key %d already exists. Duplicates not inserted.\n", key);
    }

    return root;
}

/**
 * Searches for a target key in the BST recursively.
 * Returns pointer to the matching node, or NULL if not found.
 */
Node *searchNode(Node *root, int target)
{
    // Base Cases: root is NULL (not found) or target is found at current root
    if (root == NULL || root->key == target)
    {
        return root;
    }

    // Target is larger -> search right subtree
    if (target > root->key)
    {
        return searchNode(root->right, target);
    }

    // Target is smaller -> search left subtree
    return searchNode(root->left, target);
}

/**
 * Finds the node with the minimum value in a given BST/subtree.
 * The minimum value is always located at the leftmost node.
 */
Node *findMin(Node *root)
{
    if (root == NULL)
        return NULL;

    while (root->left != NULL)
    {
        root = root->left;
    }
    return root;
}

/**
 * Finds the node with the maximum value in a given BST/subtree.
 * The maximum value is always located at the rightmost node.
 */
Node *findMax(Node *root)
{
    if (root == NULL)
        return NULL;

    while (root->right != NULL)
    {
        root = root->right;
    }
    return root;
}

/**
 * Deletes a node with the given key from the BST.
 * Returns the root of the modified subtree.
 */
Node *deleteNode(Node *root, int key)
{
    if (root == NULL)
    {
        return NULL; // Key not found in tree
    }

    // Step 1: Locate the node to delete
    if (key < root->key)
    {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = deleteNode(root->right, key);
    }
    else
    {
        // Step 2: Node found! Handle the 3 deletion cases:

        // CASE 1: Node has NO children (Leaf Node)
        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }

        // CASE 2: Node has only ONE child
        else if (root->left == NULL)
        {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        // CASE 3: Node has TWO children
        else
        {
            // Find in-order successor (smallest in right subtree)
            Node *temp = findMin(root->right);

            // Copy successor's key to current node
            root->key = temp->key;

            // Delete the duplicate successor from the right subtree
            root->right = deleteNode(root->right, temp->key);
        }
    }
    return root;
}

/**
 * In-Order Traversal: Left -> Root -> Right
 * Outputs elements in ascending sorted order.
 */
void inOrder(Node *root)
{
    if (root != NULL)
    {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}

/**
 * Pre-Order Traversal: Root -> Left -> Right
 */
void preOrder(Node *root)
{
    if (root != NULL)
    {
        printf("%d ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}

/**
 * Post-Order Traversal: Left -> Right -> Root
 */
void postOrder(Node *root)
{
    if (root != NULL)
    {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->key);
    }
}

/* ============================================================================
 * DEMO MAIN FUNCTION FOR TESTING & REVISION
 * ============================================================================ */
int main()
{
    Node *root = NULL;

    printf("=== BINARY SEARCH TREE (BST) DEMO ===\n\n");

    /*
     * Inserting keys: 50, 30, 70, 20, 40, 60, 80
     * Resulting Tree:
     *          50
     *        /    \
     *       30     70
     *      /  \   /  \
     *     20  40 60  80
     */
    root = insertNode(root, 50);
    insertNode(root, 30);
    insertNode(root, 70);
    insertNode(root, 20);
    insertNode(root, 40);
    insertNode(root, 60);
    insertNode(root, 80);

    printf("In-order Traversal (Sorted): ");
    inOrder(root);
    printf("\n");

    printf("Pre-order Traversal:         ");
    preOrder(root);
    printf("\n");

    printf("Post-order Traversal:        ");
    postOrder(root);
    printf("\n\n");

    // Min and Max
    Node *minNode = findMin(root);
    Node *maxNode = findMax(root);
    if (minNode) printf("Minimum key: %d\n", minNode->key);
    if (maxNode) printf("Maximum key: %d\n", maxNode->key);

    // Search
    int target = 40;
    Node *searchResult = searchNode(root, target);
    printf("\nSearching for %d: %s\n", target, (searchResult != NULL) ? "Found" : "Not Found");

    // Deletions
    printf("\nDeleting leaf node 20 (Case 1)...\n");
    root = deleteNode(root, 20);
    printf("In-order: ");
    inOrder(root);
    printf("\n");

    printf("Deleting node 30 with one child (Case 2)...\n");
    root = deleteNode(root, 30);
    printf("In-order: ");
    inOrder(root);
    printf("\n");

    printf("Deleting root node 50 with two children (Case 3)...\n");
    root = deleteNode(root, 50);
    printf("In-order: ");
    inOrder(root);
    printf("\n");

    return 0;
}
