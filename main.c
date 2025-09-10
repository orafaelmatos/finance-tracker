#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TRANSACTIONS 100
#define FILE_NAME "transactions.txt"

typedef enum { INCOME, EXPENSE } TransactionType;

typedef struct {
    TransactionType type;
    double amount;
    char description[50];
} Transaction;

Transaction transactions[MAX_TRANSACTIONS];
int count = 0;
double totalIncome = 0;
double totalExpense = 0;

void saveTransactionsToFile() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Error saving transactions!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        const char *typeStr = (transactions[i].type == INCOME) ? "Income" : "Expense";
        fprintf(file, "%s - %.2lf %s\n", typeStr, transactions[i].amount, transactions[i].description);
    }
    fclose(file);
}

void loadTransactionsFromFile() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return;

    char typeStr[10];
    while (fscanf(file, "%s %lf %[^\n]", typeStr, &transactions[count].amount, transactions[count].description) == 3) {
        if (strcmp(typeStr, "Income") == 0) {
            transactions[count].type = INCOME;
            totalIncome += transactions[count].amount;
        } else if (strcmp(typeStr, "Expense") == 0) {
            transactions[count].type = EXPENSE;
            totalExpense += transactions[count].amount;
        }
        count++;
    }
    fclose(file);
}

void addTransaction() {
    if (count >= MAX_TRANSACTIONS) {
        printf("Transaction limit reached!\n");
        return;
    }

    int typeChoice;
    printf("Enter type (1 = income, 2 = expense): ");
    scanf("%d", &typeChoice);

    if (typeChoice != 1 && typeChoice != 2) {
        printf("Invalid choice.\n");
        return;
    }
    transactions[count].type = (typeChoice == 1) ? INCOME : EXPENSE;

    printf("Enter amount: ");
    scanf("%lf", &transactions[count].amount);

    if (transactions[count].amount <= 0) {
        printf("Amount must be positive.\n");
        return;
    }

    printf("Enter description: ");
    getchar(); // clear newline
    fgets(transactions[count].description, sizeof(transactions[count].description), stdin);
    transactions[count].description[strcspn(transactions[count].description, "\n")] = 0;

    if (transactions[count].type == INCOME) {
        totalIncome += transactions[count].amount;
    } else {
        totalExpense += transactions[count].amount;
    }

    count++;
    saveTransactionsToFile();
    printf("Transaction added successfully!\n");
}

void listTransactions() {
    printf("\n--- Transactions ---\n");
    printf("%-4s %-10s %-10s %s\n", "No.", "Type", "Amount", "Description");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-4d %-10s %-10.2lf %s\n",
            i + 1,
            (transactions[i].type == INCOME) ? "Income" : "Expense",
            transactions[i].amount,
            transactions[i].description);
    }
}

void showSummary() {
    printf("\n--- Summary ---\n");
    printf("Total Income : %.2lf\n", totalIncome);
    printf("Total Expense: %.2lf\n", totalExpense);
    printf("Balance      : %.2lf\n", totalIncome - totalExpense);
}

int main() {
    int choice;
    loadTransactionsFromFile();  // load previous data

    do {
        printf("\n==== Personal Finance Tracker ====\n");
        printf("1. Add transaction\n");
        printf("2. List transactions\n");
        printf("3. Show summary\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTransaction(); break;
            case 2: listTransactions(); break;
            case 3: showSummary(); break;
            case 0: printf("Saving and exiting. Goodbye!\n"); break;
            default: printf("Invalid option. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}
