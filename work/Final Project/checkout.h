#ifndef checkout_H
#define checkout_H
#include "product_manager.h"
#include "promotion.h"
int check_out(
    char *cid,
    cart current_cart[],
    product **first_product_p,
    product **last_product_p,
    int *len,
    promotion promo,
    int *acc_revenue,
    int *acc_cost,
    int *acc_gift_cost
);
#endif // checkout_H
