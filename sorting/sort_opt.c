#include "sort.h"
#include <immintrin.h>

// CPU优化 - 冒泡排序
// 1. 分支预测优化
// 2. 提前退出优化
// 3. 循环展开
void bubble_sort_opt(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num < 2) return;
    
    char* begin = (char*)base;
    char* temp = malloc(size);
    if (!temp) {
        HANDLE_ERROR("Memory allocation failed");
    }

    size_t last_swap = num - 1;  // 记录最后一次交换的位置
    
    for (size_t i = 0; i < num - 1; i++) {
        size_t new_last_swap = 0;
        int swapped = 0;
        
        // 循环展开4次
        for (size_t j = 0; j + 3 < last_swap; j += 4) {
            // 预取下一组数据
            __builtin_prefetch(begin + (j + 8) * size);
            
            // 比较和交换第1对
            if (compare(begin + j * size, begin + (j + 1) * size) > 0) {
                memcpy(temp, begin + j * size, size);
                memcpy(begin + j * size, begin + (j + 1) * size, size);
                memcpy(begin + (j + 1) * size, temp, size);
                new_last_swap = j + 1;
                swapped = 1;
            }
            
            // 比较和交换第2对
            if (compare(begin + (j + 1) * size, begin + (j + 2) * size) > 0) {
                memcpy(temp, begin + (j + 1) * size, size);
                memcpy(begin + (j + 1) * size, begin + (j + 2) * size, size);
                memcpy(begin + (j + 2) * size, temp, size);
                new_last_swap = j + 2;
                swapped = 1;
            }
            
            // 比较和交换第3对
            if (compare(begin + (j + 2) * size, begin + (j + 3) * size) > 0) {
                memcpy(temp, begin + (j + 2) * size, size);
                memcpy(begin + (j + 2) * size, begin + (j + 3) * size, size);
                memcpy(begin + (j + 3) * size, temp, size);
                new_last_swap = j + 3;
                swapped = 1;
            }
            
            // 比较和交换第4对
            if (compare(begin + (j + 3) * size, begin + (j + 4) * size) > 0) {
                memcpy(temp, begin + (j + 3) * size, size);
                memcpy(begin + (j + 3) * size, begin + (j + 4) * size, size);
                memcpy(begin + (j + 4) * size, temp, size);
                new_last_swap = j + 4;
                swapped = 1;
            }
        }
        
        // 处理剩余元素
        for (size_t j = (last_swap - 3) & ~3UL; j < last_swap; j++) {
            if (compare(begin + j * size, begin + (j + 1) * size) > 0) {
                memcpy(temp, begin + j * size, size);
                memcpy(begin + j * size, begin + (j + 1) * size, size);
                memcpy(begin + (j + 1) * size, temp, size);
                new_last_swap = j + 1;
                swapped = 1;
            }
        }
        
        if (!swapped) break;  // 如果没有发生交换，数组已经有序
        last_swap = new_last_swap;  // 更新最后交换位置
    }

    free(temp);
}

// 快速排序优化
static inline void quick_sort_3way_partition(char* base, size_t num, size_t size,
                                           CompareFunc compare, char* temp) {
    if (num < 16) {  // 小数组使用插入排序
        for (size_t i = 1; i < num; i++) {
            memcpy(temp, base + i * size, size);
            size_t j = i;
            while (j > 0 && compare(base + (j - 1) * size, temp) > 0) {
                memcpy(base + j * size, base + (j - 1) * size, size);
                j--;
            }
            memcpy(base + j * size, temp, size);
        }
        return;
    }

    // 三数取中选择枢轴
    size_t mid = num / 2;
    size_t last = num - 1;
    char* a = base;
    char* b = base + mid * size;
    char* c = base + last * size;
    
    if (compare(a, b) > 0) {
        memcpy(temp, a, size);
        memcpy(a, b, size);
        memcpy(b, temp, size);
    }
    if (compare(b, c) > 0) {
        memcpy(temp, b, size);
        memcpy(b, c, size);
        memcpy(c, temp, size);
    }
    if (compare(a, b) > 0) {
        memcpy(temp, a, size);
        memcpy(a, b, size);
        memcpy(b, temp, size);
    }
    
    // 将枢轴移到倒数第二个位置
    memcpy(temp, b, size);
    memcpy(b, base + (last - 1) * size, size);
    memcpy(base + (last - 1) * size, temp, size);
    
    // 三路划分
    size_t i = 0, j = 0, k = last;
    char* pivot = base + (last - 1) * size;
    
    while (j < k) {
        int cmp = compare(base + j * size, pivot);
        if (cmp < 0) {
            if (i != j) {
                memcpy(temp, base + i * size, size);
                memcpy(base + i * size, base + j * size, size);
                memcpy(base + j * size, temp, size);
            }
            i++;
            j++;
        } else if (cmp > 0) {
            k--;
            memcpy(temp, base + j * size, size);
            memcpy(base + j * size, base + k * size, size);
            memcpy(base + k * size, temp, size);
        } else {
            j++;
        }
    }
    
    // 递归处理左右两部分
    quick_sort_3way_partition(base, i, size, compare, temp);
    quick_sort_3way_partition(base + (k + 1) * size, last - k, size, compare, temp);
}

void quick_sort_opt(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num < 2) return;
    
    char* temp = malloc(size);
    if (!temp) {
        HANDLE_ERROR("Memory allocation failed");
    }
    
    quick_sort_3way_partition(base, num, size, compare, temp);
    free(temp);
}
