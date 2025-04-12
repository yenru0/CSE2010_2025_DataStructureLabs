#include <stdio.h>
#include <stdlib.h>

// ==========
// Def. BST
// ==========

typedef struct BST {
    int size;
    int numOfNode;
    int *element;

} BST;

// ==========
// Impl. BST
// ==========

BST *BST_new(int initial_size) {
    BST *bst = malloc(sizeof(BST));
    if (bst == NULL) {
        return NULL;
    }
    int *elems = calloc(initial_size, sizeof(int));
    if (elems == NULL) {
        free(bst);
        return NULL;
    }
    bst->element = elems;
    bst->numOfNode = 0;
    bst->size = initial_size;
    return bst;
}

int BST_insert(BST *bst, int value) {
    if (bst->numOfNode >= bst->size) {
        return 0;// full
    } else {
        bst->element[bst->numOfNode++] = value;
        return 1;
    }
}

void BST_printPreorder(BST *bst, int index) {
    if (index >= bst->numOfNode) {
        return;
    }
    printf("%d ", bst->element[index]);
    BST_printPreorder(bst, (index + 1) * 2 - 1);
    BST_printPreorder(bst, (index + 1) * 2);
}

void BST_printInorder(BST *bst, int index) {
    if (index >= bst->numOfNode) {
        return;
    }
    BST_printInorder(bst, (index + 1) * 2 - 1);
    printf("%d ", bst->element[index]);
    BST_printInorder(bst, (index + 1) * 2);
}

void BST_printPostorder(BST *bst, int index) {
    if (index >= bst->numOfNode) {
        return;
    }
    BST_printPostorder(bst, (index + 1) * 2 - 1);
    BST_printPostorder(bst, (index + 1) * 2);
    printf("%d ", bst->element[index]);
}

void BST_print(BST *bst) {
    printf("Preorder: ");
    BST_printPreorder(bst, 0);
    printf("\n");
    printf("Inorder: ");
    BST_printInorder(bst, 0);
    printf("\n");
    printf("Postorder: ");
    BST_printPostorder(bst, 0);
    printf("\n");
}

void BST_delete(BST *bst) {
    free(bst->element);
    free(bst);
}

// ==========
// MAIN LOGIC
// ==========

int main(int argc, char *argv[]) {
    FILE *fi;
    BST *tree;
    int tsize;
    int tmp;

    if (argc == 1 || argc >= 3) {
        printf("illegal argument size\n");
        return 0;
    }

    fi = fopen(argv[1], "r");
    if (fi == NULL) {
        printf("no file open\n");
        return 0;
    }
    fscanf(fi, "%d", &tsize);

    tree = BST_new(tsize);

    while (fscanf(fi, "%d", &tmp) == 1) {
        if (BST_insert(tree, tmp) == 0) {
            printf("Error with node %d! Tree is already full so we cannot insert any node in the tree!\n", tmp);
        }
    }

    BST_print(tree);
    BST_delete(tree);

    fclose(fi);
}