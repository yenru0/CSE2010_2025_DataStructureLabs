#include <stdio.h>
#include <stdlib.h>

// ============
// Lib.
// ============

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int comp_int(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

// ============
// Def. AVL
// ============

typedef struct AVLNode *AVLTree;

typedef struct AVLNode {
    void *data;
    struct AVLNode *left, *right;
    int height;
} AVLNode;

typedef struct AVLInsertionResult {
    AVLTree tree;
    int signal;
} AVLInsertionResult;

int avltree_is_empty(AVLTree tree);

AVLNode *avltree_node_create();
AVLNode *avltree_node_create_with_ptr(void *x);
int avltree_node_get_height(AVLNode *node);

AVLNode *avltree_single_rotate_left(AVLNode *node);
AVLNode *avltree_single_rotate_right(AVLNode *node);// LL
AVLNode *avltree_double_rotate_right_left(AVLNode *node);
AVLNode *avltree_double_rotate_left_right(AVLNode *node);

AVLInsertionResult avltree_insert(AVLTree tree, void *x, int (*cmp)(const void *, const void *));

void avltree_print_inorder(AVLTree tree);
void avltree_delete(AVLTree tree);

// ==========
// MAIN LOGIC
// ==========

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Illegal Arguments\n");
        exit(1);
    }

    AVLTree tree = NULL;

    int key;
    int *x;
    FILE *fi = fopen(argv[1], "r");

    if (fi == NULL) {
        printf("Input file does not exists\n");
        exit(1);
    }

    while (fscanf(fi, "%d", &key) != EOF) {
        x = malloc(sizeof(int));// 추후에 회수됨
        *x = key;
        AVLInsertionResult res = avltree_insert(tree, x, comp_int);
        if (!res.signal) {
            printf("Insertion Error : %d is already in the tree!\n", *(int *) x);
            free(x);
        } else {
            tree = res.tree;
        }
    }
    fclose(fi);

    avltree_print_inorder(tree);
    printf("\n");
    avltree_delete(tree);
}

// ==========
// Impl. AVL
// ==========

int avltree_is_empty(AVLTree tree) {
    return tree == NULL;
}

AVLNode *avltree_node_create() {
    AVLNode *node = malloc(sizeof(AVLNode));
    if (node == NULL) {
        printf("Out of space!\n");
        exit(1);
    }
    node->data = NULL;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AVLNode *avltree_node_create_with_ptr(void *x) {
    AVLNode *node = avltree_node_create();
    node->data = x;
    return node;
}

int avltree_node_get_height(AVLNode *node) {
    if (node == NULL) {
        return -1;
    }
    return node->height;
}

AVLNode *avltree_single_rotate_left(AVLNode *node) {
    AVLNode *to_be_center = node->right;
    AVLNode *to_be_left_sub2 = to_be_center->left;

    to_be_center->left = node;
    node->right = to_be_left_sub2;

    node->height = 1 + max(
                               avltree_node_get_height(node->left),
                               avltree_node_get_height(node->right));
    to_be_center->height =
            1 + max(
                        avltree_node_get_height(to_be_center->left),
                        avltree_node_get_height(to_be_center->right));
    return to_be_center;
}

AVLNode *avltree_single_rotate_right(AVLNode *node) {
    // node is to_be_right
    AVLNode *to_be_center = node->left;
    AVLNode *to_be_right_sub1 = to_be_center->right;

    to_be_center->right = node;
    node->left = to_be_right_sub1;

    node->height = 1 + max(
                               avltree_node_get_height(node->left),
                               avltree_node_get_height(node->right));
    to_be_center->height = 1 + max(
                                       avltree_node_get_height(to_be_center->left),
                                       avltree_node_get_height(to_be_center->right));
    return to_be_center;
}

AVLNode *avltree_double_rotate_right_left(AVLNode *node) {
    // node is to_be_left
    node->right = avltree_single_rotate_right(node->right);
    return avltree_single_rotate_left(node);
}

AVLNode *avltree_double_rotate_left_right(AVLNode *node) {
    // node is to_be_right
    node->left = avltree_single_rotate_left(node->left);
    return avltree_single_rotate_right(node);
}

AVLInsertionResult avltree_insert(AVLTree tree, void *x, int (*cmp)(const void *, const void *)) {
    if (avltree_is_empty(tree)) {
        AVLInsertionResult ret;
        ret.tree = avltree_node_create_with_ptr(x);
        ret.signal = 1;
        return ret;
    }

    if (cmp(tree->data, x) > 0) {
        AVLInsertionResult res = avltree_insert(tree->left, x, cmp);
        if (!res.signal) {
            return res;
        }
        tree->left = res.tree;
        if (avltree_node_get_height(tree->left) - avltree_node_get_height(tree->right) == 2) {
            if (cmp(tree->left->data, x) > 0) {
                printf("There's node(%d) to be balanced! Do SingleRotateWithLeft!\n", *(int *) tree->data);
                tree = avltree_single_rotate_right(tree);
            } else {
                printf("There's node(%d) to be balanced! Do DoubleRotateWithLeft!\n", *(int *) tree->data);
                tree = avltree_double_rotate_left_right(tree);
            }
        }
    } else if (cmp(tree->data, x) < 0) {
        AVLInsertionResult res = avltree_insert(tree->right, x, cmp);
        if (!res.signal) {
            return res;
        }
        tree->right = res.tree;
        if (avltree_node_get_height(tree->right) - avltree_node_get_height(tree->left) == 2) {
            if (cmp(tree->right->data, x) < 0) {
                printf("There's node(%d) to be balanced! Do SingleRotateWithRight!\n", *(int *) tree->data);
                tree = avltree_single_rotate_left(tree);
            } else {
                printf("There's node(%d) to be balanced! Do DoubleRotateWithRight!\n", *(int *) tree->data);
                tree = avltree_double_rotate_right_left(tree);
            }
        }
    } else {
        AVLInsertionResult ret;
        ret.tree = NULL;
        ret.signal = 0;

        return ret;
    }

    tree->height = 1 + max(avltree_node_get_height(tree->left),
                           avltree_node_get_height(tree->right));

    AVLInsertionResult ret;
    ret.tree = tree;
    ret.signal = 1;
    return ret;
}

void avltree_print_inorder(AVLTree tree) {
    if (tree != NULL) {
        avltree_print_inorder(tree->left);
        printf("%d(%d) ", *(int *) tree->data, tree->height);
        avltree_print_inorder(tree->right);
    }
}

void avltree_delete(AVLTree tree) {
    if (tree != NULL) {
        avltree_delete(tree->left);
        avltree_delete(tree->right);
        if (tree->data != NULL) {
            free(tree->data);
        }

        free(tree);
    }
}
