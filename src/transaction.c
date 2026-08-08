/* =========================================================
   1. MAIN.C
   ========================================================= */

#include <stdio.h>

#include "cashdesk.h"


int main()
{
    Customer customers[MAX_CUSTOMERS];

    int customerCount;

    int choice;

    int id;


    /* Array */

    int menuNumbers[9] =
    {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };


    /* Multidimensional Array */

    char menuNames[9][40] =
    {
        "Create Customer",
        "Deposit Money",
        "Withdraw Money",
        "Peer-to-Peer Transfer",
        "Show Customer Information",
        "Show Customer Statement",
        "Show All Transaction History",
        "Reset Daily Withdrawal Limit",
        "Exit"
    };


    /* Pointer */

    Customer *customerPointer;


    customerCount =
        loadCustomers(
            customers
        );


    /* Pointer Assignment */

    customerPointer =
        customers;


    printf(
        "\n============================================\n"
    );

    printf(
        "          CASH DESK SYSTEM\n"
    );

    printf(
        "============================================\n"
    );


    /* Nested Loop */

    {
        int i;
        int j;

        for (
            i = 0;
            i < 1;
            i++
        )
        {
            for (
                j = 0;
                j < 1;
                j++
            )
            {
                printf(
                    "Welcome to Cash Desk System!\n"
                );
            }
        }
    }


    do
    {
        printf(
            "\n============== MAIN MENU ==============\n"
        );


        /* Loop + Multidimensional Array */

        {
            int i;

            for (
                i = 0;
                i < 9;
                i++
            )
            {
                printf(
                    "%d. %s\n",
                    menuNumbers[i],
                    menuNames[i]
                );
            }
        }


        printf(
            "========================================\n"
        );


        printf(
            "Enter your choice: "
        );


        scanf(
            "%d",
            &choice
        );


        /* Switch */

        switch (choice)
        {
            case 1:

                customerCount =
                    createCustomer(
                        customers,
                        customerCount
                    );


                saveCustomers(
                    customers,
                    customerCount
                );


                break;


            case 2:

                if (
                    depositMoney(
                        customers,
                        customerCount
                    )
                )
                {
                    saveCustomers(
                        customers,
                        customerCount
                    );
                }


                break;


            case 3:

                if (
                    withdrawMoney(
                        customers,
                        customerCount
                    )
                )
                {
                    saveCustomers(
                        customers,
                        customerCount
                    );
                }


                break;


            case 4:

                if (
                    transferMoney(
                        customers,
                        customerCount
                    )
                )
                {
                    saveCustomers(
                        customers,
                        customerCount
                    );
                }


                break;


            case 5:

                showCustomer(
                    customers,
                    customerCount
                );


                break;


            case 6:

                printf(
                    "\nEnter Customer ID: "
                );


                scanf(
                    "%d",
                    &id
                );


                showStatement(
                    id
                );


                break;


            case 7:

                showTransactionHistory();


                break;


            case 8:

                resetDailyWithdrawals(
                    customers,
                    customerCount
                );


                saveCustomers(
                    customers,
                    customerCount
                );


                printf(
                    "\nDaily withdrawal reset successful.\n"
                );


                break;


            case 9:

                saveCustomers(
                    customers,
                    customerCount
                );


                printf(
                    "\nData saved successfully.\n"
                );


                printf(
                    "Thank you for using Cash Desk System!\n"
                );


                break;


            default:

                printf(
                    "\nInvalid choice!\n"
                );
        }


    }
    while (
        choice != 9
    );


    return 0;
}


/* =========================================================
   2. CASHDESK.C
   ========================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cashdesk.h"


/* Find Customer */

int findCustomer(Customer customers[], int count, int id)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (customers[i].id == id)
        {
            return i;
        }
    }

    return -1;
}


/* Load Customer Data from File */

int loadCustomers(Customer customers[])
{
    FILE *file;

    int count = 0;

    file = fopen("customers.dat", "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (
        count < MAX_CUSTOMERS &&
        fread(
            &customers[count],
            sizeof(Customer),
            1,
            file
        ) == 1
    )
    {
        count++;
    }

    fclose(file);

    return count;
}


/* Save Customer Data */

int saveCustomers(Customer customers[], int count)
{
    FILE *file;

    file = fopen("customers.dat", "wb");

    if (file == NULL)
    {
        printf("\nError saving customer data.\n");

        return 0;
    }

    fwrite(
        customers,
        sizeof(Customer),
        count,
        file
    );

    fclose(file);

    return 1;
}


/* Get Maximum Balance */

int getMaxBalance(int type)
{
    if (type == PREMIUM)
    {
        return PREMIUM_MAX_BALANCE;
    }

    return NORMAL_MAX_BALANCE;
}


/* Get Daily Withdrawal Limit */

int getDailyWithdrawalLimit(int type)
{
    if (type == PREMIUM)
    {
        return PREMIUM_DAILY_WITHDRAWAL;
    }

    return NORMAL_DAILY_WITHDRAWAL;
}


/* Get Transfer Limit */

