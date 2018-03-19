//#include <stdio.h> seens not use?
#include <stdlib.h>
#include <string.h>
//#include <ctype.h> seens not use

#include "phonebook_bst.h"

/* Hash+BST version */
entry *findName(char lastName[], entry *pHead)
{
    /*   this is my old dirty code
    if(pHead != NULL){
    } else
        return NULL;
    do
    {
        if(strcasecmp(pHead->lastName, lastName) > 0 ) {
            pHead = pHead->pRight;
        }
        else if(strcasecmp(pHead->lastName, lastName) < 0) {
            pHead = pHead->pLeft;
        }
        else{
            return pHead;
        }
    } while (pHead != NULL);
    return NULL;*/
    int result = strcasecmp(lastName, pHead->lastName);
    do {
        if (result < 0)
            pHead = pHead -> pLeft;
        else
            pHead = pHead -> pRight;
    } while (pHead != NULL && (result = strcasecmp(lastName, pHead->lastName)) != 0);

    if (pHead !=NULL)
        return (pHead);
    else
        return NULL;
}

void append(char lastName[], entry *e)
{
    /* allocate memory for the new entry and put lastName */
    entry *pre;
    int a = 0 ;
    while(e!=NULL) {
        if(strcasecmp(e->lastName, lastName) < 0 ) {
            pre = e;
            e = e->pRight;
            a = 0;
        } else {
            pre = e;
            e = e->pLeft;
            a = 1;
        }
    }
    if(a == 0 ) {
        pre->pRight = (entry *) malloc(sizeof(entry));
        pre = pre->pRight;
    } else {
        pre->pLeft = (entry *) malloc(sizeof(entry));
        pre = pre->pLeft;
    }
    strcpy(pre->lastName, lastName);
    pre->pLeft = NULL;
    pre->pRight = NULL;
    return 0;

    /*   this is my old dirty code
    while(1)
    {
        if(strcasecmp(e->lastName, lastName) < 0 ){
            if(e->pRight != NULL){
                e = e->pRight;
            }
            else{
                e->pRight = (entry *) malloc(sizeof(entry));
                e = e->pRight;
                strcpy(e->lastName, lastName);
                e->pLeft = NULL;
                e->pRight = NULL;
                return e;
            }
        } else {
            if(e->pLeft != NULL){
                e = e->pLeft;
            } else {
                e->pLeft = (entry *) malloc(sizeof(entry));
                e = e->pLeft;
                strcpy(e->lastName, lastName);
                e->pLeft = NULL;
                e->pRight = NULL;
                return e;
            }
        }
    }*/
}
