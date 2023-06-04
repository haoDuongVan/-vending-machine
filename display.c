#include"display.h"

#define ARRAY_WIDTH 11
#define RESET_COLOR "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define WHITE "\033[37m"


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

void showUserMenu() {
     printf("\t1.金銭投入\n");
    printf("\t2.商品選択\n");
    printf("\t3.お釣り返却\n\n");
    printf("4.終了\n");
    printf("111.管理者画面");
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

void printCentered(char *text, char *color) {
    int padding = (ARRAY_WIDTH - strlen(text)) /2;
    printf("%*s%s%s%s%*s|", padding,"", color, text, RESET_COLOR, ARRAY_WIDTH - padding -strlen(text), "");
}
void printNumberCentered(int num, char *color) {
    char stringNumber[4];
    sprintf(stringNumber,"%d",num);
    printCentered(stringNumber, color);
}
