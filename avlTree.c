/**
 * ============================================================================
 * DATA STRUCTURE: AVL TREE (Self-Balancing Binary Search Tree)
 * ============================================================================
 * 
 * DESCRIPTION:
 * An AVL Tree (named after inventors Adelson-Velsky and Landis) is a self-balancing
 * Binary Search Tree where the height difference between left and right subtrees
 * of ANY node (Balance Factor) is at most 1.
 * 
 * FORMULA:
 * Balance Factor (BF) = Height(Left Subtree) - Height(Right Subtree)
 * Valid BF for every node: {-1, 0, +1}
 * If |BF| > 1, tree is unbalanced and must be restored using ROTATIONS.
 * 
 * THE 4 ROTATION CASES (DURING INSERTION):
 * 1. Left-Left (LL) Case:
 *    - Cause: Node is left-heavy (BF > 1) and key < left_child->key.
 *    - Fix:   Single RIGHT ROTATION on current node.
 * 
 * 2. Right-Right (RR) Case:
 *    - Cause: Node is right-heavy (BF < -1) and key > right_child->key.
 *    - Fix:   Single LEFT ROTATION on current node.
 * 
 * 3. Left-Right (LR) Case:
 *    - Cause: Node is left-heavy (BF > 1) and key > left_child->key.
 *    - Fix:   LEFT ROTATION on left child, then RIGHT ROTATION on current node.
 * 
 * 4. Right-Left (RL) Case:
 *    - Cause: Node is right-heavy (BF < -1) and key < right_child->key.
 *    - Fix:   RIGHT ROTATION on right child, then LEFT ROTATION on current node.
 * 
 * TIME COMPLEXITIES:
 * - Search:    Guaranteed O(log n)
 * - Insertion: Guaranteed O(log n)
 * - Deletion:  Guaranteed O(log n)
 * SPACE COMPLEXITY: O(n) for tree, O(log n) recursion call stack
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Definition of an AVL Tree Node
typedef struct Node {
    int key;
    int height;              // Height of subtree rooted at this node
    struct Node *left;
    struct Node *right;
} Node;

/**
 * Returns the height of a given node.
 * NULL nodes have a height of 0.
 */
int getHeight(Node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

/**
 * Helper utility to return the maximum of two integers.
 */
int max(int a, int b)
{
    return (a > b) ? a : b;
}

/**
 * Creates and allocates memory for a new AVL node.
 * Newly created leaf node always has height = 1.
 */
Node *createNode(int key)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        exit(1);
    }
    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * Calculates Balance Factor of a node: Height(Left) - Height(Right)
 */
int getBalanceFactor(Node *node)
{
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

/**
 * ============================================================================
 * RIGHT ROTATION (Used for LL Imbalance)
 * ============================================================================
 *        y                                x
 *       / \     Right Rotation (y)       / \
 *      x   T3   ----------------->      T1  y
 *     / \                                  / \
 *    T1  T2                               T2  T3
 */
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights (Order matters: update lower node y first, then x)
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    // x becomes the new root of this subtree
    return x;
}

/**
 * ============================================================================
 * LEFT ROTATION (Used for RR Imbalance)
 * ============================================================================
 *      x                                  y
 *     / \       Left Rotation (x)        / \
 *    T1  y      ----------------->      x   T3
 *       / \                            / \
 *      T2  T3                         T1  T2
 */
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights (Order matters: update lower node x first, then y)
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    // y becomes the new root of this subtree
    return y;
}

/**
 * Inserts a key into the AVL tree and rebalances if necessary.
 * Returns the root of the balanced subtree.
 */
