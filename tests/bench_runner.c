#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../sorting/sort.h"

double now_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec*1000.0 + t.tv_nsec/1e6;
}

void run_once(SortFunction f, void* data, size_t n, size_t size, CompareFunc cmp) {
    f(data, n, size, cmp);
}

int main(int argc, char** argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s N\n", argv[0]); return 1; }
    size_t N = atoi(argv[1]);
    const int repeats = 5;
    const char* names[] = {"bubble","selection","insertion","merge","quick","heap"};
    SortFunction funcs[] = {bubble_sort, selection_sort, insertion_sort, merge_sort, quick_sort, heap_sort};
    int algos = sizeof(funcs)/sizeof(funcs[0]);

    int* original = malloc(N * sizeof(int));
    int* work = malloc(N * sizeof(int));
    if (!original || !work) { perror("malloc"); return 1; }
    unsigned seed = 12345;
    srand(seed);
    for (size_t i = 0; i < N; ++i) original[i] = rand();

    // 保存生成的随机数到文件
    char dataname[256]; snprintf(dataname, sizeof(dataname), "tests/random_data_n%zu.txt", N);
    FILE* datafile = fopen(dataname, "w");
    if (datafile) {
        fprintf(datafile, "# Generated random integers (seed=%u, N=%zu)\n", seed, N);
        for (size_t i = 0; i < N; ++i) {
            fprintf(datafile, "%d\n", original[i]);
        }
        fclose(datafile);
        printf("Random data saved to %s\n", dataname);
    }

    char outname[256]; snprintf(outname, sizeof(outname), "tests/results_n%zu.csv", N);
    FILE* out = fopen(outname, "w");
    if (!out) { perror("fopen"); return 1; }
    fprintf(out, "algo,avg_ms\n");

    for (int a = 0; a < algos; ++a) {
        double sum = 0.0;
        for (int r = 0; r < repeats; ++r) {
            memcpy(work, original, N * sizeof(int));
            double t0 = now_ms();
            funcs[a](work, N, sizeof(int), compare_int);
            double t1 = now_ms();
            sum += (t1 - t0);
        }
        double avg = sum / repeats;
        fprintf(out, "%s,%.6f\n", names[a], avg);
        printf("%s avg=%.6f ms\n", names[a], avg);
    }
    fclose(out);
    free(original); free(work);
    return 0;
}
