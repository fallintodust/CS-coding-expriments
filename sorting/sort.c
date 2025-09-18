#include "sort.h"

// 通用交换函数
static void swap_generic(char* a, char* b, size_t size) {
    char temp;
    while (size--) {
        temp = *a;
        *a++ = *b;
        *b++ = temp;
    }
}

// 仅保留排序相关实现

// 通用冒泡排序实现
void bubble_sort(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) return;
    
    char* ptr = (char*)base;
    for (size_t i = 0; i < num - 1; i++) {
        for (size_t j = 0; j < num - i - 1; j++) {
            if (compare(ptr + j * size, ptr + (j + 1) * size) > 0) {
                swap_generic(ptr + j * size, ptr + (j + 1) * size, size);
            }
        }
    }
}

// 快速排序分区函数
static size_t partition(char* base, size_t num, size_t size, CompareFunc compare) {
    char* pivot = base + (num - 1) * size;
    size_t i = 0;
    
    for (size_t j = 0; j < num - 1; j++) {
        if (compare(base + j * size, pivot) <= 0) {
            swap_generic(base + i * size, base + j * size, size);
            i++;
        }
    }
    swap_generic(base + i * size, pivot, size);
    return i;
}

// 通用快速排序实现
static void quick_sort_internal(char* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) return;
    
    size_t pivot_idx = partition(base, num, size, compare);
    quick_sort_internal(base, pivot_idx, size, compare);
    quick_sort_internal(base + (pivot_idx + 1) * size, num - pivot_idx - 1, size, compare);
}

void quick_sort(void* base, size_t num, size_t size, CompareFunc compare) {
    quick_sort_internal((char*)base, num, size, compare);
}
