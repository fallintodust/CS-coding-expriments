#include "student.h"
#include <string.h>

int find_by_id(const Student* arr, int n, int id) {
    int i;
    for (i = 0; i < n; ++i) if (arr[i].id == id) return i;
    return -1;
}

int find_by_name(const Student* arr, int n, const char* name) {
    int i;
    for (i = 0; i < n; ++i) if (strcmp(arr[i].name, name) == 0) return i;
    return -1;
}
