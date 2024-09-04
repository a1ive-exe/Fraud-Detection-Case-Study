#include <stdio.h>
#include <stdlib.h>

// Transaction structure
typedef struct {
    int transactionID;
    float amount;
    int isFraud; // 0 for non-fraudulent, 1 for fraudulent
} Transaction;

// Function to count lines in the dataset
int countLines(FILE *file) {
    int lines = 0;
    char ch;
    while(!feof(file)) {
        ch = fgetc(file);
        if(ch == '\n') {
            lines++;
        }
    }
    rewind(file); // Reset file pointer to the beginning
    return lines - 1; // Exclude header line
}

// Function to load transactions from a CSV file
Transaction* loadTransactions(const char* filename, int* numTransactions) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    *numTransactions = countLines(file);

    Transaction* transactions = (Transaction*)malloc(*numTransactions * sizeof(Transaction));

    // Skip header line
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);

    // Read transactions
    for (int i = 0; i < *numTransactions; i++) {
        fscanf(file, "%d,%f,%d\n", &transactions[i].transactionID, &transactions[i].amount, &transactions[i].isFraud);
    }

    fclose(file);
    return transactions;
}

// Function to detect fraud based on a simple threshold
void detectFraud(Transaction* transactions, int numTransactions, float threshold) {
    printf("Transactions flagged as potential frauds:\n");
    printf("TransactionID\tAmount\t\tStatus\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < numTransactions; i++) {
        if (transactions[i].amount > threshold) {
            printf("%d\t\t%.2f\t\tFraud\n", transactions[i].transactionID, transactions[i].amount);
        }
    }
}

// Main function
int main() {
    const char* filename = "transactions.csv";
    float threshold = 1000.0; // Set threshold for fraud detection

    printf("Fraud Detection System\n");
    printf("----------------------\n");

    // Load transactions from file
    int numTransactions;
    Transaction* transactions = loadTransactions(filename, &numTransactions);

    // Call fraud detection function
    detectFraud(transactions, numTransactions, threshold);

    // Free dynamically allocated memory
    free(transactions);

    return 0;
}
