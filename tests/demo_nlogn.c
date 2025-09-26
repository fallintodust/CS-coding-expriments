#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../sorting/sort.h"

void show_array(int* arr, size_t n, const char* label) {
    printf("%s: ", label);
    if (n <= 20) {
        // 小数组显示所有元素
        for (size_t i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
    } else {
        // 大数组只显示前10个和后10个元素
        for (size_t i = 0; i < 10; i++) {
            printf("%d ", arr[i]);
        }
        printf("... ");
        for (size_t i = n-10; i < n; i++) {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");
}

double now_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec*1000.0 + t.tv_nsec/1e6;
}

int main(int argc, char** argv) {
    size_t N = 20;  // 默认小规模便于观察
    if (argc > 1) {
        N = atoi(argv[1]);
    }
    
    int* original = malloc(N * sizeof(int));
    int* work = malloc(N * sizeof(int));
    if (!original || !work) {
        perror("malloc");
        return 1;
    }
    
    // 生成测试数据
    srand(12345);
    for (size_t i = 0; i < N; i++) {
        original[i] = rand() % 100;  // 限制在0-99范围内便于观察
    }
    
    printf("=== O(nlogn) 排序算法分别演示 (N=%zu) ===\n\n", N);
    show_array(original, N, "原始数组");
    printf("\n");
    
    // 1. 归并排序
    printf("1. 归并排序 (Merge Sort)\n");
    printf("   算法原理：分治策略，递归分割后合并有序子数组\n");
    printf("   时间复杂度：O(nlogn) (稳定)\n");
    printf("   空间复杂度：O(n)\n");
    memcpy(work, original, N * sizeof(int));
    double t1 = now_ms();
    merge_sort(work, N, sizeof(int), compare_int);
    double t2 = now_ms();
    show_array(work, N, "   排序结果");
    printf("   运行时间: %.3f ms\n\n", t2 - t1);
    
    // 验证排序正确性
    int is_sorted = 1;
    for (size_t i = 0; i < N-1; i++) {
        if (work[i] > work[i+1]) {
            is_sorted = 0;
            break;
        }
    }
    printf("   排序验证: %s\n\n", is_sorted ? "✓ 正确" : "✗ 错误");
    
    // 2. 快速排序
    printf("2. 快速排序 (Quick Sort)\n");
    printf("   算法原理：选择基准元素，分区后递归排序\n");
    printf("   时间复杂度：O(nlogn) 平均，O(n²) 最坏\n");
    printf("   空间复杂度：O(logn)\n");
    memcpy(work, original, N * sizeof(int));
    t1 = now_ms();
    quick_sort(work, N, sizeof(int), compare_int);
    t2 = now_ms();
    show_array(work, N, "   排序结果");
    printf("   运行时间: %.3f ms\n\n", t2 - t1);
    
    // 验证排序正确性
    is_sorted = 1;
    for (size_t i = 0; i < N-1; i++) {
        if (work[i] > work[i+1]) {
            is_sorted = 0;
            break;
        }
    }
    printf("   排序验证: %s\n\n", is_sorted ? "✓ 正确" : "✗ 错误");
    
    // 3. 堆排序
    printf("3. 堆排序 (Heap Sort)\n");
    printf("   算法原理：构建最大堆，逐步取出最大元素\n");
    printf("   时间复杂度：O(nlogn) (稳定)\n");
    printf("   空间复杂度：O(1)\n");
    memcpy(work, original, N * sizeof(int));
    t1 = now_ms();
    heap_sort(work, N, sizeof(int), compare_int);
    t2 = now_ms();
    show_array(work, N, "   排序结果");
    printf("   运行时间: %.3f ms\n\n", t2 - t1);
    
    // 验证排序正确性
    is_sorted = 1;
    for (size_t i = 0; i < N-1; i++) {
        if (work[i] > work[i+1]) {
            is_sorted = 0;
            break;
        }
    }
    printf("   排序验证: %s\n\n", is_sorted ? "✓ 正确" : "✗ 错误");
    
    // 性能总结
    printf("=== 性能特点总结 ===\n");
    printf("• 归并排序：稳定排序，最坏情况仍为O(nlogn)，需要额外空间\n");
    printf("• 快速排序：平均情况最快，原地排序，但最坏情况为O(n²)\n");
    printf("• 堆排序：  最坏情况仍为O(nlogn)，原地排序，但常数因子较大\n\n");
    
    if (N >= 1000) {
        printf("=== 大规模数据建议 ===\n");
        printf("• 对于随机数据：推荐快速排序（平均性能最佳）\n");
        printf("• 对于稳定性要求：推荐归并排序\n");
        printf("• 对于内存限制：推荐堆排序（原地排序）\n");
    }
    
    free(original);
    free(work);
    return 0;
}