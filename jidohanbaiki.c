#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>


#define CHARGE_MAX 1990
#define KANRI_NUM 111
#define UNPURCHASEABLE 201

struct product {
    int no;
    char code[5];
    char name[100];
    int price;
    int inventory;
    int sold_quantity;
    int status;
};
struct transaction {
    // int transactionNo;
    char date[50];
    struct product product_r;//code, name, price, sold_quan
    int amount; // 属性：6
};
struct change {
    int no;
    int kingaku;
    int quan;
    int amount;
};
struct tempChange {
    int kingaku;
    int quan;
    int amount;
};

void showManagerMenu();
struct transaction createTransaction(char *code, const char *name, int price, int sold_quan);
void updateProduct(struct product *productList, int quan, int no);
void updateProductInventory(struct product *productList, int quan, int no);
void showTitle();
void showOneProduct(struct product pr);
void showAllProduct(struct product *pr, int quan );
int canBuy(struct product *pr, int choice, int charge);
void purchaseableAllProductDisplay(struct product *pr, int quan, int charge);
void showOneChange(struct change ch);
void showAllChange(struct change *ch, int quan);
void increaseChange(struct change *changeList, int quan, int upChange);
void decreaseChange(struct change *changeList, int quan, int backChange);
int loadAmount(struct change *ch, int quan);
int loadProfitAmount(struct transaction *rec, int quan);
void readProductFromFile(struct product *productList, int *quan);
void writeProductOut(struct product *productList, int quan);
void readTransactionFromFile(struct transaction *transactionList, int *quan);
void writeTransactionOut(struct transaction *transactionList, int quan);
void readChangeFromFile(struct change *changeList, int *quan);
void writeChangeOut(struct change *changeList, int quan);
void showErrorMessage(int *errorCode);

