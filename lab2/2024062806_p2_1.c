#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 32

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please input the number of students!\n", argc);
        return 1;
    }
    int count = atoi(argv[1]);

    if (count < 2) {
        printf("Please input the number of students more than one!\n");
        return 1;
    }

    printf("Please enter %d names:\n", count);

    char **names = malloc(count * sizeof(char *));

    for (int i = 0; i < count; i++) {
        names[i] = malloc(MAX_LENGTH * sizeof(char));
        scanf("%s", names[i]);
    }
    printf("The names you entered are\n");
    for (int i = 0; i < count; i++) {
        printf("%s\n", names[i]);
    }

    for (int i = 0; i < count; i++) {
        free(names[i]);
    }
    free(names);

    return 0;
}