#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int find_id_to_index(char *target,char **id,int len,int *found,int *found_len){
    for(int i=0;i<len;i++){
        if(strcmp(*(id+i),target)==0) {
            *(found+*found_len)=i;
            (*found_len)++;
        }
    }
    if(*found_len>0) return 0;
    printf("Cannot find corresponding index!!!!\n");
    return -1;
}
//set price
int set_price(char *product_id,int product_price,int *price,char **id,int len){
    int *found=(int*)calloc(len,sizeof(int));
    int found_len=0;
    int res=find_id_to_index(product_id,id,len,found,&found_len);
    if(res==-1) {
        printf("product ID not found!!!\n");
        free(found);
        return 1;
    }
    for(int i=0;i<found_len;i++){
        int ind=*(found+i);
        *(price+ind)=product_price;
    }
    free(found);
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
//query
int query(
    char *product_id,
    int* price,
    int* unit_cost,
    int* qty,
    char** id,
    char** name,
    int len
){
    //get all corresponding products
    int *found=(int*)calloc(len,sizeof(int));
    int found_len=0;
    int res=find_id_to_index(product_id,id,len,found,&found_len);
    if(res==-1) {
        printf("product ID not found!!!\n");
        free(found);
        return 1;
    }
    //deal with data
    int stock=0;
    for(int i=0;i<found_len;i++){
        int ind=*(found+i);
        stock+=*(qty+ind);
    }
    int ind=*(found);
    //print
    printf("QUERY %s %s ",*(id+ind),*(name+ind));
    if(*(price+ind)>0) printf("price=%d ",*(price+ind));
    else printf("price=none ");
    printf("stock=%d\n",stock);
    if(stock==0) {
        free(found);
        return 0;
    }
    for(int i=0;i<found_len;i++){
        int ind=*(found+i);
        printf("  #%d ",ind+1);
        printf("qty=%d ",*(qty+ind));
        printf("cost=%d\n",*(unit_cost+ind));
    }
    free(found);
    return 0;
}
