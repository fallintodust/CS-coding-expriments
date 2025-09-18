#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sort.h"

#define ARRAY_SIZE 5000
#define TEST_ROUNDS 5

// 测试数组结构
typedef struct {
    int* data;
    int* backup;
    size_t size;
} TestArray;

// 初始化测试数组
TestArray* create_test_array(size_t size) {
    TestArray* arr = (TestArray*)malloc(sizeof(TestArray));
    arr->data = (int*)malloc(size * sizeof(int));
    arr->backup = (int*)malloc(size * sizeof(int));
    arr->size = size;
    return arr;
}

// 释放测试数组
void free_test_array(TestArray* arr) {
    free(arr->data);
    free(arr->backup);
    free(arr);
}

// 生成随机数据
void generate_random_data(TestArray* arr) {
    for (size_t i = 0; i < arr->size; i++) {
        arr->data[i] = rand() % 10000;
    }
    memcpy(arr->backup, arr->data, arr->size * sizeof(int));
}

// 还原备份数据
void restore_data(TestArray* arr) {
    memcpy(arr->data, arr->backup, arr->size * sizeof(int));
}

// 验证排序结果
int verify_sort_result(int* arr, size_t size) {
    for (size_t i = 1; i < size; i++) {
        if (arr[i] < arr[i-1]) return 0;
    }
    return 1;
}

// 运行单个排序测试
void run_sort_test(void (*sort_func)(void*, size_t, size_t, CompareFunc),
                  const char* sort_name,
                  TestArray* arr) {
    clock_t start, end;
    double total_time = 0;
    int all_correct = 1;
    
    printf("\n=== 测试 %s ===\n", sort_name);
    
    for (int round = 0; round < TEST_ROUNDS; round++) {
        restore_data(arr);
        
        start = clock();
        sort_func(arr->data, arr->size, sizeof(int), compare_int);
        end = clock();
        
        if (!verify_sort_result(arr->data, arr->size)) {
            printf("第 %d 轮排序结果错误！\n", round + 1);
            all_correct = 0;
            break;
        }
        
        total_time += ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    }
    
    if (all_correct) {
        printf("排序正确\n");
        printf("平均执行时间: %.3f 毫秒\n", total_time / TEST_ROUNDS);
        printf("排序结果示例（前10个）: ");
        for (int i = 0; i < 10 && i < arr->size; i++) {
            printf("%d ", arr->data[i]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    TestArray* test_arr = create_test_array(ARRAY_SIZE);
    
    printf("=== 排序算法性能测试 ===\n");
    printf("数组大小: %d\n", ARRAY_SIZE);
    printf("测试轮数: %d\n", TEST_ROUNDS);
    
    // 随机数据测试
    printf("\n--- 随机数据测试 ---");
    generate_random_data(test_arr);
    run_sort_test(quick_sort, "快速排序", test_arr);
    run_sort_test(merge_sort, "归并排序", test_arr);
    run_sort_test(bubble_sort, "冒泡排序", test_arr);
    run_sort_test(quick_sort_opt, "优化快速排序", test_arr);
    
    // 近乎有序数据测试
    printf("\n--- 近乎有序数据测试 ---");
    generate_random_data(test_arr);
    quick_sort(test_arr->data, test_arr->size, sizeof(int), compare_int);
    // 随机打乱5%的元素
    for (int i = 0; i < test_arr->size * 0.05; i++) {
        int idx1 = rand() % test_arr->size;
        int idx2 = rand() % test_arr->size;
        int temp = test_arr->data[idx1];
        test_arr->data[idx1] = test_arr->data[idx2];
        test_arr->data[idx2] = temp;
    }
    memcpy(test_arr->backup, test_arr->data, test_arr->size * sizeof(int));
    
    run_sort_test(quick_sort, "快速排序", test_arr);
    run_sort_test(merge_sort, "归并排序", test_arr);
    run_sort_test(bubble_sort, "冒泡排序", test_arr);
    run_sort_test(quick_sort_opt, "优化快速排序", test_arr);
    
    // 逆序数据测试
    printf("\n--- 逆序数据测试 ---");
    for (size_t i = 0; i < test_arr->size; i++) {
        test_arr->data[i] = test_arr->size - i;
    }
    memcpy(test_arr->backup, test_arr->data, test_arr->size * sizeof(int));
    
    run_sort_test(quick_sort, "快速排序", test_arr);
    run_sort_test(merge_sort, "归并排序", test_arr);
    run_sort_test(bubble_sort, "冒泡排序", test_arr);
    run_sort_test(quick_sort_opt, "优化快速排序", test_arr);
    
    free_test_array(test_arr);
    return 0;
}