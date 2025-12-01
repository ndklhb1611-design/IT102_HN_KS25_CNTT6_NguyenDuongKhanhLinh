#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 100
#define PAGE_SIZE 10

typedef struct {
    char accountId[20];     // Ma tai khoan
    char fullName[50];      // Ho va ten chu tai khoan
    char phone[15];         // So dien thoai
    double balance;         // So du hien tai
    int status;             // Trang thai 1:Hoat dong, 0:Bi khoa
} Account;

typedef struct {
    char transId[20];       // Ma giao dich 
    char senderId[20];      // Ma tai khoan nguoi gui
    char receiverId[20];    // Ma tai khoan nguoi nhan
    double amount;          // So tien chuyen
    char type[10];          // Loai giao dich 
    char timestamp[20];
} Transaction;

// Khai bao bien toan cuc
Account accounts[MAX_ACCOUNTS];
int accountCount = 0;
Transaction transactions[MAX_TRANSACTIONS];
int transactionCount = 0;

// Khai bao nguyen mau ham (Prototypes)
void printMenu(); 
void removeNewline(char *str);
void initializeSampleData();
int  isIdDuplicate(char *id); 
int isPhoneDuplicate(char *phone, int idx);
int findAccountIndex(char *id);

void createAccount();
void editAccount();   
void lockAccount();
void searchAccount();
void displayAccountsPagination();
void sortAccounts();
void transferMoney();
void viewTransactionHistory(); 

int main() {
    int choice;
    char buffer[100];
    
    void initializeSampleData() {

    strcpy(accounts[0].accountId, "1000000000");
    strcpy(accounts[0].fullName, "Nguyen Van An");
    strcpy(accounts[0].phone, "0910000000");
    accounts[0].balance = 500000;
    accounts[0].status = 1;

    strcpy(accounts[1].accountId, "1000000001");
    strcpy(accounts[1].fullName, "Tran Thi Lan");
    strcpy(accounts[1].phone, "0910000001");
    accounts[1].balance = 100000.000;
    accounts[1].status = 0;

    strcpy(accounts[2].accountId, "1000000002");
    strcpy(accounts[2].fullName, "Le Huu Phuc");
    strcpy(accounts[2].phone, "0910000002");
    accounts[2].balance = 750000;
    accounts[2].status = 1;

    strcpy(accounts[3].accountId, "1000000003");
    strcpy(accounts[3].fullName, "Pham Minh Duc");
    strcpy(accounts[3].phone, "0910000003");
    accounts[3].balance = 21600;
    accounts[3].status = 1;

    strcpy(accounts[4].accountId, "1000000004");
    strcpy(accounts[4].fullName, "Hoang Anh Thu");
    strcpy(accounts[4].phone, "0910000004");
    accounts[4].balance = 1020;
    accounts[4].status = 1;

    strcpy(accounts[5].accountId, "1000000005");
    strcpy(accounts[5].fullName, "Vu Quoc Cuong");
    strcpy(accounts[5].phone, "0910000005");
    accounts[5].balance = 2000;
    accounts[5].status = 1;

    strcpy(accounts[6].accountId, "1000000006");
    strcpy(accounts[6].fullName, "Dang Thanh Tung");
    strcpy(accounts[6].phone, "0910000006");
    accounts[6].balance = 30000;
    accounts[6].status = 1;

    strcpy(accounts[7].accountId, "1000000007");
    strcpy(accounts[7].fullName, "Bui Mai Huong");
    strcpy(accounts[7].phone, "0910000007");
    accounts[7].balance = 4000;
    accounts[7].status = 1;

    strcpy(accounts[8].accountId, "1000000008");
    strcpy(accounts[8].fullName, "Cao Duc Trung");
    strcpy(accounts[8].phone, "0910000008");
    accounts[8].balance = 5000;
    accounts[8].status = 1;

    strcpy(accounts[9].accountId, "1000000009");
    strcpy(accounts[9].fullName, "Ngo Phuong Linh");
    strcpy(accounts[9].phone, "0910000009");
    accounts[9].balance = 60000;
    accounts[9].status = 1;

    accountCount = 10;
}

	initializeSampleData();

    while (1) {
        printMenu();
        printf("Nhap lua chon cua ban: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[0] == '\n' || strlen(buffer) == 0) {
            printf("Lua chon ko hop le , nhap lai !!!\n");
            continue; 
        }
        choice = atoi(buffer); 
        
        switch (choice) {
            case 1: 
                createAccount(); 
                break;
            case 2: 
                editAccount(); 
                break;
            case 3:
                lockAccount(); 
                break;
            case 4: 
                searchAccount(); 
                break;                  
            case 5:
            	displayAccountsPagination();
            	break;
            case 6:
            	sortAccounts();
            	break;
            case 7: 
            	transferMoney();
            	break;
            case 8:
            	viewTransactionHistory();
            	break; 
            case 9: 
                printf("Thoat chuong trinh\n");
                return 0;
            default: 
                printf("Lua chon ko hop le , nhap lai !!!\n");
        }
    }
    return 0;
}

