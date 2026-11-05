/**
 * ============================================================================
 * DATA STRUCTURE: DOUBLY LINKED LIST
 * ============================================================================
 * 
 * DESCRIPTION:
 * A Doubly Linked List (DLL) is a linear collection of data nodes where each
 * node contains three fields:
 * 1. 'data': The integer value stored in the node.
 * 2. 'prev': Pointer to the previous node in the sequence (NULL for head).
 * 3. 'next': Pointer to the next node in the sequence (NULL for tail).
 * 
 * ADVANTAGES OVER SINGLY LINKED LIST:
 * - Can be traversed in both forward and backward directions.
 * - Deletion of a given node is O(1) if pointer to that node is already known
 *   (no need to traverse from head to find the predecessor).
 * - Easy insertion before a given node.
 * 
 * TIME COMPLEXITIES:
 * - Insertion at Beginning: O(1)
 * - Insertion at End:       O(n) (or O(1) if tail pointer is maintained)
 * - Insertion at Position:  O(n)
 * - Deletion at Beginning:  O(1)
 * - Deletion at End:        O(n)
 * - Deletion at Position:   O(n)
 * - Search / Traversal:     O(n)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Definition of a Doubly Linked List Node
typedef struct Node {
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

/**
 * Creates and allocates memory for a new node storing an integer data.
 */
