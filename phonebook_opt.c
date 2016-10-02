#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "phonebook_opt.h"
#include "debug.h"

entry *findName(char lastName[], entry *pHead)
{
    size_t len = strlen( lastName);
    entry *current = pHead;
    while (current != NULL) {
        if (strncasecmp(lastName, current->lastName, len) == 0
                && (current->lastName[len] == '\n' ||
                    current->lastName[len] == '\0')) {
            current->lastName = (char *) malloc( sizeof(char) * MAX_LAST_NAME_SIZE);
            strcpy(current->lastName, lastName);
            current->dtl = (detail *) malloc( sizeof(detail));
            return current;
        }
        dprintf("find string = %s\n", current->lastName);
        current = current->pNext;
    }
    return NULL;
}

append_a *new_append_a(char *begin, char *end, int tid, int ntd, entry *start)
{
    append_a *app = (append_a *) malloc(sizeof(append_a));
    app->fBegin = begin;
    app->fEnd = end;
    app->tid = tid;
    app->nthread = ntd;
    app->pHead = app->pLast = NULL;
    app->entryStart = start;

    return app;
}

void append(void *arg)
{
    struct timespec start, end;
    double cpu_time;
    clock_gettime( CLOCK_REALTIME, &start);

    append_a *app = (append_a *) arg;
    entry *current = app->pLast = app->entryStart;
    for (char *loc = app->fBegin; loc < app->fEnd;
            loc += MAX_LAST_NAME_SIZE * app->nthread,
            current += app->nthread) {
        current->lastName = loc;
        current->pNext = app->pHead;
        app->pHead = current;
    }

    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_second(start, end);
    dprintf("thread take %lf sec, count %d\n", cpu_time);
    pthread_exit(NULL);
}

void show_entry(entry *pHead)
{
    while (pHead != NULL) {
        printf("lastName = %s\n", pHead->lastName);
        pHead = pHead->pNext;
    }
}

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}
