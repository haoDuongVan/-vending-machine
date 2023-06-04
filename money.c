#include"money.h"



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
    // usleep(30000000); //1s= 1.000.000 ms
}
int loadAmount(struct change *ch, int quan){
    int i, sum = 0;
    for(i=0; i<quan; i++) {
        sum = sum + ch[i].amount;
    }
    return sum;
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