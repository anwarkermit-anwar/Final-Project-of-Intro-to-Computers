#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "promotion.h"
#include "product_manager.h"

int find_id_to_index(char *target,product *first_product,product **found,int *found_len){
    product *p=first_product;
    while(p!=NULL){
        if(strcmp(p->id,target)==0){
            *(found+*found_len)=p;
            (*found_len)++;
        }
        p=p->next;
    }
    if(*found_len>0) return 0;
    return -1;
}
//set price
int set_price(char *product_id,int product_price,product *first_product,int len){
    product **found=(product**)calloc(len,sizeof(product*));
    int found_len=0;
    int res=find_id_to_index(product_id,first_product,found,&found_len);
    if(res==-1) {
        printf("ERR NO_PRODUCT\n");
        free(found);
        return 1;
    }
    for(int i=0;i<found_len;i++){
        (*(found+i))->price=product_price;
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

int call_set_price(char *buffer,product *first_product,int len){
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
    int sta=set_price(product_id,product_price,first_product,len);
    free(product_id);
    return sta;
}
//query
int query(
    char *product_id,
    product *first_product,
    int len
){
    //get all corresponding products
    product **found=(product**)calloc(len,sizeof(product*));
    int found_len=0;
    int res=find_id_to_index(product_id,first_product,found,&found_len);
    if(res==-1) {
        printf("ERR NO_PRODUCT\n");
        free(found);
        return 1;
    }
    //deal with data
    int stock=0;
    for(int i=0;i<found_len;i++){
        stock+=(*(found+i))->qty;
    }
    //print
    printf("QUERY %s %s ",(*found)->id,(*found)->name);
    if((*found)->price>0) printf("price=%d ",(*found)->price);
    else printf("price=none ");
    printf("stock=%d\n",stock);
    if(stock==0) {
        free(found);
        return 0;
    }
    for(int i=0;i<found_len;i++){
        if((*(found+i))->qty==0) continue;
        printf("  #%d ",(*(found+i))->batch);
        printf("qty=%d ",(*(found+i))->qty);
        printf("cost=%d\n",(*(found+i))->unit_cost);
    }
    free(found);
    return 0;
}
//Set promotion
int set_promotion(char *buffer,struct promotion *promo_p){
    int len[5]={0},st[5]={0},ind=0;
    for(char *p=buffer;*p!='\0' && *p!='\n' && ind<4;p++){
        if(*p==' '){
            ind++;
            st[ind]=(p-buffer+1);
        }
        else len[ind]++;
    }
    //id_a
    promo_p->id_a=(char*)calloc(len[0],sizeof(char));
    for(int i=0;i<len[0];i++){
        *(promo_p->id_a+i)=*(buffer+st[0]+i);
    }
    //qty_condition
    int condi=0;
    for(int i=0;i<len[1];i++){
        condi*=10;
        condi+=*(buffer+st[1]+i)-'0';
    }
    promo_p->qty_condition=condi;
    //id_b
    promo_p->id_b=(char*)calloc(len[2],sizeof(char));
    for(int i=0;i<len[2];i++){
        *(promo_p->id_b+i)=*(buffer+st[2]+i);
    }
    //percentage
    int per=0;
    for(int i=0;i<len[3];i++){
        per*=10;
        per+=*(buffer+st[3]+i)-'0';
    }
    promo_p->percentage=per;
    return 0;
}

int report(product *first_product,int revenue,int cost,int gift_cost){
    printf("REPORT\n");
    product *p=first_product;
    int acc_stock=0,acc_value=0;
    while(p!=NULL){
        acc_stock+=p->qty;
        acc_value+=p->unit_cost*p->qty;
        if(p->next==NULL || strcmp(p->next->id,p->id)!=0) {
            if(acc_stock>0) printf("  %s stock=%d value=%d\n",p->id,acc_stock,acc_value);
            acc_stock=0;
            acc_value=0;
        }
        p=p->next;
    }
    printf("REVENUE %d\n",revenue);
    printf("COGS %d\n",cost);
    printf("GIFTCOST %d\n",gift_cost);
    printf("PROFIT %d\n",revenue-cost-gift_cost);
    return 0;
}
