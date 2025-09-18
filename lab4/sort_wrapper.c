#include "sort_wrapper.h"
#include <stdlib.h>

static int cmp_course_desc(const void* a, const void* b, void* arg) {
    int idx = *(int*)arg;
    const Student* A = a;
    const Student* B = b;
    if (A->scores[idx] < B->scores[idx]) return 1;
    if (A->scores[idx] > B->scores[idx]) return -1;
    return 0;
}

void sort_by_course(Student* arr, int n, int course_index) {
    if (n <= 1) return;
#if defined(_GNU_SOURCE)
    qsort_r(arr, n, sizeof(Student), cmp_course_desc, &course_index);
#else
    /* fallback: wrap index in static var (not thread-safe) */
    static int idx;
    idx = 0; /* set below */
    idx = course_index;
    int cmp_wrapper(const void* a, const void* b) {
        const Student* A = a; const Student* B = b;
        if (A->scores[idx] < B->scores[idx]) return 1;
        if (A->scores[idx] > B->scores[idx]) return -1;
        return 0;
    }
    qsort(arr, n, sizeof(Student), cmp_wrapper);
#endif
}
