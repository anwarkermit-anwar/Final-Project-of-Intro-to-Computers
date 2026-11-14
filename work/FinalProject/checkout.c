#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "product_manager.h"
#include "promotion.h"
#define Max_cart_count 10
int cart_exist(char *cid,cart current_cart[]){
    for(int i=0;i<Max_cart_count;i++) if(current_cart[i].id!=NULL && strcmp(current_cart[i].id,cid)==0) return i;
    return -1;
}

int commodity_stock(char *product_name,product *first_product){
    int stock=0;
    product *p=first_product;
    while(p!=NULL){
        if(strcmp(p->id,product_name)==0) {
            if(p->price==0){
                return -1;
            }
            stock+=p->qty;
        }
        p=p->next;
    }
    return stock;
}

void take_stock(char *product_name,int qty,product *first_product,int *uct,int *price,int *subcost){
    product *p=first_product;
    while(p!=NULL){
            //printf("subcost=%d\n",*subcost);
        if(strcmp(product_name,p->id)==0){
            *uct=p->unit_cost;
            *price=p->price;
            if(p->qty>=qty){
                p->qty-=qty;
                *subcost+=p->unit_cost*qty;
                qty=0;
                return;
            }
            else{
                qty-=p->qty;
                *subcost+=p->unit_cost*p->qty;
                p->qty=0;
            }
        }
        p=p->next;
    }
}

void take_gift(int *gift_count,product **first_product_p,product *gift,int *take_count){
    int min_profit=999999999,gotten_product=0;
    //printf("take gift\n");
    product *p=*first_product_p,*min_profit_product=*first_product_p;
    while(p!=NULL){
        if(p!=*first_product_p && strcmp(p->id,p->last->id)==0 && p->last->qty>0) {
            p=p->next;
            continue;
        }
        if(p->price==0 || p->qty==0) {
            p=p->next;
            continue;
        }
        int profit=p->price-p->unit_cost;
        if(!gotten_product || profit<min_profit || (profit==min_profit && strcmp(p->id,min_profit_product->id)<0)){
            gotten_product=1;
            min_profit=profit;
            min_profit_product=p;
        }
        p=p->next;
    }
    if(!gotten_product){
        //no gift can take
        min_profit_product=NULL;
        gift->batch=-1;
        *take_count=-1;
        return;
    }
    //printf("gift:%s qty=%d,gift_count=%d\n",min_profit_product->id,min_profit_product->qty,*gift_count);
    //take gift
    if(*gift_count<min_profit_product->qty){
        *take_count=*gift_count;
        min_profit_product->qty-=*gift_count;
        (*gift_count)=0;
        *gift=*min_profit_product;
    }
    else{
        *take_count=min_profit_product->qty;
        (*gift_count)-=*take_count;
        *gift=*min_profit_product;
        min_profit_product->qty=0;
        //delete_a_product(min_profit_product,first_product_p,last_product_p,len);
    }
    //printf("gift count=%d,take count=%d\n",*gift_count,*take_count);
}

int check_out(
    char *cid,
    cart current_cart[],
    product **first_product_p,
    promotion promo,
    int *acc_revenue,
    int *acc_cost,
    int *acc_gift_cost
){
    int total=0,initial_cost=0,gift_cost=0;
    //check cart exist
    int cart_ind=cart_exist(cid,current_cart);
    if(cart_ind==-1){
        printf("ERR NO_CART\n");
        return 0;
    }
    //check stock and price
    product *p_inCart=current_cart[cart_ind].first_product;
    int have_promotion=0;
    while(p_inCart!=NULL){
        int stock=commodity_stock(p_inCart->id,*first_product_p);
        if(stock==-1){
            printf("ERR NO_PRICE\n");
            return 0;
        }
        if(stock<p_inCart->qty){
            printf("ERR INSUFFICIENT %s\n",p_inCart->id);
            return 0;
        }
        if(promo.percentage!=0 && strcmp(promo.id_a,p_inCart->id)==0){
            if(p_inCart->qty>=promo.qty_condition) have_promotion=1;
        }
        p_inCart=p_inCart->next;
    }
    //start take stock
    printf("RECEIPT %s\n",current_cart[cart_ind].id);
    p_inCart=current_cart[cart_ind].first_product;
    while(p_inCart!=NULL){
        int uct=0,price=0,subcost=0;
        take_stock(p_inCart->id,p_inCart->qty,*first_product_p,&uct,&price,&subcost);
        int promo_percentage=100;
        if(have_promotion && strcmp(promo.id_b,p_inCart->id)==0){
            promo_percentage=promo.percentage;
        }
        int subtotal=(p_inCart->qty*price*promo_percentage+50)/100;
        if(promo_percentage==100) printf("  %s x%d @%d = %d\n",p_inCart->id,p_inCart->qty,price,subtotal);
        else printf("  %s x%d @%d *%d%% = %d\n",p_inCart->id,p_inCart->qty,price,promo_percentage,subtotal);
        initial_cost+=subcost;
        total+=subtotal;
        p_inCart=p_inCart->next;
    }
    printf("TOTAL %d\n",total);
    printf("COST %d\n",initial_cost);
    //free empty item
    /*product *p=*first_product_p;
    while(p!=NULL){
        product *p_cpy=p;
        p=p->next;
        if(p_cpy->qty==0){
            delete_a_product(p_cpy,first_product_p,last_product_p,len);
        }
    }*/
    //take gift
    int gift_count=total/1000,actual_total_gift_taken=0;
    while(gift_count>0){
        product gift=**first_product_p;
        gift.batch=-1;
        int take_count=0;
        take_gift(&gift_count,first_product_p,&gift,&take_count);
        if(take_count==-1){
            break;
        }
        actual_total_gift_taken+=take_count;
        for(int i=1;i<=take_count;i++) printf("GIFT %s @%d\n",gift.id,gift.unit_cost);
        gift_cost+=gift.unit_cost*take_count;
    }
    if(actual_total_gift_taken==0) printf("GIFT none\n");
    if(gift_count>0) printf("GIFT_SHORT %d\n",gift_count);
    printf("PROFIT %d\n",total-initial_cost-gift_cost);
    *acc_revenue+=total;
    *acc_cost+=initial_cost;
    *acc_gift_cost+=gift_cost;
    return 0;
}