int main(){
    struct product productList[31];
    int product_quantity = 0;
    int choice, manager_choice;
    
    //利用者画面の変数
    int money = 0;//投入した金額
    int charge = 0;//チャージする金額
    int sumGiven = 0;
    char split;//割り込み処理（＠の場合はプログラムを終了する。）
    int changeBack = 0; //返すお釣りの金額
    int product_choice;
    int purchase_status;

    //釣り金を返すため、釣り金を設定する。
    struct change changeList[6];
    int change_quantity = 0;
    int Sumchange = 0;//釣銭の合計

    //レシートと売上確認
    struct transaction transactionList[1000];
    int transaction_quantity = 0;
    int transaction_ID = 1;
    int Sumprofit = 0;//総売上高 

    //error 処理
    int chargeError = 0;
    int purchaseError = 0;
    int errorCode = 0;

    char inputPassword[128];
    char PASSWORD[] = "kanrisha";
    int certificationStatus = 0;
    int authAbleCount = 3;

    int purchasable = 0;

    //get date time
    char nowDate[50];
    time_t rawTime;
    struct tm *timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    strftime(nowDate, sizeof(nowDate), "%d/%m/%Y", timeInfo);

    //外部ファイから商品を読み込む
    readProductFromFile(productList, &product_quantity);
    readChangeFromFile(changeList, &change_quantity);
    readTransactionFromFile(transactionList, &transaction_quantity);
    Sumchange = loadAmount(changeList, change_quantity);
    Sumprofit = loadProfitAmount(transactionList, transaction_quantity);

    do
    {
        printf("\n           *********************** 自動販売機 ***********************   \n\n");
        // allProductDisplay(productList, product_quantity);
        purchaseableAllProductDisplay(productList, product_quantity, sumGiven);
        printf("\n");

        // show message (error + success)
        if(errorCode != 0) {
            showErrorMessage(&errorCode);
        }
        if(purchase_status == 1) {
            printf("商品 %d を購入しました。\n", product_choice);
            purchase_status--;
        }


        printf("\n-------------------------------------------------------------\n");
        // printf("\t\t\t\t 投入金額： %d 円\n\n", sumGiven);
        printf("\t\t\t 投入金額： %d 円\n", sumGiven);
        printf("\t1.金銭投入\n");
        printf("\t2.商品選択\n");
        printf("\t3.お釣り返却\n\n");
        // printf("4.終了\n");
        //printf("111.管理者画面");
        printf("\n\t\t問い合わせ:0xx-xxxx-xxxx\n");
        printf("\n  *****************************************************   \n");
        printf("\nお客さんはこれから何を選びますか？ ");
        
        scanf("%d", &choice);


        switch (choice)
        {
        case 1: // 金銭投入 -- 
            while(1){
                printf("\n           *********************** 自動販売機 ***********************   \n\n");
                purchaseableAllProductDisplay(productList, product_quantity, sumGiven);
                printf("\n\n              **********金銭投入**********             \n\n");
                printf("\t\t\t 投入金額： %d 円\n", sumGiven);
                if(errorCode != 0) {
                    showErrorMessage(&errorCode);
                }
                printf("利用可能：10円 50円 100円 500円 1000円\n投入上限は：1990円\n");
                printf("\n\n中止の場合は「＠」を入力してください。\n");
                printf("\nお金を投入してください：");
                fflush(stdin);
                if((scanf("%d", &charge))==1) {

                    if((charge <= CHARGE_MAX) && ((charge %10) == 0) && ((charge == 10)||(charge == 50)||(charge == 100)||(charge == 500)||(charge == 1000))){
                        
                        sumGiven += charge;
                        if(sumGiven > 1990) {
                            sumGiven -= charge;
                            charge = 0;
                            break;
                        }
                        //金銭の枚数を増やして、外部ファイルに保存
                        increaseChange(changeList, change_quantity, charge);
                        writeChangeOut(changeList, change_quantity);
                        changeBack = sumGiven;
                        charge = 0;
                        // system("cls");
                        break;
                    }
                    else{
                        // error shori
                        errorCode = 101;
                        charge = 0;
                        // chargeError = 1;
                        continue;
                    }
                }
                else {
                    scanf("%c", &split);
                    if(split == '@') {
                        break;
                    }
                    else {
                        printf("\aもう一回入力してください。");
                        continue;
                    }
                }
                printf("\n\n終了の場合は「＠」を入力してください。\n");
            }
            // system("cls");
            break;
        case 2://商品選択
            while(1) {
                printf("\n           *********************** 自動販売機 ***********************   \n\n");
                purchaseableAllProductDisplay(productList, product_quantity, sumGiven);
                if(errorCode != 0) {
                    showErrorMessage(&errorCode);
                }
                printf("\n\n              ********** 商品選択 **********             \n\n");
                printf("\t\t\t 投入金額： %d 円\n\n", sumGiven);
            
                printf("\n\n終了の場合は「＠」を入力してください。\n\n");

                printf("購入商品の商品番号を入力してください(1 -> 6)：\n");
                
                if((scanf("%d", &product_choice)) == 1) {
                    //購入可能？
                    purchasable = canBuy(productList, product_choice, sumGiven);
                    //購入成功
                    if((product_choice > 0) && (product_choice <= 6) && purchasable){
                        --product_choice;
                        sumGiven = changeBack -= productList[product_choice].price;
                        // printf("product %d 's price is %d\n", product_choice, productList[product_choice-1].price);
                        productList[product_choice].inventory--;
                        purchase_status++;
                        // 売上計算を行う。
                        struct transaction transaction;
                        productList[product_choice].sold_quantity++;
                        transaction = createTransaction(productList[product_choice].code, productList[product_choice].name,productList[product_choice].price, productList[product_choice].sold_quantity);
                        transactionList[transaction_quantity++] = transaction;
                        writeTransactionOut(transactionList, transaction_quantity);
                        // deleteSameTransaction(transactionList, transaction_quantity, productList[product_choice].code, nowDate);
                        ++product_choice;

                        break;
                    }
                    else{
                        if(sumGiven < productList[product_choice-1].price) {
                            errorCode = UNPURCHASEABLE;
                        } else {
                            errorCode = 202;
                        }
                        continue;
                    }
                }
                //前の画面に戻る
                else {
                    scanf("%c", &split);
                    if(split == '@') {
                        break;
                    }
                    else {
                        printf("\aもう一回入力してください。");
                        continue;
                    }
                }
                break;

            }
            // system("cls");
            break;

        case 3://購入完了で、釣銭を返却
            sumGiven = 0;
            //お釣りを返す（投入金の残高を硬貨ごとに計算する）、機内の硬貨を減らす。
            decreaseChange(changeList, change_quantity, changeBack);
            //金銭や商品の状態を外部ファイルに保存
            writeProductOut(productList, product_quantity);
            writeChangeOut(changeList, change_quantity);
            break;
        
        case 4://終了
            writeProductOut(productList, product_quantity);
            writeChangeOut(changeList, change_quantity);
            printf("プログラムを終了しました。");
            break;

        case KANRI_NUM: //管理者用の画面に移し 
            //パスワードで認証する機能        
            do
            {
                printf("管理者のパスワードを入力してください。\n");
                fflush(stdin);
                gets(inputPassword);
                certificationStatus = strcmp(inputPassword, PASSWORD);
                if(certificationStatus == 0){
                    printf("認証できました。\n");
                    //認証できたら、
                    do {
                        int productNo;
                        char date[15];
                        char back;

                        printf("\n    ***************管理者画面***************   \n");
                        //showAllProduct(productList, product_quantity);
                        //printf("\n");
                        //printf("\n---------------------------------------------------\n");
                        showManagerMenu();

                        scanf("%d", &manager_choice);
                        struct product product;

                        switch (manager_choice)
                        {
                        case 1: //商品一覧表 --OK
                            printf("\n---------------------------------------------------\n");
                            showAllProduct(productList, product_quantity);
                            printf("\n---------------------------------------------------\n");
                            printf("戻りますか (Y/N) : ");
                            while(1) {
                                fflush(stdin);
                                back = getchar();
                                if((back == 'Y') || (back == 'y')) {
                                    break;
                                }
                            }
                            
                            break;
                        case 2://商品修正  -- OK
                        //全商品を表示する
                            printf("\n---------------------------------------------------\n");
                            showAllProduct(productList, product_quantity);
                            printf("\n---------------------------------------------------\n");
                            
                            printf("変更商品の番号を入力してください：");
                            scanf("%d", &productNo);
                            updateProduct(productList, product_quantity, productNo);
                            writeProductOut(productList, product_quantity);
                            break;

                        case 3: // 商品追加 -- OK
                            // product = createProduct();
                            // productList[product_quantity++] = product;
                            // writeProductOut(productList, product_quantity);
                            // break;

                        case 4://売上高確認
                            printf("\n");
                            printf("\n---------------------------------------------------\n");
                            // showAllProfit(transactionList, transaction_quantity);
                            printf("\n\t  売上高の総金額：  %-10d\n", Sumprofit);
                            printf("\n---------------------------------------------------\n");
                            printf("戻りますか (Y/N) : ");
                            while(1) {
                                fflush(stdin);
                                back = getchar();
                                if((back == 'Y') || (back == 'y')) {
                                    break;
                                }
                            }

                            break;

                        case 5://残高確認・回収
                            printf("\n");
                            struct change change;
                            // change = createChange();
                            // changeList[change_quantity++] = change;
                            // writeChangeOut(changeList, change_quantity);
                            printf("\n---------------------------------------------------\n");
                            showAllChange(changeList, change_quantity);
                            printf("\n\t  準備金の総金額：  %-10d\n", Sumchange);
                            printf("\n---------------------------------------------------\n");
                            printf("戻りますか (Y/N) : ");
                            while(1) {
                                fflush(stdin);
                                back = getchar();
                                if((back == 'Y') || (back == 'y')) {
                                    break;
                                }
                            }
                            break;

                        case 6://商品補充 -- OK
                            
                            printf("商品番号を入力してください：");
                            scanf("%d", &productNo);
                            updateProductInventory(productList, product_quantity, productNo);

                            break;

                        case 7://終了
                            writeProductOut(productList, product_quantity);
                            printf("プログラムを終了しました。");
                            break;
                        default:
                            printf("違います。もう一回入力してください。");
                            break;
                        }
                    } while (manager_choice != 7);
                    break;
                }
                // printf("入力したパスワードが正しくない。\n");
                authAbleCount--;
                if(authAbleCount == 0) {
                    printf("\n\t\x1b[31m 警察を読んでいます！ \x1b[0m\n");
                    printf("\n\t\x1b[31m お前はもう死んでいる！ \x1b[0m\n");
                    for(int i = 0; i < 100000; i++) {
                        for(int j = 0; j < 10000; j++) {}
                    }
                    break;
                }
            } while (certificationStatus != 0);
                     
        default:
            // printf("違います。もう一回入力してください。\n");
            break;

        }
    } while (choice != 4);
}



