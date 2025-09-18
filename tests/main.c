#include "sort.h"

typedef struct {
    const char* name;
    SortFunction func;
} SortAlgorithm;

SortAlgorithm sort_algorithms[] = {
    {"冒泡排序", bubble_sort},
    {"选择排序", selection_sort},
    {"快速排序", quick_sort},
    {"归并排序", merge_sort}
};

const int num_algorithms = sizeof(sort_algorithms) / sizeof(sort_algorithms[0]);

// 从文件读取数据
void* read_data_from_file(const char* filename, size_t* num, DataType type) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        HANDLE_ERROR("打开输入文件失败");
    }

    // 读取数据数量
    if (fscanf(fp, "%zu", num) != 1) {
        HANDLE_ERROR("读取数据数量失败");
    }

    void* data = NULL;
    switch (type) {
        case INTEGER_TYPE:
            data = malloc((*num) * sizeof(int));
            if (!data) HANDLE_ERROR("内存分配失败");
            for (size_t i = 0; i < *num; i++) {
                if (fscanf(fp, "%d", &((int*)data)[i]) != 1) {
                    HANDLE_ERROR("读取整数数据失败");
                }
            }
            break;

        case FLOAT_TYPE:
            data = malloc((*num) * sizeof(float));
            if (!data) HANDLE_ERROR("内存分配失败");
            for (size_t i = 0; i < *num; i++) {
                if (fscanf(fp, "%f", &((float*)data)[i]) != 1) {
                    HANDLE_ERROR("读取浮点数据失败");
                }
            }
            break;

        case STRUCT_TYPE:
            data = malloc((*num) * sizeof(Student));
            if (!data) HANDLE_ERROR("内存分配失败");
            for (size_t i = 0; i < *num; i++) {
                Student* s = &((Student*)data)[i];
                if (fscanf(fp, "%d %s %f", &s->id, s->name, &s->score) != 3) {
                    HANDLE_ERROR("读取结构体数据失败");
                }
            }
            break;
    }

    fclose(fp);
    return data;
}

// 将结果写入文件
void write_results_to_file(const char* filename, void* data, size_t num, size_t size, 
                          DataType type, CompareFunc compare, const char* test_name) {
    FILE* fp = fopen(filename, "a");
    if (!fp) {
        HANDLE_ERROR("打开输出文件失败");
    }

    fprintf(fp, "\n=== %s ===\n", test_name);
    fprintf(fp, "数据规模: %zu\n", num);
    fprintf(fp, "------------------------------------------------\n");
    fprintf(fp, "%-12s%-15s%-15s\n", "算法", "用时(秒)", "验证结果");
    fprintf(fp, "------------------------------------------------\n");

    void* test_array = malloc(num * size);
    if (!test_array) {
        HANDLE_ERROR("内存分配失败");
    }

    // 写入原始数据
    fprintf(fp, "\n原始数据:\n");
    print_array_to_file(fp, data, num, type);

    // 测试每种排序算法
    for (int i = 0; i < num_algorithms; i++) {
        memcpy(test_array, data, num * size);
        
        clock_t start = clock();
        sort_algorithms[i].func(test_array, num, size, compare);
        clock_t end = clock();
        
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        
        fprintf(fp, "%-12s%-15.6f", sort_algorithms[i].name, time_spent);
        
        // 验证排序结果并写入文件
        int is_sorted = 1;
        char* ptr = (char*)test_array;
        for (size_t j = 0; j < num - 1; j++) {
            if (compare(ptr + j * size, ptr + (j + 1) * size) > 0) {
                is_sorted = 0;
                break;
            }
        }
        fprintf(fp, "%s\n", is_sorted ? "排序正确" : "排序错误");

        // 写入排序后的数据
        fprintf(fp, "\n%s排序后结果:\n", sort_algorithms[i].name);
        print_array_to_file(fp, test_array, num, type);
        fprintf(fp, "\n");
    }

    fprintf(fp, "------------------------------------------------\n\n");
    free(test_array);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("用法: %s <输入文件前缀>\n", argv[0]);
        printf("程序将读取 <前缀>_int.in, <前缀>_float.in, <前缀>_struct.in\n");
        printf("结果将输出到 <前缀>_result.out\n");
        return 1;
    }

    char int_infile[256], float_infile[256], struct_infile[256], outfile[256];
    sprintf(int_infile, "%s_int.in", argv[1]);
    sprintf(float_infile, "%s_float.in", argv[1]);
    sprintf(struct_infile, "%s_struct.in", argv[1]);
    sprintf(outfile, "%s_result.out", argv[1]);

    // 删除已存在的输出文件
    remove(outfile);

    // 测试整数排序
    size_t num_int;
    void* int_data = read_data_from_file(int_infile, &num_int, INTEGER_TYPE);
    write_results_to_file(outfile, int_data, num_int, sizeof(int), INTEGER_TYPE, 
                         compare_int, "整数排序测试");
    free(int_data);

    // 测试浮点数排序
    size_t num_float;
    void* float_data = read_data_from_file(float_infile, &num_float, FLOAT_TYPE);
    write_results_to_file(outfile, float_data, num_float, sizeof(float), FLOAT_TYPE,
                         compare_float, "浮点数排序测试");
    free(float_data);

    // 测试结构体排序
    size_t num_struct;
    void* struct_data = read_data_from_file(struct_infile, &num_struct, STRUCT_TYPE);
    write_results_to_file(outfile, struct_data, num_struct, sizeof(Student), STRUCT_TYPE,
                         compare_student, "结构体排序测试（按成绩排序）");
    free(struct_data);

    printf("排序完成！结果已写入 %s\n", outfile);
    return 0;
}
