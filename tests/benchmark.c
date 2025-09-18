#include "sort.h"
#include <sys/time.h>

// 使用gettimeofday进行更精确的时间测量
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

// 验证排序结果
int verify_sorted(void* base, size_t num, size_t size, CompareFunc compare) {
    char* ptr = (char*)base;
    for (size_t i = 0; i < num - 1; i++) {
        if (compare(ptr + i * size, ptr + (i + 1) * size) > 0) {
            return 0;  // 未排序
        }
    }
    return 1;  // 已排序
}

// 复制数据以保持原始数据不变
void* copy_data(void* src, size_t num, size_t size) {
    void* dest = malloc(num * size);
    if (!dest) {
        HANDLE_ERROR("Memory allocation failed");
    }
    memcpy(dest, src, num * size);
    return dest;
}

// 运行单次排序测试
void run_sort_test(const char* name, void* data, size_t num, size_t size,
                  CompareFunc compare, void (*sort_func)(void*, size_t, size_t, CompareFunc)) {
    void* test_data = copy_data(data, num, size);
    
    // 预热CPU
    for (int i = 0; i < 3; i++) {
        void* warm_data = copy_data(data, num, size);
        sort_func(warm_data, num, size, compare);
        free(warm_data);
    }
    
    // 实际测试
    double start_time = get_time();
    sort_func(test_data, num, size, compare);
    double end_time = get_time();
    
    double time_spent = end_time - start_time;
    int is_sorted = verify_sorted(test_data, num, size, compare);
    
    printf("%-20s Time: %10.6f seconds  %s\n",
           name, time_spent, is_sorted ? "√" : "×");
    
    free(test_data);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <test_prefix>\n", argv[0]);
        return 1;
    }
    
    char int_infile[256], float_infile[256], struct_infile[256];
    sprintf(int_infile, "%s_int.in", argv[1]);
    sprintf(float_infile, "%s_float.in", argv[1]);
    sprintf(struct_infile, "%s_struct.in", argv[1]);
    
    // 测试整数排序
    size_t num_int;
    void* int_data = read_data_from_file(int_infile, &num_int, INTEGER_TYPE);
    
    printf("\n=== 整数排序性能测试 (n=%zu) ===\n", num_int);
    printf("算法                  运行时间          结果\n");
    printf("----------------------------------------\n");
    
    run_sort_test("冒泡排序(原始)", int_data, num_int, sizeof(int), compare_int, bubble_sort);
    run_sort_test("冒泡排序(优化)", int_data, num_int, sizeof(int), compare_int, bubble_sort_opt);
    run_sort_test("快速排序(原始)", int_data, num_int, sizeof(int), compare_int, quick_sort);
    run_sort_test("快速排序(优化)", int_data, num_int, sizeof(int), compare_int, quick_sort_opt);
    
    printf("----------------------------------------\n");
    free(int_data);
    
    // 测试浮点数排序
    size_t num_float;
    void* float_data = read_data_from_file(float_infile, &num_float, FLOAT_TYPE);
    
    printf("\n=== 浮点数排序性能测试 (n=%zu) ===\n", num_float);
    printf("算法                  运行时间          结果\n");
    printf("----------------------------------------\n");
    
    run_sort_test("冒泡排序(原始)", float_data, num_float, sizeof(float), compare_float, bubble_sort);
    run_sort_test("冒泡排序(优化)", float_data, num_float, sizeof(float), compare_float, bubble_sort_opt);
    run_sort_test("快速排序(原始)", float_data, num_float, sizeof(float), compare_float, quick_sort);
    run_sort_test("快速排序(优化)", float_data, num_float, sizeof(float), compare_float, quick_sort_opt);
    
    printf("----------------------------------------\n");
    free(float_data);
    
    // 测试结构体排序
    size_t num_struct;
    void* struct_data = read_data_from_file(struct_infile, &num_struct, STRUCT_TYPE);
    
    printf("\n=== 结构体排序性能测试 (n=%zu) ===\n", num_struct);
    printf("算法                  运行时间          结果\n");
    printf("----------------------------------------\n");
    
    run_sort_test("冒泡排序(原始)", struct_data, num_struct, sizeof(Student), compare_student, bubble_sort);
    run_sort_test("冒泡排序(优化)", struct_data, num_struct, sizeof(Student), compare_student, bubble_sort_opt);
    run_sort_test("快速排序(原始)", struct_data, num_struct, sizeof(Student), compare_student, quick_sort);
    run_sort_test("快速排序(优化)", struct_data, num_struct, sizeof(Student), compare_student, quick_sort_opt);
    
    printf("----------------------------------------\n");
    free(struct_data);
    
    return 0;
}