void showManagerMenu(){
    printf("1.商品一覧表\n");
    printf("2.商品変更・修正\n");
    printf("3.商品追加\n");
    printf("4.売上高確認\n");
    printf("5.金銭確認・回収\n");
    printf("6.商品補充\n");
    printf("7.終了\n");
    printf("\n\n  **************************************   \n");
    printf("\nあなたはこれから何を選びますか？ ");
}

// struct product createProduct(){
//     struct product product;
//     printf("商品番号を入力してください：");
//     scanf("%d", &product.no);
//     printf("名簿コードを入力してください：");
//     scanf("%s", product.code);
//     printf("商品名を入力してください：");
//     scanf("%s", product.name);
//     printf("単価を入力してください：");
//     scanf("%d", &product.price);
//     printf("商品の数を入力してください：");
//     scanf("%d", &product.inventory);
//     return product;
// }
// struct change createChange(){
//     struct change change;
//     printf("Noを入力してください：");
//     scanf("%d", &change.no);
//     printf("金銭の金額を入力してください：");
//     scanf("%d", &change.kingaku);
//     printf("枚数を入力してください：");
//     scanf("%d", &change.quan);
//     // change.amount = change.kingaku * change.quan;
//     return change;
// }
// struct Transaction createTransaction() {
//     struct Transaction Transaction;
//     // struct product product;
//     printf("レシート番号を入力してください：");
//     scanf("%d", &Transaction.TransactionNo);
//     printf("日付を入力してください：");
//     scanf("%s", &Transaction.date);
//     printf("名簿コードを入力してください：");
//     scanf("%s", &Transaction.product_r.code);
//     printf("商品名を入力してください：");
//     scanf("%s", &Transaction.product_r.name);
//     printf("販売数量を入力してください：");
//     scanf("%d", &Transaction.product_r.sold_quantity);
//     printf("商品単価を入力してください：");
//     scanf("%d", &Transaction.product_r.price);
//     printf("売上高を入力してください：");
//     scanf("%d", &Transaction.amount);
//     return Transaction;
// }
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

