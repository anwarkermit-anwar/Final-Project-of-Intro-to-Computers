#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void add_a_new_product(
    char* buffer,
    int** price_p,
    int** unit_cost_p,
    int** count_p,
    char*** id_p,
    char*** name_p,
    int* len,
    int* plen
){
    int *price=*price_p,*unit_cost=*unit_cost_p,*count=*count_p;
    char **id=*id_p,**name=*name_p;
    if(*len>=*plen){
        (*plen)*=2;
        price=(int*)realloc(price,*plen*sizeof(int));
        *price_p=price;
        unit_cost=(int*)realloc(unit_cost,*plen*sizeof(int));
        *unit_cost_p=unit_cost;
        count=(int*)realloc(count,*plen*sizeof(int));
        *count_p=count;
        id=(char**)realloc(id,*plen*sizeof(char*));
        *id_p=id;
        name=(char**)realloc(name,*plen*sizeof(char*));
        *name_p=name;
        for(int i=*len;i<*plen;i++){
            *(price+i)=0;
            *(unit_cost+i)=0;
            *(count+i)=0;
        }
    }
    char *p0,*p1,*p2,*p3,*p4;
    p0=buffer;
    p1=strstr(buffer,"###")+3;
    p2=strstr(buffer,"@@@")+3;
    p3=strstr(buffer,"&&&")+3;
    p4=strstr(buffer,"\n");
    int l1=p1-p0-3,l2=p2-p1-3,l3=p3-p2-3,l4=p4-p3;
    //id
    char *p=(char*)calloc(l1,sizeof(char));
    *(id+*len)=p;
    for(int i=0;i<l1;i++,p0++,p++){
        *p=*p0;
    }
    //name
    p=(char*)calloc(l2,sizeof(char));
    *(name+*len)=p;
    for(int i=0;i<l2;i++,p1++,p++){
        *p=*p1;
    }
    //qty
    int qty=0;
    for(int i=0;i<l3;i++,p2++){
        qty*=10;
        qty+=*p2-'0';
    }
    *(count+*len)=qty;
    //price
    int prc=0;
    for(int i=0;i<l4;i++,p3++){
        prc*=10;
        prc+=*p3-'0';
    }
    *(unit_cost+*len)=prc;
    //
    (*len)++;
}

int import(
    char* file_name,
    int** price,
    int** unit_cost,
    int** cnt,
    char*** id,
    char*** name,
    int* len,
    int* plen
){
    FILE *file=fopen(file_name,"r");
    if(file==NULL){
        printf("file open failed!\n");
        return -1;
    }
    char buffer[100],batches=0;
    while(fgets(buffer,sizeof(buffer),file)!=NULL){
        add_a_new_product(buffer,price,unit_cost,cnt,id,name,len,plen);
        batches++;
    }
    fclose(file);
    return batches;
}

