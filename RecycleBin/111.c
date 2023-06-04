#include <stdio.h>
#include <string.h>

int compareDates(const char* date1, const char* date2) {
    return strcmp(date1, date2);
}

int main() {
    const char* date1 = "10/05/2023";
    const char* date2 = "15/04/2023";

    int result = compareDates(date1, date2);
    if (result < 0) {
        printf("%s < %s\n", date1, date2);
    } else if (result > 0) {
        printf("%s > %s\n", date1, date2);
    } else {
        printf("%s = %s\n", date1, date2);
    }

    return 0;
}

