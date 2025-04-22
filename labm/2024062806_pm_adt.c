#include <stdio.h>
#include <stdlib.h>

#define INT_MAX 100
#define INT_MIN -100

struct Integer {
    int intValue;
};

typedef struct Integer *Integer;

Integer newIntegerZero() {
    Integer i = malloc(sizeof(struct Integer));
    i->intValue = 0;
    return i;
}

Integer newIntegerByValue(int v) {
    Integer x = malloc(sizeof(struct Integer));
    if (v > INT_MAX) {
        x->intValue = INT_MAX;
    } else if (v < INT_MIN) {
        x->intValue = INT_MIN;
    } else {
        x->intValue = v;
    }

    return x;
}

Integer newInteger(Integer i) {
    Integer x = malloc(sizeof(struct Integer));
    x->intValue = i->intValue;
    return x;
}

void deleteInteger(Integer i) {
    free(i);
}

int isZero(Integer i) {
    if (i->intValue == 0) {
        return 1;
    } else {
        return 0;
    }
}

Integer addBy(Integer x, Integer y) {
    if (x->intValue + y->intValue > INT_MAX) {
        x->intValue = INT_MAX;
    } else if (x->intValue + y->intValue < INT_MIN) {
        x->intValue = INT_MIN;
    } else {
        x->intValue += y->intValue;
    }
    return x;
}

Integer add(Integer a, Integer b) {
    Integer x = newInteger(a);
    addBy(x, b);

    return x;
}

Integer subBy(Integer x, Integer y) {
    int cal = x->intValue - y->intValue;
    if (cal > INT_MAX) {
        x->intValue = INT_MAX;
    } else if (cal < INT_MIN) {
        x->intValue = INT_MIN;
    } else {
        x->intValue = cal;
    }
    return x;
}

Integer sub(Integer a, Integer b) {
    Integer x = newInteger(a);
    subBy(x, b);

    return x;
}

Integer minus(Integer i) {
    return subBy(newIntegerZero(), i);
}

int isEqual(Integer x, Integer y) {
    return x->intValue == y->intValue;
}

int getIntValue(Integer i) {
    return i->intValue;
}

int compareTo(Integer x, Integer y) {
    Integer s = sub(x, y);
    int cmp;
    if (s->intValue > 0) {
        cmp = 1;
    } else if (s->intValue < 0) {
        cmp = -1;
    } else {
        cmp = 0;
    }

    deleteInteger(s);
    return cmp;
}

void sortInts(Integer list[], int n) {
    int min_index;
    Integer temp;
    for (int i = 0; i < n - 1; i++) {
        min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (compareTo(list[min_index], list[j]) > 0) {
                min_index = j;
            }
        }
        temp = list[i];
        list[i] = list[min_index];
        list[min_index] = temp;
    }
}

int main() {
    Integer i0 = newIntegerByValue(3);
    Integer i1 = newIntegerByValue(4);
    Integer i2 = minus(i0);

    Integer i01 = add(i0, i1);
    Integer i02 = sub(i0, i2);

    Integer is[5] = {i0, i1, i2, i01, i02};

    printf("i0 + i1: %d, i0 - i2: %d\n", getIntValue(i01), getIntValue(i02));
    printf("before sort: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", getIntValue(is[i]));
    }
    printf("\n");

    sortInts(is, 5);

    printf("after sort: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", getIntValue(is[i]));
    }
    printf("\n");

    deleteInteger(i0);
    deleteInteger(i1);
    deleteInteger(i2);
    deleteInteger(i01);
    deleteInteger(i02);
    return 0;
}