#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"

void push_a_product(product **head_p,product **tail_p,char *id,char *name,int qty,int uct,int batch,int *len){
    if((*head_p==NULL) && (*tail_p==NULL)){
        product *new_product=(product*)malloc(sizeof(product));
        *new_product=(product){id,name,0,uct,qty,batch,*tail_p,NULL};
        *head_p=new_product;
        *tail_p=new_product;
    }
    else{
        product *new_product=(product*)malloc(sizeof(product));
        *new_product=(product){id,name,0,uct,qty,batch,*tail_p,NULL};
        (*tail_p)->next=new_product;
        *tail_p=new_product;
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
    free(this_product);
    this_product=NULL;
    (*len)--;
}
