#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============
// Circular Queue Def.
// =============

struct CircularQueueStruct {
    int *key;
    int front, rear, size, max_queue_size;
};

typedef struct CircularQueueStruct *CircularQueue;

// =============
// Circular Queue Impl.
// =============

CircularQueue CircularQueue_new(int max) {
    CircularQueue queue = (CircularQueue) malloc(sizeof(struct CircularQueueStruct));
    queue->key = (int *) malloc(sizeof(int) * max);
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->max_queue_size = max;
    return queue;
}

int CircularQueue_isEmpty(CircularQueue queue) {
    return queue->size == 0;
}

int CircularQueue_isFull(CircularQueue queue) {
    return queue->size == queue->max_queue_size;
}

// ensure queue is not empty
int CircularQueue_front(CircularQueue queue) {
    return queue->key[queue->front];
}

// ensure queue is not empty
int CircularQueue_rear(CircularQueue queue) {
    return queue->key[queue->rear];
}

void CircularQueue_enqueue(CircularQueue queue, int x) {
    if (CircularQueue_isFull(queue)) {
        printf("Enqueue failed : Queue is Full!\n");
        return;
    }
    queue->rear = (queue->rear + 1) % queue->max_queue_size;
    queue->key[queue->rear] = x;
    queue->size++;
    printf("Enqueue %d\n", x);
}

void CircularQueue_dequeue(CircularQueue queue) {
    if (CircularQueue_isEmpty(queue)) {
        printf("Dequeue failed : Queue is Empty!\n");
        return;
    }
    int tmp = CircularQueue_front(queue);
    queue->front = (queue->front + 1) % queue->max_queue_size;
    queue->size--;
    printf("Dequeue %d\n", tmp);
}

void CircularQueue_delete(CircularQueue queue) {
    free(queue->key);
    free(queue);
}

void CircularQueue_printFront(CircularQueue queue) {
    if (CircularQueue_isEmpty(queue)) {
        printf("Queue is Empty!\n");
        return;
    }
    printf("Element in the front : %d\n", CircularQueue_front(queue));
}

void CircularQueue_printRear(CircularQueue queue) {
    if (CircularQueue_isEmpty(queue)) {
        printf("Queue is Empty!\n");
        return;
    }
    printf("Element in the rear : %d\n", CircularQueue_rear(queue));
}

// =============
// MAIN LOGIC
// =============

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Please enter an input file.\n");
        return 0;
    } else if (argc > 2) {
        printf("Too many arguments.\n");
        return 0;
    }

    char command;
    FILE *input;
    CircularQueue queue = NULL;

    int tmp;

    input = fopen(argv[1], "r");

    while (1) {
        command = fgetc(input);
        if (feof(input)) {
            break;
        }
        switch (command) {
            case 'n':
                fscanf(input, "%d", &tmp);
                if (queue == NULL) {
                    queue = CircularQueue_new(tmp);
                } else {
                    printf("Queue already created!\n");
                    exit(1);
                }

                break;
            case 'e':
                fscanf(input, "%d", &tmp);
                CircularQueue_enqueue(queue, tmp);
                break;
            case 'd':
                CircularQueue_dequeue(queue);
                break;
            case 'f':
                CircularQueue_printFront(queue);
                break;
            case 'r':
                CircularQueue_printRear(queue);
                break;
            case 'q':
                printf("Queue size: %d\n", queue->size);
                break;
            case ' ':
            case '\0':
            case '\t':
            case '\r':
            case '\n':
                break;
            default:
                printf("Invalid command!\n");
                break;
        }
    }

    if (queue != NULL) {
        CircularQueue_delete(queue);
        queue = NULL;
    }

    fclose(input);
    return 0;
}