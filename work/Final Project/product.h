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

#endif // product_H
