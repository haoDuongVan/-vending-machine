#include"smallCase.h"

#define CHARGE_MAX 1990

#define PURCHASE_SUCCESS_CODE 10

startup(){
    //外部ファイから商品を読み込む
    readProductFromFile(productList, &product_quantity);
    readChangeFromFile(changeList, &change_quantity);
    readTransactionFromFile(transactionList, &transaction_quantity);
    resetSoldForNewDay(transactionList, transaction_quantity, lastResetTime);
    Sumchange = loadAmount(changeList, change_quantity);
    Sumprofit = loadProfitAmount(transactionList, transaction_quantity);

    
}

inputMoney(){
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
}

choiceProduct(){
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
                struct transaction new_transaction;
                productList[product_choice].sold_quantity++;
                new_transaction = createTransaction(productList[product_choice].code, productList[product_choice].name,productList[product_choice].price, productList[product_choice].sold_quantity);
                // writeTransactionOut(new_transaction);
                // removeDuplicateLine(, );
                overwriteLine(new_transaction.date, new_transaction.product_r.code, new_transaction);
                ++product_choice;
                //購入完了するたびに、商品や金銭を保存する
                writeProductOut(productList, product_quantity);
                writeChangeOut(changeList, change_quantity);
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
            
}

takeChange(){
    sumGiven = 0;
    if(changeBack == 0){
        errorCode = 301;
        writeProductOut(productList, product_quantity);
        writeChangeOut(changeList, change_quantity);
    } else {
        //お釣りを返す（投入金の残高を硬貨ごとに計算する）、機内の硬貨を減らす。
        decreaseChange(changeList, change_quantity, &changeBack);
        //金銭や商品の状態を外部ファイルに保存
        writeProductOut(productList, product_quantity);
        writeChangeOut(changeList, change_quantity);
    }
}

shutdown(){
    //Save all data before shutdown
    writeProductOut(productList, product_quantity);
    writeChangeOut(changeList, change_quantity);
    printf("プログラムを終了しました。");
}

moveToManagementArea(){
    //パスワードで認証する機能        
    do {
        clearScreen();
        printf("\n管理者のパスワードを入力してください。\n");
        fflush(stdin);
        gets(inputPassword);
        certificationStatus = strcmp(inputPassword, PASSWORD);
        if(certificationStatus == 0){
            printf("認証できました。\n");
            //認証できたら、
            do {
                int productNo;
                // char date[15];
                char back;
                printf("\n    ***************管理者画面***************   \n\n");
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
                    showAllProfit(transactionList, transaction_quantity, nowDate);
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
                    printf("金銭回収をしますか（Y/N） : ");
                    while(1) {
                        fflush(stdin);
                        back = getchar();
                        if((back != 'Y') || (back != 'y')) {
                            break;
                        } else {
                            //　管理者が全ての紙幣を引き出す。硬貨ごと
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
}