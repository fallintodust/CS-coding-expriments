#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sort.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE 5000
#endif
#ifndef TEST_ROUNDS
#define TEST_ROUNDS 10
#endif

static void generate_random_array(int* arr, int size) {
    for (int i = 0; i < size; i++) arr[i] = rand() % 100000;
}

static int is_sorted_int(int* arr, int n) {
    for (int i = 1; i < n; i++) if (arr[i] < arr[i-1]) return 0;
    return 1;
}

static double bench_sort(void (*sort_func)(void*, size_t, size_t, CompareFunc), const char* name) {
    int* data = malloc(ARRAY_SIZE * sizeof(int));
    int* tmp = malloc(ARRAY_SIZE * sizeof(int));
    if (!data || !tmp) { fprintf(stderr, "alloc fail\n"); exit(1); }

    double total = 0.0;
    for (int r = 0; r < TEST_ROUNDS; r++) {
        generate_random_array(data, ARRAY_SIZE);
        memcpy(tmp, data, ARRAY_SIZE * sizeof(int));
        clock_t s = clock();
        sort_func(tmp, ARRAY_SIZE, sizeof(int), compare_int);
        clock_t e = clock();
        total += (double)(e - s) * 1000.0 / CLOCKS_PER_SEC;
        if (!is_sorted_int(tmp, ARRAY_SIZE)) {
            fprintf(stderr, "%s produced wrong result on round %d\n", name, r);
            break;
        }
    }
    free(data); free(tmp);
    return total / TEST_ROUNDS;
}

int main() {
    srand(time(NULL));
    printf("Running simple sorts benchmark, ARRAY_SIZE=%d, rounds=%d\n", ARRAY_SIZE, TEST_ROUNDS);

    double t1 = bench_sort(bubble_sort, "bubble_sort");
    printf("bubble_sort,avg_ms,%.6f\n", t1);

    double t2 = bench_sort(selection_sort, "selection_sort");
    printf("selection_sort,avg_ms,%.6f\n", t2);

    double t3 = bench_sort(insertion_sort, "insertion_sort");
    printf("insertion_sort,avg_ms,%.6f\n", t3);

    FILE* f = fopen("bench_bsi.csv", "w");
    if (f) {
        fprintf(f, "algorithm,avg_ms\n");
        fprintf(f, "bubble_sort,%.6f\n", t1);
        fprintf(f, "selection_sort,%.6f\n", t2);
        fprintf(f, "insertion_sort,%.6f\n", t3);
        fclose(f);
    }
    return 0;
}
