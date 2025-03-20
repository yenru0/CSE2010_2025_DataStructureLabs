#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 32

typedef struct Student {
    char *name;
    char *major;
    int student_id;

} Student;

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Please enter the input file or output file!\n");
        return 1;
    }
    char *input_path, *output_path;
    FILE *fi, *fo;
    int num;

    Student *students;

    input_path = argv[1];
    output_path = argv[2];

    fi = fopen(input_path, "r");
    fo = fopen(output_path, "w");

    fscanf(fi, "%d", &num);

    students = calloc(num, sizeof(Student));
    for (int i = 0; i < num; i++) {
        students[i].name = calloc(MAX_LENGTH, sizeof(char));
        students[i].major = calloc(MAX_LENGTH, sizeof(char));
    }

    for (int i = 0; i < num; i++) {
        fscanf(fi, "%s %s %d", students[i].name, students[i].major, &students[i].student_id);
    }
    for (int i = 0; i < num; i++) {
        fprintf(fo, "%s\t%s\t%d\n", students[i].name, students[i].major, students[i].student_id);
    }

    for (int i = 0; i < num; i++) {
        free(students[i].name);
        free(students[i].major);
    }

    free(students);
    return 0;
}