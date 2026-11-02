#ifndef SMALL_OPERATIONS_H
#define SMALL_OPERATIONS_H
int call_set_price(char *buffer,int *price,char **id,int len);
int query(
    char *product_id,
    int* price,
    int* unit_cost,
    int* qty,
    char** id,
    char** name,
    int len
);
#endif // SMALL_OPERATIONS_H
