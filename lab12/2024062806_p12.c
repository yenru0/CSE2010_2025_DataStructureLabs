#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define LIST_INITIAL 16

//
// Def.Impl. Dynamic List
//

typedef struct List {
    int _max_size;
    int len;
    void **arr;
} List;

typedef List ListInt;
typedef List ListEdge;

List *list_new() {
    List *list = malloc(sizeof(List));
    list->_max_size = LIST_INITIAL;
    list->len = 0;
    list->arr = calloc(list->_max_size, sizeof(void *));
    return list;
}

//
// General Method of General List
//

int list_is_full(List *list) {
    return list->_max_size == list->len;
}

int list_is_empty(List *list) {
    return list->len == 0;
}

void list_expand(List *list) {
    list->arr = realloc(list->arr, sizeof(void *) * list->_max_size * 2);
    list->_max_size *= 2;
}
void list_append(List *list, void *element) {
    if (list_is_full(list)) {
        list_expand(list);
    }
    list->arr[list->len] = element;
    list->len++;
}

void list_set_at(List *list, int index, void *element) {
    if (list->arr[index] != NULL) {
        free(list->arr[index]);
    }
    list->arr[index] = element;
}

void *list_get_at(List *list, int index) {
    return list->arr[index];
}

void list_free(List *list) {
    void *curr = list->arr[0];
    int cnt = 0;
    while (curr != NULL) {
        free(curr);
        cnt++;
        curr = list->arr[cnt];
    }
    free(list->arr);
    free(list);
}

//
// List Int
//

void list_int_append(ListInt *list, int element) {
    int *temp = malloc(sizeof(int));
    *temp = element;
    list_append(list, temp);
}

void list_int_set_at(ListInt *list, int index, int element) {
    int *temp = malloc(sizeof(int));
    *temp = element;
    list_set_at(list, index, temp);
}

int list_int_get_at(ListInt *list, int index) {
    return *(int *) list_get_at(list, index);
}

void list_int_free(ListInt *list) {
    list_free(list);
}

//
// Def.Impl. PQ
//

typedef struct Pair {
    int distance, curr;
} Pair;

typedef struct MinEdgeHeap {
    int capacity;
    int size;
    Pair *data;
} MinEdgeHeap;

MinEdgeHeap *min_edge_heap_create() {
    MinEdgeHeap *heap = malloc(sizeof(MinEdgeHeap));
    if (heap == NULL) {
        printf("fatal error making heap.");
        return NULL;
    }
    heap->size = 0;
    heap->capacity = LIST_INITIAL;
    heap->data = calloc(LIST_INITIAL, sizeof(Pair));
    return heap;
}

int min_edge_heap_delete(MinEdgeHeap *heap) {
    if (heap == NULL) {
        return 0;
    } else {
        free(heap->data);
        free(heap);
    }
    return 1;
}

int min_edge_heap_is_full(MinEdgeHeap *heap) {
    return heap->capacity == heap->size;
}
int min_edge_heap_is_empty(MinEdgeHeap *heap) {
    return heap->size == 0;
}

void min_edge_heap_insert(MinEdgeHeap *heap, Pair edge) {
    if (min_edge_heap_is_full(heap)) {
        heap->capacity *= 2;
        heap->data = realloc(heap->data, sizeof(Pair) * heap->capacity);
        if (heap->data == NULL) {
            exit(1);
        }
    }
    heap->data[heap->size] = edge;
    int i = heap->size;
    heap->size++;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->data[parent].distance > heap->data[i].distance) {
            Pair temp = heap->data[parent];
            heap->data[parent] = heap->data[i];
            heap->data[i] = temp;
            i = parent;
        } else {
            break;
        }
    }
}

Pair min_edge_heap_pop(MinEdgeHeap *heap) {
    if (min_edge_heap_is_empty(heap)) {
        return (Pair) {-1, -1};
    }
    Pair out = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    int i = 0;
    while (i < heap->size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < heap->size && heap->data[left].distance < heap->data[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right].distance < heap->data[smallest].distance) {
            smallest = right;
        }
        if (smallest != i) {
            Pair temp = heap->data[i];
            heap->data[i] = heap->data[smallest];
            heap->data[smallest] = temp;
            i = smallest;
        } else {
            break;
        }
    }
    return out;
}

//
// Def. Edge
//

