#ifndef CASHDESK_H
#define CASHDESK_H

#define MAX_CUSTOMERS 100
#define MAX_NAME 50
#define MAX_TRANSACTIONS 1000

#define NORMAL 1
#define PREMIUM 2

#define NORMAL_MAX_BALANCE 100000.0
#define PREMIUM_MAX_BALANCE 1000000.0

#define NORMAL_DAILY_WITHDRAWAL 20000.0
#define PREMIUM_DAILY_WITHDRAWAL 100000.0

#define NORMAL_TRANSFER_LIMIT 10000.0
#define PREMIUM_TRANSFER_LIMIT 100000.0


/* C Structure */

typedef struct
{
    int id;
    char name[MAX_NAME];
    int type;
    double balance;
    double dailyWithdrawn;
} Customer;


typedef struct
{
    int transactionId;
    int customerId;
    char type[30];
    double amount;
    int receiverId;
    char date[30];
} Transaction;


/* Function Prototypes */

int loadCustomers(Customer customers[]);
int saveCustomers(Customer customers[], int count);

int createCustomer(Customer customers[], int count);

int findCustomer(Customer customers[], int count, int id);

int depositMoney(Customer customers[], int count);

int withdrawMoney(Customer customers[], int count);

int transferMoney(Customer customers[], int count);

int addTransaction(
    int customerId,
    char type[],
    double amount,
    int receiverId
);

int showCustomer(Customer customers[], int count);

int showTransactionHistory();

int showStatement(int customerId);

int resetDailyWithdrawals(
    Customer customers[],
    int count
);

int getMaxBalance(int type);

int getDailyWithdrawalLimit(int type);

int getTransferLimit(int type);

#endif
