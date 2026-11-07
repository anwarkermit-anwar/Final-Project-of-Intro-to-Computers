#ifndef product_manager_H
#define product_manager_H
#include "product.h"
void push_a_product(product **head_p,product **tail_p,char *id,char *name,int qty,int uct,int batch,int *len);
void delete_a_product(product *this_product,product **head_p,product **tail_p,int *len);
int add_a_new_cart(char *id,cart *current_cart);
int delete_a_cart(char *id,cart *current_cart);
int add_product_to_a_cart(char *buffer,cart *current_cart,product *first_product_in_list);
#endif // product_manager_H
