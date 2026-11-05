

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int height;              
    struct Node *left;
    struct Node *right;
} Node;

int getHeight(Node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

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

int getBalanceFactor(Node *node)
{
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    
    x->right = y;
    y->left = T2;

    
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    
    return x;
}

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    
    y->left = x;
    x->right = T2;

    
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    
    return y;
}

Node *insertNode(Node *node, int key)
{
    
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
        
        return node;
    }

    
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    
    int balance = getBalanceFactor(node);

    

    
    if (balance > 1 && key < node->left->key)
    {
        return rightRotate(node);
    }

    
    if (balance < -1 && key > node->right->key)
    {
        return leftRotate(node);
    }

    
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    
    return node;
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

Node *deleteNode(Node *root, int key)
{
    
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
        
        if (root->left == NULL || root->right == NULL)
        {
            Node *temp = (root->left != NULL) ? root->left : root->right;

            
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else 
            {
                *root = *temp; 
            }
            free(temp);
        }
        else
        {
            
            Node *temp = findMin(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    
    if (root == NULL)
        return NULL;

    
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    
    int balance = getBalanceFactor(root);

    

    
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    
    if (balance > 1 && getBalanceFactor(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    
    if (balance < -1 && getBalanceFactor(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node *searchNode(Node *root, int target)
{
    if (root == NULL || root->key == target)
        return root;

    if (target < root->key)
        return searchNode(root->left, target);

    return searchNode(root->right, target);
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

int main()
{
    Node *root = NULL;

    printf("=== AVL TREE DEMO (SELF-BALANCING BST) ===\n\n");

    
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

    
    int target = 30;
    Node *found = searchNode(root, target);
    printf("\nSearch for %d: %s\n", target, (found != NULL) ? "Found" : "Not Found");

    
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
