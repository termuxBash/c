

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

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

Node *insertNode(Node *root, int key)
{
    
    if (root == NULL)
    {
        return createNode(key);
    }

    
    if (key < root->key)
    {
        root->left = insertNode(root->left, key);
    }
    
    else if (key > root->key)
    {
        root->right = insertNode(root->right, key);
    }
    
    else
    {
        printf("[Notice] Key %d already exists. Duplicates not inserted.\n", key);
    }

    return root;
}

Node *searchNode(Node *root, int target)
{
    
    if (root == NULL || root->key == target)
    {
        return root;
    }

    
    if (target > root->key)
    {
        return searchNode(root->right, target);
    }

    
    return searchNode(root->left, target);
}

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

Node *deleteNode(Node *root, int key)
{
    if (root == NULL)
    {
        return NULL; 
    }

    
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
        

        
        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }

        
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

        
        else
        {
            
            Node *temp = findMin(root->right);

            
            root->key = temp->key;

            
            root->right = deleteNode(root->right, temp->key);
        }
    }
    return root;
}

void inOrder(Node *root)
{
    if (root != NULL)
    {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}

void preOrder(Node *root)
{
    if (root != NULL)
    {
        printf("%d ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void postOrder(Node *root)
{
    if (root != NULL)
    {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->key);
    }
}

int main()
{
    Node *root = NULL;

    printf("=== BINARY SEARCH TREE (BST) DEMO ===\n\n");

    
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

    
    Node *minNode = findMin(root);
    Node *maxNode = findMax(root);
    if (minNode) printf("Minimum key: %d\n", minNode->key);
    if (maxNode) printf("Maximum key: %d\n", maxNode->key);

    
    int target = 40;
    Node *searchResult = searchNode(root, target);
    printf("\nSearching for %d: %s\n", target, (searchResult != NULL) ? "Found" : "Not Found");

    
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
