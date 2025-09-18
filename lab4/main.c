#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "fileop.h"
#include "sort_wrapper.h"
#include "calculate.c"
#include "print.c"
#include "analysis.c"
#include "search.c"
#include <pthread.h>
#include "monitor.h"
#include <sys/stat.h>
#include <dirent.h>

/* Note: For simplicity this main is single-file includes of helpers.
   It implements a menu with operations requested (append/search/modify/delete/calc/sort/stats/save/load/exit)
*/

int main(void) {
    Student arr[MAX_STUDENTS];
    int n = 0;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_t mon_tid = 0;
    const char* imports_dir = "imports";
    mkdir(imports_dir, 0755);
    monitor_start(&mon_tid, arr, &n, &lock, imports_dir);
    // perform an initial scan of imports dir so records present before menu show up immediately
    {
        DIR* d = opendir(imports_dir);
        if (d) {
            struct dirent* e;
            while ((e = readdir(d)) != NULL) {
                const char* name = e->d_name;
                const char* ext = strrchr(name, '.');
                if (!ext) continue;
                if (strcmp(ext, ".txt") != 0) continue;
                char path[4096];
                snprintf(path, sizeof(path), "%s/%s", imports_dir, name);
                int tempn = 0;
                Student tmp[MAX_STUDENTS];
                if (load_text(path, tmp, &tempn) == 0) {
                    pthread_mutex_lock(&lock);
                    for (int i = 0; i < tempn && n < MAX_STUDENTS; ++i) arr[n++] = tmp[i];
                    pthread_mutex_unlock(&lock);
                    char newpath[4096];
                    snprintf(newpath, sizeof(newpath), "%s/%s.imported", imports_dir, name);
                    rename(path, newpath);
                }
            }
            closedir(d);
        }
    }
    char line[256];
    int choice = -1;

    while (1) {
        printf("\nStudent Management Menu:\n");
        printf("1.Append  2.Search 3.Modify 4.Delete 5.Calculate totals/avg 6.Sort by course\n");
        printf("7.Show all 8.Stats per course 9.Save text 10.Load text 11.Save bin 12.Load bin 13.Clear all\n");
        printf("0.Exit\n");
        printf("Choice: ");
        if (!fgets(line, sizeof(line), stdin)) break;
        choice = atoi(line);
        if (choice == 0) break;
            if (choice == 1) {
            pthread_mutex_lock(&lock);
            if (n >= MAX_STUDENTS) { pthread_mutex_unlock(&lock); printf("Full\n"); continue; }
            Student s; memset(&s,0,sizeof(s));
            pthread_mutex_unlock(&lock);
            printf("Enter id: "); fgets(line,sizeof(line),stdin); s.id = atoi(line);
            printf("Enter name: "); fgets(line,sizeof(line),stdin); line[strcspn(line, "\n")] = '\0'; strncpy(s.name,line, sizeof(s.name)-1);
            int i; for (i=0;i<4;++i) { printf("Score %d: ", i+1); fgets(line,sizeof(line),stdin); s.scores[i] = atof(line);} 
            pthread_mutex_lock(&lock);
            arr[n++] = s; pthread_mutex_unlock(&lock);
            printf("Appended.\n");
        } else if (choice == 2) {
            printf("Search by id: "); fgets(line,sizeof(line),stdin);
            int id = atoi(line);
            pthread_mutex_lock(&lock);
            int idx = find_by_id(arr,n,id);
            pthread_mutex_unlock(&lock);
            if (idx<0) printf("Not found\n"); else { print_student_header(); print_student(&arr[idx]); }
        } else if (choice == 3) {
            printf("Modify by id: "); fgets(line,sizeof(line),stdin);
            int id = atoi(line);
            pthread_mutex_lock(&lock);
            int idx = find_by_id(arr,n,id);
            if (idx<0) { pthread_mutex_unlock(&lock); printf("Not found\n"); continue; }
            pthread_mutex_unlock(&lock);
            printf("Enter new name (blank to keep): "); fgets(line,sizeof(line),stdin);
            if (line[0] != '\n') { line[strcspn(line, "\n")] = '\0'; pthread_mutex_lock(&lock); strncpy(arr[idx].name,line,sizeof(arr[idx].name)-1); pthread_mutex_unlock(&lock); }
            int i; for (i=0;i<4;++i) { printf("Score %d (blank keep): ", i+1); fgets(line,sizeof(line),stdin); if (line[0] != '\n') { pthread_mutex_lock(&lock); arr[idx].scores[i] = atof(line); pthread_mutex_unlock(&lock); } }
            printf("Modified.\n");
        } else if (choice == 4) {
            printf("Delete by id: "); fgets(line,sizeof(line),stdin);
            int id = atoi(line);
            pthread_mutex_lock(&lock);
            int idx = find_by_id(arr,n,id);
            if (idx<0) { pthread_mutex_unlock(&lock); printf("Not found\n"); continue; }
            memmove(&arr[idx], &arr[idx+1], sizeof(Student)*(n-idx-1)); n--; pthread_mutex_unlock(&lock); printf("Deleted.\n");
        } else if (choice == 5) {
            int i;
            pthread_mutex_lock(&lock);
            print_student_header(); for (i=0;i<n;++i) { print_student(&arr[i]); printf("  Total: %.2f Avg: %.2f\n", student_total(&arr[i]), student_average(&arr[i])); }
            pthread_mutex_unlock(&lock);
        } else if (choice == 6) {
            printf("Sort by course index 0-3: "); fgets(line,sizeof(line),stdin); int idx = atoi(line); if (idx<0||idx>3) { printf("Bad index\n"); continue; }
            pthread_mutex_lock(&lock);
            sort_by_course(arr,n,idx); pthread_mutex_unlock(&lock);
            printf("Sorted.\n");
        } else if (choice == 7) {
            print_student_header(); int i; for (i=0;i<n;++i) print_student(&arr[i]);
        } else if (choice == 8) {
            const char* names[4] = {"iCS","PDP","DS","DL"}; int i;
            pthread_mutex_lock(&lock);
            for (i=0;i<4;++i) print_course_stats(arr,n,i,names[i]);
            pthread_mutex_unlock(&lock);
        } else if (choice == 9) {
            printf("Save text filename: "); fgets(line,sizeof(line),stdin); line[strcspn(line, "\n")] = '\0'; pthread_mutex_lock(&lock); if (save_text(line, arr, n)==0) printf("Saved\n"); pthread_mutex_unlock(&lock);
        } else if (choice == 10) {
            printf("Load text filename: "); fgets(line,sizeof(line),stdin); line[strcspn(line, "\n")] = '\0'; int newn=0; if (load_text(line, arr, &newn)==0) { pthread_mutex_lock(&lock); n=newn; pthread_mutex_unlock(&lock); printf("Loaded %d\n", n);} 
        } else if (choice == 11) {
            printf("Save bin filename: "); fgets(line,sizeof(line),stdin); line[strcspn(line, "\n")] = '\0'; pthread_mutex_lock(&lock); if (save_bin(line, arr, n)==0) printf("Saved bin\n"); pthread_mutex_unlock(&lock);
        } else if (choice == 12) {
            printf("Load bin filename: "); fgets(line,sizeof(line),stdin); line[strcspn(line, "\n")] = '\0'; int newn=0; if (load_bin(line, arr, &newn)==0) { pthread_mutex_lock(&lock); n=newn; pthread_mutex_unlock(&lock); printf("Loaded bin %d\n", n);} 
        } else if (choice == 13) { n = 0; printf("Cleared.\n"); }
        else { printf("Unknown choice\n"); }
    }

    // stop monitor
    monitor_stop(mon_tid);
    if (mon_tid) pthread_join(mon_tid, NULL);
    pthread_mutex_destroy(&lock);
    printf("Exiting.\n");
    return 0;
}
