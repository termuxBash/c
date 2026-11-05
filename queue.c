/**
 * ============================================================================
 * DATA STRUCTURE: QUEUE (Linear Array Implementation)
 * ============================================================================
 * 
 * DESCRIPTION:
 * A Queue is a linear data structure that follows the FIFO principle:
 * First-In, First-Out. The element inserted first is the first one removed.
 * 
 * KEY CONCEPTS & EXAM POINTERS:
 * 1. Pointers:
 *    - 'front': Tracks the index of the first element (where deletion happens).
 *    - 'rear':  Tracks the index of the last element (where insertion happens).
 *    - Initially, front = -1 and rear = -1 (indicates empty queue).
 * 2. Enqueue:
 *    - If empty (front == -1), set front = 0.
 *    - Increment rear and place element at queue[rear].
 * 3. Dequeue:
 *    - Retrieve element at queue[front].
 *    - If only one element was left (front == rear), reset both to -1.
 *    - Otherwise, increment front.
 * 4. Overflow & Underflow:
 *    - Overflow: rear == MAX - 1 (in linear array queue).
 *    - Underflow: front == -1 or front > rear.
 * 
 * TIME COMPLEXITIES:
 * - Enqueue: O(1)
 * - Dequeue: O(1)
 * - Peek:    O(1)
 * - IsEmpty: O(1)
 * - IsFull:  O(1)
 * SPACE COMPLEXITY: O(MAX)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX 100 // Maximum size of the queue

// Global queue storage and pointers
int queue[MAX];
int front = -1;
int rear = -1;

/**
 * Checks whether the queue is empty.
 * Returns 1 (true) if empty, 0 (false) otherwise.
 */
int isEmpty()
{
    return (front == -1);
}

/**
 * Checks whether the queue is full.
 * Returns 1 (true) if full, 0 (false) otherwise.
 */
int isFull()
{
    return (rear == MAX - 1);
}

/**
 * Inserts (enqueues) an integer into the rear of the queue.
 */
void enqueue(int value)
{
    if (isFull())
    {
        printf("[Error] Queue Overflow! Cannot enqueue %d.\n", value);
        return;
    }
    // If inserting into an empty queue, initialize front to 0
    if (isEmpty())
    {
        front = 0;
    }
    rear++;
    queue[rear] = value;
    printf("Enqueued: %d\n", value);
}

/**
 * Removes (dequeues) and returns the front integer from the queue.
 */
int dequeue()
{
    if (isEmpty())
    {
        printf("[Error] Queue Underflow! Queue is empty.\n");
        return -1;
    }
    int value = queue[front];

    // If the queue had only one element, reset it to empty state
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

/**
 * Returns the front integer without removing it.
 */
int peek()
{
    if (isEmpty())
    {
        printf("[Error] Queue is empty! No front element.\n");
        return -1;
    }
    return queue[front];
}

/**
 * Prints all elements currently present in the queue from front to rear.
 */
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

/* ============================================================================
 * DEMO MAIN FUNCTION FOR TESTING & REVISION
 * ============================================================================ */
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
