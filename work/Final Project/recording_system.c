#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "promotion.h"
#include "product_manager.h"
#include "small_operations.h"

int product_count(product *first_product){
    int count=0;
    product *p=first_product;
    while(p!=NULL){
        if(p->last==NULL || strcmp(p->last->id,p->id)!=0) count++;
        p=p->next;
    }
    return count;
}

int batch_count(product *first_product){
    int count=0;
    product *p=first_product;
    while(p!=NULL){
        if(p->qty>0) count++;
        p=p->next;
    }
    return count;
}

int save(
    char* saving_file,
    product *first_product,
    promotion promo,
    int revenue,
    int cost,
    int gift_cost,
    int next_batch
){
    FILE *file=fopen(saving_file,"w");
    if(file==NULL){
        printf("ERR FILE_OPEN\n");
        return 0;
    }
    //next_batch
    fprintf(file,"NEXT_BATCH %d\n",next_batch);
    //products
    fprintf(file,"PRODUCTS %d\n",product_count(first_product));
    product *p=first_product;
    while(p!=NULL){
        if(p->next==NULL || strcmp(p->next->id,p->id)!=0) {
            if(p->price==0) fprintf(file,"%s %d\n%s\n",p->id,-1,p->name);
            else fprintf(file,"%s %d\n%s\n",p->id,p->price,p->name);
        }
        p=p->next;
    }
    //promotion
    if(promo.percentage==0) fprintf(file,"PROMO 0\n");
    else{
        fprintf(file,"PROMO 1\n");
        fprintf(file,"%s %d %s %d\n",promo.id_a,promo.qty_condition,promo.id_b,promo.percentage);
    }
    //total
    fprintf(file,"TOTAL %d %d %d\n",revenue,cost,gift_cost);
    //batches
    p=first_product;
    fprintf(file,"BATCHES %d\n",batch_count(first_product));
    while(p!=NULL){
        if(p->qty>0){
            fprintf(file,"%s %d %d %d\n",p->id,p->batch,p->qty,p->unit_cost);
        }
        p=p->next;
    }
    return 0;
}

void clear_product_list(product **first_product_p,product **last_product_p,int *len){
    while(*first_product_p!=NULL){
        delete_a_product(*first_product_p,first_product_p,last_product_p,len);
    }
}

product* find_product(char *id,product *first_commodity){
    product *p=first_commodity;
    while(p!=NULL){
        if(strcmp(p->id,id)==0) return p;
    }
    return NULL;
}

int load(
    char* loading_file,
    product **first_product_p,
    product **last_product_p,
    int *len,
    promotion *promo_p,
    int *revenue_p,
    int *cost_p,
    int *gift_cost_p,
    int *next_batch_p
){
    FILE *file=fopen(loading_file,"r");
    if(file==NULL){
        printf("ERR FILE_OPEN\n");
        return 0;
    }
    //clear old data
    clear_product_list(first_product_p,last_product_p,len);
    promo_p->percentage=0;
    promo_p->qty_condition=0;
    *revenue_p=0;
    *cost_p=0;
    *gift_cost_p=0;
    //next_batch
    *next_batch_p=0;
    char buffer[1000];
    fgets(buffer,sizeof(buffer),file);
    for(int i=11;buffer[i]!='\n';i++){
        (*next_batch_p)*=10;
        (*next_batch_p)+=buffer[i]-'0';
    }
    //product list
    product *first_commodity=NULL,*last_commodity=NULL;
    int commodity_len=0;
    fgets(buffer,sizeof(buffer),file);
    int commodity_count=0;
    for(int i=9;buffer[i]!='\n';i++){
        commodity_count*=10;
        commodity_count+=buffer[i]-'0';
    }
    while(commodity_count--){
        fgets(buffer,sizeof(buffer),file);
        char *id_end=strstr(buffer," ");
        char *id=(char*)calloc(id_end-buffer,sizeof(char));
        char *price_st=id_end+1;
        int price=0;
        for(int i=0;*(price_st+i)!='\n';i++){
            price*=10;
            price+=*(price_st+i)-'0';
        }
        fgets(buffer,sizeof(buffer),file);
        buffer[strcspn(buffer,"\n")]='\0';
        char *name=(char*)malloc((strlen(buffer)+1)*sizeof(char));
        strcpy(name,buffer);
        push_a_product(&first_commodity,&last_commodity,id,name,0,0,0,price,&commodity_len);
    }
    //promotion
    fgets(buffer,sizeof(buffer),file);
    if(buffer[6]=='1'){
        fgets(buffer,sizeof(buffer),file);
        buffer[strcspn(buffer,"\n")]='\0';
        set_promotion(buffer,promo_p);
    }
    //total
    fgets(buffer,sizeof(buffer),file);
    int ty=1;
    for(int i=6;buffer[i]!='\n';i++){
        if(buffer[i]==' ') {
            ty++;
            continue;
        }
        if(ty==1){
            (*revenue_p)*=10;
            (*revenue_p)+=buffer[i]-'0';
        }
        if(ty==2){
            (*cost_p)*=10;
            (*cost_p)+=buffer[i]-'0';
        }
        if(ty==3){
            (*gift_cost_p)*=10;
            (*gift_cost_p)+=buffer[i]-'0';
        }
    }
    //batches
    fgets(buffer,sizeof(buffer),file);
    int batches=0;
    for(int i=8;buffer[i]!='\n';i++){
        batches*=10;
        batches+=buffer[i]-'0';
    }
    while(batches--){

    }
    return 0;
}
