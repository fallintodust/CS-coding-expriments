#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"

#define ARRAY_SIZE 65536

int main() {
    int *data = malloc(ARRAY_SIZE * sizeof(int));
    
    // 初始化随机数据
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        data[i] = rand();
    }
    
    // 测试快速排序
    printf("Testing quicksort with %d elements...\n", ARRAY_SIZE);
    quick_sort(data, ARRAY_SIZE, sizeof(int), compare_int);
    
    // 验证排序结果
    int is_sorted = 1;
    for (int i = 1; i < ARRAY_SIZE; i++) {
        if (data[i] < data[i-1]) {
            is_sorted = 0;
            break;
        }
    }
    printf("Sort result: %s\n", is_sorted ? "Correct" : "Incorrect");
    
    free(data);
    return 0;
}