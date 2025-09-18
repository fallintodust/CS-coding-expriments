#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

/* 调试宏定义 */
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

/* 错误处理宏 */
#define HANDLE_ERROR(msg) \
    do { \
        fprintf(stderr, "%s failed: %s (errno: %d)\n", msg, strerror(errno), errno); \
        exit(EXIT_FAILURE); \
    } while (0)

/* 比较函数类型定义 */
typedef int (*CompareFunc)(const void*, const void*);

/* 排序算法类型枚举 */
typedef enum {
    BUBBLE_SORT,
    SELECTION_SORT,
    INSERTION_SORT,
    MERGE_SORT,
    QUICK_SORT,
    HEAP_SORT
} SortType;

/* 数据类型枚举 */
typedef enum {
    INTEGER_TYPE,
    FLOAT_TYPE,
    STRUCT_TYPE
} DataType;

/* 示例结构体定义 */
typedef struct {
    int id;
    char name[50];
    float score;
} Student;

/* 通用排序函数指针类型 */
typedef void (*SortFunction)(void*, size_t, size_t, CompareFunc);

/* 排序算法接口声明 */
// 基本排序算法
void bubble_sort(void* base, size_t num, size_t size, CompareFunc compare);
void quick_sort(void* base, size_t num, size_t size, CompareFunc compare);
void insertion_sort(void* base, size_t num, size_t size, CompareFunc compare);
void selection_sort(void* base, size_t num, size_t size, CompareFunc compare);

// 优化版本的排序算法
void bubble_sort_opt(void* base, size_t num, size_t size, CompareFunc compare);
void quick_sort_opt(void* base, size_t num, size_t size, CompareFunc compare);
void heap_sort(void* base, size_t num, size_t size, CompareFunc compare);
void heap_sort_opt(void* base, size_t num, size_t size, CompareFunc compare);

/* 比较函数声明 */
int compare_int(const void* a, const void* b);
int compare_float(const void* a, const void* b);
int compare_student(const void* a, const void* b);

/* 工具函数声明 */
void* generate_data_for_file(size_t num, DataType type);
void generate_test_file(const char* filename, size_t num, DataType type);
void generate_test_files(const char* prefix, size_t num);
void print_array(void* array, size_t num, DataType type);
void print_array_to_file(FILE* fp, void* array, size_t num, DataType type);
void verify_sort(void* array, size_t num, size_t size, CompareFunc compare);

#endif /* SORT_H */
