#include "sort.h"

void selection_sort(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) return;
    
    char* ptr = (char*)base;
    char* temp = malloc(size);
    if (!temp) {
        HANDLE_ERROR("Memory allocation");
    }

    for (size_t i = 0; i < num - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < num; j++) {
            if (compare(ptr + j * size, ptr + min_idx * size) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            memcpy(temp, ptr + i * size, size);
            memcpy(ptr + i * size, ptr + min_idx * size, size);
            memcpy(ptr + min_idx * size, temp, size);
        }
    }

    free(temp);
}
