#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<conio.h>
typedef struct {
    int accountNumber;
    char name[50];
    char address[100];
    char phone[15];
    char email[50];
    char accountType[20]; // Savings, Current, etc.
    double balance;
    double loanAmount;
} Account;

void addAccount();
void viewAccounts();
void viewAccountDetails();
void depositMoney();
void withdrawMoney();
void manageLoans();

const char *filename = "accounts.dat";

void clearBuffer() {
    while (getchar() != '\n'); // Clear the input buffer
}

void addAccount() {
    FILE *file = fopen(filename, "ab");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Account acc;
    printf("\n--- Add New Account ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &acc.accountNumber);
    clearBuffer();
    printf("Enter Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    strtok(acc.name, "\n"); // Remove newline character
    printf("Enter Address: ");
    fgets(acc.address, sizeof(acc.address), stdin);
    strtok(acc.address, "\n");
    printf("Enter Phone: ");
    scanf("%s", acc.phone);
    printf("Enter Email: ");
    scanf("%s", acc.email);
    printf("Enter Account Type (Savings/Current): ");
    scanf("%s", acc.accountType);
    printf("Enter Initial Balance: ");
    scanf("%lf", &acc.balance);
    acc.loanAmount = 0.0; // Default loan amount

    fwrite(&acc, sizeof(Account), 1, file);
    fclose(file);

    printf("Account added successfully!\n");
}

void viewAccounts() {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("No accounts found.\n");
        return;
    }

    Account acc;
    printf("\n--- All Accounts ---\n");
    while (fread(&acc, sizeof(Account), 1, file)) {
        printf("Account Number: %d\n", acc.accountNumber);
        printf("Name: %s\n", acc.name);
        printf("Account Type: %s\n", acc.accountType);
        printf("Balance: %.2f\n", acc.balance);
        printf("Loan Amount: %.2f\n", acc.loanAmount);
        printf("-----------------------------\n");
    }
    fclose(file);
}

void viewAccountDetails() {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("No accounts found.\n");
        return;
    }

    int accountNumber;
    printf("\nEnter Account Number to View Details: ");
    scanf("%d", &accountNumber);

    Account acc;
    int found = 0;
    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.accountNumber == accountNumber) {
            found = 1;
            printf("\n--- Account Details ---\n");
            printf("Account Number: %d\n", acc.accountNumber);
            printf("Name: %s\n", acc.name);
            printf("Address: %s\n", acc.address);
            printf("Phone: %s\n", acc.phone);
            printf("Email: %s\n", acc.email);
            printf("Account Type: %s\n", acc.accountType);
            printf("Balance: %.2f\n", acc.balance);
            printf("Loan Amount: %.2f\n", acc.loanAmount);
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Account not found.\n");
    }
}

void depositMoney() {
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("No accounts found.\n");
        return;
    }

    int accountNumber;
    double amount;
    printf("\nEnter Account Number to Deposit Money: ");
    scanf("%d", &accountNumber);
    printf("Enter Amount to Deposit: ");
    scanf("%lf", &amount);

    Account acc;
    int found = 0;
    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.accountNumber == accountNumber) {
            found = 1;
            acc.balance += amount;
            fseek(file, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, file);
            printf("Money deposited successfully! New Balance: %.2f\n", acc.balance);
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Account not found.\n");
    }
}

void withdrawMoney() {
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("No accounts found.\n");
        return;
    }

    int accountNumber;
    double amount;
    printf("\nEnter Account Number to Withdraw Money: ");
    scanf("%d", &accountNumber);
    printf("Enter Amount to Withdraw: ");
    scanf("%lf", &amount);

    Account acc;
    int found = 0;
    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.accountNumber == accountNumber) {
            found = 1;
            if (acc.balance >= amount) {
                acc.balance -= amount;
                fseek(file, -sizeof(Account), SEEK_CUR);
                fwrite(&acc, sizeof(Account), 1, file);
                printf("Money withdrawn successfully! New Balance: %.2f\n", acc.balance);
            } else {
                printf("Insufficient balance!\n");
            }
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Account not found.\n");
    }
}

void manageLoans() {
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("No accounts found.\n");
        return;
    }

    int accountNumber;
    double loanAmount;
    printf("\nEnter Account Number to Manage Loan: ");
    scanf("%d", &accountNumber);
    printf("Enter Loan Amount to Add/Update: ");
    scanf("%lf", &loanAmount);

    Account acc;
    int found = 0;
    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.accountNumber == accountNumber) {
            found = 1;
            acc.loanAmount = loanAmount;
            fseek(file, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, file);
            printf("Loan updated successfully! New Loan Amount: %.2f\n", acc.loanAmount);
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Account not found.\n");
    }
}

int main() {
    int choice;
    while (1) {
        printf("\n--- Bank Management System ---\n");
        printf("1. Add Account\n");
        printf("2. View All Accounts\n");
        printf("3. View Account Details\n");
        printf("4. Deposit Money\n");
        printf("5. Withdraw Money\n");
        printf("6. Manage Loans\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearBuffer();
            continue;
        }

        switch (choice) {
            case 1:
                addAccount();
                break;
            case 2:
                viewAccounts();
                break;
            case 3:
                viewAccountDetails();
                break;
            case 4:
                depositMoney();
                break;
            case 5:
                withdrawMoney();
                break;
            case 6:
                manageLoans();
                break;
            case 7:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
