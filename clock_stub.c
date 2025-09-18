#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

static FILE* log_file = NULL;
static clock_t start_time = 0;
static int in_sort = 0;

// 原始clock函数的指针
static clock_t (*original_clock)(void) = NULL;

__attribute__((constructor))
static void init(void) {
    // 获取原始clock函数
    original_clock = dlsym(RTLD_NEXT, "clock");
    
    // 打开日志文件
    log_file = fopen("sort_timing.txt", "w");
    if (!log_file) {
        fprintf(stderr, "无法创建日志文件\n");
        exit(1);
    }
    fprintf(log_file, "排序函数性能测试日志\n");
    fprintf(log_file, "----------------------------------------\n");
    fflush(log_file);
}

__attribute__((destructor))
static void cleanup(void) {
    if (log_file) {
        fclose(log_file);
    }
}

clock_t clock(void) {
    // 获取真实的时钟值
    clock_t real_time = original_clock();
    
    // 获取调用栈信息
    void *caller = __builtin_return_address(0);
    Dl_info info;
    
    if (dladdr(caller, &info)) {
        // 检查是否在排序函数中
        if (info.dli_fname && strstr(info.dli_fname, "libsort.so")) {
            if (!in_sort) {
                // 排序开始
                start_time = real_time;
                in_sort = 1;
            } else {
                // 排序结束
                clock_t duration = real_time - start_time;
                double seconds = (double)duration / CLOCKS_PER_SEC;
                
                fprintf(log_file, "函数: %s\n", info.dli_sname ? info.dli_sname : "unknown");
                fprintf(log_file, "运行时间: %.6f 秒\n", seconds);
                fprintf(log_file, "----------------------------------------\n");
                fflush(log_file);
                
                in_sort = 0;
            }
        }
    }
    
    return real_time;
}
