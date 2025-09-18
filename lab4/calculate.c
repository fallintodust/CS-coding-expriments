#include "student.h"
#include <stddef.h>

double student_total(const Student* s) {
    double t = 0.0; size_t i;
    for (i = 0; i < 4; ++i) t += s->scores[i];
    return t;
}

double student_average(const Student* s) {
    return student_total(s) / 4.0;
}

double course_average(const Student* arr, int n, int course_idx) {
    if (n <= 0 || course_idx < 0 || course_idx >= 4) return 0.0;
    double sum = 0.0; int i;
    for (i = 0; i < n; ++i) sum += arr[i].scores[course_idx];
    return sum / n;
}