Node *insertNode(Node *node, int key)
{
    // Step 1: Standard BST Insertion
    if (node == NULL)
    {
        return createNode(key);
    }

    if (key < node->key)
    {
        node->left = insertNode(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = insertNode(node->right, key);
    }
    else
    {
        // Duplicate keys not permitted
        return node;
    }

    // Step 2: Update height of this ancestor node
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Step 3: Get Balance Factor to check whether this node became unbalanced
    int balance = getBalanceFactor(node);

    // Step 4: If unbalanced, handle one of the 4 cases:

    // Case 1: Left-Left (LL) -> Single Right Rotation
    if (balance > 1 && key < node->left->key)
    {
        return rightRotate(node);
    }

    // Case 2: Right-Right (RR) -> Single Left Rotation
    if (balance < -1 && key > node->right->key)
    {
        return leftRotate(node);
    }

    // Case 3: Left-Right (LR) -> Left Rotate left child, then Right Rotate node
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Case 4: Right-Left (RL) -> Right Rotate right child, then Left Rotate node
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the (unchanged) node pointer if balanced
    return node;
}

/**
 * Finds the node with the minimum key (leftmost node).
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
 * Deletes a key from the AVL Tree and restores balance.
 */
Node *deleteNode(Node *root, int key)
{
    // Step 1: Standard BST Deletion
    if (root == NULL)
        return root;

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
        // Node with only one child or no child
        if (root->left == NULL || root->right == NULL)
        {
            Node *temp = (root->left != NULL) ? root->left : root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
            {
                *root = *temp; // Copy child contents
            }
            free(temp);
        }
        else
        {
            // Node with two children: Get inorder successor
            Node *temp = findMin(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node and it was deleted
    if (root == NULL)
        return NULL;

    // Step 2: Update height
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // Step 3: Check Balance Factor
    int balance = getBalanceFactor(root);

    // Step 4: Rebalance if needed

    // LL Case after deletion
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    // LR Case after deletion
    if (balance > 1 && getBalanceFactor(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RR Case after deletion
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // RL Case after deletion
    if (balance < -1 && getBalanceFactor(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/**
 * Searches for a target key in the AVL tree.
 */
Node *searchNode(Node *root, int target)
{
    if (root == NULL || root->key == target)
        return root;

    if (target < root->key)
        return searchNode(root->left, target);

    return searchNode(root->right, target);
}

/**
 * In-Order Traversal: Left -> Root -> Right (Sorted order)
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
 * Prints each node's key, height, and balance factor.
 */
void displayDetails(Node *root)
{
    if (root != NULL)
    {
        displayDetails(root->left);
        printf("Key: %3d | Height: %d | BF: %2d\n",
               root->key, root->height, getBalanceFactor(root));
        displayDetails(root->right);
    }
}

/* ============================================================================
 * DEMO MAIN FUNCTION FOR TESTING & REVISION
 * ============================================================================ */
int main()
{
    Node *root = NULL;

    printf("=== AVL TREE DEMO (SELF-BALANCING BST) ===\n\n");

    /*
     * Inserting values in increasing order: 10, 20, 30, 40, 50, 25
     * In a regular BST, inserting sorted numbers produces a skewed O(n) line.
     * In AVL, rotations rebalance it to stay O(log n) height!
     */
    int keys[] = {10, 20, 30, 40, 50, 25};
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++)
    {
        printf("Inserting %d...\n", keys[i]);
        root = insertNode(root, keys[i]);
    }

    printf("\nIn-order Traversal (Sorted): ");
    inOrder(root);
    printf("\n");

    printf("Pre-order Traversal:         ");
    preOrder(root);
    printf("\n\n");

    printf("--- Node Balancing Details ---\n");
    displayDetails(root);

    // Search
    int target = 30;
    Node *found = searchNode(root, target);
    printf("\nSearch for %d: %s\n", target, (found != NULL) ? "Found" : "Not Found");

    // Deletion
    printf("\nDeleting key 30 (causing rebalance)...\n");
    root = deleteNode(root, 30);

    printf("In-order after deletion: ");
    inOrder(root);
    printf("\n");

    printf("Pre-order after deletion: ");
    preOrder(root);
    printf("\n");

    return 0;
}