//TRIEN KHAI CAC HAM 

void printMenu() {
    printf("\n+-------------------------MENU---------------------------+\n");
    printf("| %-54s |\n","1. Them tai khoan moi");
    printf("| %-54s |\n","2. Cap nhat thong tin");
    printf("| %-54s |\n","3. Quan ly trang thai (Khoa/Xoa)");
    printf("| %-54s |\n","4. Tim kiem tai khoan");
    printf("| %-54s |\n","5. Danh sach (Phan trang)");
    printf("| %-54s |\n","6. Sap xep danh sach");
    printf("| %-54s |\n","7. Giao dich chuyen khoan");
    printf("| %-54s |\n","8. Lich su giao dich");
    printf("| %-54s |\n","9. Thoat");
    printf("+--------------------------------------------------------+\n");
}

void removeNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int isIdDuplicate(char *id) {
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].accountId, id) == 0) {
            return 1;
        }
    }
    return 0;
}

int isPhoneDuplicate(char *phone, int currentIndex) {
    for (int i = 0; i < accountCount; i++) {
        if (i != currentIndex && strcmp(accounts[i].phone, phone) == 0) {
            return 1;
        }
    }
    return 0;
}

void trimSpaces(char *str) {
    if (str == NULL || *str == '\0') return;

    int len = strlen(str);
    while (len > 0 && str[len - 1] == ' ') {
        str[--len] = '\0';
    }

    char *start = str;
    while (*start == ' ') { 
        start++;
    }

    if (start != str) {
        memmove(str, start, strlen(start) + 1); 
    }
}

void toLowerCase(char str[]){
        for(int i =0 ;str[i] != '\0';i++){
                str[i] = tolower(str[i]);
		}
}		

int findAccountIndex(char *id) {
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].accountId, id) == 0) {
            return i;
        }
    }
    return -1;
}

// TASK 1: Them tai khoan 
void createAccount() {
    Account newAcc;
    
    // Kiem tra mang day
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Danh sach da day, khong the them moi!\n");
        return;
    }
    printf("\n      *** Them moi tai khoan ***\n\n");
    // Nhap ID
    while (1) {
        printf("Nhap ID (Account ID): ");
        fgets(newAcc.accountId, sizeof(newAcc.accountId), stdin);
        removeNewline(newAcc.accountId);
        trimSpaces(newAcc.accountId);

        if (strlen(newAcc.accountId) == 0) {
            printf("ID khong duoc de trong!\n");
        } else if (isIdDuplicate(newAcc.accountId)) { 
            printf("ID da ton tai! Vui long nhap ID khac\n");
        }else{
            break;
        }
    }

    // Nhap Ten
    while (1) {
        printf("Nhap Ho va Ten: ");
        fgets(newAcc.fullName, sizeof(newAcc.fullName), stdin);
        removeNewline(newAcc.fullName);
        trimSpaces(newAcc.accountId);
        
        if (strlen(newAcc.fullName) == 0) {
            printf("Ho ten khong duoc de trong!\n");
        } else{
            break;
        }
    }

    // Nhap Phone
    while (1) {
        printf("Nhap So dien thoai (toi thieu 10 so): ");
        fgets(newAcc.phone, sizeof(newAcc.phone), stdin);
        removeNewline(newAcc.phone);
        trimSpaces(newAcc.accountId);
        
        size_t len = strlen(newAcc.phone);
        
        if (strlen(newAcc.phone) == 0) {
            printf("So dien thoai khong duoc de trong!\n");
        } else if (strlen(newAcc.phone) != 10){
        	printf("So dien thoai khong hop le! Vui long nhap lai\n" );
        } else if (isPhoneDuplicate(newAcc.phone, -1)){
             printf("So dien thoai nay da ton tai!\n");
		}else {
        	break;
    }
}

    newAcc.balance = 0;
    newAcc.status = 1;

    // Luu vao bien toan cuc
    accounts[accountCount] = newAcc;
    accountCount++;
    printf("Them tai khoan thanh cong!\n");
}

