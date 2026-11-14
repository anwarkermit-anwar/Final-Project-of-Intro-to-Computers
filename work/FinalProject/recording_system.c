#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "promotion.h"
#include "product_manager.h"
#include "small_operations.h"
#define Max_cart_count 10

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
    fprintf(file,"TOTALS %d %d %d\n",revenue,cost,gift_cost);
    //batches
    p=first_product;
    fprintf(file,"BATCHES %d\n",batch_count(first_product));
    while(p!=NULL){
        if(p->qty>0){
            fprintf(file,"%s %d %d %d\n",p->id,p->batch,p->qty,p->unit_cost);
        }
        p=p->next;
    }
    fclose(file);
    printf("OK SAVE\n");
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
        p=p->next;
    }
    return NULL;
}

char* find_first_space(char* s){
    while(*s!=' ' && *s!='\0') s++;
    if(*s=='\0') return NULL;
    return s;
}

int have_product(char *id,product *first_product){
    product *p=first_product;
    while(p!=NULL){
        if(strcmp(id,p->id)==0) return 1;
        p=p->next;
    }
    return 0;
}

void visit_all_commodity_in_list(product *first_commodity,product **first_product_p,product **last_product_p,int *len){
    product *p_com=first_commodity;
    while(p_com!=NULL){
        if(!have_product(p_com->id,*first_product_p)){
            char *id=(char*)calloc(strlen(p_com->id)+1,sizeof(char));
            char *name=(char*)calloc(strlen(p_com->name)+1,sizeof(char));
            strcpy(id,p_com->id);
            strcpy(name,p_com->name);
            push_a_product(first_product_p,last_product_p,id,name,0,0,0,p_com->price,len);
        }
        p_com=p_com->next;
    }
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
    int *next_batch_p,
    cart current_cart[]
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
    for(int i=0;i<Max_cart_count;i++){
        if(current_cart[i].id!=NULL){
            char *id_cpy=(char*)calloc(strlen(current_cart[i].id)+1,sizeof(char));
            strcpy(id_cpy,current_cart[i].id);
            delete_a_cart(id_cpy,current_cart);
            free(id_cpy);
        }
    }
    //printf("clear old data");
    //next_batch
    *next_batch_p=0;
    char buffer[1000];
    fgets(buffer,sizeof(buffer),file);
    for(int i=11;buffer[i]!='\n';i++){
        (*next_batch_p)*=10;
        (*next_batch_p)+=buffer[i]-'0';
    }
    //printf("update next_batch=%d\n",*next_batch_p);
    //commodity list
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
        char *id_end=find_first_space(buffer);
        if(id_end==NULL){
            printf("Space Not Found In Buffer!\n");
            return 1;
        }
        char *id=(char*)calloc(id_end-buffer+1,sizeof(char));
        for(int i=0;(buffer+i)!=id_end;i++){
            *(id+i)=buffer[i];
        }
        char *price_st=id_end+1;
        int price=0,sign=1;
        for(int i=0;*(price_st+i)!='\n';i++){
            if(*(price_st+i)=='-'){
                sign=-1;
                continue;
            }
            price*=10;
            price+=*(price_st+i)-'0';
        }
        price*=sign;
        if(price==-1) price=0;
        fgets(buffer,sizeof(buffer),file);
        buffer[strcspn(buffer,"\n")]='\0';
        char *name=(char*)malloc((strlen(buffer)+1)*sizeof(char));
        strcpy(name,buffer);
        push_a_product(&first_commodity,&last_commodity,id,name,0,0,0,price,&commodity_len);
    }
    //printf("get commodity list\n");
    //Print_current_commodity_status(first_commodity);
    //promotion
    fgets(buffer,sizeof(buffer),file);
    if(buffer[6]=='1'){
        fgets(buffer,sizeof(buffer),file);
        buffer[strcspn(buffer,"\n")]='\0';
        set_promotion(buffer,promo_p);
    }
    //printf("update promotion=%s,%d,%s,%d%%\n",promo_p->id_a,promo_p->qty_condition,promo_p->id_b,promo_p->percentage);
    //total
    fgets(buffer,sizeof(buffer),file);
    int ty=1;
    for(int i=7;buffer[i]!='\n';i++){
        if(buffer[i]==' ') {
            ty++;
            continue;
        }
        if(ty==1){
            (*revenue_p)*=10;
            (*revenue_p)+=buffer[i]-'0';
            //printf("ty=%d,revenue+=%d\n",ty,buffer[i]-'0');
        }
        if(ty==2){
            (*cost_p)*=10;
            (*cost_p)+=buffer[i]-'0';
            //printf("ty=%d,cost+=%d\n",ty,buffer[i]-'0');
        }
        if(ty==3){
            (*gift_cost_p)*=10;
            (*gift_cost_p)+=buffer[i]-'0';
            //printf("ty=%d,gift_cost+=%d\n",ty,buffer[i]-'0');
        }
    }
    //printf("update: revenue=%d,cost=%d,gift_cost=%d\n",*revenue_p,*cost_p,*gift_cost_p);
    //batches
    fgets(buffer,sizeof(buffer),file);
    int batches=0;
    for(int i=8;buffer[i]!='\n';i++){
        batches*=10;
        batches+=buffer[i]-'0';
    }
    while(batches--){
        fgets(buffer,sizeof(buffer),file);
        int st[5],en[5],stage=0;
        st[0]=0;
        for(int i=0;i<(int)strlen(buffer);i++){
            if(buffer[i]==' ' || buffer[i]=='\n'){
                en[stage]=i-1;
                stage++;
                st[stage]=i+1;
                if(buffer[i]=='\n') break;
            }
        }
        char *id=(char*)calloc(en[0]-st[0]+2,sizeof(char));
        for(int i=st[0];i<=en[0];i++){
            *(id+i)=buffer[i];
        }
        *(id+en[0]+1)='\0';
        int batch_id=0;
        for(int i=st[1];i<=en[1];i++){
            batch_id*=10;
            batch_id+=buffer[i]-'0';
        }
        int qty=0;
        for(int i=st[2];i<=en[2];i++){
            qty*=10;
            qty+=buffer[i]-'0';
        }
        int uct=0;
        for(int i=st[3];i<=en[3];i++){
            uct*=10;
            uct+=buffer[i]-'0';
        }
        //printf("id=%s,qty=%d,uct=%d\n",id,qty,uct);
        product *commodity=find_product(id,first_commodity);
        if(commodity==NULL){
            printf("ERROR Commodity Not Found!!!\n");
            fclose(file);
            clear_product_list(&first_commodity,&last_commodity,&commodity_len);
            return 1;
        }
        char *name=(char*)calloc(strlen(commodity->name)+1,sizeof(char));
        strcpy(name,commodity->name);
        push_a_product(
            first_product_p,
            last_product_p,
            id,
            name,
            qty,
            uct,
            batch_id,
            commodity->price,
            len
        );
    }
    visit_all_commodity_in_list(first_commodity,first_product_p,last_product_p,len);
    //printf("complete product list\n");
    clear_product_list(&first_commodity,&last_commodity,&commodity_len);
    fclose(file);
    printf("OK LOAD\n");
    return 0;
}
