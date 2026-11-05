

#include <stdio.h>
#include <stdlib.h>

#define MAX 100 

int queue[MAX];
int front = -1;
int rear = -1;

int isEmpty()
{
    return (front == -1);
}

int isFull()
{
    return (rear == MAX - 1);
}

void enqueue(int value)
{
    if (isFull())
    {
        printf("[Error] Queue Overflow! Cannot enqueue %d.\n", value);
        return;
    }
    
    if (isEmpty())
    {
        front = 0;
    }
    rear++;
    queue[rear] = value;
    printf("Enqueued: %d\n", value);
}

int dequeue()
{
    if (isEmpty())
    {
        printf("[Error] Queue Underflow! Queue is empty.\n");
        return -1;
    }
    int value = queue[front];

    
    if (front == rear)
    {
        front = -1;
        rear = -1;
    }
    else
    {
        front++;
    }
    return value;
}

int peek()
{
    if (isEmpty())
    {
        printf("[Error] Queue is empty! No front element.\n");
        return -1;
    }
    return queue[front];
}

void display()
{
    if (isEmpty())
    {
        printf("Queue is empty.\n");
        return;
    }
    printf("Queue (front to rear): ");
    for (int i = front; i <= rear; i++)
    {
        printf("%d ", queue[i]);
    }
    printf("\n");
}

int main()
{
    printf("=== QUEUE DEMO (FIFO) ===\n\n");

    enqueue(10);
    enqueue(20);
    enqueue(30);
    enqueue(40);

    display();

    printf("Front element (peek): %d\n", peek());

    printf("Dequeued: %d\n", dequeue());
    printf("Dequeued: %d\n", dequeue());

    display();

    enqueue(50);
    display();

    printf("Queue is empty? %s\n", isEmpty() ? "Yes" : "No");

    return 0;
}