// TASK 2: Cap nhat thong tin 
void editAccount() {
    char searchId[20];
    int foundIndex = -1;
	printf("\n      *** Cap nhat thong tin ***\n\n");
    printf("Nhap ID tai khoan can sua: ");
    fgets(searchId, sizeof(searchId), stdin);
    removeNewline(searchId);
    trimSpaces(searchId);

    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].accountId, searchId) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Khong tim thay tai khoan voi ID nay\n");
        return;
    }

    printf("Thong tin hien tai: %s - %s\n", accounts[foundIndex].fullName, accounts[foundIndex].phone);

    // Nhap Ten moi
    char tempName[50];
    printf("Nhap Ten moi(Enter de giu nguyen): ");
    fgets(tempName, sizeof(tempName), stdin);
    removeNewline(tempName);
    trimSpaces(tempName);

    if (strlen(tempName) > 0) {
        strcpy(accounts[foundIndex].fullName, tempName);
    }

    // Nhap SDT moi
    char tempPhone[15];
    while (1) {
        printf("Nhap so dien thoai moi (Enter de giu nguyen): ");
        fgets(tempPhone, sizeof(tempPhone), stdin);
        removeNewline(tempPhone);
        trimSpaces(tempPhone);

        if (strlen(tempPhone) == 0) {
            break;
        }
        if (strlen(tempPhone) != 10) { 
            printf("So dien thoai khong hop le!Vui long nhap lai\n");
            continue;
        }
        if (isPhoneDuplicate(tempPhone, foundIndex)) { 
            printf("So dien thoai nay da duoc su dung boi tai khoan khac!\n");
        } else {
            strcpy(accounts[foundIndex].phone, tempPhone);
            break;
        }
    }

    printf("Cap nhat thanh cong!\n");
}

// TASK 3: Quan ly trang thai (Khoa/Xoa)
void lockAccount() {
    char searchId[20];
    int foundIndex = -1;
	printf("\n    *** Quan ly trang thai (Khoa/Xoa) ***\n\n");
    printf("Nhap ID tai khoan can khoa/xoa: ");
    fgets(searchId, sizeof(searchId), stdin);
    removeNewline(searchId);
    trimSpaces(searchId);

    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].accountId, searchId) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Khong tim thay tai khoan voi ID: %s\n", searchId);
        return;
    }

    printf("Tim thay tai khoan:\n");
    printf("----------------------------------------\n");
    printf("ID: %s\n", accounts[foundIndex].accountId);
    printf("Ho Ten: %s\n", accounts[foundIndex].fullName);
    printf("Trang thai: %s\n", accounts[foundIndex].status == 1 ? "Dang Hoat Dong" : "Da Bi Khoa");
    printf("----------------------------------------\n");

    if (accounts[foundIndex].status == 1) {
        printf("Ban co chac chan muon khoa tai khoan nay? (y/n): ");
    } else {
        printf("Tai khoan nay dang bi khoa. Ban co muon mo khoa ? (y/n): ");
    }

    char confirm;
    scanf("%c", &confirm);
    getchar(); 

    if (confirm == 'y' || confirm == 'Y') {
        if (accounts[foundIndex].status == 1) {
            accounts[foundIndex].status = 0;
            printf("Tai khoan da bi khoa!\n");
        } else {
            accounts[foundIndex].status = 1;
            printf("Tai khoan da duoc mo khoa!\n");
        }
    } else {
        printf("Huy thao tac. Trang thai giu nguyen.\n");
    }
}

