#include <stdio.h>
#include <stdlib.h>

// Struct lưu trữ thông tin số tiền tạm thời
typedef struct {
    int amount;         // Số tiền
    // Các trường thông tin khác liên quan
} TemporaryMoney;

// Hàm kiểm tra xem có thông tin số tiền tạm thời đã được lưu trữ hay không
int hasTemporaryMoney() {
    // Kiểm tra sự tồn tại của file hoặc truy vấn cơ sở dữ liệu
    // Trả về 1 nếu có thông tin, 0 nếu không có
    // Cần cài đặt phù hợp với hệ thống lưu trữ dữ liệu của bạn
    // Ví dụ:
    FILE *file = fopen("temporary_money.txt", "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

// Hàm trừ số tiền tạm thời từ tổng số tiền
void subtractTemporaryMoney(int *totalMoney) {
    // Đọc thông tin số tiền tạm thời từ file hoặc cơ sở dữ liệu
    // Trừ số tiền tạm thời từ tổng số tiền
    // Cần cài đặt phù hợp với hệ thống lưu trữ dữ liệu của bạn
    // Ví dụ:
    FILE *file = fopen("temporary_money.txt", "r");
    if (file != NULL) {
        TemporaryMoney tempMoney;
        fread(&tempMoney, sizeof(TemporaryMoney), 1, file);
        fclose(file);
        *totalMoney -= tempMoney.amount;

        // Xóa file hoặc cập nhật trạng thái của cơ sở dữ liệu
        remove("temporary_money.txt");
    }
}

int main() {
    int totalMoney = 1000; // Tổng số tiền của khách hàng

    if (hasTemporaryMoney()) {
        subtractTemporaryMoney(&totalMoney);
        printf("Số tiền tạm thời đã được trừ từ tổng số tiền.\n");
    } else {
        printf("Không có thông tin số tiền tạm thời.\n");
    }

    printf("Tổng số tiền còn lại: %d\n", totalMoney);

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>

// // Struct lưu trữ thông tin số tiền tạm thời
// typedef struct {
//     int amount;         // Số tiền
// } TemporaryMoney;

// // Hàm ghi số tiền tạm thời vào file
// void saveTemporaryMoney(int amount) {
//     // Mở file để ghi thông tin số tiền tạm thời
//     FILE *file = fopen("temporary_money.txt", "a");
//     if (file != NULL) {
//         TemporaryMoney tempMoney;
//         tempMoney.amount = amount;

//         // Ghi thông tin số tiền tạm thời vào file
//         fwrite(&tempMoney, sizeof(TemporaryMoney), 1, file);
//         fclose(file);
//     }
// }

// // Hàm trả lại số tiền tạm thời cho khách hàng
// void returnTemporaryMoney() {
//     // Kiểm tra sự tồn tại của file
//     FILE *file = fopen("temporary_money.txt", "r");
//     if (file != NULL) {
//         // Đọc thông tin số tiền tạm thời từ file
//         TemporaryMoney tempMoney;
//         while (fread(&tempMoney, sizeof(TemporaryMoney), 1, file) == 1) {
//             // Trả lại số tiền tạm thời cho khách hàng
//             printf("Trả lại số tiền: %d\n", tempMoney.amount);
//         }
//         fclose(file);

//         // Xóa file sau khi đã trả lại số tiền
//         remove("temporary_money.txt");
//     }
// }

// int main() {
//     int totalMoney = 0; // Tổng số tiền của khách hàng

//     char choice;
//     do {
//         // Hiển thị menu cho khách hàng
//         printf("Chọn loại tiền (0: Trả lại, 1: 10, 2: 50, 3: 100, 4: 500, 5: 1000): ");
//         scanf(" %c", &choice);

//         int amount = 0;
//         switch (choice) {
//             case '0':
//                 returnTemporaryMoney();
//                 break;
//             case '1':
//                 amount = 10;
//                 totalMoney += amount;
//                 saveTemporaryMoney(amount);
//                 break;
//             case '2':
//                 amount = 50;
//                 totalMoney += amount;
//                 saveTemporaryMoney(amount);
//                 break;
//             case '3':
//                 amount = 100;
//                 totalMoney += amount;
//                 saveTemporaryMoney(amount);
//                 break;
//             case '4':
//                 amount = 500;
//                 totalMoney += amount;
//                 saveTemporaryMoney(amount);
//                 break;
//             case '5':
//                 amount = 1000;
//                 totalMoney += amount;
//                 saveTemporaryMoney(amount);
//                 break;
//             default:
//                 printf("Lựa chọn không hợp lệ.\n");
//                 break;
//         }
//     } while (choice != '0');

//     printf("Tổng số tiền của khách hàng: %d\n", totalMoney);

//     return 0;
// }
