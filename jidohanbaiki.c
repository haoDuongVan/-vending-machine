#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>

#define CHARGE_MAX 1990
#define KANRI_NUM 111
#define PURCHASE_SUCCESS_CODE 10
#define ARRAY_WIDTH 11
#define RESET_COLOR "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define WHITE "\033[37m"
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
void showUserMenu();
void showScreen(struct product *pr, int quan, int charge, int *errorCode, int choice);
struct transaction createTransaction(char *code, const char *name, int price, int sold_quan);
void updateProduct(struct product *productList, int quan, int no);
void updateProductInventory(struct product *productList, int quan, int no);
void showOneProduct(struct product pr);
void showAllProduct(struct product *pr, int quan );
int canBuy(struct product *pr, int choice, int charge);
void purchaseableAllProductDisplay(struct product *pr, int quan, int charge);
void showOneChange(struct change ch);
void showAllChange(struct change *ch, int quan);
void increaseChange(struct change *changeList, int quan, int upChange);
void decreaseChange(struct change *changeList, int quan, int *backChange);
int loadAmount(struct change *ch, int quan);
int loadProfitAmount(struct transaction *rec, int quan);
void readProductFromFile(struct product *productList, int *quan);
void writeProductOut(struct product *productList, int quan);
void readTransactionFromFile(struct transaction *transactionList, int *quan);
void writeTransactionOut(struct transaction *transactionList, int quan, char *date , char *code);
void readChangeFromFile(struct change *changeList, int *quan);
void writeChangeOut(struct change *changeList, int quan);
void showErrorMessage(int *errorCode, int product_choice);
void clearScreen();
void printCentered(char *text, char *color);
void printNumberCentered(int num, char *color);

