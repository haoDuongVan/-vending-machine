#ifndef PRODUCT_H
#define PRODUCT_H

struct product {
    int no;
    char code[5];
    char name[15];
    int price;
    int inventory;
    int sold_quantity;
    int status;
};

void readProductFromFile(struct product *productList, int *quan);
void writeProductOut(struct product *productList, int quan);
void showOneProduct(struct product pr);
void showAllProduct(struct product *pr, int quan );
void updateProduct(struct product *productList, int quan, int no);
void updateProductInventory(struct product *productList, int quan, int no);
void deleteProduct(struct product *productList, int quan, int no);
int canBuy(struct product *pr, int choice, int charge);

#endif