// void deleteSameTransaction(struct transaction *transactionList, int quan, char *code, char *date){
//     FILE *transactionFileOut = fopen("TransactionHistory.csv", "w");
//     int i;
//     for(i=0; i<quan; i++){
//         struct transaction rec = transactionList[i];
//         if(((strcmp(transactionList[i].product_r.code,code)) == 0) && ((strcmp(transactionList[i].date,date)) == 0)){
//             for(int j =i; j < quan; j++){
//                 transactionList[j] = transactionList[j + 1];
//             }
//             quan--;
//         }
//         fprintf(transactionFileOut, "%s,%s,%s,%d,%d,%d\n", rec.date,rec.product_r.code,
//         rec.product_r.name,rec.product_r.price,rec.product_r.sold_quantity,rec.amount);
//     }
//     fclose(transactionFileOut);
// }

void updateProduct(struct product *productList, int quan, int no) {
    for(int i = 0; i < quan; i++) {
        if(productList[i].no == no) {
            printf("商品番号を入力してください：");
            scanf("%d", &productList[i].no);
            printf("名簿コードを入力してください：");
            scanf("%s", productList[i].code);
            printf("商品名を入力してください：");
            scanf("%s", productList[i].name);
            printf("単価を入力してください：");
            scanf("%d", &productList[i].price);
        }
    }
}

