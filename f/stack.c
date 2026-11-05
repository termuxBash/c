

#include <stdio.h>
#include <stdlib.h>

#define MAX 100 

int stack[MAX];
int top = -1;

int isEmpty()
{
    return (top == -1);
}

int isFull()
{
    return (top == MAX - 1);
}

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

int peek()
{
    if (isEmpty())
    {
        printf("[Error] Stack is empty! No top element.\n");
        return -1;
    }
    return stack[top];
}

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
