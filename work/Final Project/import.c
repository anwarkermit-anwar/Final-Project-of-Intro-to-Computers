#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void add_a_new_product(char* buffer,int** price_p,int** count_p,char*** id_p,char*** name_p,int* len,int* plen);

int import(char* file_name,int** price,int** count,char*** id,char*** name,int* len,int* plen){
    FILE *file=fopen(file_name,"r");
    if(file==NULL){
        printf("file open failed!\n");
        return -1;
    }
    char buffer[100],batches=0;
    while(fgets(buffer,sizeof(buffer),file)!=NULL){
        add_a_new_product(buffer,price,count,id,name,len,plen);
    }
    fclose(file);
    return batches;
}

void add_a_new_product(char* buffer,int** price_p,int** count_p,char*** id_p,char*** name_p,int* len,int* plen){
    int *price=*price_p,*count=*count_p;
    char **id=*id_p,**name=*name_p;
    if(*len>=*plen){
        *plen*=2;
        price=(int*)realloc(price,*plen*sizeof(int));
        *price_p=price;
        count=(int*)realloc(price,*plen*sizeof(int));
        *count_p=count;
        id=(char**)realloc(id,*plen*sizeof(char*));
        *id_p=id;
        name=(char**)realloc(name,*plen*sizeof(char*));
        *name_p=name;
    }
    char *p0,*p1,*p2,*p3,*p4;
    p0=buffer;
    p1=strstr(buffer,"###")+3;
    p2=strstr(buffer,"@@@")+3;
    p3=strstr(buffer,"&&&")+3;
    p4=strstr(buffer,"\n");
    int l1=p1-p0-3,l2=p2-p1-3,l3=p3-p2-3,l4=p4-p3;
    //
    char *p=*(id+*len);
    p=(char*)calloc(l1,sizeof(char));
    for(int i=0;i<l1;i++,p0++,p++){
        *p=*p0;
    }
    //
    p=*(id+*len);
    p=(char*)calloc(l1,sizeof(char));
    for(int i=0;i<l1;i++,p0++,p++){
        *p=*p0;
    }
    *len++;
}
