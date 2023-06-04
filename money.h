#ifndef MONEY_H
#define MONEY_H

struct change {
    int kingaku;
    int quan;
    int amount;
};

void showOneChange(struct change ch);
void showAllChange(struct change *ch, int quan);
void increaseChange(struct change *changeList, int quan, int upChange);
void decreaseChange(struct change *changeList, int quan, int *backChange);
int loadAmount(struct change *ch, int quan);
void readChangeFromFile(struct change *changeList, int *quan);
void writeChangeOut(struct change *changeList, int quan);


#endif
