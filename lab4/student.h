#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 45
#define NAME_LEN 64
#define COURSE_COUNT 4

typedef struct {
    int id;
    char name[NAME_LEN];
    double scores[COURSE_COUNT]; /* iCS, PDP, DS, DL */
} Student;

/* helper to print header */
void print_student_header(void);
/* print one student */
void print_student(const Student* s);

#endif
