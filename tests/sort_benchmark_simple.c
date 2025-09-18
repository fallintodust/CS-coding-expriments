#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sort.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE 5000
#endif
#ifndef TEST_ROUNDS
#define TEST_ROUNDS 10
#endif

// 计时器结构体
typedef struct {
    clock_t start;
    clock_t end;
} Timer;

// 开始计时
void start_timer(Timer* timer) {
    timer->start = clock();
}

// 结束计时并返回毫秒数
double stop_timer(Timer* timer) {
    timer->end = clock();
    return (double)(timer->end - timer->start) * 1000.0 / CLOCKS_PER_SEC;
}

// 生成随机数组
void generate_random_array(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 10000;
    }
}

// 验证数组是否已排序
int is_sorted(int* arr, int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i-1]) return 0;
    }
    return 1;
}

// 性能测试函数
void benchmark_sort(void (*sort_func)(void*, size_t, size_t, CompareFunc), 
                   const char* sort_name, int* data, int size, FILE* csv, const char* data_type) {
    Timer timer;
    double total_time = 0.0;
    int* test_array = malloc(size * sizeof(int));
    
    printf("\n=== 测试 %s ===\n", sort_name);
    
    for (int round = 0; round < TEST_ROUNDS; round++) {
        // 复制原始数据
        memcpy(test_array, data, size * sizeof(int));
        
        // 开始计时并排序
        start_timer(&timer);
        sort_func(test_array, size, sizeof(int), compare_int);
        total_time += stop_timer(&timer);
        
        // 验证排序结果
        if (!is_sorted(test_array, size)) {
            printf("警告：%s 排序结果有误！\n", sort_name);
            break;
        }
    }
    
    double avg = total_time / TEST_ROUNDS;
    printf("数组大小: %d\n", size);
    printf("平均执行时间: %.3f 毫秒\n", avg);
    if (csv) {
        fprintf(csv, "%s,%s,%.6f\n", sort_name, data_type, avg);
        fflush(csv);
    }
    
    // 展示部分排序结果
    printf("排序结果示例（前10个元素）: ");
    for (int i = 0; i < (size < 10 ? size : 10); i++) {
        printf("%d ", test_array[i]);
    }
    printf("\n");
    
    free(test_array);
}

int main() {
    srand(time(NULL));
    
    // 生成测试数据
    int* original_data = malloc(ARRAY_SIZE * sizeof(int));
    
    printf("\n=== 排序性能测试报告 ===\n");
    printf("数组大小: %d\n", ARRAY_SIZE);
    printf("测试轮数: %d\n", TEST_ROUNDS);
    
    // 打开 CSV
    FILE* csv = fopen("bench5000.csv", "w");
    if (csv) {
        fprintf(csv, "algorithm,data_type,avg_ms\n");
    }

    // 1. 随机数据测试
    printf("\n--- 随机数据测试 ---");
    generate_random_array(original_data, ARRAY_SIZE);
    benchmark_sort(quick_sort, "快速排序", original_data, ARRAY_SIZE, csv, "random");
    benchmark_sort(quick_sort_opt, "快速排序_opt", original_data, ARRAY_SIZE, csv, "random");
    benchmark_sort(merge_sort, "归并排序", original_data, ARRAY_SIZE, csv, "random");
    benchmark_sort(heap_sort, "堆排序", original_data, ARRAY_SIZE, csv, "random");
    
    // 2. 近乎有序数据测试
    printf("\n--- 近乎有序数据测试 ---");
    generate_random_array(original_data, ARRAY_SIZE);
    quick_sort(original_data, ARRAY_SIZE, sizeof(int), compare_int);
    // 随机交换5%的元素
    for(int i = 0; i < ARRAY_SIZE * 0.05; i++) {
        int idx1 = rand() % ARRAY_SIZE;
        int idx2 = rand() % ARRAY_SIZE;
        int temp = original_data[idx1];
        original_data[idx1] = original_data[idx2];
        original_data[idx2] = temp;
    }
    benchmark_sort(quick_sort, "快速排序", original_data, ARRAY_SIZE, csv, "nearly_sorted");
    benchmark_sort(quick_sort_opt, "快速排序_opt", original_data, ARRAY_SIZE, csv, "nearly_sorted");
    benchmark_sort(merge_sort, "归并排序", original_data, ARRAY_SIZE, csv, "nearly_sorted");
    benchmark_sort(heap_sort, "堆排序", original_data, ARRAY_SIZE, csv, "nearly_sorted");
    
    // 3. 逆序数据测试
    printf("\n--- 逆序数据测试 ---");
    for(int i = 0; i < ARRAY_SIZE; i++) {
        original_data[i] = ARRAY_SIZE - i;
    }
    benchmark_sort(quick_sort, "快速排序", original_data, ARRAY_SIZE, csv, "reversed");
    benchmark_sort(quick_sort_opt, "快速排序_opt", original_data, ARRAY_SIZE, csv, "reversed");
    benchmark_sort(merge_sort, "归并排序", original_data, ARRAY_SIZE, csv, "reversed");
    benchmark_sort(heap_sort, "堆排序", original_data, ARRAY_SIZE, csv, "reversed");

    if (csv) fclose(csv);
    
    free(original_data);
    return 0;
}