

#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYS 3
#define MIN_KEYS 1
#define MAX_CHILDREN (MAX_KEYS + 1)

typedef struct BTreeNode {
    int keys[MAX_KEYS];                       
    struct BTreeNode *children[MAX_CHILDREN]; 
    int numKeys;                              
    int isLeaf;                               
} BTreeNode;

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

BTreeNode *search(BTreeNode *root, int key)
{
    if (root == NULL)
        return NULL;

    int i = 0;
    
    while (i < root->numKeys && key > root->keys[i])
    {
        i++;
    }

    
    if (i < root->numKeys && key == root->keys[i])
    {
        return root;
    }

    
    if (root->isLeaf)
    {
        return NULL;
    }

    
    return search(root->children[i], key);
}

void splitChild(BTreeNode *parent, int index, BTreeNode *child)
{
    
    BTreeNode *newChild = createNode(child->isLeaf);
    newChild->numKeys = MIN_KEYS;

    
    for (int i = 0; i < MIN_KEYS; i++)
    {
        newChild->keys[i] = child->keys[i + MIN_KEYS + 1];
    }

    
    if (!child->isLeaf)
    {
        for (int i = 0; i <= MIN_KEYS; i++)
        {
            newChild->children[i] = child->children[i + MIN_KEYS + 1];
        }
    }

    
    child->numKeys = MIN_KEYS;

    
    for (int i = parent->numKeys; i >= index + 1; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newChild;

    
    for (int i = parent->numKeys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }

    
    parent->keys[index] = child->keys[MIN_KEYS];
    parent->numKeys++;
}

void insertNonFull(BTreeNode *node, int key)
{
    int i = node->numKeys - 1;

    
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
    
    else
    {
        
        while (i >= 0 && node->keys[i] > key)
        {
            i--;
        }
        i++;

        
        if (node->children[i]->numKeys == MAX_KEYS)
        {
            splitChild(node, i, node->children[i]);

            
            
            if (key > node->keys[i])
            {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

void insert(BTreeNode **root, int key)
{
    BTreeNode *r = *root;

    
    if (r == NULL)
    {
        *root = createNode(1);
        (*root)->keys[0] = key;
        (*root)->numKeys = 1;
        return;
    }

    
    if (r->numKeys == MAX_KEYS)
    {
        BTreeNode *newRoot = createNode(0);
        newRoot->children[0] = r;

        
        splitChild(newRoot, 0, r);

        
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

void traverse(BTreeNode *root)
{
    if (root == NULL)
        return;

    int i;
    for (i = 0; i < root->numKeys; i++)
    {
        
        if (!root->isLeaf)
        {
            traverse(root->children[i]);
        }
        printf("%d ", root->keys[i]);
    }

    
    if (!root->isLeaf)
    {
        traverse(root->children[i]);
    }
}

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

int main()
{
    BTreeNode *root = NULL;

    printf("=== B-TREE DEMO (ORDER 4 / MAX 3 KEYS PER NODE) ===\n\n");

    
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

    
    searchKey(root, 6);
    searchKey(root, 15);
    searchKey(root, 30);

    return 0;
}
