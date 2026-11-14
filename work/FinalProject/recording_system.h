#ifndef recording_system_H
#define recording_system_H
#include "promotion.h"
#include "product_manager.h"
int save(
    char* saving_file,
    product *first_product,
    promotion promo,
    int revenue,
    int cost,
    int gift_cost,
    int next_batch
);
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
);
#endif // recording_system
