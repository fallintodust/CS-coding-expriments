#include "sort.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

// 测试用的结构体
typedef struct {
    int key;
    char data[20];
} TestStruct;

// 比较函数 (整数比较使用 utils.c 中的 compare_int)
int compare_teststruct(const void* a, const void* b) {
    return ((TestStruct*)a)->key - ((TestStruct*)b)->key;
}

// 打印数组内容
void print_array_int(int* arr, size_t n) {
    printf("Array content: ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void print_array_struct(TestStruct* arr, size_t n) {
    printf("Struct array content: ");
    for (size_t i = 0; i < n; i++) {
        printf("(%d,%s) ", arr[i].key, arr[i].data);
    }
    printf("\n");
}

// 验证排序结果
int verify_sorted_int(int* arr, size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (arr[i-1] > arr[i]) {
            return 0;
        }
    }
    return 1;
}

int verify_sorted_struct(TestStruct* arr, size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (arr[i-1].key > arr[i].key) {
            return 0;
        }
    }
    return 1;
}

// 获取当前时间（微秒）
long long get_time_usec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000 + tv.tv_usec;
}

// 运行单次测试并返回执行时间（微秒）
long long run_test(void* arr, size_t n, size_t size, CompareFunc compare, 
                  void (*sort_func)(void*, size_t, size_t, CompareFunc)) {
    long long start = get_time_usec();
    sort_func(arr, n, size, compare);
    return get_time_usec() - start;
}

// 主测试函数
void test_robustness() {
    size_t test_sizes[] = {0, 1, 5};
    const char* size_names[] = {"零元素", "单元素", "五元素"};
    
    printf("\n=== 鲁棒性测试开始 ===\n");
    
    // 测试整数排序
    printf("\n=== 测试整数排序 ===\n");
    for (size_t i = 0; i < sizeof(test_sizes)/sizeof(test_sizes[0]); i++) {
        size_t n = test_sizes[i];
        printf("\n--- 测试 %s 数组 ---\n", size_names[i]);
        
        // 分配并初始化数组
        int* arr_original = malloc(n * sizeof(int));
        int* arr_quick = malloc(n * sizeof(int));
        int* arr_quick_opt = malloc(n * sizeof(int));
        
        if (n > 0 && (!arr_original || !arr_quick || !arr_quick_opt)) {
            printf("内存分配失败！\n");
            return;
        }
        
        // 初始化数据
        for (size_t j = 0; j < n; j++) {
            arr_original[j] = rand() % 100;
        }
        
        // 复制数组
        if (n > 0) {
            memcpy(arr_quick, arr_original, n * sizeof(int));
            memcpy(arr_quick_opt, arr_original, n * sizeof(int));
        }
        
        // 打印原始数组
        if (n > 0) {
            printf("原始数组: ");
            print_array_int(arr_original, n);
        } else {
            printf("空数组测试\n");
        }
        
        // 测试普通快速排序
        printf("\n普通快速排序: ");
        long long time_quick = run_test(arr_quick, n, sizeof(int), compare_int, quick_sort);
        if (n > 0) {
            print_array_int(arr_quick, n);
            printf("验证结果: %s\n", verify_sorted_int(arr_quick, n) ? "正确" : "错误");
        }
        printf("执行时间: %lld 微秒\n", time_quick);
        
        // 测试优化快速排序
        printf("\n优化快速排序: ");
        long long time_quick_opt = run_test(arr_quick_opt, n, sizeof(int), compare_int, quick_sort_opt);
        if (n > 0) {
            print_array_int(arr_quick_opt, n);
            printf("验证结果: %s\n", verify_sorted_int(arr_quick_opt, n) ? "正确" : "错误");
        }
        printf("执行时间: %lld 微秒\n", time_quick_opt);
        
        // 释放内存
        free(arr_original);
        free(arr_quick);
        free(arr_quick_opt);
    }
    
    // 测试结构体排序
    printf("\n=== 测试结构体排序 ===\n");
    for (size_t i = 0; i < sizeof(test_sizes)/sizeof(test_sizes[0]); i++) {
        size_t n = test_sizes[i];
        printf("\n--- 测试 %s 数组 ---\n", size_names[i]);
        
        // 分配并初始化数组
        TestStruct* arr_original = malloc(n * sizeof(TestStruct));
        TestStruct* arr_quick = malloc(n * sizeof(TestStruct));
        TestStruct* arr_quick_opt = malloc(n * sizeof(TestStruct));
        
        if (n > 0 && (!arr_original || !arr_quick || !arr_quick_opt)) {
            printf("内存分配失败！\n");
            return;
        }
        
        // 初始化数据
        for (size_t j = 0; j < n; j++) {
            arr_original[j].key = rand() % 100;
            sprintf(arr_original[j].data, "data-%zu", j);
        }
        
        // 复制数组
        if (n > 0) {
            memcpy(arr_quick, arr_original, n * sizeof(TestStruct));
            memcpy(arr_quick_opt, arr_original, n * sizeof(TestStruct));
        }
        
        // 打印原始数组
        if (n > 0) {
            printf("原始数组: ");
            print_array_struct(arr_original, n);
        } else {
            printf("空数组测试\n");
        }
        
        // 测试普通快速排序
        printf("\n普通快速排序: ");
        long long time_quick = run_test(arr_quick, n, sizeof(TestStruct), compare_teststruct, quick_sort);
        if (n > 0) {
            print_array_struct(arr_quick, n);
            printf("验证结果: %s\n", verify_sorted_struct(arr_quick, n) ? "正确" : "错误");
        }
        printf("执行时间: %lld 微秒\n", time_quick);
        
        // 测试优化快速排序
        printf("\n优化快速排序: ");
        long long time_quick_opt = run_test(arr_quick_opt, n, sizeof(TestStruct), compare_teststruct, quick_sort_opt);
        if (n > 0) {
            print_array_struct(arr_quick_opt, n);
            printf("验证结果: %s\n", verify_sorted_struct(arr_quick_opt, n) ? "正确" : "错误");
        }
        printf("执行时间: %lld 微秒\n", time_quick_opt);
        
        // 释放内存
        free(arr_original);
        free(arr_quick);
        free(arr_quick_opt);
    }
    
    printf("\n=== 鲁棒性测试结束 ===\n");
}

int main() {
    srand(time(NULL));  // 初始化随机数生成器
    test_robustness();
    return 0;
}
