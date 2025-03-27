#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============
// Stack Def.
// =============

typedef struct Stack {
    int *key;
    int top, max_stack_size;
} Stack;

// =============
// Stack Impl.
// =============

Stack *Stack_new(int max) {
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->key = (int *) malloc(sizeof(int) * max);
    stack->top = -1;
    stack->max_stack_size = max;
    return stack;
}

void Stack_push(Stack *s, int x) {
    if (s->top == s->max_stack_size - 1) {
        printf("\nStack overflow!\n");
        exit(1);
        return;
    }
    s->key[++s->top] = x;
}

int Stack_pop(Stack *s) {
    if (s->top == -1) {
        printf("\nStack underflow!\n");
        exit(1);
        return -1;
    }
    return s->key[s->top--];
}

int Stack_top(Stack *s) {
    if (s->top == -1) {
        printf("\nStack underflow!\n");
        exit(1);
        return -1;
    }
    return s->key[s->top];
}

void Stack_delete(Stack *s) {
    free(s->key);
    free(s);
}

int Stack_isEmpty(Stack *s) {
    return s->top == -1;
}

int Stack_isFull(Stack *s) {
    return s->top == s->max_stack_size - 1;
}

void Stack_postfix(Stack *s, char input_char) {
    int a, b;
    switch (input_char) {
        case '+':
            a = Stack_pop(s);
            b = Stack_pop(s);
            Stack_push(s, b + a);
            break;
        case '-':
            a = Stack_pop(s);
            b = Stack_pop(s);
            Stack_push(s, b - a);
            break;
        case '*':
            a = Stack_pop(s);
            b = Stack_pop(s);
            Stack_push(s, b * a);
            break;
        case '/':
            a = Stack_pop(s);
            b = Stack_pop(s);
            Stack_push(s, b / a);
            break;
        case '%':
            a = Stack_pop(s);
            b = Stack_pop(s);
            Stack_push(s, b % a);
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            Stack_push(s, input_char - '0');
            break;
        default:
            printf("\nInvalid operator!\n");
            exit(1);
            break;
    }
}

// =============
// MAIN LOGIC
// =============

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Please enter an input file.\n");
        return 0;
    }

    FILE *fi = fopen(argv[1], "r");

    if (fi == NULL) {
        printf("There is no file.\n");
        return 0;
    }

    Stack *stack = Stack_new(16);
    char c;
    printf("Top numbers: \n");
    while (1) {
        fscanf(fi, "%c", &c);
        if (c == '!') {
            printf("\nCalculating Done!");
            break;
        }
        Stack_postfix(stack, c);
        printf("%d ", Stack_top(stack));
    }
    printf("\n");
    printf("evaluation result: %d\n", Stack_pop(stack));
    Stack_delete(stack);
    return 0;
}