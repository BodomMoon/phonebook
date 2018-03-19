#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define DICT_FILE "lastname.txt"

#define MAX_TABLE_SIZE 4096
#define MAX_LAST_NAME_SIZE 20

typedef struct node
{
    int time;
    char name;
    struct node* Lnode;
    struct node* Rnode;
}Letter;

char* itobs(int n, char *ps) {
    int i = 32;
    while(i)
    {
        if( (n|0x7fffffff) != 0xffffffff)
            break;
        n<<=1; 
        i--;
    }
    n<<=1;
    i--; //now the most left is what we want
    int a = 0;
    while(i--) {
        int d = ((n >> 31) & 1);
        ps[a] =  d + '0';
        //printf("%d", d);
        n<<=1;
        a++;
    }
    ps[a]='\0';
    return ps;
}


void inti(Letter *root)
{
    root->Lnode = NULL;
    root->Rnode = NULL;
    root->name = 'B';
}

void printTree(Letter *root , int level)
{
    char code[32];

    if(root->Lnode!=NULL){
        printTree(root->Lnode ,level*2+1);
        //printf("time = %d\n",root->time);
    }
    if(root->Rnode!=NULL){
        printTree(root->Rnode,level*2);
        //printf("time = %d\n", root->time);
    }

    if( (root->Rnode==NULL) && (root->Rnode==NULL)){
        itobs(level,code);
        printf("time = %6d name =  %c code = %s \n",root->time, root->name , code);
    }

}

int main(int argc, char *argv[])
{
    FILE *fp;
    int all = 0;
    char line[MAX_LAST_NAME_SIZE];
    int counter;
    int min;
    Letter *min1,*min2;
    Letter *head , *head2;

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
    int alphabet[27] = {0};
    while(fgets(line, sizeof(line), fp)!=NULL){
        for(int i = 0; i < sizeof(line); i++){
            int test = 0;
            test = line[i]-'a';
            alphabet[test]++;
        }
    }
    fclose(fp);
    puts("read end");
    Letter *letter[27];
    for (int i = 0; i < 26; i++){
        letter[i] = (Letter *) malloc(sizeof(Letter));
        inti(letter[i]);
        letter[i]-> time = alphabet[i];
        all+=alphabet[i];
        letter[i]-> name =(char)'a' + i;
        printf("%d\n",letter[i]->time);
    }
    alphabet[26] = 896410;
    letter[26] = (Letter *) malloc(sizeof(Letter));
    inti(letter[26]);
    letter[26]->time = 896410;
    letter[26]->name = 'A';
    printf("%d\n",letter[26]->time);
    all+= 896410;



    printf("all = %d\n", all);
    puts("start count");

    do
    {
        int flag = 0;
        for (int i = 0 ,min = 20000000; i <= 26; i++){
            if(alphabet[i] < min){
                min = alphabet[i];
                min1 = letter[i];
                flag = i;
            }
        }
        
       // printf("%d\n", alphabet[flag]);
        alphabet[flag] = 20000000;

        for (int i = 0 ,min = 20000000; i <= 26; i++){
            if(alphabet[i] < min){
                min = alphabet[i];
                min2 = letter[i];
                flag = i;
            }
        }
        //printf("%d\n", alphabet[flag]);
        alphabet[flag] = (*min1).time + (*min2).time;
        //printf("%d\n", alphabet[flag]);


        head = (Letter *) malloc(sizeof(Letter));
        head ->time = (*min1).time + (*min2).time;

        if(min1->time < min2->time){
            head -> Lnode = min1;
            head -> Rnode = min2;
        }else{
            head -> Lnode = min2;
            head -> Rnode = min1;
        }
        letter[flag] = head;
        //printf("min1 = %c\n", min1->name);
        //printf("min2 = %c\n", min2->name);
        //getchar();
    }
    while(head->time != all);
    //printf("%d\n", head->time);


    printTree(head , 0xfffffffe);
    free(head->Lnode);
    free(head->Rnode);
    free(head);



}


