#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#include <pthread.h>
#include <time.h>

#define MAX_LAST_NAME_SIZE 16
#define THREAD_NUM 4
#define OPT 1

typedef struct _detail {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
} detail;

typedef struct __PHONE_BOOK_ENTRY {
    char *lastName;
    struct __PHONE_BOOK_ENTRY *pNext;
    detail *dtl;
} entry;

typedef struct _append_a {
    char *fBegin, *fEnd;
    int tid, nthread;
    entry *entryStart, *pHead, *pLast;
} append_a;


entry *findName(char lastname[], entry *pHead);
append_a *new_append_a(char *begin, char *end, int tid, int ntd, entry *start);
void append(void *arg);
void show_entry(entry *pHead);
static double diff_in_second(struct timespec t1, struct timespec t2);

#endif