int getTransferLimit(int type)
{
    if (type == PREMIUM)
    {
        return PREMIUM_TRANSFER_LIMIT;
    }

    return NORMAL_TRANSFER_LIMIT;
}


/* Create New Customer */

int createCustomer(
    Customer customers[],
    int count
)
{
    int id;
    int type;

    if (count >= MAX_CUSTOMERS)
    {
        printf(
            "\nCustomer limit reached.\n"
        );

        return count;
    }

    printf(
        "\nEnter Customer ID: "
    );

    scanf("%d", &id);


    if (
        findCustomer(
            customers,
            count,
            id
        ) != -1
    )
    {
        printf(
            "\nCustomer ID already exists.\n"
        );

        return count;
    }


    customers[count].id = id;


    printf(
        "Enter Customer Name: "
    );

    scanf(
        " %49[^\n]",
        customers[count].name
    );


    printf(
        "\nSelect Customer Type:\n"
    );

    printf(
        "1. Normal Customer\n"
    );

    printf(
        "2. Premium Customer\n"
    );

    printf(
        "Enter choice: "
    );

    scanf(
        "%d",
        &type
    );


    if (
        type != NORMAL &&
        type != PREMIUM
    )
    {
        printf(
            "\nInvalid customer type.\n"
        );

        return count;
    }


    customers[count].type = type;

    customers[count].balance = 0;

    customers[count].dailyWithdrawn = 0;


    count++;


    printf(
        "\nCustomer created successfully!\n"
    );


    return count;
}


/* Deposit Money */

int depositMoney(
    Customer customers[],
    int count
)
{
    int id;
    int index;

    double amount;
    double maxBalance;


    printf(
        "\nEnter Customer ID: "
    );

    scanf(
        "%d",
        &id
    );


    index =
        findCustomer(
            customers,
            count,
            id
        );


    if (index == -1)
    {
        printf(
            "\nCustomer not found.\n"
        );

        return 0;
    }


    printf(
        "Enter Deposit Amount: "
    );

    scanf(
        "%lf",
        &amount
    );


    if (amount <= 0)
    {
        printf(
            "\nInvalid amount.\n"
        );

        return 0;
    }


    maxBalance =
        getMaxBalance(
            customers[index].type
        );


    if (
        customers[index].balance +
        amount >
        maxBalance
    )
    {
        printf(
            "\nMaximum balance limit exceeded.\n"
        );

        return 0;
    }


    customers[index].balance += amount;


    addTransaction(
        id,
        "DEPOSIT",
        amount,
        0
    );


    printf(
        "\nDeposit Successful!\n"
    );

    printf(
        "New Balance: $%.2lf\n",
        customers[index].balance
    );


    return 1;
}


/* Withdraw Money */

int withdrawMoney(
    Customer customers[],
    int count
)
{
    int id;
    int index;

    double amount;
    double dailyLimit;


    printf(
        "\nEnter Customer ID: "
    );

    scanf(
        "%d",
        &id
    );


    index =
        findCustomer(
            customers,
            count,
            id
        );


    if (index == -1)
    {
        printf(
            "\nCustomer not found.\n"
        );

        return 0;
    }


    printf(
        "Enter Withdrawal Amount: "
    );

    scanf(
        "%lf",
        &amount
    );


    dailyLimit =
        getDailyWithdrawalLimit(
            customers[index].type
        );


    if (
        customers[index].dailyWithdrawn +
        amount >
        dailyLimit
    )
    {
        printf(
            "\nDaily withdrawal limit exceeded.\n"
        );

        return 0;
    }


    if (
        amount >
        customers[index].balance
    )
    {
        printf(
            "\nInsufficient balance.\n"
        );

        return 0;
    }


    customers[index].balance -= amount;

    customers[index].dailyWithdrawn += amount;


    addTransaction(
        id,
        "WITHDRAW",
        amount,
        0
    );


    printf(
        "\nWithdrawal Successful!\n"
    );

    printf(
        "New Balance: $%.2lf\n",
        customers[index].balance
    );


    return 1;
}


/* Peer-to-Peer Transfer */

int transferMoney(
    Customer customers[],
    int count
)
{
    int senderId;
    int receiverId;

    int senderIndex;
    int receiverIndex;

    double amount;
    double transferLimit;


    printf(
        "\nEnter Sender Customer ID: "
    );

    scanf(
        "%d",
        &senderId
    );


    printf(
        "Enter Receiver Customer ID: "
    );

    scanf(
        "%d",
        &receiverId
    );


    senderIndex =
        findCustomer(
            customers,
            count,
            senderId
        );


    receiverIndex =
        findCustomer(
            customers,
            count,
            receiverId
        );


    if (
        senderIndex == -1 ||
        receiverIndex == -1
    )
    {
        printf(
            "\nInvalid Sender or Receiver ID.\n"
        );

        return 0;
    }


    if (
        senderId == receiverId
    )
    {
        printf(
            "\nCannot transfer to same account.\n"
        );

        return 0;
    }


    printf(
        "Enter Transfer Amount: "
    );

    scanf(
        "%lf",
        &amount
    );


    transferLimit =
        getTransferLimit(
            customers[senderIndex].type
        );


    if (
        amount >
        transferLimit
    )
    {
        printf(
            "\nTransfer limit exceeded.\n"
        );

        return 0;
    }


    if (
        amount >
        customers[senderIndex].balance
    )
    {
        printf(
            "\nInsufficient balance.\n"
        );

        return 0;
    }


    if (
        customers[receiverIndex].balance +
        amount >
        getMaxBalance(
            customers[receiverIndex].type
        )
    )
    {
        printf(
            "\nReceiver maximum balance limit exceeded.\n"
        );

        return 0;
    }


    customers[senderIndex].balance -= amount;

    customers[receiverIndex].balance += amount;


    addTransaction(
        senderId,
        "TRANSFER_SENT",
        amount,
        receiverId
    );


    addTransaction(
        receiverId,
        "TRANSFER_RECEIVED",
        amount,
        senderId
    );


    printf(
        "\nTransfer Successful!\n"
    );


    return 1;
}


