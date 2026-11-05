#ifndef product_manager_H
#define product_manager_H
#include "product.h"
void push_a_product(product **head_p,product **tail_p,char *id,char *name,int qty,int uct,int batch,int *len);
void delete_a_product(product *this_product,product **head_p,product **tail_p,int *len);
#endif // product_manager_H
