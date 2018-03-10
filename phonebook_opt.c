//#include <stdio.h> seens not use?
#include <stdlib.h>
#include <string.h>
//#include <ctype.h> seens not use

#include "phonebook_opt.h"

/* original version */
entry *findName(char lastName[], entry *pHead)
{
    do {
        if (strcasecmp(lastName, pHead->lastName) != 0)
            pHead = pHead->pNext;
        else
            return pHead;
    } while (pHead != NULL);
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    /* allocate memory for the new entry and put lastName */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}