void updateProductInventory(struct product *productList, int quan, int no) {
    for(int i = 0; i < quan; i++) {
        if(productList[i].no == no) {
            printf("商品の数を入力してください：");
            scanf("%d", &productList[i].inventory);
            
        }
    }
}

// void deleteProduct(struct product *productList, int quan, int no){
//     for(int i = 0; i < quan; i++){
//         if(productList[i].no == no){
//             for(int j =i; j < quan; j++){
//                 productList[j] = productList[j + 1];
//             }
//             quan--;
//             break;
//         }
//     }
// }

void showTitle(){
    printf("%-5s %-7s \t%-10s \t%-7s    %-5s\n", "番号","名簿コード", "商品名", "値段", "残り");
}

void showOneProduct(struct product pr){
    printf("%-5d %-15s %-15s %-10d %-5d\n", pr.no,pr.code, pr.name, pr.price, pr.inventory);
}
void showAllProduct(struct product *pr, int quan ){
    showTitle();
    printf("---------------------------------------------------\n");
    int i;
    for(i=0; i<quan; i++) {
        showOneProduct(pr[i]);
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

// void purchaseErrorMessage_out_of_stock(){
//     printf("\n\x1b[31m 在庫がない為、購入できませんでした。\x1b[0m\n\n");
// }
// void purchaseErrorMessage_not_enough_money(){
//     printf("\n\x1b[31m 購入できませんでした。\x1b[0m\n\n");
// }
// void chargeErrorMessage(){
//     printf("\n\x1b[31m 該当な金銭を投入してください。\x1b[0m\n\n");
// }

void purchaseableAllProductDisplay(struct product *pr, int quan, int charge){
    int i;
    printf("  --------------------------------------------------------------------------------\n");
    printf(" | %-10s", "番号   |");
    for(i=0; i<quan; i++) {
        printf("   %3d     |", i+1);
    }
    printf("\n | %-10s", "商品名 |");
    for(i=0; i<quan; i++) {
        printf("  %-8s |", pr[i].name);
    }
    printf("\n | %-10s", "値段   |");
    for(i=0; i<quan; i++) {
        if(charge >= pr[i].price){
            printf("\x1b[32m %6d \x1b[0m   |", pr[i].price);
        }
        else{
            printf(" %6d    |", pr[i].price);
        }
    }
    printf("\n |        |");
    for(i=0; i<quan; i++) {
        printf(" %7s   |", " ");
        if(pr[i].inventory > 0) {
            pr[i].status = 1;
        } else {
            pr[i].status = 0;
        }
    }
    printf("\n | %-10s", "在庫   |");
    for(i=0; i<quan; i++) {
        if(pr[i].status == 0){
            printf("\x1b[31m  %-12s\x1b[0m |", "売り切れ");
        }else {
            printf("\x1b[32m    %-8s\x1b[0m |", "あり");
        }
    }
    printf("\n  --------------------------------------------------------------------------------\n");

}
void showOneChange(struct change ch){
    printf("%-5d %-10d %-10d %-10d\n", ch.no, ch.kingaku, ch.quan, ch.amount);
}

void showAllChange(struct change *ch, int quan){
    printf("%s  %s     %s\t    %s\n", "番号", " 金額", "枚数", "集計");
    printf("---------------------------------\n");
    int i;
    for(i=0; i<quan; i++) {
        showOneChange(ch[i]);
    }
    printf("---------------------------------\n");
}
// void showOneProfit(struct transaction rec){
//     printf("%-15s %-10s %-15s %-8d %-8d %-10d %-10d\n", rec.date,
//         rec.product_r.code,rec.product_r.name,rec.product_r.sold_quantity,
//         rec.product_r.price, rec.amount, rec.product_r.price*rec.product_r.sold_quantity);
// }
// void showAllProfit(struct transaction *rec, int quan){
//     printf("%s  %s  %s  %s   %s   %s   %s\n","日付", "名簿コード", "商品名", "販売個数", "単価", "売上高", "集計");
//     printf("---------------------------------\n");
//     int i;
//     for(i=0; i<quan; i++) {
//         showOneProfit(rec[i]);
//     }
//     printf("---------------------------------\n");
// }

void increaseChange(struct change *changeList, int quan, int upChange){
    for(int i = 0; i < quan; i++) {
        if(changeList[i].kingaku == upChange) {
            changeList[i].quan +=1;
        }
    }
}
void decreaseChange(struct change *changeList, int quan, int backChange){
    int numNote;
    printf("お客様に %d 円を返します。\nそな中に：\n", backChange);
    for(int i = quan -1; i >= 0; i--) {
        numNote = backChange / changeList[i].kingaku;
        // printf("%d回 back change %d\n", i, backChange);
        // printf("%d回 kingaku %d\n", i, changeList[i].kingaku);
        // printf("%d回 maisu %d \n\n", i, numNote);
        if(numNote > 0) {
            if(changeList[i].quan >= numNote){
                changeList[i].quan -= numNote;
            }
            else {
                continue;
            }
            printf("\t%s %d : %d 枚\n", (changeList[i].kingaku >= 1000) ? "(紙幣)" : "(硬貨)", changeList[i].kingaku, numNote);
            backChange -= numNote * changeList[i].kingaku;
        }
    }
    for(int j = 0; j < 10000; j++){
        //画面を止まわせる
        for(int k = 0; k < 1000000/2; k++){}
    }
    printf("ありがとうございました。\n");
}

int loadAmount(struct change *ch, int quan){
    int i, sum = 0;
    for(i=0; i<quan; i++) {
        sum = sum + ch[i].amount;
    }
    return sum;
}
int loadProfitAmount(struct transaction *rec, int quan){
    int i, sum = 0;
    for(i=0; i<quan; i++) {
        sum = sum + rec[i].amount;
    }
    return sum;
}
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
void writeTransactionOut(struct transaction *transactionList, int quan) {
    FILE *transactionFileOut = fopen("TransactionHistory.csv", "w");
    int i;
    for(i=0; i<quan; i++){
        struct transaction rec = transactionList[i];
        fprintf(transactionFileOut, "%s,%s,%s,%d,%d,%d\n", rec.date,rec.product_r.code,
        rec.product_r.name,rec.product_r.price,rec.product_r.sold_quantity,rec.amount);
    }
    fclose(transactionFileOut);
}

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
void readChangeFromFile(struct change *changeList, int *quan) {
    FILE *changeFile = fopen("change.csv", "r");
    int i;
    while (fscanf(changeFile, "%d,%d,%d,%d\n", &changeList[i].no, &changeList[i].kingaku , &changeList[i].quan, &changeList[i].amount) == 4) {
        i++;
    }
    fclose(changeFile);
    *quan = i;
}
void writeChangeOut(struct change *changeList, int quan) {
    FILE *changeFileOut = fopen("change.csv", "w");
    int i;
    for(i=0; i<quan; i++){
        struct change ch = changeList[i];
        fprintf(changeFileOut, "%d,%d,%d,%d\n", ch.no, ch.kingaku , ch.quan, ch.kingaku*ch.quan);
    }
    fclose(changeFileOut);
}
void showErrorMessage(int *errorCode){
    switch (*errorCode)
    {
    case 0:
        printf("");printf("\n\x1b[31m 購入できませんでした。\x1b[0m\n\n");
        break;
    case UNPURCHASEABLE:
        printf("\n\x1b[31m お金が足りないので、購入できませんでした。\x1b[0m\n\n");
        *errorCode = 0;
        break;
    case 101:
        printf("\n\x1b[31m 該当な金銭を投入してください。\x1b[0m\n\n");
        *errorCode = 0;
        break;
    case 202:
        printf("\n\x1b[31m 在庫がない為、購入できませんでした。\x1b[0m\n\n");
        *errorCode = 0;
        break;
    default:
        break;
    }
}