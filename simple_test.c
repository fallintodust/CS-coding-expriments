#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sort.h"

#define SIZE 5000

// 生成随机数据
void generate_random_data(int* arr, int size) {
    for(int i = 0; i < size; i++) {
        arr[i] = rand() % 10000;
    }
}

// 检查是否已排序
int check_sorted(const int* arr, int size) {
    for(int i = 1; i < size; i++) {
        if(arr[i] < arr[i-1]) return 0;
    }
    return 1;
}

// 显示部分数组内容
void show_array(const int* arr, int size, const char* msg) {
    printf("%s: ", msg);
    for(int i = 0; i < 10 && i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    srand(time(NULL));
    
    // 分配内存
    int* data = (int*)malloc(SIZE * sizeof(int));
    int* temp = (int*)malloc(SIZE * sizeof(int));
    
    if (!data || !temp) {
        printf("内存分配失败！\n");
        return 1;
    }
    
    printf("\n=== 排序算法性能测试 (数组大小: %d) ===\n", SIZE);
    
    // 1. 快速排序测试
    printf("\n--- 快速排序 ---\n");
    
    // 随机数据测试
    printf("\n测试随机数据:\n");
    generate_random_data(data, SIZE);
    show_array(data, SIZE, "排序前");
    
    clock_t start = clock();
    quick_sort(data, SIZE, sizeof(int), compare_int);
    clock_t end = clock();
    
    show_array(data, SIZE, "排序后");
    printf("排序结果: %s\n", check_sorted(data, SIZE) ? "正确" : "错误");
    printf("执行时间: %.3f 毫秒\n", (double)(end - start) * 1000 / CLOCKS_PER_SEC);
    
    // 2. 归并排序测试
    printf("\n--- 归并排序 ---\n");
    
    // 随机数据测试
    printf("\n测试随机数据:\n");
    generate_random_data(data, SIZE);
    show_array(data, SIZE, "排序前");
    
    start = clock();
    merge_sort(data, SIZE, sizeof(int), compare_int);
    end = clock();
    
    show_array(data, SIZE, "排序后");
    printf("排序结果: %s\n", check_sorted(data, SIZE) ? "正确" : "错误");
    printf("执行时间: %.3f 毫秒\n", (double)(end - start) * 1000 / CLOCKS_PER_SEC);
    
    // 释放内存
    free(data);
    free(temp);
    
    return 0;
}