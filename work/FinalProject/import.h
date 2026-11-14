#ifndef import_H
#define import_H
#include "product.h"
int import(
    char* file_name,
    product **first_product,
    product **last_product,
    int* len,
    int* next_batch
);
#endif // import_H
