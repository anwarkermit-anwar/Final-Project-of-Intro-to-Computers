#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "import.h"
#include "recording_system.h"
#include "small_operations.h"
#include "promotion.h"
#include "product_manager.h"
#include "checkout.h"
#define Max_cart_count 10


const int Is_printing_current_sta=0;
const int Is_printing_current_cart_sta=1;
const int buffering=0;

product *first_product=NULL,*last_product=NULL;
char s[1000],status[9]="BYE";
int len=1,next_batch=1;
int acc_cost,acc_gift_cost,acc_revenue;
struct promotion promo;
cart current_cart[Max_cart_count];

void print_current_commodity_status(){
    if(!Is_printing_current_sta) return;
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

void print_current_cart_status(){
    if(!Is_printing_current_cart_sta) return;
    printf("Cart List:\n");
    for(int i=0;i<Max_cart_count;i++){
        printf("Cart ID=%s:\n",current_cart[i].id);
        product *p=current_cart[i].first_product;
        while(p!=NULL){
            printf("%s qty=%d\n",p->id,p->qty);
            p=p->next;
        }
        printf("\n");
    }
    printf("--------\n");
}

int call_import(){
    int batches=import(s+7,&first_product,&last_product,&len,&next_batch);
    if(batches==-1) return 0;

    print_current_commodity_status();
    printf("OK IMPORT %d\n",batches);
    return 0;
}

void initialization(){
    len=0;
    promo.qty_condition=0;
    promo.percentage=0;
    for(int i=0;i<Max_cart_count;i++) current_cart[i]=(cart){NULL,NULL,NULL,0};
}

void free_all(){

}

int main(){
    char output_buffer[100001];
    if(buffering){
        setvbuf(stdout,output_buffer,_IOFBF,sizeof(output_buffer));
    }
    initialization();
    while(fgets(s,900,stdin)!=NULL){
        s[strcspn(s,"\n")]='\0';
        if(strcmp(s,"QUIT")==0) break;
        if(strncmp(s,"IMPORT ",7)==0){
            call_import();
        }
        if(strncmp(s,"PRICE ",6)==0){
            if(!call_set_price(s,first_product,len)) printf("OK %s\n",s);
            print_current_commodity_status();
        }
        if(strncmp(s,"QUERY ",6)==0){
            query(s+6,first_product,len);
        }
        if(strncmp(s,"PROMO ",6)==0){
            if(set_promotion(s+6,&promo)){
                strcpy(status,"Error!!!");
                break;
            }
            printf("OK PROMO\n");
        }
        if(strncmp(s,"CART ",5)==0){
            if(!add_a_new_cart(s+5,current_cart)) printf("OK %s\n",s);
            print_current_cart_status();
        }
        if(strncmp(s,"ADD ",4)==0){
            if(!add_product_to_a_cart(s+4,current_cart,first_product)) printf("OK %s\n",s);
            print_current_cart_status();
        }
        if(strncmp(s,"CHECKOUT ",9)==0){
            check_out(s+9,current_cart,&first_product,&last_product,&len,promo,&acc_revenue,&acc_cost,&acc_gift_cost);
        }
    }
    printf("%s\n",status);
    free_all();
}
/*
IMPORT inventory.txt
PRICE P001 300
PROMO P001 3 P002 80
CART C001
ADD C001 P002 3
ADD C001 P001 1
CHECKOUT C002
QUIT
*/
