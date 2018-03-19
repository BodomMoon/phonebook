#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define DICT_FILE "lastname.txt"

#define MAX_TABLE_SIZE 4096
#define MAX_LAST_NAME_SIZE 20
#define SET_LAST_NAME_SIZE 8


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
    char lastName[SET_LAST_NAME_SIZE];
    struct __PHONE_BOOK_ENTRY *pNext;
    detail *pDetail;
} entry;


int main()
{
    FILE *fp;
    char line[MAX_LAST_NAME_SIZE];
    int max = 0;
    int codeLen[27]={3,6, 5,6,4, 7,6,5,4, 8,5, 4, 5,4, 4, 6,  9,4, 4,5, 5, 6,7,  9, 6,7,3};
    int code[27]   ={7,3,12,0,1,32,2,9,3,66,5,13,23,7,10,17,134,5,12,4,13,45,3,135,44,2,4};

    fp = fopen(DICT_FILE, "r");
    if (fp != NULL) {
        /*
        fopen successful, continue execution.
        */
    } else {
        printf("cannot open the file\n");
        return -1;
    }
    int avg=0;
    int boom = 0;

    while(fgets(line, sizeof(line), fp)!=NULL){
        int all = 0;
        for(int i = 0; i < strlen(line); i++){
            int test = 0;
            test = line[i]-'a';
            all+=codeLen[test];
        
        }
        all+=3;
        /*if(all<10)
        {
            line[strlen(line)-1] = '\0';
            puts(line);
        }*/
        avg+=all;
        if(all>64)
        {
            boom++;
        }
        if(all > max)
        {
            printf("all = %d\n",all);
            max = all;
        }
    }
    printf("max = %d avg = %d boom = %d \n", max,avg/896410,boom);
    printf("%ld\n", sizeof(entry));
}
