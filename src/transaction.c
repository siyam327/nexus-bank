#include <stdio.h>
#include <string.h>

#include "transaction.h"
#include "account.h"


/* Record transaction */
int record_txn(int account_id, TxnType type, double amount,
               int related_id, double balance_after, const char *note)
{
    FILE *fp;

    fp = fopen(TRANSACTIONS_FILE, "a");

    if(fp == NULL)
    {
        printf("Transaction file cannot be opened.\n");
        return 0;
    }

    fprintf(fp, "%d %.2lf %.2lf %d %s\n",
            account_id,
            amount,
            balance_after,
            related_id,
            note);

    fclose(fp);

    return 1;
}


/* Deposit */
int deposit(Session *s)
{
    Account acc;
    double amount;
    double max_balance;

    if(find_account(s->account.id, &acc) == 0)
    {
        printf("Account not found.\n");
        return 0;
    }

    printf("Enter deposit amount: ");
    scanf("%lf", &amount);

    if(amount <= 0)
    {
        printf("Invalid amount.\n");
        return 0;
    }

    if(acc.tier == PREMIUM)
    {
        max_balance = PREMIUM_MAX_BALANCE;
    }
    else
    {
        max_balance = NORMAL_MAX_BALANCE;
    }

    if(acc.balance + amount > max_balance)
    {
        printf("Maximum balance limit exceeded.\n");
        return 0;
    }

    acc.balance = acc.balance + amount;

    if(update_balance(acc.id, acc.balance) == 0)
    {
        printf("Balance update failed.\n");
        return 0;
    }

    record_txn(
        acc.id,
        TXN_DEPOSIT,
        amount,
        0,
        acc.balance,
        "Deposit"
    );

    printf("Deposit successful.\n");
    printf("New balance: %.2lf\n", acc.balance);

    return 1;
}


/* Withdraw */
int withdraw(Session *s)
{
    Account acc;
    double amount;
    double limit;

    if(find_account(s->account.id, &acc) == 0)
    {
        printf("Account not found.\n");
        return 0;
    }

    printf("Enter withdrawal amount: ");
    scanf("%lf", &amount);

    if(amount <= 0)
    {
        printf("Invalid amount.\n");
        return 0;
    }

    if(acc.tier == PREMIUM)
    {
        limit = PREMIUM_DAILY_WITHDRAW;
    }
    else
    {
        limit = NORMAL_DAILY_WITHDRAW;
    }

    if(acc.withdrawn_today + amount > limit)
    {
        printf("Daily withdrawal limit exceeded.\n");
        printf("Your limit is %.2lf\n", limit);
        return 0;
    }

    if(amount > acc.balance)
    {
        printf("Insufficient balance.\n");
        return 0;
    }

    acc.balance = acc.balance - amount;

    if(update_balance(acc.id, acc.balance) == 0)
    {
        printf("Balance update failed.\n");
        return 0;
    }

    record_txn(
        acc.id,
        TXN_WITHDRAW,
        amount,
        0,
        acc.balance,
        "Withdraw"
    );

    printf("Withdrawal successful.\n");
    printf("New balance: %.2lf\n", acc.balance);

    return 1;
}