// TASK 4: Tim kiem tai khoan 
void searchAccount() {
    char keyword[50];
    int foundCount = 0;
    system("cls");
    
    printf("\n      *** TIM KIEM TAI KHOAN ***\n\n");
    printf("Nhap tu khoa can tim (ID hoac Ten): ");
    fgets(keyword, sizeof(keyword), stdin);
    removeNewline(keyword);
    trimSpaces(keyword);
    
    if (strlen(keyword) == 0) {
        printf("Tu khoa tim kiem khong duoc de trong!\n");
        printf("\nNhan enter de quay lai Menu chinh\n");
        getchar();
        return;
    }

    char lowerKeyword[50];
    strcpy(lowerKeyword, keyword);
    toLowerCase(lowerKeyword);

    printf("\n+-------+--------------------+----------------------+-----------------+-----------------+-----------------+\n");
    printf("| %-5s | %-18s | %-20s | %-15s | %-15s | %-15s |\n", "STT", "ID", "Ho Va Ten", "So Dien Thoai", "So Du", "T.Thai");
    printf("+-------+--------------------+----------------------+-----------------+-----------------+-----------------+\n");
    
    for (int i = 0; i < accountCount; i++) {
        char lowerFullName[50]; 
        strcpy(lowerFullName, accounts[i].fullName);
        toLowerCase(lowerFullName);

        char lowerAccountId[50]; 
        strcpy(lowerAccountId, accounts[i].accountId);
        toLowerCase(lowerAccountId);

        if (strstr(lowerFullName, lowerKeyword) != NULL || strstr(lowerAccountId, lowerKeyword) != NULL) 
        {

            const char* status_str = accounts[i].status == 1 ? "Dang hoat dong" : "Da bi khoa";
            
            printf("| %-5d | %-18s | %-20s | %-15s | %-15.3f | %-15s |\n", 
                    foundCount + 1, 
                    accounts[i].accountId, 
                    accounts[i].fullName, 
                    accounts[i].phone,
                    accounts[i].balance, 
                    status_str);         
            foundCount++;
        }
    }

    printf("+-------+--------------------+----------------------+-----------------+-----------------+-----------------+\n");

    if (foundCount == 0) {
        printf("| %-103s |\n", "Khong tim thay ket qua phu hop.");
        printf("+---------------------------------------------------------------------------------------------------------+\n");
    } else {
        printf("| %-103s |\n", "Tim thay thanh cong.");
        printf("+---------------------------------------------------------------------------------------------------------+\n");
        printf("Tong so tai khoan tim thay: %d\n", foundCount);
    }
    
    printf("\nNhan enter de quay lai Menu chinh\n");
    getchar(); 
}

