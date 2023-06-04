#include"product.h"

void readProductFromFile(struct product *productList, int *quan){
    FILE *productFile = fopen("shohin.csv", "r");
    int i =0;
    while (fscanf(productFile, "%d,%[^,],%[^,],%d,%d,%d,%d\n", &productList[i].no, productList[i].code, productList[i].name, &productList[i].price, &productList[i].inventory, &productList[i].sold_quantity, &productList[i].status) == 7) {
        i++;
    }
    fclose(productFile);
    *quan = i;
}
void writeProductOut(struct product *productList, int quan) {
    FILE *productFileOut = fopen("shohin.csv", "w");
    int i;
    for(i=0; i<quan; i++){
        struct product pr = productList[i];
        fprintf(productFileOut, "%d,%s,%s,%d,%d,%d,%d\n", pr.no, pr.code, pr.name, pr.price, pr.inventory, pr.sold_quantity, pr.status);
    }
    fclose(productFileOut);
}
void showOneProduct(struct product pr){
    printf(" %-4d %-6s  %-15s %-7d %-5d\n", pr.no,pr.code, pr.name, pr.price, pr.inventory);
}
void showAllProduct(struct product *pr, int quan ){
    printf("%s  %s  %s      %s    %s\n", "番号","コード", "商品名", "値段", "残り");
    printf("---------------------------------------------------\n");
    int i;
    for(i=0; i<quan; i++) {
        showOneProduct(pr[i]);
    }
    printf("---------------------------------------------------\n");
}
void updateProduct(struct product *productList, int quan, int no) {
    for(int i = 0; i < quan; i++) {
        if(productList[i].no == no) {
            printf("\n名簿コードを入力してください：");
            scanf("%s", productList[i].code);
            fflush(stdin);
            printf("\n商品名を入力してください：");
            scanf("%s", productList[i].name);
            fflush(stdin);
            // getchar();
            printf("\n単価を入力してください：");
            scanf("%d", &productList[i].price);
            getchar();
            if(scanf("%s", productList[i].code) != 1 || scanf("%s", productList[i].name) != 1 || scanf("%d", &productList[i].price) != 1) {
                printf("入力したデータが正しくないため、う一度入力してください。\n");
                break;
            }
        }
    }
}
void updateProductInventory(struct product *productList, int quan, int no) {
    for(int i = 0; i < quan; i++) {
        if(productList[i].no == no) {
            printf("\n商品の数を入力してください： ");
            scanf("%d", &productList[i].inventory);
            getchar();
        }
    }
}
void deleteProduct(struct product *productList, int quan, int no){
    for(int i = 0; i < quan; i++){
        if(productList[i].no == no){
            for(int j =i; j < quan; j++){
                productList[j] = productList[j + 1];
            }
            quan--;
            break;
        }
    }
}
int canBuy(struct product *pr, int choice, int charge){
    choice--;
    if((pr[choice].inventory > 0) && (charge >= pr[choice].price)) {
        pr[choice].status = 1;
        return 1;
    } else {
        pr[choice].status = 0;
        return 0;
    }
}