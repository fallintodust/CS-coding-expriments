#include "sort.h"

void bubble_sort(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) return;
    
    char* ptr = (char*)base;
    char* temp = malloc(size);
    if (!temp) {
        HANDLE_ERROR("Memory allocation");
    }

    for (size_t i = 0; i < num - 1; i++) {
        for (size_t j = 0; j < num - i - 1; j++) {
            if (compare(ptr + j * size, ptr + (j + 1) * size) > 0) {
                memcpy(temp, ptr + j * size, size);
                memcpy(ptr + j * size, ptr + (j + 1) * size, size);
                memcpy(ptr + (j + 1) * size, temp, size);
            }
        }
    }

    free(temp);
}
