#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#define Max_cart_count 10

void Print_current_commodity_status(product *first_product){
    printf("Commodity List:\n");
    printf("%6s %6s %15s %10s %10s %10s\n","Batch","ID","Name","Count","Unit Cost","Price");
    //printf("head batch=%d\n",first_product->batch);
    product *p=first_product;
    while(p!=NULL){
        printf("%6d %6s %15s %10d %10d %10d\n",p->batch,p->id,p->name,p->qty,p->unit_cost,p->price);
        p=p->next;
    }
    printf("--------\n");
}

void push_a_product(product **head_p,product **tail_p,char *id,char *name,int qty,int uct,int batch,int price,int *len){
    if((*head_p==NULL) && (*tail_p==NULL)){
        product *new_product=(product*)malloc(sizeof(product));
        *new_product=(product){id,name,price,uct,qty,batch,*tail_p,NULL};
        *head_p=new_product;
        *tail_p=new_product;
    }
    else{
        product *p=*head_p;
        while(p!=NULL && strcmp(id,p->id)>=0) p=p->next;
        product *new_product=(product*)malloc(sizeof(product));
        *new_product=(product){id,name,price,uct,qty,batch,NULL,NULL};
        if(p==*head_p){
            (*head_p)->last=new_product;
            new_product->next=*head_p;
            *head_p=new_product;
        }
        else if(p==NULL){
            (*tail_p)->next=new_product;
            new_product->last=*tail_p;
            *tail_p=new_product;
        }
        else{
            product *last=p->last;
            last->next=new_product;
            p->last=new_product;
            new_product->last=last;
            new_product->next=p;
        }
    }
    (*len)++;
}

void delete_a_product(product *this_product,product **head_p,product **tail_p,int *len){
    product *last=this_product->last,*next=this_product->next;
    if(last!=NULL){
        last->next=next;
    }
    else if(*head_p==this_product){
        (*head_p)=this_product->next;
    }
    if(next!=NULL){
        next->last=last;
    }
    else if(*tail_p==this_product){
        (*tail_p)=this_product->last;
    }
    free(this_product->name);
    free(this_product->id);
    free(this_product);
    this_product=NULL;
    (*len)--;
}

int add_a_new_cart(char *id,cart *current_cart){
    int open_cart_successfully=0;
    for(int i=0;i<Max_cart_count;i++){
        if(current_cart[i].id==NULL){
            current_cart[i].id=(char*)calloc(strlen(id),sizeof(char));
            strcpy(current_cart[i].id,id);
            open_cart_successfully=1;
            break;
        }
    }
    return !open_cart_successfully;
}

int delete_a_cart(char *id,cart *current_cart){
    int delete_cart_successfully=0;
    for(int i=0;i<Max_cart_count;i++){
        if(current_cart[i].id!=NULL && strcmp(id,current_cart[i].id)==0){
            free(current_cart[i].id);
            current_cart[i].id=NULL;
            product *p=current_cart[i].first_product;
            while(p!=NULL){
                product *next=p->next;
                free(p);
                p=next;
            }
            current_cart[i].first_product=NULL;
            current_cart[i].last_product=NULL;
            current_cart[i].product_list_len=0;
            delete_cart_successfully=1;
            break;
        }
    }
    return !delete_cart_successfully;
}

int find_cart_id(char *cid,cart *current_cart){
    for(int i=0;i<Max_cart_count;i++){
        if(current_cart[i].id!=NULL && strcmp(cid,current_cart[i].id)==0) return i;
    }
    return -1;
}

int check_product_id_exist(char *id,product *first_product){
    product *p=first_product;
    while(p!=NULL){
        if(strcmp(id,p->id)==0) return 1;
        p=p->next;
    }
    return 0;
}

int add_product_to_a_cart(char *buffer,cart *current_cart,product *first_product_in_list){
    int st[4]={0},len[4]={0},ind=0;
    st[0]=0;
    for(char *p=buffer;*p!='\0' && *p!='\n' && ind<3;p++){
        if(*p==' '){
            ind++;
            st[ind]=(p-buffer+1);
        }
        else{
            len[ind]++;
        }
    }
    char *cid=(char*)calloc(len[0],sizeof(char));
    for(int i=0;i<len[0];i++) *(cid+i)=*(buffer+i+st[0]);
    char *pid=(char*)calloc(len[1],sizeof(char));
    for(int i=0;i<len[1];i++) *(pid+i)=*(buffer+i+st[1]);;
    int qty=0;
    for(int i=0;i<len[2];i++){
        qty*=10;
        qty+=*(buffer+i+st[2])-'0';
    }
    int i=find_cart_id(cid,current_cart);
    if(i==-1){
        printf("ERR NO_CART\n");
        return 1;
    }
    if(!check_product_id_exist(pid,first_product_in_list)){
        printf("ERR NO_PRODUCT\n");
        return 1;
    }
    int add_success=0;
    product *p=current_cart[i].first_product;
    while(p!=NULL){
        if(strcmp(p->id,pid)==0){
            p->qty+=qty;
            add_success=1;
            break;
        }
        p=p->next;
    }
    if(!add_success){
        push_a_product(&(current_cart[i].first_product),&(current_cart[i].last_product),pid,NULL,qty,0,0,0,&(current_cart[i].product_list_len));
    }
    return 0;
}
