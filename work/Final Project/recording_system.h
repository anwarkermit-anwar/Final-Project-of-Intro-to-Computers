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
int load(char* loading_file);
#endif // recording_system
