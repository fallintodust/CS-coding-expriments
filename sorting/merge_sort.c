#include "sort.h"

static void merge(char* base, size_t size, int left, int mid, int right, CompareFunc compare) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    char* L = malloc(n1 * size);
    char* R = malloc(n2 * size);
    if (!L || !R) {
        HANDLE_ERROR("Memory allocation");
    }

    // 复制数据到临时数组
    memcpy(L, base + left * size, n1 * size);
    memcpy(R, base + (mid + 1) * size, n2 * size);

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (compare(L + i * size, R + j * size) <= 0) {
            memcpy(base + k * size, L + i * size, size);
            i++;
        } else {
            memcpy(base + k * size, R + j * size, size);
            j++;
        }
        k++;
    }

    // 复制剩余的元素
    while (i < n1) {
        memcpy(base + k * size, L + i * size, size);
        i++;
        k++;
    }
    while (j < n2) {
        memcpy(base + k * size, R + j * size, size);
        j++;
        k++;
    }

    free(L);
    free(R);
}

static void _merge_sort(char* base, size_t size, int left, int right, CompareFunc compare) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        _merge_sort(base, size, left, mid, compare);
        _merge_sort(base, size, mid + 1, right, compare);
        merge(base, size, left, mid, right, compare);
    }
}

void merge_sort(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) return;
    _merge_sort((char*)base, size, 0, num - 1, compare);
}
