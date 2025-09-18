#include "student.h"
#include <stdio.h>

void print_student_header(void) {
    printf("ID   Name                         iCS    PDP    DS    DL\n");
    printf("--------------------------------------------------------\n");
}

void print_student(const Student* s) {
    printf("%-4d %-25s %6.2f %6.2f %6.2f %6.2f\n", s->id, s->name,
           s->scores[0], s->scores[1], s->scores[2], s->scores[3]);
}