// TASK 5: Danh sach(Phan trang)
void displayAccountsPagination() {
    int pageNumber;
    const int pageSize = 5;
    int totalPages;
    int startIndex, endIndex;
    char inputBuffer[20];
    
    if (accountCount == 0) {
        system("cls");
        printf("\n      *** DANH SACH TAI KHOAN (PAGINATION) ***\n\n");
        printf("Danh sach tai khoan trong!\n");
        printf("\nNhan enter de quay lai Menu chinh\n");
        getchar();
        return;
    }
    
    system("cls");
    printf("\n      *** DANH SACH TAI KHOAN (PAGINATION) ***\n\n");
    totalPages = (accountCount + pageSize - 1) / pageSize;

    do {
        printf("Hien tai co %d tai khoan, chia thanh %d trang.\n", accountCount, totalPages);
        printf("Nhap so trang can xem (1 - %d, nhap 0 de thoat): ", totalPages); 
        
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL || sscanf(inputBuffer, "%d", &pageNumber) != 1) {
             printf("\nVui long nhap mot so nguyen.\n");
             pageNumber = -1; 
        }
        
        if (pageNumber == 0) {
            return;
        }

        if (pageNumber < 1 || pageNumber > totalPages) {
            printf("\nLoi du lieu nhap khong hop le. Vui long kiem tra lai!\n");
            printf("--------------------------------------------------\n");
        }
    } while (pageNumber < 1 || pageNumber > totalPages);
    
    int currentPage = pageNumber;
    
    while (currentPage >= 1 && currentPage <= totalPages) {
        
        startIndex = (currentPage - 1) * pageSize;
        endIndex = startIndex + pageSize;
        if (endIndex > accountCount) {
            endIndex = accountCount;
        }

        system("cls"); 
        printf("\n      *** DANH SACH TAI KHOAN (PAGINATION) ***\n\n");
        printf("Dang hien thi trang %d / %d. Tong so tai khoan: %d.\n", currentPage, totalPages, accountCount);
        
        printf("\n+-------+--------------------+----------------------+-----------------+-----------------+-----------------+\n");
        printf("| %-5s | %-18s | %-20s | %-15s | %-15s | %-15s |\n", "STT", "ID", "Ho Va Ten", "So Du", "T.Thai", "SDT");
        printf("+-------+--------------------+----------------------+-----------------+-----------------+-----------------+\n");

        for (int i = startIndex; i < endIndex; i++) {
            const char* status_str = accounts[i].status == 1 ? "Dang hoat dong" : "Da bi khoa";
            printf("| %-5d | %-18s | %-20s | %-15.3f | %-15s | %-15s |\n",
                    i + 1, accounts[i].accountId, accounts[i].fullName, accounts[i].balance, status_str, accounts[i].phone);
        }
        printf("+-------+--------------------+----------------------+-----------------+-----------------+-----------------+\n");
        
        
        printf("\n-------------------------------------------------\n");
        if (currentPage < totalPages) {
            printf("Ban co muon doc trang tiep theo khong? (Nhap so trang 1 - %d hoac 'E' de thoat): ", totalPages);
        } else {
            printf("*** DA HIEN THI HET DANH SACH ***\n");
            printf("Nhap 'E' de quay lai Menu chinh: ");
        }

        char actionBuffer[20];
        if (fgets(actionBuffer, sizeof(actionBuffer), stdin) == NULL) {
            continue; 
        }
        removeNewline(actionBuffer);
        
        if (strcmp(actionBuffer, "e") == 0 || strcmp(actionBuffer, "E") == 0) {
            break; 
        }
        
        int next_page;
        if (sscanf(actionBuffer, "%d", &next_page) == 1) {
            if (next_page >= 1 && next_page <= totalPages) {
                currentPage = next_page;
            } else {
                printf("\nSo trang nhap vao khong hop le (%d). Nhan enter de hien thi lai trang hien tai.\n", next_page);
                getchar();
            }
        } else {
            printf("\nDu lieu nhap khong hop le. Nhan enter de hien thi lai trang hien tai.\n");
            getchar();
        }
    }
}

//TASK 6: Sap xep danh sach