int main(){
    struct product productList[31];
    int product_quantity = 0;
    int choice, manager_choice;
    
    //利用者画面の変数
    int charge = 0;//チャージする金額
    int sumGiven = 0;
    char split;//割り込み処理（＠の場合はプログラムを終了する。）
    int changeBack = 0; //返すお釣りの金額
    int product_choice;

    //釣り金を返すため、釣り金を設定する。
    struct change changeList[6];
    int change_quantity = 0;
    int Sumchange = 0;//釣銭の合計

    //レシートと売上確認
    struct transaction transactionList[1000];
    int transaction_quantity = 0;
    int Sumprofit = 0;//総売上高 

    //error 処理
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
    {   showScreen(productList, product_quantity, sumGiven, &errorCode, product_choice);
        printf("\t\t\t 投入金額： %d 円\n\n", sumGiven);
        showUserMenu();
        printf("\nお客さんはこれから何を選びますか？ ");
        
        scanf("%d", &choice);
        getchar();
        // clearScreen();

        switch (choice)
        {
        case 1: // 金銭投入 -- 
            while(1){
                showScreen(productList, product_quantity, sumGiven, &errorCode, product_choice);
                printf("\n\n              **********金銭投入**********             \n\n");
                printf("\t\t\t 投入金額： %d 円\n\n", sumGiven);
                printf("利用可能：10円 50円 100円 500円 1000円\n投入上限は：1990円\n");
                printf("\n\n中止の場合は「＠」を入力してください。\n");
                printf("\nお金を投入してください： ");
                fflush(stdin);
                if((scanf("%d", &charge))==1) {
                    // clearScreen();

                    if((charge <= CHARGE_MAX) && ((charge == 10)||(charge == 50)||(charge == 100)||(charge == 500)||(charge == 1000))){
                        
                        sumGiven += charge;
                        if(sumGiven > 1990) {
                            errorCode = 102;
                            sumGiven -= charge;
                            charge = 0;
                            break;
                        }
                        //金銭の枚数を増やして、外部ファイルに保存
                        increaseChange(changeList, change_quantity, charge);
                        writeChangeOut(changeList, change_quantity);
                        changeBack = sumGiven;
                        charge = 0;
                        break;
                    }
                    else{
                        errorCode = 101;
                        charge = 0;
                        continue;
                    }
                }
                else {
                    scanf("%c", &split);
                    clearScreen();
                    if(split == '@') {
                        break;
                    }
                    else {
                        errorCode = 100;
                        continue;
                    }
                }
            }
            break;
        case 2://商品選択
            while(1) {
                showScreen(productList, product_quantity, sumGiven, &errorCode, product_choice);
                printf("\n\n              ********** 商品選択 **********             \n\n");
                printf("\t\t\t 投入金額： %d 円\n\n", sumGiven);
                printf("\n\n終了の場合は「＠」を入力してください。\n\n");
                printf("購入商品の商品番号を入力してください(1 -> 6)： ");
                
                if((scanf("%d", &product_choice)) == 1) {
                    getchar();
                    //購入可能？
                    purchasable = canBuy(productList, product_choice, sumGiven);
                    //購入成功
                    if((product_choice > 0) && (product_choice <= 6) && purchasable){
                        --product_choice;
                        sumGiven = changeBack -= productList[product_choice].price;
                        productList[product_choice].inventory--;
                        errorCode = PURCHASE_SUCCESS_CODE;
                        // 売上計算を行う。
                        struct transaction transaction;
                        productList[product_choice].sold_quantity++;
                        transaction = createTransaction(productList[product_choice].code, productList[product_choice].name,productList[product_choice].price, productList[product_choice].sold_quantity);
                        transactionList[transaction_quantity++] = transaction;
                        writeTransactionOut(transactionList, transaction_quantity, nowDate, productList[product_choice].code);
                        // deleteSameTransaction(transactionList, transaction_quantity, productList[product_choice].code, nowDate);
                        ++product_choice;

                        break;
                    }
                    else{
                        if((product_choice >6) || (product_choice < 1)){
                            errorCode = 200;
                        } else if(sumGiven < productList[product_choice-1].price) {
                            errorCode = 201;
                        } else {
                            errorCode = 202;
                        }
                        continue;
                    }
                }
                //前の画面に戻る
                else {
                    scanf("%c", &split);
                    getchar();
                    // clearScreen();
                    if(split == '@') {
                        break;
                    }
                    else {
                        errorCode = 200;
                        continue;
                    }
                }
                break;
            }
            break;

        case 3://購入完了で、釣銭を返却
            sumGiven = 0;
            if(changeBack == 0){
                errorCode = 301;
                writeProductOut(productList, product_quantity);
                writeChangeOut(changeList, change_quantity);
                break;
            } else {
                //お釣りを返す（投入金の残高を硬貨ごとに計算する）、機内の硬貨を減らす。
                decreaseChange(changeList, change_quantity, &changeBack);
                //金銭や商品の状態を外部ファイルに保存
                writeProductOut(productList, product_quantity);
                writeChangeOut(changeList, change_quantity);
                break;
            }
        
        case 4://終了
            writeProductOut(productList, product_quantity);
            writeChangeOut(changeList, change_quantity);
            printf("プログラムを終了しました。");
            break;

        case KANRI_NUM: //管理者用の画面に移し 
            //パスワードで認証する機能        
            do {
                printf("\n管理者のパスワードを入力してください。\n");
                fflush(stdin);
                gets(inputPassword);
                clearScreen();
                certificationStatus = strcmp(inputPassword, PASSWORD);
                if(certificationStatus == 0){
                    printf("認証できました。\n");
                    //認証できたら、
                    do {
                        int productNo;
                        // char date[15];
                        char back;
                        printf("\n    ***************管理者画面***************   \n");
                        showManagerMenu();

                        scanf("%d", &manager_choice);
                        getchar();
                        clearScreen();
                        // struct product product;

                        switch (manager_choice)
                        {
                        case 1: //商品一覧表 --OK
                            printf("\n============== 商品一覧表 ==================\n\n");

                            showAllProduct(productList, product_quantity);
                            printf("戻りますか (Y) : ");
                            while(1) {
                                fflush(stdin);
                                back = getchar();
                                clearScreen();
                                if((back == 'Y') || (back == 'y')) {
                                    break;
                                }
                            }
                            break;
                        case 2://商品修正  -- OK
                        //全商品を表示する
                            clearScreen();
                            printf("\n============== 商品修正==================\n\n");
                            showAllProduct(productList, product_quantity);
                            printf("\n\n戻りの場合は「＠」を入力してください。\n\n");
                            printf("\n変更商品の番号を入力してください：");
                            if((scanf("%d", &productNo))== 1){
                                getchar();
                                updateProduct(productList, product_quantity, productNo);
                                writeProductOut(productList, product_quantity);
                            }
                            else {
                                scanf("%c", &split);
                                getchar();
                                clearScreen();
                                if(split == '@') {
                                    break;
                                }
                                else {
                                    printf("\aもう一回入力してください。");
                                    continue;
                                }
                            }
                            break;

                        case 3: // 商品追加 -- OK
                            // product = createProduct();
                            // productList[product_quantity++] = product;
                            // writeProductOut(productList, product_quantity);
                            // break;

                        case 4://売上高確認
                            printf("\n============== 売上高確認 ==================\n");
                            printf("\n");
                            printf("\n---------------------------------------------------\n");
                            // showAllProfit(transactionList, transaction_quantity);
                            printf("\n\t  売上高の総金額：  %-10d\n", Sumprofit);
                            printf("\n---------------------------------------------------\n");
                            printf("戻りますか (Y) : ");
                            while(1) {
                                fflush(stdin);
                                back = getchar();
                                clearScreen();
                                if((back == 'Y') || (back == 'y')) {
                                    break;
                                }
                            }
                            break;

                        case 5://残高確認・回収
                            printf("\n============== 残高確認・回収 ==================\n\n");
                            clearScreen();
                            struct change change;
                            // change = createChange();
                            // changeList[change_quantity++] = change;
                            // writeChangeOut(changeList, change_quantity);
                            printf("\n---------------------------------------------------\n");
                            showAllChange(changeList, change_quantity);
                            printf("\n\t  準備金の総金額：  %-10d\n", Sumchange);
                            printf("\n---------------------------------------------------\n");
                            printf("戻りますか (Y) : ");
                            while(1) {
                                fflush(stdin);
                                back = getchar();
                                if((back == 'Y') || (back == 'y')) {
                                    break;
                                }
                            }
                            break;

                        case 6://商品補充 -- OK
                            clearScreen();
                            printf("\n================ 商品補充 ================\n\n");
                            showAllProduct(productList, product_quantity);
                            printf("\n\n戻りの場合は「＠」を入力してください。\n\n");
                            printf("\n商品番号を入力してください：");
                            if((scanf("%d", &productNo)) == 1){
                                getchar();
                                updateProductInventory(productList, product_quantity, productNo);
                            } else {
                                scanf("%c", &split);
                                getchar();
                                if(split == '@') {
                                    break;
                                }
                                else {
                                    printf("\aもう一回入力してください。");
                                    continue;
                                }
                            }
                            break;

                        case 7://終了
                            clearScreen();
                            writeProductOut(productList, product_quantity);
                            printf("プログラムを終了しました。");
                            break;
                        default:
                        clearScreen();
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
    return 0;
}

void showUserMenu() {
     printf("\t1.金銭投入\n");
    printf("\t2.商品選択\n");
    printf("\t3.お釣り返却\n\n");
    // printf("4.終了\n");
    //printf("111.管理者画面");
    printf("\n\t\t問い合わせ:0xx-xxxx-xxxx\n");
    printf("\n  *****************************************************   \n");
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
void showScreen(struct product *pr, int quan, int Sumcharge, int *errorCode, int product_choice){
    clearScreen();
    printf("\n           *********************** 自動販売機 ***********************   \n\n");
    purchaseableAllProductDisplay(pr, quan, Sumcharge);
    printf("\n");
    // show message (error + success)
    if(*errorCode != 0) {
        showErrorMessage(errorCode, product_choice);
    }
    printf("\n          -------------------------------------------------------------\n");
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
void updateProduct(struct product *productList, int quan, int no) {
    for(int i = 0; i < quan; i++) {
        if(productList[i].no == no) {
            printf("\n名簿コードを入力してください：");
            scanf("%s", productList[i].code);
            getchar();
            printf("\n商品名を入力してください：");
            scanf("%s", productList[i].name);
            getchar();
            printf("\n単価を入力してください：");
            scanf("%d", &productList[i].price);
            fflush(stdin);
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
void purchaseableAllProductDisplay(struct product *pr, int quan, int charge){
    int i;
    printf("  --------------------------------------------------------------------------------\n");
    printf(" | %-10s", "番号   |");
    for(i=0; i<quan; i++) {
        printNumberCentered(i+1, WHITE);
        // printf("   %3d     |", i+1);
    }
    printf("\n | %-10s", "商品名 |");
    for(i=0; i<quan; i++) {
        printCentered(pr[i].name, WHITE);
        // printf("  %-8s |", pr[i].name);
    }
    printf("\n | %-10s", "値段   |");
    for(i=0; i<quan; i++) {
        if((charge >= pr[i].price) && (pr[i].status != 0)){
            printNumberCentered(pr[i].price, GREEN);
            // printf("\x1b[32m %6d \x1b[0m   |", pr[i].price);
        }
        else{
            // printf(" %6d    |", pr[i].price);
            printNumberCentered(pr[i].price, WHITE);

        }
    }
    printf("\n |        |");
    for(i=0; i<quan; i++) {
        printCentered(" ", WHITE);
        // printf(" %7s   |", " ");
        if(pr[i].inventory > 0) {
            pr[i].status = 1;
        } else {
            pr[i].status = 0;
        }
    }
    printf("\n | %-10s", "在庫   |");
    for(i=0; i<quan; i++) {
        if(pr[i].status == 0){
            // printCentered("売り切れ ", RED);
            printf(" %s%-12s%s  |",RED, "売り切れ", RESET_COLOR);
        }else {
            printf("    %s%-8s%s |",GREEN, "あり", RESET_COLOR);
            // printCentered("  あり  ", GREEN);
        }
    }
    printf("\n  --------------------------------------------------------------------------------\n");
}
void showOneChange(struct change ch){
    printf("%-10d %-10d %-10d\n", ch.kingaku, ch.quan, ch.amount);
}
void showAllChange(struct change *ch, int quan){
    printf("%s     %s\t    %s\n", " 金額", "枚数", "集計");
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
void printCentered(char *text, char *color) {
    int padding = (ARRAY_WIDTH - strlen(text)) /2;
    printf("%*s%s%s%s%*s|", padding,"", color, text, RESET_COLOR, ARRAY_WIDTH - padding -strlen(text), "");
}
void printNumberCentered(int num, char *color) {
    char stringNumber[4];
    sprintf(stringNumber,"%d",num);
    printCentered(stringNumber, color);
}
void increaseChange(struct change *changeList, int quan, int upChange){
    for(int i = 0; i < quan; i++) {
        if(changeList[i].kingaku == upChange) {
            changeList[i].quan +=1;
        }
    }
}
void decreaseChange(struct change *changeList, int quan, int *backChange){
    int numNote;
    printf("お客様に %d 円を返します。\nそな中に：\n", *backChange);
    for(int i = quan -1; i >= 0; i--) {
        numNote = *backChange / changeList[i].kingaku;
        if(numNote > 0) {
            if(changeList[i].quan >= numNote){
                changeList[i].quan -= numNote;
            }
            else {
                continue;
            }
            printf("\t%s %d : %d 枚\n", (changeList[i].kingaku >= 1000) ? "(紙幣)" : "(硬貨)", changeList[i].kingaku, numNote);
            *backChange -= numNote * changeList[i].kingaku;
        }
    }
    printf("ありがとうございました。\n");
    printf("お釣りを忘れずに、早くお取りください。\n");
    //画面を止まわせる
    for(int j = 0; j < 10000; j++){
        for(int k = 0; k < 1000000/2; k++){}
    }
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
void writeTransactionOut(struct transaction *transactionList, int quan, char *date, char *code) {
    FILE *transactionFileOut = fopen("TransactionHistory.csv", "w");
    for(int i=0; i<quan; i++){
        struct transaction rec = transactionList[i];
        //
        if((strcmp(rec.date, date) != 0)){
            rec.product_r.sold_quantity = 0;
        }
        if((strcmp(rec.product_r.code, code) == 0)){
            rec.product_r.sold_quantity++;
            fseek(transactionFileOut, 0, SEEK_SET);
            char line[100];
            int lineNum = 0;

            while(fgets(line, sizeof(line), transactionFileOut)){
                lineNum++;
                char *p = strtok(line,",");
                char *storedTransactionDate =p;
                char *productCode = strtok(NULL,",");

                if((strcmp(productCode, code) == 0) && (strcmp(storedTransactionDate,date) == 0)) break;

                fseek(transactionFileOut, 0, SEEK_END);
                long position = ftell(transactionFileOut);
                fseek(transactionFileOut, position - strlen(line), SEEK_SET);
                fprintf(transactionFileOut, "%s,%s,%s,%d,%d,%d\n", rec.date,rec.product_r.code,
                rec.product_r.name,rec.product_r.price,rec.product_r.sold_quantity,rec.amount);
                fclose(transactionFileOut);
            }
        } 
        else {
            transactionFileOut = fopen("TransactionHistory.csv", "a");
            fprintf(transactionFileOut, "%s,%s,%s,%d,%d,%d\n", rec.date,rec.product_r.code,
            rec.product_r.name,rec.product_r.price,rec.product_r.sold_quantity,rec.amount);
        }
        fclose(transactionFileOut);
    }
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
    while (fscanf(changeFile, "%d,%d,%d\n", &changeList[i].kingaku , &changeList[i].quan, &changeList[i].amount) == 3) {
        i++;
        // printf("%d,%d,%d\n", changeList[i].kingaku , changeList[i].quan, changeList[i].amount);
    }
    fclose(changeFile);
    *quan = i;
}
void writeChangeOut(struct change *changeList, int quan) {
    FILE *changeFileOut = fopen("change.csv", "w");
    int i;
    for(i=0; i<quan; i++){
        struct change ch = changeList[i];
        // printf("%d,%d,%d\n", changeList[i].kingaku , changeList[i].quan, changeList[i].amount);

        fprintf(changeFileOut, "%d,%d,%d\n", ch.kingaku , ch.quan, ch.kingaku*ch.quan);
    }
    fclose(changeFileOut);
}
void clearScreen() {
    printf("\033[2J");
    printf("\033[H");
    fflush(stdout);
}
void showErrorMessage(int *errorCode, int product_choice){
    switch (*errorCode)
    {
    case 0:
        printf("");
        break;
    case 100:
        printf("\n %s変な入力をやめてください！%s\n", RED, RESET_COLOR);
        *errorCode = 0;
        break;
    case 101:
        printf("\n %s該当な金銭を投入してください。%s\n", RED, RESET_COLOR);
        *errorCode = 0;
        break;
    case 102:
        printf("\n %s申し訳ございません。投入上限は1990円ですです。%s\n", RED, RESET_COLOR);
        *errorCode = 0;
        break;
    case 200:
        printf("\n %s１から６までの番号、あるいは「@」を入力してください！%s\n", RED, RESET_COLOR);
        *errorCode = 0;
        break;
    case 201:
        printf("\n %sお金が足りないので、購入できませんでした。%s\n", RED, RESET_COLOR);
        *errorCode = 0;
        break;
    case 202:
        printf("\n %s在庫がない為、購入できませんでした。%s\n", RED, RESET_COLOR);
        *errorCode = 0;
        break;
    case 301:
        printf("\n %sお釣りはありません。%s\n", RED, RESET_COLOR);
        *errorCode = 0;
        break;
    case PURCHASE_SUCCESS_CODE:
        printf("商品 %d を購入しました。\n\n", product_choice);
        *errorCode = 0;
        // product_choice = NULL;
        break;
    default:
        break;
    }
}