typedef struct Edge {
    int start, end;
    int weight;
} Edge;

//
// List of Edge
//

void list_edge_append(ListEdge *list, Edge element) {
    Edge *temp = malloc(sizeof(Edge));
    *temp = element;
    list_append(list, temp);
}

void list_edge_set_at(ListEdge *list, int index, Edge element) {
    Edge *temp = malloc(sizeof(Edge));
    *temp = element;
    list_set_at(list, index, temp);
}

Edge list_edge_get_at(ListEdge *list, int index) {
    return *(Edge *) list_get_at(list, index);
}

void list_edge_free(ListEdge *list) {
    list_free(list);
}

//
// Def.Impl Graph
//

typedef struct Graph {
    int vertex;
    List **edges;
} Graph;

Graph *graph_new(int vertex) {
    Graph *graph = malloc(sizeof(Graph));

    graph->vertex = vertex;
    graph->edges = calloc(graph->vertex, sizeof(List *));
    for (int i = 0; i < graph->vertex; i++) {
        graph->edges[i] = list_new();
    }
    return graph;
}

void graph_free(Graph *graph) {
    for (int i = 0; i < graph->vertex; i++) {
        list_edge_free(graph->edges[i]);
    }
    free(graph->edges);
    free(graph);
}

void graph_add_edge(Graph *graph, Edge *edge) {
    list_append(graph->edges[edge->start], edge);
}

//
// Def.Impl. Dijkstra
//

typedef struct ListPair {
    List *distances;
    List *prevs;
} ListPair;

ListPair dijkstra_helper(Graph *graph, int start) {
    ListInt *distances = list_new();
    ListInt *prevs = list_new();

    // Initializing
    for (int i = 0; i < graph->vertex; i++) {
        list_int_append(distances, INT_MAX);
        list_int_append(prevs, -1);
    }

    list_int_set_at(distances, start, 0);

    MinEdgeHeap *heap = min_edge_heap_create();

    min_edge_heap_insert(heap, (Pair) {0, start});// dist & now

    while (!min_edge_heap_is_empty(heap)) {
        Pair curr = min_edge_heap_pop(heap);

        ListEdge *edges = graph->edges[curr.curr];

        for (int i = 0; i < edges->len; i++) {
            Edge conn = list_edge_get_at(edges, i);

            int start = conn.start;
            int target = conn.end;
            int dist_of_conn = conn.weight;
            int new_dist = list_int_get_at(distances, start) + dist_of_conn;
            if (list_int_get_at(distances, target) > new_dist) {
                list_int_set_at(distances, target, new_dist);
                list_int_set_at(prevs, target, start);
                min_edge_heap_insert(heap, (Pair) {list_int_get_at(distances, target), target});
            }
        }
    }

    min_edge_heap_delete(heap);

    ListPair res;
    res.distances = distances;
    res.prevs = prevs;
    return res;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *fi = fopen(argv[1], "r");
    if (fi == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }

    int size;
    fscanf(fi, "%d", &size);

    Graph *graph = graph_new(size);

    int start, end, weight;
    while (fscanf(fi, "%d %d %d", &start, &end, &weight) != EOF) {
        Edge *edge = malloc(sizeof(Edge));
        edge->start = start - 1;// index from 0
        edge->end = end - 1;    // index from 0
        edge->weight = weight;
        graph_add_edge(graph, edge);
        //printf("Added edge from %d to %d with weight %d\n", start, end, weight);
    }

    fclose(fi);

    int global_start = 0;

    ListPair res = dijkstra_helper(graph, global_start);

    for (int i = 0; i < graph->vertex; i++) {
        int now = i;
        if (now == global_start) {
            continue;
        } else if (list_int_get_at(res.prevs, now) == -1) {
            printf("Unreachable Node, %d\n", now + 1);
            continue;
        }

        ListInt *history = list_new();

        while (now != -1) {
            list_int_append(history, now);
            now = list_int_get_at(res.prevs, now);
        }

        for (int j = 0; j < history->len - 1; j++) {
            printf("%d<-", list_int_get_at(history, j) + 1);
        }
        printf("%d ", list_int_get_at(history, history->len - 1) + 1);
        printf("cost: %d\n", list_int_get_at(res.distances, i));

        list_int_free(history);
    }

    list_int_free(res.distances);
    list_int_free(res.prevs);

    graph_free(graph);

    return 0;
}