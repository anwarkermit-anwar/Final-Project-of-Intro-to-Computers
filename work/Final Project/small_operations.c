#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int find_id_to_index(char *target,char **id,int len){
    for(int i=0;i<len;i++){
        if(strcmp(*(id+i),target)==0) return i;
    }
    printf("Cannot find corresponding index!!!!\n");
    return -1;
}

int set_price(char *product_id,int product_price,int *price,char **id,int len){
    int ind=find_id_to_index(product_id,id,len);
    if(ind==-1) {
        printf("Set price failed!!!\n");
        return 1;
    }
    *(price+ind)=product_price;
    return 0;
}

int get_segment_len(char *s){
    int cnt=0;
    while(*s!=' ' && *s!='\n' && *s!='\0') {
        cnt++;
        s++;
    }
    return cnt;
}

int call_set_price(char *buffer,int *price,char **id,int len){
    char *p0=buffer+6;
    char *p1=p0+get_segment_len(p0)+1;
    char *p2=p1+get_segment_len(p1);
    int l1=p1-p0-1,l2=p2-p1;
    char *product_id=(char*)calloc(l1,sizeof(char));
    for(int i=0;i<l1;i++) *(product_id+i)=*(p0+i);
    int product_price=0;
    for(int i=0;i<l2;i++){
        product_price*=10;
        product_price+=*(p1+i)-'0';
    }
    int sta=set_price(product_id,product_price,price,id,len);
    free(product_id);
    return sta;
}
