

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct FibNode {
    int key;                    
    int degree;                 
    int mark;                   
    struct FibNode *parent;     
    struct FibNode *child;      
    struct FibNode *left;       
    struct FibNode *right;      
} FibNode;

typedef struct FibHeap {
    FibNode *min;               
    int n;                      
} FibHeap;

void consolidate(FibHeap *H);

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
    
    x->left = x;
    x->right = x;
    return x;
}

static void insertAfter(FibNode *a, FibNode *x)
{
    x->right = a->right;
    x->left = a;
    a->right->left = x;
    a->right = x;
}

static void removeFromList(FibNode *x)
{
    x->left->right = x->right;
    x->right->left = x->left;
    x->left = x;
    x->right = x;
}

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

FibNode *fibInsert(FibHeap *H, int key)
{
    FibNode *x = makeNode(key);
    addToRootList(H, x);
    H->n++;
    return x;
}

FibNode *fibMinimum(FibHeap *H)
{
    return (H == NULL) ? NULL : H->min;
}

FibNode *fibExtractMin(FibHeap *H)
{
    if (H == NULL || H->min == NULL)
        return NULL;

    FibNode *z = H->min;

    
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

    
    if (z->right == z)
    {
        H->min = NULL; 
    }
    else
    {
        H->min = z->right;
        removeFromList(z);
    }
    H->n--;

    
    if (H->min != NULL)
    {
        consolidate(H);
    }

    z->left = z;
    z->right = z;
    z->parent = NULL;
    return z;
}

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

    
    for (int i = 0; i < rootCount; i++)
    {
        FibNode *x = roots[i];
        int degree = x->degree;

        
        while (table[degree] != NULL)
        {
            FibNode *y = table[degree];
            
            if (y->key < x->key)
            {
                FibNode *temp = x;
                x = y;
                y = temp;
            }
            table[degree] = NULL;

            
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

void cascadingCut(FibHeap *H, FibNode *y)
{
    FibNode *z = y->parent;
    if (z != NULL)
    {
        if (y->mark == 0)
        {
            
            y->mark = 1;
        }
        else
        {
            
            cut(H, y, z);
            cascadingCut(H, z);
        }
    }
}

int fibDecreaseKey(FibHeap *H, FibNode *x, int newKey)
{
    if (H == NULL || x == NULL || newKey > x->key)
    {
        printf("[Error] New key cannot be greater than current key!\n");
        return 0;
    }

    x->key = newKey;
    FibNode *y = x->parent;

    
    if (y != NULL && x->key < y->key)
    {
        cut(H, x, y);
        cascadingCut(H, y);
    }

    
    if (H->min == NULL || x->key < H->min->key)
    {
        H->min = x;
    }
    return 1;
}

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

int main()
{
    printf("=== FIBONACCI HEAP DEMO (PRIORITY QUEUE) ===\n\n");

    FibHeap *heap = makeHeap();

    
    printf("Inserting keys: 30, 10, 20, 5, 40...\n");
    fibInsert(heap, 30);
    fibInsert(heap, 10);
    FibNode *node20 = fibInsert(heap, 20);
    fibInsert(heap, 5);
    FibNode *node40 = fibInsert(heap, 40);

    printf("Total nodes: %d\n", heap->n);
    printf("Minimum key: %d\n\n", fibMinimum(heap)->key);

    
    FibNode *minNode = fibExtractMin(heap);
    printf("Extracted Min: %d\n", minNode->key);
    free(minNode);

    printf("New Minimum key after extraction: %d\n\n", fibMinimum(heap)->key);

    
    printf("Decreasing key of node (40 -> 2)...\n");
    fibDecreaseKey(heap, node40, 2);
    printf("New Minimum key: %d\n\n", fibMinimum(heap)->key);

    
    printf("Extracting all remaining elements:\n");
    while (heap->min != NULL)
    {
        FibNode *curr = fibExtractMin(heap);
        printf("%d ", curr->key);
        free(curr);
    }
    printf("\n");

    free(heap);
    (void)node20; 
    return 0;
}
