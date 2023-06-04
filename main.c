#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"product.h"
#include"transaction.h"
#include"display.h"
#include"money.h"
#include"smallCase.h"

#define KANRI_NUM 111

int main(){
    /*プログラムの変数を宣言する*/
    struct product productList[6];
    int product_quantity = 0;

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

    //get date and time info
    char nowDate[50]; //現在の日付
    time_t rawTime;
    struct tm *timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    strftime(nowDate, sizeof(nowDate), "%d/%m/%Y", timeInfo);

    struct tm *lastResetTime; // 販売した個数をリセットする
    time_t currentTime;
    time(&currentTime);
    lastResetTime = localtime(&currentTime);

    int choice, manager_choice;
    
    startup();
    do { 
        showScreen(productList, product_quantity, sumGiven, &errorCode, product_choice);
        printf("\t\t\t 投入金額： %d 円\n\n", sumGiven);
        showUserMenu();
        printf("\nお客さんはこれから何を選びますか？ ");
        
        scanf("%d", &choice);
        getchar();
        // clearScreen();

        switch (choice)
        {
        case 1: // 金銭投入 -- 
            inputMoney();
            
            break;
        case 2://商品選択
            choiceProduct();
            break;

        case 3://購入完了で、釣銭を返却
            takeChange();
            break;
        case 4://終了
            shutdown();
            break;

        case KANRI_NUM: //管理者用の画面に移し 
            moveToManagementArea();
            break;
            
        default:
            // printf("違います。もう一回入力してください。\n");
            break;
        }
    } while (choice != 4);
    return 0;
}


