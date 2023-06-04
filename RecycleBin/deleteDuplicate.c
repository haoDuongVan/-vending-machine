// #include<stdio.h>
// #include<stdlib.h>
// #include<string.h>
// #include<time.h>


// struct product {
//     int no;
//     char code[5];
//     char name[100];
//     int price;
//     int inventory;
//     int sold_quantity;
//     int status;
// };

// struct transaction {
//     // int transactionNo;
//     char date[50];
//     struct product product_r;//code, name, price, sold_quan
//     int amount; // 属性：6
// };
// void readTransactionFromFile(struct transaction *transactionList, int *quan){
//     FILE *transactionFile = fopen("hanbai.csv", "r");
//     int i =0;
//     while (fscanf(transactionFile, "%[^,],%[^,],%[^,],%d,%d,%d\n",transactionList[i].date,transactionList[i].product_r.code,
//             transactionList[i].product_r.name,&transactionList[i].product_r.price,&transactionList[i].product_r.sold_quantity,&transactionList[i].amount) == 6) {
//         i++;
//     }
//     fclose(transactionFile);
//     *quan = i;
// }
// void deleteDuplicateTranslation(struct transaction *transactionList, int quan){
//     FILE *p = fopen("hanbai.csv", "w") ;
    
//     for (int i = quan -1; i >= 0; i--) {
//         for(int j = i -1 ; j = 0; j--) {
//             //  printf("code[i] %s  :  code[j] %s\n", transactionList[i].product_r.code, transactionList[j].product_r.code);
//             if(strcmp(transactionList[i].date,transactionList[j].date) == 0) {
//                 if(strcmp(transactionList[i].product_r.code,transactionList[j].product_r.code) == 0) {
//                     transactionList[i] = transactionList[j];
//                     transactionList[j] = transactionList[j + 1];
//                     quan--;
//                     // printf("\nIt same \n");

//                     // for(int k= j; k =0; k--){
//                     //     transactionList[k] = transactionList[k-1];
//                     // }
//                     // quan--;
//                 }
                
//             }
//         }
//     }
//     for(int i= 0; i <quan; i++) {
//         struct transaction tr = transactionList[i];
//         printf("----------------------%d\n", i);
//         printf("%s %s %s %d %d %d\n", tr.date, tr.product_r.code, tr.product_r.name, tr.product_r.price, tr.product_r.sold_quantity,tr.product_r.sold_quantity*tr.product_r.price );
//         printf("----------------------\n");
    
//     }
//     fclose(p);
// }

// int main(){
//     struct transaction transactionList[1000];
//     int transaction_quantity = 0;
    
//     readTransactionFromFile(transactionList, &transaction_quantity);

//     deleteDuplicateTranslation(transactionList, transaction_quantity);

//     return 0;
// }

#include <stdio.h>
#include <string.h>

void overwriteOrAppendLine(const char* filename, const char* date, const char* productName, const char* newLine) {
    // M? file g?c ?? ??c
    FILE* file = fopen("hanbai.csv", "r+");
    if (file == NULL) {
        printf("Khong the mo file.\n");
        return;
    }

    char line[100];
    long int duplicatePos = -1;
    int isDuplicate = 0;

    // Tim dong trung va v? tri c?a no
    while (fgets(line, sizeof(line), file) != NULL) {
        char storedDate[20];
        char storedProductName[50];

        // L?y ngay thang va ten s?n ph?m t? dong ?ang xet
        sscanf(line, "%[^,],%[^,]", storedDate, storedProductName);
        printf("%s va %s\n", storedDate, storedProductName);
        printf("\tdate nhap:%s va gia tri so sanh:%d\n", date, strcmp(storedDate, date));
        printf("\t\tname nhap:%s va gia tri so sanh:%d\n\n", productName, strcmp(storedProductName, productName));
        // Ki?m tra xem dong ?ang xet co trung ngay thang va ten s?n ph?m khong
        if (strcmp(storedDate, date) == 0 && strcmp(storedProductName, productName) == 0) {
            // N?u trung, ?anh d?u la dong trung va l?u v? tri c?a dong
            isDuplicate = 1;
            duplicatePos = ftell(file);
            break;
        }
    }

    // N?u co dong trung, di chuy?n con tr? ??n v? tri dong trung va ghi ?e dong m?i
    if (isDuplicate && duplicatePos != -1) {
        fseek(file, duplicatePos - strlen(line) , SEEK_SET);
        fputs(newLine, file);
        fflush(stdin);
        printf("Da ghi de dong trung lap.\n");
    }
    // N?u khong co dong trung, di chuy?n con tr? ??n cu?i file va ghi dong m?i
    else {
        fseek(file, 0, SEEK_END);
        fputs(newLine, file);
        printf("Da them dong moi.\n");
    }

    // ?ong file
    fclose(file);
}

int main() {
    // G?i ham vi?t ?e ho?c them dong
    overwriteOrAppendLine("hanbai.csv", "03/06/2023", "A102", "03/06/2023,A102,mugicha,120,100,12000\n");

    return 0;
}
