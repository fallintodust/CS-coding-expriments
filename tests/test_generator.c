#include "sort.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("用法: %s <输入文件前缀> <数据规模>\n", argv[0]);
        printf("将生成 <前缀>_int.in, <前缀>_float.in, <前缀>_struct.in 三个测试文件\n");
        return 1;
    }

    const char* prefix = argv[1];
    size_t num = atoi(argv[2]);
    
    if (num <= 0) {
        printf("错误：数据规模必须大于0\n");
        return 1;
    }

    srand(time(NULL));
    generate_test_files(prefix, num);

    return 0;
}
