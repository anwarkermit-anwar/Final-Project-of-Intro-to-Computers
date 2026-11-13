#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "product_manager.h"

void add_a_new_product(
    char* buffer,
    product **first_product,
    product **last_product,
    int* len,
    int* next_batch
){
    char *p0,*p1,*p2,*p3,*p4;
    p0=buffer;
    p1=strstr(buffer,"###")+3;
    p2=strstr(buffer,"@@@")+3;
    p3=strstr(buffer,"&&&")+3;
    p4=strstr(buffer,"\n");
    int l1=p1-p0-3,l2=p2-p1-3,l3=p3-p2-3,l4=p4-p3;
    //id
    char *pid=(char*)calloc(l1,sizeof(char));
    for(int i=0;i<l1;i++,p0++){
        *(pid+i)=*p0;
    }
    //name
    char *pname=(char*)calloc(l2,sizeof(char));
    for(int i=0;i<l2;i++,p1++){
        *(pname+i)=*p1;
    }
    //qty
    int qty=0;
    for(int i=0;i<l3;i++,p2++){
        qty*=10;
        qty+=*p2-'0';
    }
    //unit_cost
    int uct=0;
    for(int i=0;i<l4;i++,p3++){
        uct*=10;
        uct+=*p3-'0';
    }
    push_a_product(first_product,last_product,pid,pname,qty,uct,*next_batch,0,len);
    (*next_batch)++;
    //
    (*len)++;
}

int import(
    char* file_name,
    product **first_product,
    product **last_product,
    int* len,
    int* next_batch
){
    FILE *file=fopen(file_name,"r");
    if(file==NULL){
        printf("ERR FILE_OPEN\n");
        return -1;
    }
    char buffer[100],batches=0;
    while(fgets(buffer,sizeof(buffer),file)!=NULL){
        add_a_new_product(buffer,first_product,last_product,len,next_batch);
        batches++;
    }

    fclose(file);
    return batches;
}

