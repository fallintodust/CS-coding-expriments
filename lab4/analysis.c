#include "student.h"
#include <stdio.h>

typedef struct {
    int a_count;
    int b_count;
    int c_count;
    int d_count;
    int f_count;
} GradeBands;

static GradeBands bands_for_course(const Student* arr, int n, int idx) {
    GradeBands g = {0,0,0,0,0};
    int i;
    for (i = 0; i < n; ++i) {
        double sc = arr[i].scores[idx];
        if (sc >= 90) g.a_count++;
        else if (sc >= 80) g.b_count++;
        else if (sc >= 70) g.c_count++;
        else if (sc >= 60) g.d_count++;
        else g.f_count++;
    }
    return g;
}

void print_course_stats(const Student* arr, int n, int idx, const char* name) {
    GradeBands g = bands_for_course(arr, n, idx);
    printf("Stats for %s (n=%d): A:%d B:%d C:%d D:%d F:%d\n",
           name, n, g.a_count, g.b_count, g.c_count, g.d_count, g.f_count);
}
