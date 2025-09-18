#ifndef FILEOP_H
#define FILEOP_H

#include "student.h"

int save_text(const char* filename, Student* arr, int n);
int load_text(const char* filename, Student* arr, int* pn);
int save_bin(const char* filename, Student* arr, int n);
int load_bin(const char* filename, Student* arr, int* pn);

#endif
