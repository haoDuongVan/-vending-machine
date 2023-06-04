#include <stdio.h>
#include <string.h>

void overwriteLine(const char* filename, const char* date, const char* productName, const char* newLine) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Không thể mở file.\n");
        return;
    }

    // Tạo một file tạm thời
    FILE* tempFile = fopen("temp.csv", "w");
    if (tempFile == NULL) {
        printf("Không thể tạo file tạm.\n");
        fclose(file);
        return;
    }

    char line[100];
    int isDuplicate = 0;

    // Ghi dòng mới vào file tạm thời
    while (fgets(line, sizeof(line), file) != NULL) {
        char storedDate[20];
        char storedProductName[50];

        sscanf(line, "%[^,],%[^,]", storedDate, storedProductName);

        // Kiểm tra xem dòng đang xét có trùng ngày và tên sản phẩm không
        if (strcmp(storedDate, date) == 0 && strcmp(storedProductName, productName) == 0) {
            // Nếu trùng, ghi dòng mới vào file tạm thời
            fputs(newLine, tempFile);
            isDuplicate = 1;
        } else {
            // Nếu không trùng, ghi dòng cũ vào file tạm thời
            fputs(line, tempFile);
        }
    }

    // Nếu không có dòng trùng, ghi dòng mới vào cuối file tạm thời
    if (!isDuplicate) {
        fputs(newLine, tempFile);
    }

    // Đóng file
    fclose(file);
    fclose(tempFile);

    // // Xóa file gốc
    // remove(filename);

    // // Đổi tên file tạm thời thành tên file gốc
    // rename("temp.csv", filename);

    printf("Đã ghi đè dòng trùng lặp.\n");
}

int main() {
    // Gọi hàm ghi đè dòng
    overwriteLine("hanbai.csv", "03/06/2023", "A103", "03/06/2023,A103,ocha,120,11,1320\n");

    return 0;
}
