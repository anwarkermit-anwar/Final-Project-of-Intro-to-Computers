#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "import.h"
#include "recording_system.h"
#include "small_operations.h"

const int Is_printing_current_sta=0;

char s[1000],status[9]="BYE";
int *unit_cost,*price,*count;
char **id,**name;
int len=1,plen=0;


void print_current_commodity_status(){
    if(!Is_printing_current_sta) return;
    printf("Commodity List:\n");
    printf("%6s %15s %10s %10s %10s\n","ID","Name","Count","Unit Cost","Price");
    for(int i=0;i<len;i++){
        printf("%6s %15s %10d %10d %10d\n",*(id+i),*(name+i),*(count+i),*(unit_cost+i),*(price+i));
    }
    printf("--------\n");
}

int call_import(){
    int batches=import(s+7,&price,&unit_cost,&count,&id,&name,&len,&plen);
    if(batches==-1) return 1;
    print_current_commodity_status();
    printf("OK IMPORT %d\n",batches);
    return 0;
}

void initialization(){
    price=(int*)calloc(1,sizeof(int));
    unit_cost=(int*)calloc(1,sizeof(int));
    count=(int*)calloc(1,sizeof(int));
    id=(char**)calloc(1,sizeof(char*));
    name=(char**)calloc(1,sizeof(char*));
    len=0;
    plen=1;
    *price=0;
    *unit_cost=0;
    *count=0;
}

void free_all(){
    for(int i=0;i<len;i++){
        free(*(id+i));
        free(*(name+i));
    }
    free(id);
    free(name);
    free(unit_cost);
    free(count);
    free(price);
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
        if(strncmp(s,"PRICE ",6)==0){
            if(call_set_price(s,price,id,len)){
                strcpy(status,"Error!!!");
                print_current_commodity_status();
                break;
            }
            print_current_commodity_status();
            printf("OK %s\n",s);
        }
        if(strncmp(s,"QUERY ",6)==0){
            if(query(s+6,price,unit_cost,count,id,name,len)){
                strcpy(status,"Error!!!");
                break;
            }
        }
    }
    printf("%s\n",status);
    free_all();
}
/*
IMPORT inventory.txt
PRICE P001 300
PRICE P002 250
QUERY P002
QUERY P005
QUERY P001
QUIT
*/