/* Add Transaction */

int addTransaction(
    int customerId,
    char type[],
    double amount,
    int receiverId
)
{
    FILE *file;

    Transaction transaction;

    time_t currentTime;

    struct tm *localTime;


    file =
        fopen(
            "transactions.txt",
            "a"
        );


    if (file == NULL)
    {
        return 0;
    }


    currentTime =
        time(NULL);


    localTime =
        localtime(
            &currentTime
        );


    transaction.transactionId =
        (int)currentTime;


    transaction.customerId =
        customerId;


    strcpy(
        transaction.type,
        type
    );


    transaction.amount =
        amount;


    transaction.receiverId =
        receiverId;


    strftime(
        transaction.date,
        sizeof(
            transaction.date
        ),
        "%Y-%m-%d %H:%M:%S",
        localTime
    );


    fprintf(
        file,
        "%d|%d|%s|%.2lf|%d|%s\n",

        transaction.transactionId,

        transaction.customerId,

        transaction.type,

        transaction.amount,

        transaction.receiverId,

        transaction.date
    );


    fclose(file);


    return 1;
}


/* Show Customer Information */

int showCustomer(
    Customer customers[],
    int count
)
{
    int id;

    int index;


    printf(
        "\nEnter Customer ID: "
    );

    scanf(
        "%d",
        &id
    );


    index =
        findCustomer(
            customers,
            count,
            id
        );


    if (index == -1)
    {
        printf(
            "\nCustomer not found.\n"
        );

        return 0;
    }


    printf(
        "\nCustomer ID: %d\n",
        customers[index].id
    );


    printf(
        "Name: %s\n",
        customers[index].name
    );


    if (
        customers[index].type ==
        PREMIUM
    )
    {
        printf(
            "Type: Premium\n"
        );
    }
    else
    {
        printf(
            "Type: Normal\n"
        );
    }


    printf(
        "Balance: $%.2lf\n",
        customers[index].balance
    );


    return 1;
}


/* Show All Transaction History */

int showTransactionHistory()
{
    FILE *file;

    char line[300];


    file =
        fopen(
            "transactions.txt",
            "r"
        );


    if (file == NULL)
    {
        printf(
            "\nNo transaction history found.\n"
        );

        return 0;
    }


    printf(
        "\n========== TRANSACTION HISTORY ==========\n"
    );


    while (
        fgets(
            line,
            sizeof(line),
            file
        ) != NULL
    )
    {
        printf(
            "%s",
            line
        );
    }


    fclose(file);


    return 1;
}


/* Customer Statement */

int showStatement(
    int customerId
)
{
    FILE *file;

    int transactionId;
    int customer;

    char type[30];

    char date[30];

    double amount;

    int receiverId;

    int found = 0;


    file =
        fopen(
            "transactions.txt",
            "r"
        );


    if (file == NULL)
    {
        printf(
            "\nNo transactions found.\n"
        );

        return 0;
    }


    printf(
        "\n========== ACCOUNT STATEMENT ==========\n"
    );


    while (
        fscanf(
            file,
            "%d|%d|%29[^|]|%lf|%d|%29[^\n]\n",

            &transactionId,

            &customer,

            type,

            &amount,

            &receiverId,

            date
        ) == 6
    )
    {
        if (
            customer ==
            customerId
        )
        {
            printf(
                "\nTransaction ID: %d",
                transactionId
            );

            printf(
                "\nType: %s",
                type
            );

            printf(
                "\nAmount: $%.2lf",
                amount
            );

            printf(
                "\nDate: %s\n",
                date
            );

            found = 1;
        }
    }


    fclose(file);


    if (found == 0)
    {
        printf(
            "\nNo transactions found.\n"
        );
    }


    return found;
}


/* Reset Daily Withdrawal */

int resetDailyWithdrawals(
    Customer customers[],
    int count
)
{
    int i;


    for (
        i = 0;
        i < count;
        i++
    )
    {
        customers[i].dailyWithdrawn = 0;
    }


    return 1;
}


/* =========================================================
   3. CASHDESK.H
   ========================================================= */

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
