#include <stdio.h>
#include <stdlib.h>

//
// Def.Impl. Queue
//

typedef struct Queue {
    int front;
    int rear;
    int size;
    int capacity;
    void **array;
} Queue;

Queue *queue_create(int capacity) {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = 0;
    queue->size = 0;
    queue->rear = capacity - 1;// rear is initialized to the last index
    queue->array = (void **) malloc(queue->capacity * sizeof(void *));
    return queue;
}

int queue_is_full(Queue *queue) {
    return (queue->size == queue->capacity);
}

int queue_is_empty(Queue *queue) {
    return (queue->size == 0);
}

void queue_enqueue(Queue *queue, void *item) {
    if (queue_is_full(queue)) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;// wrap around
    queue->array[queue->rear] = item;
    queue->size++;
}

void *queue_dequeue(Queue *queue) {
    if (queue_is_empty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return NULL;
    }
    void *item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;// wrap around
    queue->size--;
    return item;
}

void *queue_peek(Queue *queue) {
    if (queue_is_empty(queue)) {
        printf("Queue is empty. Cannot peek.\n");
        return NULL;
    }
    return queue->array[queue->front];
}

void queue_free(Queue *queue) {
    if (queue) {
        for (int i = 0; i < queue->size; i++) {
            free(queue->array[(queue->front + i) % queue->capacity]);
        }
        free(queue->array);
        free(queue);
    }
}

typedef Queue QueueInt;

QueueInt *queue_int_create(int capacity) {
    return (QueueInt *) queue_create(capacity);
}

void queue_int_enqueue(QueueInt *queue, int item) {
    int *item_ptr = (int *) malloc(sizeof(int));
    *item_ptr = item;
    queue_enqueue(queue, item_ptr);
}

int queue_int_dequeue(QueueInt *queue) {
    int *item_ptr = (int *) queue_dequeue(queue);
    if (item_ptr == NULL) {
        return -1;// or some other error value
    }
    int item = *item_ptr;
    free(item_ptr);
    return item;
}

int queue_int_peek(QueueInt *queue) {
    int *item_ptr = (int *) queue_peek(queue);
    if (item_ptr == NULL) {
        return -1;// or some other error value
    }
    return *item_ptr;
}

void queue_int_free(QueueInt *queue) {
    if (queue) {
        queue_free(queue);
    }
}

//
// Def.Impl. Graph
//

typedef struct Graph {
    int vertices;// Number of vertices
    int **adj;
} Graph;

Graph *graph_new(int vertices) {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph->vertices = vertices;
    graph->adj = (int **) malloc(vertices * sizeof(int *));
    for (int i = 0; i < vertices; i++) {
        graph->adj[i] = (int *) calloc(vertices, sizeof(int));
    }
    return graph;
}

void graph_free(Graph *graph) {
    if (graph) {
        for (int i = 0; i < graph->vertices; i++) {
            free(graph->adj[i]);
        }
        free(graph->adj);
        free(graph);
    }
}

void graph_add_edge(Graph *graph, int start, int end) {
    if (start < 0 || start >= graph->vertices || end < 0 || end >= graph->vertices) {
        printf("Invalid edge (%d, %d)\n", start, end);
        return;
    }
    graph->adj[start][end] = 1;
}

//
// TopoSort Utils
//

int *get_indegree_from_graph(Graph *graph) {
    int *indegree = (int *) calloc(graph->vertices, sizeof(int));
    for (int i = 0; i < graph->vertices; i++) {
        for (int j = 0; j < graph->vertices; j++) {
            if (graph->adj[j][i]) {
                indegree[i]++;
            }
        }
    }
    return indegree;
}

void vertices_add_label(int *vertices_lbl, int index, int label) {
    vertices_lbl[index] = label;
}

int get_verticies_from_label(int *vertices_lbl, int label, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        if (vertices_lbl[i] == label) {
            return i;
        }
    }
    return -1;// Not found
}

int *get_toposort(Graph *graph) {// return index

    int *toposorted_vertices = (int *) malloc(graph->vertices * sizeof(int));
    int *indegree = get_indegree_from_graph(graph);
    QueueInt *queue = queue_int_create(graph->vertices);

    for (int i = 0; i < graph->vertices; i++) {
        if (indegree[i] == 0) {
            queue_int_enqueue(queue, i);
        }
    }
    int curr_index = 0;
    while (!queue_is_empty(queue)) {
        int curr = queue_int_dequeue(queue);
        toposorted_vertices[curr_index] = curr;// Store the vertex index
        curr_index++;
        for (int i = 0; i < graph->vertices; i++) {
            if (graph->adj[curr][i]) {// If there is an edge from current_vertex to i
                indegree[i]--;
                if (indegree[i] == 0) {
                    queue_int_enqueue(queue, i);
                }
            }
        }
    }

    if (curr_index != graph->vertices) {
        free(toposorted_vertices);

        toposorted_vertices = NULL;
    }

    free(indegree);
    queue_int_free(queue);

    return toposorted_vertices;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments.\n");
        return 1;
    }

    FILE *fi = fopen(argv[1], "r");
    if (fi == NULL) {
        printf("Error opening file %s.\n", argv[1]);
        return 1;
    }
    int num_vertices, s, e, s_lbl, e_lbl;
    fscanf(fi, "%d", &num_vertices);
    Graph *graph = graph_new(num_vertices);
    int *vertices_label = calloc(num_vertices, sizeof(int));
    for (int i = 0; i < num_vertices; i++) {
        int lbl;
        fscanf(fi, "%d", &lbl);
        vertices_add_label(vertices_label, i, lbl);
    }
    while (fscanf(fi, "%d %d", &s_lbl, &e_lbl) != EOF) {
        s = get_verticies_from_label(vertices_label, s_lbl, num_vertices);
        e = get_verticies_from_label(vertices_label, e_lbl, num_vertices);
        //printf("Adding edge from %d as %d to %d as %d\n", s_lbl, s, e_lbl, e);
        graph_add_edge(graph, s, e);
    }
    fclose(fi);
    fi = NULL;

    int *toposorted_vertices = get_toposort(graph);
    if (toposorted_vertices == NULL) {
        printf("Graph has a cycle or is not a DAG.\n");
    } else {
        for (int i = 0; i < num_vertices; i++) {
            printf("%d ", vertices_label[toposorted_vertices[i]]);
        }
        printf("\n");
        free(toposorted_vertices);
        toposorted_vertices = NULL;
    }

    free(vertices_label);
    vertices_label = NULL;
    graph_free(graph);
    graph = NULL;

    return 0;
}
