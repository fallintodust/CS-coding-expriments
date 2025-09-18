#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"

void run_and_show(const char* name, SortFunction f, int* data, size_t n) {
    int* copy = malloc(n * sizeof(int));
    memcpy(copy, data, n * sizeof(int));
    printf("\n--- %s (n=%zu) ---\n", name, n);
    if (n == 0) {
        printf("空数组测试\n");
        free(copy);
        return;
    }
    printf("原始: ");
    for (size_t i = 0; i < n; i++) printf("%d ", copy[i]);
    printf("\n");

    f(copy, n, sizeof(int), compare_int);

    printf("排序后: ");
    for (size_t i = 0; i < n; i++) printf("%d ", copy[i]);
    printf("\n");

    // 验证
    int ok = 1;
    for (size_t i = 1; i < n; i++) if (copy[i] < copy[i-1]) { ok = 0; break; }
    printf("验证: %s\n", ok ? "正确" : "错误");

    free(copy);
}

int main(int argc, char** argv) {
    // 支持 stdin 或者文件重定向
    size_t n;
    if (scanf("%zu", &n) != 1) {
        fprintf(stderr, "请输入元素个数 n（例如：0,1,5）然后回车，再输入 n 个整数（或用重定向文件）\n");
        return 1;
    }
    int* data = malloc((n>0?n:1) * sizeof(int));
    for (size_t i = 0; i < n; i++) {
        if (scanf("%d", &data[i]) != 1) {
            fprintf(stderr, "输入数量少于 n\n");
            free(data);
            return 1;
        }
    }

    run_and_show("冒泡排序", (SortFunction)bubble_sort, data, n);
    run_and_show("选择排序", (SortFunction)selection_sort, data, n);
    run_and_show("插入排序", (SortFunction)insertion_sort, data, n);

    free(data);
    return 0;
}
