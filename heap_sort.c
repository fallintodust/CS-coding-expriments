#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"

// 维护最大堆性质的函数
static void heapify(void *arr, size_t n, size_t i, size_t size, int (*compare)(const void *, const void *)) {
    char *base = (char *)arr;
    size_t largest = i;         // 初始化最大元素为根
    size_t left = 2 * i + 1;   // 左子节点
    size_t right = 2 * i + 2;  // 右子节点
    char *temp = (char *)malloc(size);

    // 如果左子节点大于根节点
    if (left < n && compare(base + left * size, base + largest * size) > 0) {
        largest = left;
    }

    // 如果右子节点大于目前找到的最大值
    if (right < n && compare(base + right * size, base + largest * size) > 0) {
        largest = right;
    }

    // 如果最大值不是根节点
    if (largest != i) {
        // 交换根节点和最大值
        memcpy(temp, base + i * size, size);
        memcpy(base + i * size, base + largest * size, size);
        memcpy(base + largest * size, temp, size);

        // 递归地维护受影响的子堆
        heapify(arr, n, largest, size, compare);
    }

    free(temp);
}

// 堆排序主函数
void heap_sort(void *arr, size_t n, size_t size, int (*compare)(const void *, const void *)) {
    if (n <= 1) return;  // 处理边界情况

    char *base = (char *)arr;
    char *temp = (char *)malloc(size);
    
    // 构建最大堆（从最后一个非叶节点开始）
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, size, compare);
    }

    // 逐个从堆中提取元素
    for (size_t i = n - 1; i > 0; i--) {
        // 将当前根（最大值）移到末尾
        memcpy(temp, base, size);
        memcpy(base, base + i * size, size);
        memcpy(base + i * size, temp, size);

        // 在减小的堆上调用heapify
        heapify(arr, i, 0, size, compare);
    }

    free(temp);
}

// 优化版堆排序（使用更大的缓存行和循环展开）
void heap_sort_opt(void *arr, size_t n, size_t size, int (*compare)(const void *, const void *)) {
    if (n <= 1) return;

    char *base = (char *)arr;
    char *temp = (char *)malloc(size);
    
    // 使用块处理来构建初始堆
    #define BLOCK_SIZE 4
    for (int i = (n / 2 - 1) / BLOCK_SIZE * BLOCK_SIZE; i >= 0; i -= BLOCK_SIZE) {
        for (int j = 0; j < BLOCK_SIZE && i + j >= 0; j++) {
            heapify(arr, n, i + j, size, compare);
        }
    }

    // 使用预取优化的堆排序
    for (size_t i = n - 1; i > 0; i--) {
        // 预取下一个要处理的元素
        if (i > 1) {
            __builtin_prefetch(base + (i-1) * size);
        }

        // 交换根和最后一个元素
        memcpy(temp, base, size);
        memcpy(base, base + i * size, size);
        memcpy(base + i * size, temp, size);

        // 在减小的堆上调用heapify
        heapify(arr, i, 0, size, compare);
    }

    free(temp);
}