#define _GNU_SOURCE
#include "monitor.h"
#include "fileop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

static volatile int monitor_running = 0;

static void process_file(const char* dir, const char* fname, Student* arr, int* pn, pthread_mutex_t* lock) {
    char path[4096];
    snprintf(path, sizeof(path), "%s/%s", dir, fname);
    int tempn = 0;
    Student tmp[ MAX_STUDENTS ];
    if (load_text(path, tmp, &tempn) != 0) {
        // failed to load; skip
        return;
    }
    pthread_mutex_lock(lock);
    int i;
    for (i = 0; i < tempn && *pn < MAX_STUDENTS; ++i) {
        arr[(*pn)++] = tmp[i];
    }
    pthread_mutex_unlock(lock);

    // rename processed file
    char newpath[4096];
    snprintf(newpath, sizeof(newpath), "%s/%s.imported", dir, fname);
    if (rename(path, newpath) != 0) {
        // silently ignore
    }
}

typedef struct monitor_args { Student* arr; int* pn; pthread_mutex_t* lock; char dir[1024]; } monitor_args;

static void* monitor_thread(void* arg) {
    monitor_args *m = arg;
    monitor_running = 1;
    while (monitor_running) {
        DIR* d = opendir(m->dir);
        if (d) {
            struct dirent* e;
            while ((e = readdir(d)) != NULL) {
                if (e->d_type == DT_REG) {
                    const char* ext = strrchr(e->d_name, '.');
                    if (ext && strcmp(ext, ".txt") == 0) {
                        process_file(m->dir, e->d_name, m->arr, m->pn, m->lock);
                    }
                }
            }
            closedir(d);
        }
        sleep(1);
    }
    free(m);
    return NULL;
}

int monitor_start(pthread_t* out_tid, Student* arr, int* pn, pthread_mutex_t* lock, const char* dir) {
    struct monitor_args* m = calloc(1, sizeof(*m));
    if (!m) return -1;
    m->arr = arr; m->pn = pn; m->lock = lock; strncpy(m->dir, dir, sizeof(m->dir)-1);
    pthread_t tid;
    if (pthread_create(&tid, NULL, monitor_thread, m) != 0) { free(m); return -1; }
    if (out_tid) *out_tid = tid;
    return 0;
}

int monitor_stop(pthread_t tid) {
    monitor_running = 0;
    // no join here; caller can join
    return 0;
}