void sortAccounts() {
    int choice;
    Account temp;

    system("cls");
    printf("\n    *** SAP XEP DANH SACH TAI KHOAN ***\n\n");

    if (accountCount == 0) {
        printf("Khong co du lieu can sap xep!\n");
        printf("\nNhan enter de quay lai menu chinh\n");
        getchar();
        return;
    }

    printf("Chon phuong thuc sap xep:\n");
    printf("1.Theo so du giam dan\n");
    printf("2.Theo ten A-Z\n");
    printf("Lua chon cua ban: ");
    
    if (scanf("%d", &choice) != 1) {
        while(getchar() != '\n'); 
        choice = -1; 
    }
    while(getchar() != '\n'); 

    if (choice == 1) {
        for (int i = 0; i < accountCount - 1; i++) {
            for (int j = 0; j < accountCount - i - 1; j++) {
                if (accounts[j].balance < accounts[j + 1].balance) {
                    temp = accounts[j];
                    accounts[j] = accounts[j + 1];
                    accounts[j + 1] = temp;
                }
            }
        }
        printf("\nDa sap xep xong theo so du giam dan!\n");
    } 
    else if (choice == 2) {
        for (int i = 0; i < accountCount - 1; i++) {
            for (int j = 0; j < accountCount - i - 1; j++) {
                if (strcmp(accounts[j].fullName, accounts[j + 1].fullName) > 0) {
                    temp = accounts[j];
                    accounts[j] = accounts[j + 1];
                    accounts[j + 1] = temp;
                }
            }
        }
        printf("\nDa sap xep xong theo ten A-Z!\n");
    } 
    else {
        printf("\nLua chon khong hop le!\n");
    }

    printf("\nNhan enter de quay lai menu chinh\n");
    getchar();
}

// TASK 7: Giao dich chuyen khoan (F07)
void transferMoney() {
    char senderId[20], receiverId[20];
    double amount;
    int senderIndex = -1, receiverIndex = -1;
    char inputBuffer[50];
    char continue_choice = 'y';
    
    time_t current_time;
    struct tm *tm_info;
    char timestamp_str[20];

    system("cls");
    
    do {
        senderIndex = -1; 
        receiverIndex = -1;
        
        printf("\n      *** GIAO DICH CHUYEN KHOAN ***\n\n");

        while (senderIndex == -1) {
            printf("Nhap ID nguoi gui (Sender ID, nhap 0 de thoat): ");
            fgets(senderId, sizeof(senderId), stdin);
            removeNewline(senderId);
            trimSpaces(senderId);

            if (strcmp(senderId, "0") == 0) return;

            senderIndex = findAccountIndex(senderId);

            if (senderIndex == -1) {
                printf("ID nguoi gui khong ton ton tai. Vui long kiem tra lai.\n");
                continue;
            }
            
            if (accounts[senderIndex].status == 0) {
                printf("Tai khoan nguoi gui %s da bi khoa. Khong the thuc hien giao dich.\n", senderId);
                senderIndex = -1;
            }
        }
        
        while (receiverIndex == -1) {
            printf("Nhap ID nguoi nhan (Receiver ID, nhap 0 de thoat): ");
            fgets(receiverId, sizeof(receiverId), stdin);
            removeNewline(receiverId);
            trimSpaces(receiverId);

            if (strcmp(receiverId, "0") == 0) return;
            
            receiverIndex = findAccountIndex(receiverId);

            if (receiverIndex == -1) {
                printf("ID nguoi nhan khong ton tai. Vui long kiem tra lai.\n");
                continue;
            }
            
            if (strcmp(senderId, receiverId) == 0) {
                printf("ID nguoi gui va nguoi nhan phai khac nhau.\n");
                receiverIndex = -1;
            }
        }
        
        while (1) {
            printf("Nhap so tien chuyen (So du hien tai: %.2f): ", accounts[senderIndex].balance);
            
            if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL || sscanf(inputBuffer, "%lf", &amount) != 1) {
                 printf("Vui long nhap mot so hop le.\n");
                 continue;
            }

            if (amount < 0.5) {
                printf("So tien chuyen toi thieu phai la 0.5.\n");
                continue;
            }
            
            if (amount > accounts[senderIndex].balance) {
                printf("So du khong du de thuc hien giao dich.\n");
                continue;
            }

            break;
        }
        
        char confirm_transfer;
        printf("\nChuyen %.2f tu %s sang %s. Ban co chac chan? (y/n): ", amount, senderId, receiverId);
        scanf(" %c", &confirm_transfer);
        while(getchar() != '\n'); 
        
        if (confirm_transfer != 'y' && confirm_transfer != 'Y') {
            printf("\nGiao dich da bi huy bo.\n");
            goto ask_continue;
        }
        
        accounts[senderIndex].balance -= amount;
        accounts[receiverIndex].balance += amount;
        
        if (transactionCount < MAX_TRANSACTIONS) {
            Transaction newTrans;
            
            time(&current_time);
            tm_info = localtime(&current_time);
            strftime(timestamp_str, sizeof(timestamp_str), "%Y-%m-%d %H:%M:%S", tm_info); 
            
            strcpy(newTrans.senderId, senderId);
            strcpy(newTrans.receiverId, receiverId);
            newTrans.amount = amount;
            strcpy(newTrans.timestamp, timestamp_str);
            
            transactions[transactionCount] = newTrans;
            transactionCount++;
        }

        printf("\nChuyen khoan thanh cong!\n");
        printf("So du moi cua Sender [%s]: %.2f\n", senderId, accounts[senderIndex].balance);
        printf("So du moi cua Receiver [%s]: %.2f\n", receiverId, accounts[receiverIndex].balance);
        
        ask_continue: ;

        printf("\nBan co muon thuc hien them giao dich nao nua khong? (y/n): ");
        scanf(" %c", &continue_choice);
        while(getchar() != '\n');
        
        system("cls");
        
    } while (continue_choice == 'y' || continue_choice == 'Y');
    
    printf("\nKet thuc giao dich. Nhan enter de quay lai Menu chinh...");
    getchar(); 
}

