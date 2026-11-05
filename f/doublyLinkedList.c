

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

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

void insertAtEnd(Node **head, int data)
{
    Node *newNode = createNode(data);

    
    if (*head == NULL)
    {
        *head = newNode;
        printf("Inserted %d at end (as head).\n", data);
        return;
    }

    
    Node *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->prev = temp;
    printf("Inserted %d at end.\n", data);
}

void insertAtPosition(Node **head, int data, int position)
{
    if (position < 1)
    {
        printf("[Error] Invalid position %d! Position must be >= 1.\n", position);
        return;
    }

    
    if (position == 1)
    {
        insertAtBeginning(head, data);
        return;
    }

    
    Node *temp = *head;
    for (int i = 1; i < position - 1 && temp != NULL; i++)
    {
        temp = temp->next;
    }

    
    if (temp == NULL)
    {
        printf("[Error] Position %d out of bounds!\n", position);
        return;
    }

    Node *newNode = createNode(data);

    
    newNode->next = temp->next;
    newNode->prev = temp;

    
    if (temp->next != NULL)
    {
        temp->next->prev = newNode;
    }
    temp->next = newNode;
    printf("Inserted %d at position %d.\n", data, position);
}

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

void deleteAtEnd(Node **head)
{
    if (*head == NULL)
    {
        printf("[Error] List is empty! Nothing to delete.\n");
        return;
    }

    Node *temp = *head;

    
    if (temp->next == NULL)
    {
        *head = NULL;
        printf("Deleted %d from end (list is now empty).\n", temp->data);
        free(temp);
        return;
    }

    
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    
    temp->prev->next = NULL;
    printf("Deleted %d from end.\n", temp->data);
    free(temp);
}

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

    
    temp->prev->next = temp->next;
    if (temp->next != NULL)
    {
        temp->next->prev = temp->prev;
    }

    printf("Deleted %d from position %d.\n", temp->data, position);
    free(temp);
}

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

void displayBackward(Node *head)
{
    if (head == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    
    Node *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    
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

int main()
{
    Node *head = NULL;

    printf("=== DOUBLY LINKED LIST DEMO ===\n\n");

    insertAtEnd(&head, 10);
    insertAtEnd(&head, 20);
    insertAtBeginning(&head, 5);
    insertAtEnd(&head, 30);
    insertAtPosition(&head, 15, 3); 

    displayForward(head);
    displayBackward(head);

    printf("Total nodes: %d\n\n", countNodes(head));

    
    int target = 15;
    Node *found = searchNode(head, target);
    printf("Searching for %d: %s\n\n", target, (found != NULL) ? "Found" : "Not Found");

    
    deleteAtBeginning(&head);
    displayForward(head);

    deleteAtEnd(&head);
    displayForward(head);

    deleteAtPosition(&head, 2);
    displayForward(head);

    return 0;
}
