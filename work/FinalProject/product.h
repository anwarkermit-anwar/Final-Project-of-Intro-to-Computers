#ifndef product_H
#define product_H
typedef struct product{
    char *id;
    char *name;
    int price;
    int unit_cost;
    int qty;
    int batch;
    struct product *last;
    struct product *next;
}product;
typedef struct cart{
    char *id;
    product *first_product;
    product *last_product;
    int product_list_len;
}cart;

#endif // product_H
