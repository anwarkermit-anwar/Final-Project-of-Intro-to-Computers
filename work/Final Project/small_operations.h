#ifndef SMALL_OPERATIONS_H
#define SMALL_OPERATIONS_H
#include "promotion.h"
int call_set_price(char *buffer,product *first_product,int len);
int query(
    char *product_id,
    product *first_product,
    int len
);
int set_promotion(char *buffer,struct promotion *promo_p);
#endif // SMALL_OPERATIONS_H
