#include"transaction.h"
#include"product.h"


void readTransactionFromFile(struct transaction *transactionList, int *quan){
    FILE *transactionFile = fopen("TransactionHistory.csv", "r");
    int i =0;
    while (fscanf(transactionFile, "%[^,],%[^,],%[^,],%d,%d,%d\n",transactionList[i].date,transactionList[i].product_r.code,
            transactionList[i].product_r.name,&transactionList[i].product_r.price,&transactionList[i].product_r.sold_quantity,&transactionList[i].amount) == 6) {
        i++;
    }
    fclose(transactionFile);
    *quan = i;
}
void overwriteLine(const char* date, const char* productName, struct transaction new_t) {
    FILE* file = fopen("TransactionHistory.csv", "r");
    // if (file == NULL) {
    //     printf("ファイルを開けない.\n");
    //     return;
    // }
    char newLine[100];
    sprintf(newLine, "%s,%s,%s,%d,%d,%d\n", new_t.date,
    new_t.product_r.code, new_t.product_r.name,new_t.product_r.price,
    new_t.product_r.sold_quantity, new_t.product_r.price * new_t.product_r.sold_quantity);
    
    FILE* tempFile = fopen("temp.csv", "w");
    // if (tempFile == NULL) {
    //     printf("新規ファイルを作成できない.\n");
    //     fclose(file);
    //     return;
    // }

    char line[100];
    int isDuplicate = 0;

    // 元のファイルの一行ずつ新規ファイルに書き込む
    while (fgets(line, sizeof(line), file) != NULL) {
        char storedDate[20];
        char storedProductName[50];

        sscanf(line, "%[^,],%[^,]", storedDate, storedProductName);

        // 替えるかどうか確認（一致の場合：元の行を書き替える）
        if (strcmp(storedDate, date) == 0 && strcmp(storedProductName, productName) == 0) {
            // 一致すれば、新しい引取の歴史を出力
            fputs(newLine, tempFile);
            isDuplicate = 1;
        } else {
            // 一致しない：元の歴史（１行）を出力
            fputs(line, tempFile);
        }
    }

    // 日付や商品コードが一致しない場合は新規ファイルの最後に出力
    if (!isDuplicate) {
        fputs(newLine, tempFile);
    }

    fclose(file);
    fclose(tempFile);
    remove("TransactionHistory.csv");
    rename("temp.csv", "TransactionHistory.csv");
    // printf("条件を一致した行を書き換えました。\n");
}

void showOneProfit(struct transaction rec){
    printf("%-15s %-10s %-15s %-8d %-8d %-10d\n", rec.date,
        rec.product_r.code,rec.product_r.name,rec.product_r.sold_quantity,
        rec.product_r.price, rec.amount);
}
void showAllProfit(struct transaction *rec, int quan, char *date){
    printf("    %s\t%s \t%s    %10s   %s   %s \n","日付", "コード", "商品名", "販売個数", "単価", "集計");
    printf("---------------------------------\n");
    int i;
    for(i=0; i<quan; i++) {
        if(strcmp(rec[i].date,date)==0){
            showOneProfit(rec[i]);
        }
    }
    printf("---------------------------------\n");
}
void resetSoldForNewDay(struct transaction *transactionList, int quan, struct tm *lastResetTime) {
    if(isResetTime(lastResetTime)) {
        for(int i=0; i<quan; i++){
            transactionList[i].product_r.sold_quantity = 0;
        }

    time_t currentTime;
    time(&currentTime);
    *lastResetTime = *localtime(&currentTime);
    }
}
struct transaction createTransaction(char *code, const char *name, int price, int sold_quan){
    struct transaction rec;
    strcpy(rec.product_r.code, code);
    strcpy(rec.product_r.name, name);
    rec.product_r.price = price;
    rec.product_r.sold_quantity = sold_quan;
    rec.amount = price * sold_quan;
    //日付を登録
    time_t rawTime;
    struct tm *timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    strftime(rec.date, sizeof(rec.date), "%d/%m/%Y", timeInfo);
    return rec;
}

int loadProfitAmount(struct transaction *rec, int quan){
    int i, sum = 0;
    for(i=0; i<quan; i++) {
        sum = sum + rec[i].amount;
    }
    return sum;
}

int isResetTime(struct tm *lastResetTime){
    time_t currentTime;
    time(&currentTime);
    struct tm *currentTimeInfo = localtime(&currentTime);
    if(currentTimeInfo->tm_yday > lastResetTime->tm_yday || currentTimeInfo->tm_yday == lastResetTime->tm_yday && currentTimeInfo->tm_hour >= lastResetTime->tm_hour) {
        return 1;
    }
    return 0;
} 
