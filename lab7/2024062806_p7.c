#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==============================
// Tree Def.
// ==============================

typedef struct TreeNode *Tree;
typedef struct TreeNode *TreeNode;
struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
};

// =============================
// Tree Signatures
// =============================

void Tree_delete(Tree tree);

Tree Tree_insert(Tree tree, int data);
Tree Tree_pop(Tree tree, int data);

TreeNode Tree_find(Tree tree, int value);
TreeNode Tree_findMin(Tree tree);
TreeNode Tree_findMax(Tree tree);
void Tree_print(Tree tree);
void Tree_printInOrder(Tree tree);

// ==============================
// MAIN LOGIC
// ==============================

int main(int argc, char *argv[]) {
    FILE *file_input;

    Tree tree = NULL;

    char command;
    int value;

    TreeNode temp;

    int endflag = 0;

    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    file_input = fopen(argv[1], "r");
    if (file_input == NULL) {
        printf("File %s open failed\n", argv[1]);
        return 1;
    }

    while (!endflag) {
        command = fgetc(file_input);
        if (feof(file_input)) {
            break;
        }

        switch (command) {
            case 'i':
                fscanf(file_input, "%d", &value);

                tree = Tree_insert(tree, value);

                break;
            case 'd':
                fscanf(file_input, "%d", &value);
                tree = Tree_pop(tree, value);
                break;
            case 'p':
                printf("Inorder : ");
                Tree_printInOrder(tree);
                printf("\n");
                break;
            case 'f':
                fscanf(file_input, "%d", &value);
                temp = Tree_find(tree, value);
                if (temp == NULL) {
                    printf("Find failed : Element %d not found.\n", value);
                } else {
                    printf("element %d exist.\n", temp->data);
                }
                break;
            case 'm':
                Tree_print(tree);
                break;
            case 'q':
                endflag = 1;
                break;
            case '\n':
            case ' ':
            case '\t':
            case '\r':
                break;
            default:

                printf("Unknown command: %c\n", command);
        }
    }
    fclose(file_input);

    Tree_delete(tree);
    tree = NULL;
    return 0;
}

// ==============================
// Tree Inmp.
// ==============================

void Tree_delete(Tree tree) {
    if (tree == NULL) {
        return;
    }
    Tree_delete(tree->left);
    Tree_delete(tree->right);
    free(tree);
}

Tree Tree_create(int data) {
    Tree tree = malloc(sizeof(struct TreeNode));
    if (tree == NULL) {
        return tree;
    }
    tree->data = data;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}

Tree Tree_insert(Tree tree, int data) {
    if (tree == NULL) {
        tree = Tree_create(data);
    } else if (data < tree->data) {
        tree->left = Tree_insert(tree->left, data);
    } else if (data > tree->data) {
        tree->right = Tree_insert(tree->right, data);
    } else {
        printf("Insert failed : Element %d is existed.\n", data);
    }
    return tree;
}

Tree Tree_pop(Tree tree, int data) {
    if (tree == NULL) {
        printf("Delete failed : Element %d not found.\n", data);
    }
    if (data < tree->data) {
        tree->left = Tree_pop(tree->left, data);
    } else if (data > tree->data) {
        tree->right = Tree_pop(tree->right, data);
    } else {
        if (tree->left == NULL) {
            TreeNode temp = tree->right;
            free(tree);
            return temp;
        } else if (tree->right == NULL) {
            TreeNode temp = tree->left;
            free(tree);
            return temp;
        }
        TreeNode temp = Tree_findMin(tree->right);
        tree->data = temp->data;
        tree->right = Tree_pop(tree->right, temp->data);
    }
    return tree;
}

TreeNode Tree_find(Tree tree, int value) {
    if (tree == NULL || tree->data == value) {
        return tree;
    }
    if (value < tree->data) {
        return Tree_find(tree->left, value);
    }
    return Tree_find(tree->right, value);
}

TreeNode Tree_findMin(Tree tree) {
    TreeNode current = tree;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

TreeNode Tree_findMax(Tree tree) {
    TreeNode current = tree;
    while (current && current->right != NULL) {
        current = current->right;
    }
    return current;
}

void Tree_print(Tree tree) {
    TreeNode min = Tree_findMin(tree);
    TreeNode max = Tree_findMax(tree);
    if (min == NULL) {
        printf("Min : NULL\nMax : NULL\n");
    } else {
        printf("Min : %d\nMax : %d\n", min->data, max->data);
    }
}

void Tree_printInOrder(Tree tree) {
    if (tree == NULL) {
        return;
    }
    Tree_printInOrder(tree->left);
    printf("%d ", tree->data);
    Tree_printInOrder(tree->right);
}
