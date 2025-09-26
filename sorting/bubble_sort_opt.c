#include "sort.h"
#include <stdint.h>
#include <immintrin.h>  // 用于SIMD优化

// CPU优化版本 - 带循环展开、分支预测优化和提前退出优化
void bubble_sort_cpu_opt(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num < 2) return;
    
    char* begin = (char*)base;
    char* temp = malloc(size);
    if (!temp) {
        HANDLE_ERROR("Memory allocation for temp in bubble sort");
    }

    // 循环展开因子为4
    for (size_t i = 0; i < num - 1; i++) {
        int swapped = 0;  // 分支预测优化
        for (size_t j = 0; j < num - 1 - i; j += 4) {
            // 预取数据
            __builtin_prefetch(begin + (j + 8) * size);
            
            // 第一次比较
            if (j < num - 1 - i && compare(begin + j * size, begin + (j + 1) * size) > 0) {
                memcpy(temp, begin + j * size, size);
                memcpy(begin + j * size, begin + (j + 1) * size, size);
                memcpy(begin + (j + 1) * size, temp, size);
                swapped = 1;
            }
            
            // 第二次比较
            if (j + 1 < num - 1 - i && compare(begin + (j + 1) * size, begin + (j + 2) * size) > 0) {
                memcpy(temp, begin + (j + 1) * size, size);
                memcpy(begin + (j + 1) * size, begin + (j + 2) * size, size);
                memcpy(begin + (j + 2) * size, temp, size);
                swapped = 1;
            }
            
            // 第三次比较
            if (j + 2 < num - 1 - i && compare(begin + (j + 2) * size, begin + (j + 3) * size) > 0) {
                memcpy(temp, begin + (j + 2) * size, size);
                memcpy(begin + (j + 2) * size, begin + (j + 3) * size, size);
                memcpy(begin + (j + 3) * size, temp, size);
                swapped = 1;
            }
            
            // 第四次比较
            if (j + 3 < num - 1 - i && compare(begin + (j + 3) * size, begin + (j + 4) * size) > 0) {
                memcpy(temp, begin + (j + 3) * size, size);
                memcpy(begin + (j + 3) * size, begin + (j + 4) * size, size);
                memcpy(begin + (j + 4) * size, temp, size);
                swapped = 1;
            }
        }
        // 如果没有交换发生，提前退出
        if (!swapped) break;
    }

    free(temp);
}

// Cache优化版本 - 分块处理
#define CACHE_LINE_SIZE 64
#define BLOCK_SIZE (CACHE_LINE_SIZE / sizeof(int))  // 对于int类型数据优化

// SIMD优化版本（仅支持int类型）
void bubble_sort_simd(void* base, size_t num) {
    if (num < 2) return;
    
    int* arr = (int*)base;
    
    // 使用SSE指令集进行向量化
    for (size_t i = 0; i < num - 1; i++) {
        int swapped = 0;
        for (size_t j = 0; j < num - 4 - i; j += 4) {
            // 加载4个连续的int到128位向量寄存器
            __m128i v1 = _mm_loadu_si128((__m128i*)&arr[j]);
            __m128i v2 = _mm_loadu_si128((__m128i*)&arr[j+1]);
            
            // 比较相邻元素
            __m128i cmp = _mm_cmpgt_epi32(v1, v2);
            
            // 如果需要交换任何元素
            if (_mm_movemask_epi8(cmp)) {
                for (size_t k = 0; k < 4; k++) {
                    if (arr[j+k] > arr[j+k+1]) {
                        int temp = arr[j+k];
                        arr[j+k] = arr[j+k+1];
                        arr[j+k+1] = temp;
                        swapped = 1;
                    }
                }
            }
        }
        
        // 处理剩余元素
        for (size_t j = (num - 4 - i + 3) & ~3; j < num - 1 - i; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                swapped = 1;
            }
        }
        
        if (!swapped) break;
    }
}

void bubble_sort_cache_opt(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num < 2) return;
    
    char* begin = (char*)base;
    char* temp = malloc(size);
    if (!temp) {
        HANDLE_ERROR("Memory allocation for temp in bubble sort");
    }

    // 对齐数据到cache line边界
    begin = (char*)((uintptr_t)(begin + CACHE_LINE_SIZE - 1) & ~(CACHE_LINE_SIZE - 1));

    // 使用分块策略处理数据
    for (size_t block = 0; block < num; block += BLOCK_SIZE) {
        size_t block_end = (block + BLOCK_SIZE < num) ? block + BLOCK_SIZE : num;
        
        // 对当前块进行排序
        for (size_t i = block; i < block_end - 1; i++) {
            int swapped = 0;
            // 预取下一个cache line的数据
            __builtin_prefetch(begin + (i + BLOCK_SIZE) * size);
            
            for (size_t j = block; j < block_end - 1; j++) {
                // 预取下一个元素
                __builtin_prefetch(begin + (j + 2) * size);
                
                if (compare(begin + j * size, begin + (j + 1) * size) > 0) {
                    // 直接在寄存器中进行交换操作
                    memcpy(temp, begin + j * size, size);
                    memcpy(begin + j * size, begin + (j + 1) * size, size);
                    memcpy(begin + (j + 1) * size, temp, size);
                    swapped = 1;
                }
            }
            if (!swapped) break;  // 当前块已经有序
        }
    }

    // 合并已排序的块
    for (size_t i = 0; i < num - 1; i++) {
        int swapped = 0;
        for (size_t j = 0; j < num - 1 - i; j++) {
            __builtin_prefetch(begin + (j + 2) * size);
            
            if (compare(begin + j * size, begin + (j + 1) * size) > 0) {
                memcpy(temp, begin + j * size, size);
                memcpy(begin + j * size, begin + (j + 1) * size, size);
                memcpy(begin + (j + 1) * size, temp, size);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }

    free(temp);
}
