//#include <stdio.h> seens not use?
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#include <ctype.h> seens not use

#include "phonebook_huf.h"

int loadCode(){
    //TODO fix this
    FILE *encode = fopen(ENCODE_FILE, "r");
    char input[MAX_CODE_LENTH + 1];
    int codeNumber = 0;
    fscanf(encode,"%d",&codeNumber);
    int counter = 0 ,place = 0;
    do{
        fscanf(encode,"%c",&Letter[place]);
        fscanf(encode,"%s",input);
        int size = strlen(input);
        do{
            place = place * 2 + (input[counter] -'0');
        }while(counter - size - 1);
        
    }while(--codeNumber);
    fclose(encode);
    return 0;
    
}

/* Hash+BST+Huffman version */
entry *findName(unsigned long int lastName, entry *pHead){
    //int result = strcasecmp(lastName, pHead->lastName);

    do {
        if (lastName < pHead->lastName)
        {
            //printf("small\n");
            pHead = pHead -> pLeft;
        }

        else{
            //printf("big\n");
            pHead = pHead -> pRight;
        }
    } while ( (pHead) && (lastName != pHead->lastName));

    if (pHead)
        return (pHead);
    else
        return NULL;
}

int append(unsigned long int lastName, entry *e)
{
    /* allocate memory for the new entry and put lastName */
    entry *pre;
    int a;
    do {
        pre = e;
        //a = (int)((e->lastName-lastName)>>63)+1; // if -> a = 0 else a = 1        
        //printf("a = %d\n", a);
        if(e->lastName < lastName) {
            e = e->pRight;
            a=1;
                
        } else {
            e = e->pLeft;        
            a=0;
        }
    } while(e);
    if(!a) {
        pre->pLeft = (entry *) malloc(sizeof(entry));
        pre = pre->pLeft;
        //printf("small\n");
    } else {
        pre->pRight = (entry *) malloc(sizeof(entry));
        pre = pre->pRight;
        //printf("big\n");
    }
    pre->lastName =  lastName;
    pre->pLeft = NULL;
    pre->pRight = NULL;
    return 1;
}

int appendExtra(char lastName[], extra *e)
{
    /* allocate memory for the new entry and put lastName */
    extra *Ne = (extra *) malloc(sizeof(extra));
    strcpy(Ne->lastName, lastName);
    Ne->pNext = e->pNext;
    e->pNext = Ne;
    return 1;
}

unsigned long int encode(char line[])
{
    int codeLen[27]=    {3,6, 5,6,4, 7,6,5,4, 8,5, 4, 5,4, 4, 6,  9,4, 4,5, 5, 6,7,  9, 6,7,3};
    long int code[27]  ={7,3,12,0,1,32,2,9,3,66,5,13,23,7,10,17,134,5,12,4,13,45,3,135,44,2,4};
//                       a b  c d e  f g h i  j k  l  m n  o  p   q r  s t  u  v w   x  y z a
//This is a tricky way to encode,Really not a good choice

    int move = BIT_OF_LONG;
        unsigned long int out = 0;
        //int flag = 0;
        for(int i = 0 ; i < strlen(line); i++){
            int test = 0;
            test = line[i]-'a';
            move -= codeLen[test];
            if(move >= 3){
                out |= (code[test] << move);
                //printf("move = %d char = %c code = %ld out = %ld \n", move ,line[i],(code[test] << move),out);
            }
            else{
                exit(1);
            }
        }
        move -= codeLen[26];
        out |= (code[26] << move);

        return out;
}