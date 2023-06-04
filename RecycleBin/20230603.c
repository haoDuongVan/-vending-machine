#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "Hello,World,Welcome,To,C";
    const char delimiters[] = ",";

    // L?n ??u g?i strtok truy?n chu?i ban ??u
    char *token = strtok(str, delimiters);
	printf("%s\n", token);
	token = strtok(NULL, delimiters);
	printf("%s", token);
    // L?p l?i vi?c g?i strtok ?? l?y cac ph?n t? ti?p theo
//    while (token != NULL) {
//        printf("%s\n", token);
//        token = strtok(NULL, delimiters);
//    }

    return 0;
}

