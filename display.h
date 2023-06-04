#ifndef DISPLAY_H
#define DISPLAY_H

void showManagerMenu();
void showUserMenu();
void showScreen(struct product *pr, int quan, int charge, int *errorCode, int choice);
void purchaseableAllProductDisplay(struct product *pr, int quan, int charge);
void showErrorMessage(int *errorCode, int product_choice);
void clearScreen();
void printCentered(char *text, char *color);
void printNumberCentered(int num, char *color);

#endif