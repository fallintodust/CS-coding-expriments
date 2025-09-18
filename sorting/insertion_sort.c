#include <stdlib.h>
#include <string.h>
#include "sort.h"

void insertion_sort(void* base, size_t n, size_t size, int (*compare)(const void*, const void*)) {
    if (n <= 1) return;
    char* arr = (char*)base;
    char* key = malloc(size);

    for (size_t i = 1; i < n; i++) {
        memcpy(key, arr + i * size, size);
        size_t j = i;
        while (j > 0 && compare(arr + (j-1) * size, key) > 0) {
            memcpy(arr + j * size, arr + (j-1) * size, size);
            j--;
        }
        memcpy(arr + j * size, key, size);
    }

    free(key);
}
