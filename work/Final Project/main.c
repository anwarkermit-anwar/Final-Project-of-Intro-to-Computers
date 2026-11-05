#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "import.h"
#include "recording_system.h"
#include "small_operations.h"
#include "promotion.h"
#include "product_manager.h"


const int Is_printing_current_sta=1;
const int buffering=0;

product *first_product=NULL,*last_product=NULL;
char s[1000],status[9]="BYE";
int len=1,next_batch=1;
struct promotion promo;

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

int call_import(){
    int batches=import(s+7,&first_product,&last_product,&len,&next_batch);
    if(batches==-1) return 1;

    print_current_commodity_status();
    printf("OK IMPORT %d\n",batches);
    return 0;
}

void initialization(){
    len=0;
    promo.qty_condition=0;
    promo.percentage=0;
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
            if(call_import()) {
                strcpy(status,"Error!!!");
                break;
            }
        }
        if(strncmp(s,"PRICE ",6)==0){
            if(call_set_price(s,first_product,len)){
                strcpy(status,"Error!!!");
                break;
            }
            print_current_commodity_status();
            printf("OK %s\n",s);
        }
        if(strncmp(s,"QUERY ",6)==0){
            if(query(s+6,first_product,len)){
                strcpy(status,"Error!!!");
                break;
            }
        }
        if(strncmp(s,"PROMO ",6)==0){
            if(set_promotion(s+6,&promo)){
                strcpy(status,"Error!!!");
                break;
            }
            printf("OK PROMO\n");
        }
    }
    printf("%s\n",status);
    free_all();
}
/*
IMPORT inventory.txt
PRICE P001 300
PRICE P002 250
PROMO P001 3 P002 80
QUERY P002
QUERY P001
QUERY P003
QUIT
*/
