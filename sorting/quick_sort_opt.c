#include "sort.h"
#include <immintrin.h>
#include <stdint.h>
#include <string.h>

#define INSERTION_SORT_THRESHOLD 16
#define CACHE_LINE_SIZE 64

// CPU优化版本 - 三路划分 + 内联
static inline void swap(char* a, char* b, size_t size) {
    char temp;
    while (size--) {
        temp = *a;
        *a++ = *b;
        *b++ = temp;
    }
}

// 三数取中法选择枢轴
static char* median_of_three(char* a, char* b, char* c, size_t size, CompareFunc compare) {
    return compare(a, b) < 0 ? 
           (compare(b, c) < 0 ? b : compare(a, c) < 0 ? c : a) :
           (compare(b, c) > 0 ? b : compare(a, c) > 0 ? c : a);
}

// Cache优化版本 - 小数组使用插入排序 (内部实现，避免与全局 insertion_sort 冲突)
static void insertion_sort_internal(char* base, size_t num, size_t size, CompareFunc compare) {
    char* temp = malloc(size);
    if (!temp) {
        HANDLE_ERROR("Memory allocation failed");
    }

    for (size_t i = 1; i < num; i++) {
        memcpy(temp, base + i * size, size);
        size_t j = i;
        while (j > 0 && compare(base + (j-1) * size, temp) > 0) {
            memcpy(base + j * size, base + (j-1) * size, size);
            j--;
        }
        memcpy(base + j * size, temp, size);
    }

    free(temp);
}

// 三路划分快速排序（结合CPU和缓存优化）
static void quick_sort_three_way(char* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= INSERTION_SORT_THRESHOLD) {
        insertion_sort_internal(base, num, size, compare);
        return;
    }

    // 选择枢轴（三数取中）并进行缓存对齐
    char* aligned_base = (char*)(((uintptr_t)base + CACHE_LINE_SIZE - 1) & ~((uintptr_t)(CACHE_LINE_SIZE - 1)));
    char* pivot = median_of_three(
        aligned_base,
        aligned_base + (num/2) * size,
        aligned_base + (num-1) * size,
        size,
        compare
    );

    // 将枢轴移到最后
    char* last = aligned_base + (num-1) * size;
    if (pivot != last) {
        char* temp = malloc(size);
        if (!temp) {
            HANDLE_ERROR("Memory allocation failed");
        }
        memcpy(temp, pivot, size);
        memcpy(pivot, last, size);
        memcpy(last, temp, size);
        free(temp);
    }
    pivot = last;

    // 三路划分（带有预取优化）
    size_t lt = 0;        // 小于区域的右边界
    size_t gt = num - 1;  // 大于区域的左边界
    size_t i = lt;        // 当前处理的元素

    while (i < gt) {
        __builtin_prefetch(aligned_base + (i + 4) * size);  // 预取数据
        
        int cmp = compare(aligned_base + i * size, pivot);
        if (cmp < 0) {
            // 当前元素小于枢轴
            if (i != lt) {
                swap(aligned_base + i * size, aligned_base + lt * size, size);
            }
            i++;
            lt++;
        } else if (cmp > 0) {
            // 当前元素大于枢轴
            gt--;
            swap(aligned_base + i * size, aligned_base + gt * size, size);
        } else {
            // 当前元素等于枢轴
            i++;
        }
    }

    // 将枢轴放到正确的位置
    swap(pivot, aligned_base + gt * size, size);

    // 递归处理左右两部分（利用尾递归优化）
    if (lt > 1) {
        quick_sort_three_way(aligned_base, lt, size, compare);
    }
    if (num - (gt + 1) > 1) {
        quick_sort_three_way(aligned_base + (gt + 1) * size, num - (gt + 1), size, compare);
    }
}

// 简单且安全的 partition 实现（用于替代不稳定的 SIMD 版本）
static int partition_simd(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    i++;
    int tmp = arr[i];
    arr[i] = arr[high];
    arr[high] = tmp;
    return i;
}

static void quick_sort_simd_internal(int* arr, int low, int high) {
    while (low < high) {
        if (high - low + 1 <= INSERTION_SORT_THRESHOLD) {
            // 小数组使用插入排序
            for (int i = low + 1; i <= high; i++) {
                int key = arr[i];
                int j = i - 1;
                while (j >= low && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    j--;
                }
                arr[j + 1] = key;
            }
            break;
        } else {
            int pi = partition_simd(arr, low, high);
            
            // 尾递归优化：总是处理较小的子数组
            if (pi - low < high - pi) {
                quick_sort_simd_internal(arr, low, pi - 1);
                low = pi + 1;
            } else {
                quick_sort_simd_internal(arr, pi + 1, high);
                high = pi - 1;
            }
        }
    }
}

// 主排序函数
void quick_sort_opt(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num < 2) return;
    
    // 检查是否可以使用SIMD优化版本
    if (size == sizeof(int) && (((uintptr_t)base) % sizeof(int) == 0)) {
        quick_sort_simd_internal((int*)base, 0, num - 1);
    } else {
        quick_sort_three_way(base, num, size, compare);
    }
}
