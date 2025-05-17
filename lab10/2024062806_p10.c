#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//
// Def. & Impl
//

typedef struct Disjoints {
    size_t size;
    size_t *parents;
} Disjoints;

Disjoints *disjoints_new(size_t size) {
    Disjoints *sets = malloc(sizeof(Disjoints));
    sets->size = size;
    sets->parents = calloc(size, sizeof(size_t));
    return sets;
}

void disjoints_init(Disjoints *disj) {
    for (size_t i = 0; i < disj->size; i++) {
        disj->parents[i] = i;
    }
}

int disjoints_find(Disjoints *disj, size_t x) {
    size_t root = x;
    while (disj->parents[root] != root) {
        root = disj->parents[root];
    }
    return root;
}

int disjoints_union(Disjoints *disj, size_t i, size_t j) {
    size_t root_i = disjoints_find(disj, i);
    size_t root_j = disjoints_find(disj, j);

    if (root_i == root_j) {
        return 0;
    }

    if (disj->parents[root_i] < disj->parents[root_j]) {
        disj->parents[root_j] = root_i;
    } else {
        disj->parents[root_i] = root_j;
    }
    return 1;
}

typedef struct Maze {
    size_t side;
    size_t maze_wall_data_size;
    int *maze_wall_data;
    Disjoints *disjoints;
} Maze;

Maze *maze_new(size_t side) {
    Maze *maze = malloc(sizeof(Maze));
    maze->side = side;
    maze->maze_wall_data_size = side * (side - 1) * 2;
    maze->maze_wall_data = calloc(maze->maze_wall_data_size, sizeof(int));
    maze->disjoints = disjoints_new(side * side);
    return maze;
}

void maze_init(Maze *maze) {
    for (size_t i = 0; i < maze->maze_wall_data_size; i++) {
        maze->maze_wall_data[i] = 1;
    }
    disjoints_init(maze->disjoints);
}

// left, up only
size_t maze_get_cell_by_wall_num(Maze *maze, size_t wall) {
    size_t wall_div = wall / (maze->side * (maze->side - 1));
    size_t wall_mod = wall % (maze->side * (maze->side - 1));

    if (wall_div == 0) {
        size_t mod = wall_mod % (maze->side - 1);
        size_t div = wall_mod / (maze->side - 1);
        return div * maze->side + mod;
    } else {
        size_t mod = wall_mod % maze->side;
        size_t div = wall_mod / maze->side;
        return div * maze->side + mod;
    }
}

size_t maze_get_cell_next(Maze *maze, size_t cell, int is_vertical) {
    // invalid is not allowed if not have next
    if (!is_vertical) {
        return cell + 1;
    } else {
        return cell + maze->side;
    }
}

int maze_check(Maze *maze) {
    return disjoints_find(maze->disjoints, 0) == disjoints_find(maze->disjoints, maze->side * maze->side - 1);
}

void maze_create(Maze *maze) {
    srand(100);
    while (maze_check(maze) == 0) {
        size_t wall = rand() % maze->maze_wall_data_size;
        if (maze->maze_wall_data[wall] == 0) {
            continue;
        }

        size_t cell_cur = maze_get_cell_by_wall_num(maze, wall);
        int is_vertical = wall / (maze->side * (maze->side - 1));
        size_t cell_nxt = maze_get_cell_next(maze, cell_cur, is_vertical);

        if (disjoints_union(maze->disjoints, cell_cur, cell_nxt)) {
            //printf("Union %zu %zu at %zu\n", cell_cur, cell_nxt, wall);
            maze->maze_wall_data[wall] = 0;
        }
    }
}

void maze_print(Maze *maze) {
    for (size_t i = 0; i < maze->side; i++) {
        printf("*-");
    }
    printf("*\n");

    for (size_t row = 0; row < maze->side; row++) {
        if (row != 0) {
            printf("| ");
        } else {
            printf("  ");
        }
        for (size_t col = 0; col < maze->side - 1; col++) {
            size_t wall = col + row * (maze->side - 1);
            if (maze->maze_wall_data[wall] == 1) {
                printf("| ");
            } else {
                printf("  ");
            }
        }

        if (row == maze->side - 1) {
            printf(" \n");
        } else {
            printf("|\n");
        }
        if (row == maze->side - 1) {
            break;
        }
        for (size_t col = 0; col < maze->side; col++) {
            size_t wall_v = (maze->side - 1) * maze->side + row * maze->side + col;
            if (maze->maze_wall_data[wall_v] == 1) {
                printf("*-");
            } else {
                printf("* ");
            }
        }
        printf("*\n");
    }
    for (size_t i = 0; i < maze->side; i++) {
        printf("*-");
    }
    printf("*\n");
}

void maze_free(Maze *maze) {
    free(maze->maze_wall_data);
    free(maze->disjoints->parents);
    free(maze->disjoints);
    free(maze);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid arguments, usage: %s <size>\n", argv[0]);
        return 1;
    }

    int num;
    FILE *fi = fopen(argv[1], "r");
    if (fi == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }
    fscanf(fi, "%d", &num);
    fclose(fi);
    fi = NULL;

    Maze *maze = maze_new(num);
    maze_init(maze);
    //maze_print(maze); //before create
    maze_create(maze);
    maze_print(maze);

    maze_free(maze);

    return 0;
}