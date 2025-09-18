#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sort.h"

// 测试数据大小
#define SIZE 5000

// 打印数组前几个元素
void print_array_preview(int arr[], int size, int preview_size) {
    printf("前%d个元素: ", preview_size);
    for(int i = 0; i < preview_size && i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 检查是否已排序
int is_sorted(int arr[], int size) {
    for(int i = 1; i < size; i++) {
        if(arr[i] < arr[i-1]) return 0;
    }
    return 1;
}

// 测试单个排序算法
void test_sort_algorithm(void (*sort_func)(void*, size_t, size_t, CompareFunc),
                        const char* sort_name,
                        int* original_data,
                        int size) {
    // 创建测试数据的副本
    int* test_data = malloc(size * sizeof(int));
    memcpy(test_data, original_data, size * sizeof(int));
    
    printf("\n=== 测试 %s ===\n", sort_name);
    printf("数组大小: %d\n", size);
    
    printf("排序前");
    print_array_preview(test_data, size, 10);
    
    // 记录开始时间
    clock_t start = clock();
    
    // 执行排序
    sort_func(test_data, size, sizeof(int), compare_int);
    
    // 记录结束时间
    clock_t end = clock();
    double time_spent = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    
    printf("排序后");
    print_array_preview(test_data, size, 10);
    
    // 验证排序结果
    if(is_sorted(test_data, size)) {
        printf("排序正确!\n");
    } else {
        printf("排序错误!\n");
    }
    
    printf("执行时间: %.3f 毫秒\n", time_spent);
    
    free(test_data);
}

int main() {
    // 初始化随机数生成器
    srand(time(NULL));
    
    // 分配内存并生成测试数据
    int *original_data = malloc(SIZE * sizeof(int));
    if (!original_data) {
        printf("内存分配失败\n");
        return 1;
    }
    
    // 1. 随机数据测试
    printf("\n=== 随机数据测试 ===\n");
    for(int i = 0; i < SIZE; i++) {
        original_data[i] = rand() % 10000;
    }
    
    test_sort_algorithm(quick_sort, "快速排序", original_data, SIZE);
    test_sort_algorithm(merge_sort, "归并排序", original_data, SIZE);
    
    // 2. 近乎有序数据测试
    printf("\n=== 近乎有序数据测试 ===\n");
    quick_sort(original_data, SIZE, sizeof(int), compare_int);
    // 随机交换5%的元素
    for(int i = 0; i < SIZE * 0.05; i++) {
        int idx1 = rand() % SIZE;
        int idx2 = rand() % SIZE;
        int temp = original_data[idx1];
        original_data[idx1] = original_data[idx2];
        original_data[idx2] = temp;
    }
    
    test_sort_algorithm(quick_sort, "快速排序", original_data, SIZE);
    test_sort_algorithm(merge_sort, "归并排序", original_data, SIZE);
    
    // 3. 逆序数据测试
    printf("\n=== 逆序数据测试 ===\n");
    for(int i = 0; i < SIZE; i++) {
        original_data[i] = SIZE - i;
    }
    
    test_sort_algorithm(quick_sort, "快速排序", original_data, SIZE);
    test_sort_algorithm(merge_sort, "归并排序", original_data, SIZE);
    
    free(original_data);
    return 0;
}