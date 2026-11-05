/**
 * ============================================================================
 * DATA STRUCTURE: STACK (Array Implementation)
 * ============================================================================
 * 
 * DESCRIPTION:
 * A Stack is a linear data structure that follows the LIFO principle:
 * Last-In, First-Out. The element inserted last is the first one to be removed.
 * 
 * KEY CONCEPTS & EXAM POINTERS:
 * 1. 'top': Points to the index of the top-most element in the stack.
 *    - Initially, top = -1 (indicates empty stack).
 * 2. Overflow condition:
 *    - Occurs when attempting to push onto a full stack (top == MAX - 1).
 * 3. Underflow condition:
 *    - Occurs when attempting to pop/peek from an empty stack (top == -1).
 * 
 * TIME COMPLEXITIES:
 * - Push:    O(1)
 * - Pop:     O(1)
 * - Peek:    O(1)
 * - IsEmpty: O(1)
 * - IsFull:  O(1)
 * SPACE COMPLEXITY: O(MAX)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX 100 // Maximum capacity of the stack

// Global stack array and top pointer
int stack[MAX];
int top = -1;

/**
 * Checks whether the stack is empty.
 * Returns 1 (true) if empty, 0 (false) otherwise.
 */
int isEmpty()
{
    return (top == -1);
}

/**
 * Checks whether the stack is full.
 * Returns 1 (true) if full, 0 (false) otherwise.
 */
int isFull()
{
    return (top == MAX - 1);
}

/**
 * Inserts (pushes) an element onto the top of the stack.
 * Step 1: Check for Overflow.
 * Step 2: Increment top pointer.
 * Step 3: Insert item at stack[top].
 */
void push(int item)
{
    if (isFull())
    {
        printf("[Error] Stack Overflow! Cannot push %d.\n", item);
        return;
    }
    top++;
    stack[top] = item;
    printf("Pushed: %d\n", item);
}

/**
 * Removes and returns the top-most element from the stack.
 * Step 1: Check for Underflow.
 * Step 2: Retrieve the item at stack[top].
 * Step 3: Decrement top pointer.
 */
int pop()
{
    if (isEmpty())
    {
        printf("[Error] Stack Underflow! Stack is empty.\n");
        return -1;
    }
    int item = stack[top];
    top--;
    return item;
}

/**
 * Returns the top-most element without removing it.
 * Step 1: Check for Underflow.
 * Step 2: Return stack[top].
 */
int peek()
{
    if (isEmpty())
    {
        printf("[Error] Stack is empty! No top element.\n");
        return -1;
    }
    return stack[top];
}

/**
 * Displays all elements currently stored in the stack from top to bottom.
 */
void display()
{
    if (isEmpty())
    {
        printf("Stack is empty.\n");
        return;
    }
    printf("Stack (top to bottom): ");
    for (int i = top; i >= 0; i--)
    {
        printf("%d ", stack[i]);
    }
    printf("\n");
}

/* ============================================================================
 * DEMO MAIN FUNCTION FOR TESTING & REVISION
 * ============================================================================ */
int main()
{
    printf("=== STACK DEMO (LIFO) ===\n\n");

    push(10);
    push(20);
    push(30);
    push(40);

    display();

    printf("Top element (peek): %d\n", peek());

    printf("Popped: %d\n", pop());
    printf("Popped: %d\n", pop());

    display();

    printf("Stack is empty? %s\n", isEmpty() ? "Yes" : "No");

    return 0;
}
