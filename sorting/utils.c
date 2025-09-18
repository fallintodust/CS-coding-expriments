#include "sort.h"

int compare_int(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int compare_float(const void* a, const void* b) {
    float diff = (*(float*)a - *(float*)b);
    return (diff > 0) - (diff < 0);
}

int compare_student(const void* a, const void* b) {
    return ((Student*)a)->score - ((Student*)b)->score;
}

void print_array(void* array, size_t num, DataType type) {
    switch (type) {
        case INTEGER_TYPE:
            for (size_t i = 0; i < num; i++) {
                printf("%d ", ((int*)array)[i]);
            }
            break;
        
        case FLOAT_TYPE:
            for (size_t i = 0; i < num; i++) {
                printf("%.2f ", ((float*)array)[i]);
            }
            break;
        
        case STRUCT_TYPE:
            for (size_t i = 0; i < num; i++) {
                Student* s = &((Student*)array)[i];
                printf("(ID:%d, Name:%s, Score:%.2f) ", s->id, s->name, s->score);
            }
            break;
    }
    printf("\n");
}

void print_array_to_file(FILE* fp, void* array, size_t num, DataType type) {
    switch (type) {
        case INTEGER_TYPE:
            for (size_t i = 0; i < num; i++) {
                fprintf(fp, "%d ", ((int*)array)[i]);
            }
            break;
        
        case FLOAT_TYPE:
            for (size_t i = 0; i < num; i++) {
                fprintf(fp, "%.2f ", ((float*)array)[i]);
            }
            break;
        
        case STRUCT_TYPE:
            for (size_t i = 0; i < num; i++) {
                Student* s = &((Student*)array)[i];
                fprintf(fp, "(ID:%d, Name:%s, Score:%.2f) ", s->id, s->name, s->score);
            }
            break;
    }
    fprintf(fp, "\n");
}

void verify_sort(void* array, size_t num, size_t size, CompareFunc compare) {
    char* ptr = (char*)array;
    for (size_t i = 0; i < num - 1; i++) {
        if (compare(ptr + i * size, ptr + (i + 1) * size) > 0) {
            printf("排序错误\n");
            return;
        }
    }
    printf("排序正确\n");
}

void* generate_data_for_file(size_t num, DataType type) {
    void* data = NULL;
    
    switch (type) {
        case INTEGER_TYPE:
            data = malloc(num * sizeof(int));
            if (!data) HANDLE_ERROR("内存分配失败");
            for (size_t i = 0; i < num; i++) {
                ((int*)data)[i] = rand() % 1000;
            }
            break;
            
        case FLOAT_TYPE:
            data = malloc(num * sizeof(float));
            if (!data) HANDLE_ERROR("内存分配失败");
            for (size_t i = 0; i < num; i++) {
                ((float*)data)[i] = (float)rand() / RAND_MAX * 100.0;
            }
            break;
            
        case STRUCT_TYPE:
            data = malloc(num * sizeof(Student));
            if (!data) HANDLE_ERROR("内存分配失败");
            for (size_t i = 0; i < num; i++) {
                Student* s = &((Student*)data)[i];
                s->id = i + 1;
                sprintf(s->name, "学生%zu", i + 1);
                s->score = (float)rand() / RAND_MAX * 100.0;
            }
            break;
    }
    
    return data;
}

void generate_test_file(const char* filename, size_t num, DataType type) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        HANDLE_ERROR("打开文件失败");
    }

    void* data = generate_data_for_file(num, type);

    // 写入数据数量
    fprintf(fp, "%zu\n", num);

    // 写入数据
    switch (type) {
        case INTEGER_TYPE:
            for (size_t i = 0; i < num; i++) {
                fprintf(fp, "%d\n", ((int*)data)[i]);
            }
            break;
            
        case FLOAT_TYPE:
            for (size_t i = 0; i < num; i++) {
                fprintf(fp, "%.2f\n", ((float*)data)[i]);
            }
            break;
            
        case STRUCT_TYPE:
            for (size_t i = 0; i < num; i++) {
                Student* s = &((Student*)data)[i];
                fprintf(fp, "%d %s %.2f\n", s->id, s->name, s->score);
            }
            break;
    }

    free(data);
    fclose(fp);
}

void generate_test_files(const char* prefix, size_t num) {
    char filename[256];
    
    sprintf(filename, "%s_int.in", prefix);
    generate_test_file(filename, num, INTEGER_TYPE);
    
    sprintf(filename, "%s_float.in", prefix);
    generate_test_file(filename, num, FLOAT_TYPE);
    
    sprintf(filename, "%s_struct.in", prefix);
    generate_test_file(filename, num, STRUCT_TYPE);
    
    printf("测试文件生成完成！\n");
    printf("整数测试文件：%s_int.in\n", prefix);
    printf("浮点数测试文件：%s_float.in\n", prefix);
    printf("结构体测试文件：%s_struct.in\n", prefix);
}
