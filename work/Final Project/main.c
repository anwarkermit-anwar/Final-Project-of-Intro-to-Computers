#include<stdio.h>
#include<string.h>
#include "import.h"
#include "recording_system.h"

char s[1000],status[9]="BYE";
int* price,count;
char** id,name;
int len=1,plen=0;

int call_import(){
    int batches=import(s+7,&price,&count,&id,&name,&len,&plen);
    if(batches==-1) return 1;
    printf("OK IMPORT %d\n",batches);
    return 0;
}

void initialization(){
    price=(int*)calloc(1,sizeof(int));
    count=(int*)calloc(1,sizeof(int));
    id=(char**)calloc(1,sizeof(char*));
    name=(char**)calloc(1,sizeof(char*));
    len=0;
    plen=1;
}

int main(){
    initialization();
    while(fgets(s,900,stdin)!=NULL){
        s[strcspn(s,"\n")]='\0';
        if(strcmp(s,"QUIT")==0) break;
        if(strncmp(s,"IMPORT ",7)==0){
            if(call_import()) {
                strcpy(status,"Error!!!");
                break;
            }
        }
    }
    printf("%s\n",status);
}
/*
IMPORT text.txt
QUIT
*/
