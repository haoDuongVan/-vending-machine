#ifndef TRANSACTION_H
#define TRANSACTION_H

struct transaction {
    // int transactionNo;
    char date[15];
    struct product product_r;//code, name, price, sold_quan
    int amount; // 属性：6
};

void readTransactionFromFile(struct transaction *transactionList, int *quan);
void overwriteLine(const char* date, const char* productName, struct transaction new_t);
struct transaction createTransaction(char *code, const char *name, int price, int sold_quan);
void showAllProfit(struct transaction *rec, int quan, char *date);
void showOneProfit(struct transaction rec);
void resetSoldForNewDay(struct transaction *transactionList, int quan, struct tm *lastResetTime);
int isResetTime(struct tm *lastResetTime);
int loadProfitAmount(struct transaction *rec, int quan);


#endif