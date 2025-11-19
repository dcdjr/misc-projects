#include <stdio.h>
#include <stdlib.h>

int my_strlen(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

char *my_strcpy(char *dst, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return dst;
}

char *my_strdup(const char *str) {
    int buff_length = my_strlen(str) + 1;
    char *dup = malloc(buff_length);
    if (dup == NULL) {
        return NULL;
    }
    for (int i = 0; i < buff_length; i++) {
        dup[i] = str[i];
    }
    return dup;
}

int main(void) {
    return 0;
}