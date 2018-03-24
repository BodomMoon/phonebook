#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define DICT_FILE "lastname3.txt"
#define OUT_FILE "lastName_DEC.txt"

#define MAX_TABLE_SIZE 4096
#define MAX_LAST_NAME_SIZE 20
#define SET_LAST_NAME_SIZE 8
#define BIT_OF_LONG 64


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
    int codeLen[27]=    {3,6, 5,6,4, 7,6,5,4, 8,5, 4, 5,4, 4, 6,  9,4, 4,5, 5, 6,7,  9, 6,7,3};
    long int code[27]  ={7,3,12,0,1,32,2,9,3,66,5,13,23,7,10,17,134,5,12,4,13,45,3,135,44,2,4};
//                       a b  c d e  f g h i  j k  l  m n  o  p   q r  s t  u  v w   x  y z a
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

    FILE *output = fopen(OUT_FILE, "a");
    while(fgets(line, sizeof(line), fp)!=NULL){
        int move = BIT_OF_LONG;
        unsigned long int out = 0;
        int flag = 0;
        for(int i = 0 ; i < strlen(line)-1; i++){
            int test = 0;
            test = line[i]-'a';
            move -= codeLen[test];
            if(move >= 3){
                out |= (code[test] << move);
                //printf("move = %d char = %c code = %ld out = %ld \n", move ,line[i],(code[test] << move),out);
            }
            else{
                flag = 1;
                line[strlen(line)-1] = '\0';
                //puts("boom");
                break;
            }
        }

        if(flag == 0){
            move -= codeLen[26];
            out |= (code[26] << move);
            fprintf(output,"%lu\n",out);
        }else{
            fprintf(output, "%s\n", line);
        }
    }

    fclose(output);
}