/* Transfer */
int transfer(Session *s)
{
    Account sender;
    Account receiver;

    int receiver_id;

    double amount;
    double limit;
    double max_balance;


    if(find_account(s->account.id, &sender) == 0)
    {
        printf("Sender account not found.\n");
        return 0;
    }


    printf("Enter receiver account ID: ");
    scanf("%d", &receiver_id);


    if(receiver_id == sender.id)
    {
        printf("You cannot transfer to your own account.\n");
        return 0;
    }


    if(find_account(receiver_id, &receiver) == 0)
    {
        printf("Receiver account not found.\n");
        return 0;
    }


    printf("Enter transfer amount: ");
    scanf("%lf", &amount);


    if(amount <= 0)
    {
        printf("Invalid amount.\n");
        return 0;
    }


    if(sender.tier == PREMIUM)
    {
        limit = PREMIUM_TRANSFER_LIMIT;
    }
    else
    {
        limit = NORMAL_TRANSFER_LIMIT;
    }


    if(amount > limit)
    {
        printf("Transfer limit exceeded.\n");
        printf("Your limit is %.2lf\n", limit);
        return 0;
    }


    if(amount > sender.balance)
    {
        printf("Insufficient balance.\n");
        return 0;
    }


    if(receiver.tier == PREMIUM)
    {
        max_balance = PREMIUM_MAX_BALANCE;
    }
    else
    {
        max_balance = NORMAL_MAX_BALANCE;
    }


    if(receiver.balance + amount > max_balance)
    {
        printf("Receiver balance limit exceeded.\n");
        return 0;
    }


    sender.balance = sender.balance - amount;
    receiver.balance = receiver.balance + amount;


    if(update_balance(sender.id, sender.balance) == 0)
    {
        printf("Sender balance update failed.\n");
        return 0;
    }


    if(update_balance(receiver.id, receiver.balance) == 0)
    {
        printf("Receiver balance update failed.\n");
        return 0;
    }


    record_txn(
        sender.id,
        TXN_TRANSFER_OUT,
        amount,
        receiver.id,
        sender.balance,
        "Transfer sent"
    );


    record_txn(
        receiver.id,
        TXN_TRANSFER_IN,
        amount,
        sender.id,
        receiver.balance,
        "Transfer received"
    );


    printf("Transfer successful.\n");
    printf("Your new balance: %.2lf\n", sender.balance);

    return 1;
}


/* View transaction history */
int view_history(const Session *s)
{
    FILE *fp;

    int account_id;
    double amount;
    double balance;
    int related_id;
    char note[50];

    int found = 0;


    fp = fopen(TRANSACTIONS_FILE, "r");

    if(fp == NULL)
    {
        printf("No transaction history found.\n");
        return 0;
    }


    printf("\n===== TRANSACTION HISTORY =====\n");


    while(fscanf(fp, "%d %lf %lf %d %s",
                 &account_id,
                 &amount,
                 &balance,
                 &related_id,
                 note) == 5)
    {
        if(account_id == s->account.id)
        {
            printf("\nAmount: %.2lf", amount);
            printf("\nBalance: %.2lf", balance);
            printf("\nRelated Account: %d", related_id);
            printf("\nType: %s\n", note);

            found = 1;
        }
    }


    fclose(fp);


    if(found == 0)
    {
        printf("No transactions found.\n");
    }

    return found;
}


/* Export statement */
int export_statement(const Session *s)
{
    FILE *fp;
    FILE *out;

    int account_id;
    double amount;
    double balance;
    int related_id;
    char note[50];

    int found = 0;


    fp = fopen(TRANSACTIONS_FILE, "r");

    if(fp == NULL)
    {
        printf("No transaction history found.\n");
        return 0;
    }


    out = fopen("statement.txt", "w");

    if(out == NULL)
    {
        fclose(fp);
        printf("Cannot create statement.\n");
        return 0;
    }


    fprintf(out, "===== ACCOUNT STATEMENT =====\n");
    fprintf(out, "Account ID: %d\n", s->account.id);
    fprintf(out, "Name: %s\n\n", s->account.name);


    while(fscanf(fp, "%d %lf %lf %d %s",
                 &account_id,
                 &amount,
                 &balance,
                 &related_id,
                 note) == 5)
    {
        if(account_id == s->account.id)
        {
            fprintf(out, "Amount: %.2lf\n", amount);
            fprintf(out, "Balance: %.2lf\n", balance);
            fprintf(out, "Related Account: %d\n", related_id);
            fprintf(out, "Type: %s\n\n", note);

            found = 1;
        }
    }


    fclose(fp);
    fclose(out);


    if(found == 0)
    {
        printf("No transactions found.\n");
        return 0;
    }


    printf("Statement created successfully.\n");

    return 1;
}