Node *createNode(int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

/**
 * Inserts a new node with 'data' at the beginning (head) of the list.
 * 
 * Pointer Logic:
 * 1. newNode->next = *head;
 * 2. If list is not empty: (*head)->prev = newNode;
 * 3. *head = newNode;
 */
void insertAtBeginning(Node **head, int data)
{
    Node *newNode = createNode(data);

    newNode->next = *head;
    newNode->prev = NULL;

    if (*head != NULL)
    {
        (*head)->prev = newNode;
    }
    *head = newNode;
    printf("Inserted %d at beginning.\n", data);
}

/**
 * Inserts a new node with 'data' at the end of the list.
 * 
 * Pointer Logic:
 * 1. If list is empty: *head = newNode;
 * 2. Otherwise traverse to last node (temp->next == NULL).
 * 3. temp->next = newNode;
 * 4. newNode->prev = temp;
 */
void insertAtEnd(Node **head, int data)
{
    Node *newNode = createNode(data);

    // If list is empty, new node becomes the head
    if (*head == NULL)
    {
        *head = newNode;
        printf("Inserted %d at end (as head).\n", data);
        return;
    }

    // Traverse to the last node
    Node *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->prev = temp;
    printf("Inserted %d at end.\n", data);
}

/**
 * Inserts a new node with 'data' at a specific 1-based position.
 * Position 1 = beginning of list.
 */
void insertAtPosition(Node **head, int data, int position)
{
    if (position < 1)
    {
        printf("[Error] Invalid position %d! Position must be >= 1.\n", position);
        return;
    }

    // If inserting at the very beginning
    if (position == 1)
    {
        insertAtBeginning(head, data);
        return;
    }

    // Traverse to (position - 1) node
    Node *temp = *head;
    for (int i = 1; i < position - 1 && temp != NULL; i++)
    {
        temp = temp->next;
    }

    // If position is beyond (length + 1)
    if (temp == NULL)
    {
        printf("[Error] Position %d out of bounds!\n", position);
        return;
    }

    Node *newNode = createNode(data);

    // Wire up new node's next and prev
    newNode->next = temp->next;
    newNode->prev = temp;

    // If not inserting at the very end, update successor's prev pointer
    if (temp->next != NULL)
    {
        temp->next->prev = newNode;
    }
    temp->next = newNode;
    printf("Inserted %d at position %d.\n", data, position);
}

/**
 * Deletes the first node (head) of the list.
 * 
 * Pointer Logic:
 * 1. Save temp = *head;
 * 2. *head = (*head)->next;
 * 3. If new head != NULL, (*head)->prev = NULL;
 * 4. free(temp);
 */
void deleteAtBeginning(Node **head)
{
    if (*head == NULL)
    {
        printf("[Error] List is empty! Nothing to delete.\n");
        return;
    }

    Node *temp = *head;
    *head = (*head)->next;

    if (*head != NULL)
    {
        (*head)->prev = NULL;
    }

    printf("Deleted %d from beginning.\n", temp->data);
    free(temp);
}

/**
 * Deletes the last node of the list.
 * 
 * Pointer Logic:
 * 1. If only 1 node: free and set *head = NULL.
 * 2. Otherwise traverse to last node (temp->next == NULL).
 * 3. temp->prev->next = NULL;
 * 4. free(temp);
 */
void deleteAtEnd(Node **head)
{
    if (*head == NULL)
    {
        printf("[Error] List is empty! Nothing to delete.\n");
        return;
    }

    Node *temp = *head;

    // Case: Single node in list
    if (temp->next == NULL)
    {
        *head = NULL;
        printf("Deleted %d from end (list is now empty).\n", temp->data);
        free(temp);
        return;
    }

    // Traverse to the last node
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    // Disconnect last node from second-to-last node
    temp->prev->next = NULL;
    printf("Deleted %d from end.\n", temp->data);
    free(temp);
}

/**
 * Deletes the node at a given 1-based position.
 */
void deleteAtPosition(Node **head, int position)
{
    if (*head == NULL)
    {
        printf("[Error] List is empty! Nothing to delete.\n");
        return;
    }

    if (position < 1)
    {
        printf("[Error] Invalid position %d!\n", position);
        return;
    }

    if (position == 1)
    {
        deleteAtBeginning(head);
        return;
    }

    Node *temp = *head;
    for (int i = 1; i < position && temp != NULL; i++)
    {
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("[Error] Position %d out of bounds!\n", position);
        return;
    }

    // Bypass temp in both directions
    temp->prev->next = temp->next;
    if (temp->next != NULL)
    {
        temp->next->prev = temp->prev;
    }

    printf("Deleted %d from position %d.\n", temp->data, position);
    free(temp);
}

/**
 * Searches for a node with the given target integer.
 * Returns pointer to node if found, or NULL if not found.
 */
Node *searchNode(Node *head, int target)
{
    Node *temp = head;
    while (temp != NULL)
    {
        if (temp->data == target)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/**
 * Counts and returns the total number of nodes in the list.
 */
int countNodes(Node *head)
{
    int count = 0;
    Node *temp = head;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

/**
 * Displays the list elements in forward order (head to tail).
 */
void displayForward(Node *head)
{
    if (head == NULL)
    {
        printf("List is empty.\n");
        return;
    }
    printf("Forward:  NULL <- ");
    Node *temp = head;
    while (temp != NULL)
    {
        printf("[%d]", temp->data);
        if (temp->next != NULL)
            printf(" <-> ");
        temp = temp->next;
    }
    printf(" -> NULL\n");
}

/**
 * Displays the list elements in reverse order (tail to head).
 */
void displayBackward(Node *head)
{
    if (head == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    // Traverse to the last node
    Node *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    // Traverse backwards using prev pointers
    printf("Backward: NULL <- ");
    while (temp != NULL)
    {
        printf("[%d]", temp->data);
        if (temp->prev != NULL)
            printf(" <-> ");
        temp = temp->prev;
    }
    printf(" -> NULL\n");
}

/* ============================================================================
 * DEMO MAIN FUNCTION FOR TESTING & REVISION
 * ============================================================================ */
int main()
{
    Node *head = NULL;

    printf("=== DOUBLY LINKED LIST DEMO ===\n\n");

    insertAtEnd(&head, 10);
    insertAtEnd(&head, 20);
    insertAtBeginning(&head, 5);
    insertAtEnd(&head, 30);
    insertAtPosition(&head, 15, 3); // Insert 15 at index 3

    displayForward(head);
    displayBackward(head);

    printf("Total nodes: %d\n\n", countNodes(head));

    // Search
    int target = 15;
    Node *found = searchNode(head, target);
    printf("Searching for %d: %s\n\n", target, (found != NULL) ? "Found" : "Not Found");

    // Deletions
    deleteAtBeginning(&head);
    displayForward(head);

    deleteAtEnd(&head);
    displayForward(head);

    deleteAtPosition(&head, 2);
    displayForward(head);

    return 0;
}
