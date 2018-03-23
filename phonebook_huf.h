#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 20
#define MAX_CODE_LENTH 9
#define ENCODE_FILE "./dictionary/huffman_code.txt"
#define BIT_OF_LONG 64

/* TODO: After modifying the original version, uncomment the following
 * line to set OPT properly */
#define HUF 1
#define BST 1
#define OPT 1

typedef struct __PHONE_BOOK_DETAIL_ENTRY {
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
    unsigned long int lastName;
    struct __PHONE_BOOK_ENTRY *pLeft;
    struct __PHONE_BOOK_ENTRY *pRight;
    //detail *pDetail;
} entry;

typedef struct __PHONE_BOOK_EXTRA {
    char lastName[MAX_LAST_NAME_SIZE];
    struct __PHONE_BOOK_EXTRA *pNext;
    detail *pDetail;
} extra;

char Letter[512];


entry *findName(unsigned long int lastName, entry *pHead);
int append(unsigned long int lastName, entry *e);
int loadCode();
int appendExtra(char lastName[], extra *e);
unsigned long int encode(char line[]);
#endif