// TASK 8: Xem lich su giao dich
void viewTransactionHistory() {
    char targetId[20];
    int targetIndex = -1;
    int transactionFoundCount = 0;
    system("cls");

    printf("\n      *** XEM LICH SU GIAO DICH ***\n\n");
    
    while (1) {
        printf("Nhap ID tai khoan can xem lich su (Nhap 0 de thoat): ");
        fgets(targetId, sizeof(targetId), stdin);
        removeNewline(targetId);
        trimSpaces(targetId);

        if (strcmp(targetId, "0") == 0) return;

        targetIndex = findAccountIndex(targetId);

        if (targetIndex == -1) {
            printf("Tai khoan %s khong ton tai. Vui long thu lai!\n", targetId);
            continue;
        }
        break;
    }

    if (transactionCount == 0) {
        printf("\n He thong hien tai chua co bat ky giao dich nao duoc ghi lai.\n");
        printf("\nNhan enter de quay lai Menu chinh...");
        getchar();
        return;
    }

    printf("\n--- Lich su giao dich cua tai khoan ID: %s ---\n", targetId);   
    printf("+-------+----------+----------------------+-----------------+---------------------+\n");
    printf("| %-5s | %-8s | %-20s | %-15s | %-19s |\n", "STT", "Loai", "Tai Khoan Lien Quan", "So Tien", "Thoi Gian");
    printf("+-------+----------+----------------------+-----------------+---------------------+\n");

    for (int i = 0; i < transactionCount; i++) {
        const char* type = NULL;
        char partyId[20]; 

        if (strcmp(transactions[i].senderId, targetId) == 0) {
            type = "Gui di";
            strcpy(partyId, transactions[i].receiverId);
        }
        else if (strcmp(transactions[i].receiverId, targetId) == 0) {
            type = "Nhan ve";
            strcpy(partyId, transactions[i].senderId);
        }
        
        if (type != NULL) {
            printf("| %-5d | %-8s | %-20s | %-15.2f | %-18s |\n",
                    transactionFoundCount + 1,
                    type,
                    partyId,
                    transactions[i].amount,
                    transactions[i].timestamp);
            transactionFoundCount++;
        }
    }

    printf("+-------+----------+----------------------+-----------------+---------------------+\n");

    if (transactionFoundCount == 0) {
        printf("| %-79s |\n", "Tai khoan nay chua co giao dich nao duoc ghi lai."); 
        printf("+---------------------------------------------------------------------------------+\n"); 
    } else {
        printf("| %-79s |\n", "Tim thay lich su giao dich thanh cong.");
        printf("+---------------------------------------------------------------------------------+\n");
        printf("Tong so giao dich tim thay: %d\n", transactionFoundCount);
    }
    
    printf("\n Nhan enter de quay lai Menu chinh...");
    getchar();
}
