#ifndef MONITOR_H
#define MONITOR_H
#include <pthread.h>
#include "student.h"

int monitor_start(pthread_t* out_tid, Student* arr, int* pn, pthread_mutex_t* lock, const char* dir);
int monitor_stop(pthread_t tid);

#endif
