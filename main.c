#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL
#define MAX_TABLE_SIZE 4096
#define DICT_FILE "./dictionary/words.txt"

#if BST == 1
#define OUT_FILE "bst.txt"
entry *hashTable[MAX_TABLE_SIZE] = {NULL};
entry *tableHead[MAX_TABLE_SIZE] = {NULL};
#elif OPT == 1
#define OUT_FILE "opt.txt"
entry *hashTable[MAX_TABLE_SIZE] = {NULL};
entry *tableHead[MAX_TABLE_SIZE] = {NULL};
#else
#define OUT_FILE "orig.txt"
#endif



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

unsigned int BKDRhash(char *cPtr)
{
    unsigned int hash = 0;
    unsigned int seed =31;
    do {
        hash = hash * seed + (*cPtr++);
    } while (*cPtr | 0);
    return (hash & (MAX_TABLE_SIZE -1)); //we learn from class

    /* *cPtr | 0 = *cPtr != '\0' gcc will compile then to the same thing */
}

int main(int argc, char *argv[])
{
    FILE *fp;
    //int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp != NULL) {
        /*
        fopen successful, continue execution.
        */
    } else {
        printf("cannot open the file\n");
        return -1;
    }
    /* build the entry */
    entry *pHead, *e;
#if OPT ==1
    pHead = NULL;
    int counter = 0;
    printf("size of entry : %lu bytes\n", sizeof(entry));
    do {
#if BST ==1
        hashTable[counter] = (entry*) malloc(sizeof(entry));
        tableHead[counter ] = hashTable[counter ];
        *hashTable[(counter++)] -> lastName = 0;
#else
        hashTable[(counter++)] = (entry*) malloc(sizeof(entry));
        tableHead[counter -1 ] = hashTable[counter - 1];
#endif
    } while (counter < MAX_TABLE_SIZE);

#else
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
#if ENT==1 //editable version
    puts("enter the words you want to append");
    fgets(line, sizeof(line), stdin);
#else
    fgets(line, sizeof(line), fp);
#endif
    do {
        line[strlen(line) -1] = '\0'; //change while loop to strlen
#if OPT == 1 //compilering OPT version
        unsigned int hashValue = BKDRhash(line);
#if BST != 1
        hashTable[hashValue] = append(line, hashTable[hashValue]);
#else
        append(line, tableHead[hashValue]);
#endif
#else       //compilering orgin version  
        e = append(line, e);
#endif
    } while (fgets(line, sizeof(line), fp));
    /* close file as soon as possible */
    fclose(fp);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* the givn last name to find */


#if OPT == 1 && ENT!=1//compilering OPT version
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
    e = tableHead[BKDRhash(input)];
#elif ENT == 1 //editable version
    puts("enter the words you want to search");
    fflush(stdin);
    char input[MAX_LAST_NAME_SIZE];
    fgets(input, MAX_LAST_NAME_SIZE, stdin);
    input[strlen(input)-1] = '\0';
    e = tableHead[BKDRhash(input)];
#else
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
    e = pHead;
#endif

    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, input));



#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    //printf("%s",findName(input, e)->lastName );
    findName(input, e);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);
    printf("execution clock of findName() : %ld clock\n",end.tv_nsec - start.tv_nsec );

#if BST == 1 && OPT== 1//compilering OPT version
    counter = 0;
    do {
        free(tableHead[counter] -> pLeft);
        free(tableHead[counter] -> pRight);
        free(tableHead[(counter++)] );

    } while (counter < MAX_TABLE_SIZE);
#elif OPT == 1 && BST != 1
    counter = 0;
    do {
        free(tableHead[counter] -> pNext);
        free(tableHead[(counter++)] );

    } while (counter < MAX_TABLE_SIZE);
#else
    if (pHead != NULL) {
        free(pHead->pNext);
        free(pHead);
    }
#endif

    return 0;
}

