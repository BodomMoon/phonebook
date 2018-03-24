#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

#include IMPL
#define MAX_TABLE_SIZE 4096

#if HUF==1
#define DICT_FILE "./dictionary/lastName_DEC.txt"
#else
#define DICT_FILE "./dictionary/lastname3.txt"
#endif

#if BST == 1
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#if HUF!=1
#define OUT_FILE "bst.txt"
#else
#define OUT_FILE "huf.txt"
#endif
//entry *hashTable[MAX_TABLE_SIZE] = {NULL};
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
#if HUF ==1
unsigned int hash(unsigned long int x) 
{ 
    x = ((x >> 32) ^ x) * 0x45d9f3b; 
    x = ((x >> 32) ^ x) * 0x45d9f3b; 
    x = ((x >> 32) ^ x); 
    return (unsigned int) (x & (MAX_TABLE_SIZE -1)); 
}
#else
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
#endif


int main(int argc, char *argv[])
{
    FILE *fp;
    //int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;
#if HUF ==1
    unsigned long int inputL;
#endif
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
        tableHead[counter] = (entry*) malloc(sizeof(entry));
        //tableHead[counter ] = hashTable[counter ];
#if HUF ==1
        tableHead[(counter++)] -> lastName = 0;
#else   
        *tableHead[(counter++)] -> lastName = 0;
#endif     
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
#elif HUF==1
    extra *hHead = (extra *) malloc(sizeof(extra));
    hHead->pNext = NULL;
    //loadCode();
    int statu = fscanf(fp,"%lu",&inputL);
#else
    fgets(line, sizeof(line), fp);
#endif
#if HUF!=1
    do {
        line[strlen(line) -1] = '\0'; //change while loop to strlen
#if OPT == 1  //compilering OPT version
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
#else
    //int c=0;
    do{
        if(statu){
            append(inputL,tableHead[hash(inputL)]);

            //printf("hash = %u\n", hash(inputL));
            //printf("%lu\n", inputL);
        }
        else{
            statu = fscanf(fp,"%s",line);
            line[strlen(line)-1] = '\0';
            appendExtra(line,hHead);
        }
        //printf("%d\n",(c++) );
    } while((statu = fscanf(fp,"%lu",&inputL))!= EOF);
#endif
    /* close file as soon as possible */
    fclose(fp);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* the givn last name to find */


#if OPT == 1 && ENT!=1 && HUF!=1//compilering OPT version
    char input[MAX_LAST_NAME_SIZE] = "ear";
    e = tableHead[BKDRhash(input)];
#elif ENT == 1 //editable version
    puts("enter the words you want to search");
    fflush(stdin);
    char input[MAX_LAST_NAME_SIZE];
    fgets(input, MAX_LAST_NAME_SIZE, stdin);
    input[strlen(input)-1] = '\0';
    e = tableHead[BKDRhash(input)];
#elif HUF ==1
    char inputC[MAX_LAST_NAME_SIZE] = "ear";
    unsigned long int input = encode(inputC); 
    //printf("input = %lu\n", input);
    /*for(int i = 0;i<64;i++)
    {
        printf("%lu",(input >> 63));
        input<<=1;
    }
    //printf("\n");
    printf("hash = %u\n", hash(inputL));*/
    e = tableHead[hash(input)];
#else
    char input[MAX_LAST_NAME_SIZE] = "ear";
    e = pHead;
#endif

#if HUF==1
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert((findName(input, e)->lastName == input));
#else
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, input));
#endif



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
    
    //printf("avg = %d max = %d min = %d",all/MAX_TABLE_SIZE,max,min );
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


