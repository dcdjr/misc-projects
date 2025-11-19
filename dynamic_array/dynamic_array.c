#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} DynamicArray;

void init(DynamicArray *vec) {
    vec->size = 0;
    vec->capacity = 4;
    vec->data = malloc(vec->capacity * sizeof(int));
}

void resize(DynamicArray *vec) {
    vec->capacity *= 2;
    int *newData = malloc(vec->capacity * sizeof(int));
    for (int i = 0; i < vec->size; i++) {
        // same as *(newData + i) = *((vec->data) + i);
        newData[i] = vec->data[i];
    }
    free(vec->data);
    vec->data = newData;
}

void push(DynamicArray *vec, int value) {
    if (vec->size == vec->capacity) {
        resize(vec);
    }
    vec->data[vec->size] = value;
    vec->size++;
}

int pop(DynamicArray *vec) {
    if (vec->size == 0) {
        return -1;
    }
    int popped_value = vec->data[vec->size - 1];
    vec->size--;
    return popped_value;
}

int get(DynamicArray *vec, int index) {
    if (index < 0 || index >= vec->size) return -1;
    return vec->data[index];
}

void set(DynamicArray *vec, int index, int value) {
    if (index < 0 || index >= vec->size) return;
    vec->data[index] = value;
}

int main(void) {
    return 0;
}