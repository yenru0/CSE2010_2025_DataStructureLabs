#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node {
    ElementType element;
    Position next;
};

List MakeEmptyList();
int isLast(Position pos);
void Delete(ElementType x, List list);
Position FindPrevious(ElementType x, List list);
Position Find(ElementType x, List list);
void Insert(ElementType x, Position pos, List list);
void DeleteList(List list);
void PrintList(List list);

// =================
// Impl. LinkedList.
// =================

List MakeEmptyList() {

    List list = (List) malloc(sizeof(struct Node));
    list->element = -1;
    list->next = NULL;
    return list;
}

int isLast(Position p) {
    return p->next == NULL;
}

Position FindPrevious(ElementType x, List list) {
    // returns position s.t. pos->next->element == x or if there is no x in list s.t. pos->next == NULL
    Position now = list;
    while (now->next != NULL) {
        if (now->next->element == x) {
            break;
        }
        now = now->next;
    }

    return now;
}

Position Find(ElementType x, List list) {
    // returns Position s.t. element == x or NULL
    Position now = list;
    while (now != NULL && now->element != x) {
        now = now->next;
    }

    return now;
}

void Insert(ElementType x, Position pos /*nullable*/, List list) {
    if (pos == NULL) {
        printf("Insertion(%d) Failed: cannot find the location to be inserted.\n", x);
        return;
    }

    Position tmp_cell = malloc(sizeof(struct Node));
    if (tmp_cell == NULL) {
        printf("Insertion(%d) Error: malloc result is NULL.\n", x);
        return;
    }
    tmp_cell->element = x;
    tmp_cell->next = pos->next;
    pos->next = tmp_cell;
    return;
}

void Delete(ElementType x, List list) {
    Position p = FindPrevious(x, list);
    if (isLast(p)) {
        printf("Deletion Failed: element %d is not in the list.\n", x);
        return;
    }

    Position d_after = p->next->next;

    free(p->next);
    p->next = d_after;

    return;
}

void DeleteList(List list) {
    Position now = list;
    Position next;
    while (now != NULL) {
        next = now->next;
        free(now);
        now = next;
    }
}

void PrintList(List list) {
    PtrToNode tmp = NULL;
    tmp = list->next;
    if (tmp == NULL) {
        printf("your list is empty!\n");
    }
    while (tmp != NULL) {
        printf("key: %d  ", tmp->element);
        tmp = tmp->next;
    }
    printf("\n");
}

// =============
// MAIN LOGIC
// =============

int main(int argc, char *argv[]) {
    char command;
    int key1, key2;
    FILE *input, *output;

    Position header = NULL, tmp = NULL;

    if (argc <= 1) {
        printf("please enter an input file.");
        return 0;
    } else
        input = fopen(argv[1], "r");

    header = MakeEmptyList();
    while (1) {
        command = fgetc(input);
        if (feof(input)) break;

        switch (command) {
            case 'i':
                fscanf(input, "%d %d", &key1, &key2);
                tmp = Find(key2, header);// nullable
                Insert(key1, tmp, header);
                break;
            case 'd':
                fscanf(input, "%d", &key1);
                Delete(key1, header);
                break;
            case 'f':
                fscanf(input, "%d", &key1);
                tmp = FindPrevious(key1, header);

                if (isLast(tmp)) {
                    printf("Could not find %d in the list.\n", key1);
                } else if (tmp->element > 0) {
                    printf("Key of the previous node of %d is %d.\n", key1, tmp->element);
                } else {
                    printf("Key of the previous node of %d is header.\n", key1);
                }
                break;
            case 'p':
                PrintList(header);
            default:;
        }
    }
    DeleteList(header);
}
