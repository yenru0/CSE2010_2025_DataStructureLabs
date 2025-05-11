#include <stdio.h>
#include <stdlib.h>

//
// Def. Heap(Int)
//

typedef struct Heap {
    int capacity;
    int size;
    int *data;// index is starting from 0,
} Heap;

/**
 * it creates empty Heap with size.
 * if not successful, it returns NULL.
 * 
 * @param[in] size initial size
 * @return created Heap pointer
 */
Heap *heap_create(int size);

/**
 * delete heap.
 * @return signal is successful
 */
int heap_delete(Heap *heap);
/**
 * check heap is full
 * @return 1 if is full else 0
 */
int heap_is_full(Heap *heap);
/**
 * check heap is empty
 * @return 1 if is empty else 0
 */
int heap_is_empty(Heap *heap);
/**
 * inserts value in the heap.
 * @param value value inserted
 * 
 * @return if successful 1, else 0;
 * if value is already in heap, return -1.
 */
int heap_insert(Heap *heap, int value);
/**
 * find value of index of heap.
 * 
 * @param value finding value
 * 
 * @return index of value, -1 if not found
 */
int heap_find(Heap *heap, int value);
/**
 * find value and pop it from heap.
 * 
 * @param[out] out value popped
 * 
 * @return if successful 1, else 0;
 */
int heap_pop(Heap *heap, int *out);
/**
 * print heap.
 */
void heap_print(Heap *heap);

//
// MAIN LOGIC
//

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid arguments, usage: %s <filename>\n", argv[0]);
        return 1;
    }
    FILE *fi = fopen(argv[1], "r");

    if (fi == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }

    char cv;
    Heap *heap = NULL;
    int value;
    int out;
    int signal;

    while (!feof(fi)) {
        cv = ' ';
        fscanf(fi, "%c", &cv);
        switch (cv) {
            case 'n':
                fscanf(fi, "%d", &value);
                if (heap != NULL) {
                    printf("Create Error : Max Heap is already created.\n");
                    break;
                }
                heap = heap_create(value);
                break;
            case 'i':
                if (heap == NULL) {
                    printf("Logic Error : Max Heap is not created.\n");
                    break;
                }
                fscanf(fi, "%d", &value);
                signal = heap_insert(heap, value);
                if (signal == -1) {
                    printf("%d is already in the heap.\n", value);
                    break;
                } else if (signal == 0) {
                    printf("Insertion Error : Max Heap is full!\n");
                    break;
                } else {
                    printf("Insert %d\n", value);
                }
                break;
            case 'd':
                if (heap == NULL) {
                    printf("Logic Error : Max Heap is not created.\n");
                    break;
                }

                signal = heap_pop(heap, &out);
                if (signal == 0) {
                    printf("Deletion Error : Max Heap is empty!\n");
                    break;
                } else {
                    printf("Max Element %d is deleted.\n", out);
                }
                break;
            case 'f':
                if (heap == NULL) {
                    printf("Logic Error : Max Heap is not created.\n");
                    break;
                }
                fscanf(fi, "%d", &value);
                if (heap_find(heap, value) != -1) {
                    printf("%d is in the heap.\n", value);
                } else {
                    printf("%d is not in the heap.\n", value);
                }
                break;
            case 'p':
                if (heap == NULL) {
                    printf("Logic Error : Max Heap is not created.\n");
                    break;
                }

                if (heap_is_empty(heap)) {
                    printf("Print Error : Max Heap is empty!\n");
                    break;
                }

                heap_print(heap);
                break;
            case ' ':
            case '\n':
            case '\t':
            case '\r':
                break;

            default:
                printf("Logic Error : Invalid command: %c\n", cv);
                break;
        }
    }
    heap_delete(heap);
    heap = NULL;
    fclose(fi);
    fi = NULL;
    return 0;
}

//
// Impl. Heap
//

Heap *heap_create(int size) {
    Heap *heap = malloc(sizeof(Heap));
    if (heap == NULL) {
        printf("fatal error making heap.");
        return NULL;
    }
    heap->size = size;
    heap->capacity = 0;
    heap->data = calloc(size, sizeof(int));
    return heap;
}

int heap_delete(Heap *heap) {
    if (heap == NULL) {
        return 0;
    } else {
        free(heap->data);
        free(heap);
    }
    return 1;
}

int heap_is_full(Heap *heap) {
    return heap->capacity == heap->size;
}

int heap_is_empty(Heap *heap) {
    return heap->capacity == 0;
}

int heap_insert(Heap *heap, int value) {
    if (heap_is_full(heap)) {
        return 0;
    } else if (heap_find(heap, value) != -1) {
        return -1;
    }
    heap->data[heap->capacity] = value;
    int i = heap->capacity;
    heap->capacity++;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->data[parent] < heap->data[i]) {
            int temp = heap->data[parent];
            heap->data[parent] = heap->data[i];
            heap->data[i] = temp;
            i = parent;
        } else {
            break;
        }
    }
    return 1;
}

int heap_find(Heap *heap, int value) {
    for (int i = 0; i < heap->capacity; i++) {
        if (heap->data[i] == value) {
            return i;
        }
    }
    return -1;
}

int heap_pop(Heap *heap, int *out) {
    if (heap_is_empty(heap)) {
        return 0;
    }
    *out = heap->data[0];
    heap->data[0] = heap->data[heap->capacity - 1];
    heap->capacity--;

    int i = 0;
    while (i < heap->capacity) {
        int left = i * 2 + 1;
        int right = i * 2 + 2;
        if (left >= heap->capacity) {
            break;
        }
        int max_child = left;
        if (right < heap->capacity && heap->data[right] > heap->data[left]) {
            max_child = right;
        }
        if (heap->data[i] < heap->data[max_child]) {
            int temp = heap->data[i];
            heap->data[i] = heap->data[max_child];
            heap->data[max_child] = temp;
            i = max_child;
        } else {
            break;
        }
    }
    return 1;
}

void heap_print(Heap *heap) {
    for (int i = 0; i < heap->capacity; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
}
