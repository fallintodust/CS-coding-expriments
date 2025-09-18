#define _GNU_SOURCE
#include "fileop.h"
#include <errno.h>

int save_text(const char* filename, Student* arr, int n) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "error %d : %s\n", errno, strerror(errno));
        return -1;
    }
    fprintf(f, "%d\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d %s", arr[i].id, arr[i].name);
        for (int j = 0; j < 4; j++) fprintf(f, " %.2f", arr[i].scores[j]);
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}

int load_text(const char* filename, Student* arr, int* pn) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "error %d : %s\n", errno, strerror(errno));
        return -1;
    }
    int n = 0;
    if (fscanf(f, "%d", &n) != 1) {
        fclose(f);
        return -2;
    }
    for (int i = 0; i < n && i < MAX_STUDENTS; i++) {
        if (fscanf(f, "%d %63s %lf %lf %lf %lf", &arr[i].id, arr[i].name,
                   &arr[i].scores[0], &arr[i].scores[1], &arr[i].scores[2], &arr[i].scores[3]) != 6) {
            fclose(f);
            return -3;
        }
    }
    *pn = n;
    fclose(f);
    return 0;
}

int save_bin(const char* filename, Student* arr, int n) {
    FILE* f = fopen(filename, "wb");
    if (!f) { fprintf(stderr, "error %d : %s\n", errno, strerror(errno)); return -1; }
    if (fwrite(&n, sizeof(int), 1, f) != 1) { fclose(f); return -2; }
    if (fwrite(arr, sizeof(Student), n, f) != (size_t)n) { fclose(f); return -3; }
    fclose(f);
    return 0;
}

int load_bin(const char* filename, Student* arr, int* pn) {
    FILE* f = fopen(filename, "rb");
    if (!f) { fprintf(stderr, "error %d : %s\n", errno, strerror(errno)); return -1; }
    int n = 0;
    if (fread(&n, sizeof(int), 1, f) != 1) { fclose(f); return -2; }
    if (n > MAX_STUDENTS) n = MAX_STUDENTS;
    if (fread(arr, sizeof(Student), n, f) != (size_t)n) { fclose(f); return -3; }
    *pn = n;
    fclose(f);
    return 0;
}
