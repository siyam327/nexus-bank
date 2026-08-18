/*
   transaction.c
   Owner: Busrat Jahan
   ID:2026-2-60-032
*/

#include <stdio.h>
#include <string.h>

#include "../include/types.h"
#include "../include/account.h"
#include "../include/banking.h"


/* Save transaction in file */

int record_txn(int account_id, TxnType type, double amount,
               int related_id, double balance_after, const char *note)
{
    FILE *fp;

    /* Open transaction file */
    fp = fopen("transactions.txt", "a");

    if (fp == NULL)
    {
        printf("Could not open transaction file.\n");
        return 0;
    }

    /* Write transaction data */
    fprintf(fp, "%d %d %.2lf %.2lf %d %s\n",
            account_id,
            type,
            amount,
            balance_after,
            related_id,
            note);

    fclose(fp);

    return 1;
}


/* Deposit money */

int deposit(Session *s)
{
    Account acc;
    double amount;
    double max_balance;

    /* Find account */
    if (find_account(s->account.id, &acc) == 0)
    {
        printf("Account not found.\n");
        return 0;
    }

    printf("\n=== DEPOSIT FUNDS ===\n");
    printf("Current Balance: %.2lf\n", acc.balance);

    /* Take deposit amount */
    printf("Enter deposit amount: ");
    scanf("%lf", &amount);

    /* Check amount */
    if (amount <= 0)
    {
        printf("Invalid amount.\n");
        return 0;
    }

    /* Set maximum balance */
    if (acc.tier == PREMIUM)
    {
        max_balance = PREMIUM_MAX_BALANCE;
    }
    else
    {
        max_balance = NORMAL_MAX_BALANCE;
    }

    /* Check balance limit */
    if (acc.balance + amount > max_balance)
    {
        printf("Maximum balance limit exceeded.\n");
        return 0;
    }

    /* Add money */
    acc.balance = acc.balance + amount;

    /* Update account balance */
    if (update_balance(acc.id, acc.balance) == 0)
    {
        printf("Balance update failed.\n");
        return 0;
    }

    /* Update session balance */
    s->account.balance = acc.balance;

    /* Save transaction */
    record_txn(acc.id,
               TXN_DEPOSIT,
               amount,
               0,
               acc.balance,
               "Deposit");

    printf("Deposit successful.\n");
    printf("New Balance: %.2lf\n", acc.balance);

    return 1;
}


/* Withdraw money */

int withdraw(Session *s)
{
    Account acc;
    double amount;
    double daily_limit;

    /* Find account */
    if (find_account(s->account.id, &acc) == 0)
    {
        printf("Account not found.\n");
        return 0;
    }

    printf("\n=== WITHDRAW FUNDS ===\n");
    printf("Current Balance: %.2lf\n", acc.balance);

    /* Take withdrawal amount */
    printf("Enter withdrawal amount: ");
    scanf("%lf", &amount);

    /* Check amount */
    if (amount <= 0)
    {
        printf("Invalid amount.\n");
        return 0;
    }

    /* Set daily limit */
    if (acc.tier == PREMIUM)
    {
        daily_limit = PREMIUM_DAILY_WITHDRAW;
    }
    else
    {
        daily_limit = NORMAL_DAILY_WITHDRAW;
    }

    /* Check daily limit */
    if (acc.withdrawn_today + amount > daily_limit)
    {
        printf("Daily withdrawal limit exceeded.\n");
        return 0;
    }

    /* Check balance */
    if (amount > acc.balance)
    {
        printf("Insufficient balance.\n");
        return 0;
    }

    /* Subtract money */
    acc.balance = acc.balance - amount;

    /* Update daily withdrawal */
    acc.withdrawn_today = acc.withdrawn_today + amount;

    /* Update account balance */
    if (update_balance(acc.id, acc.balance) == 0)
    {
        printf("Balance update failed.\n");
        return 0;
    }

    /* Update session balance */
    s->account.balance = acc.balance;

    /* Save transaction */
    record_txn(acc.id,
               TXN_WITHDRAW,
               amount,
               0,
               acc.balance,
               "Withdraw");

    printf("Withdrawal successful.\n");
    printf("New Balance: %.2lf\n", acc.balance);

    return 1;
}


/* Transfer money */

int transfer(Session *s)
{
    Account sender;
    Account receiver;

    int receiver_id;
    double amount;
    double transfer_limit;
    double max_receiver_balance;

    /* Find sender account */
    if (find_account(s->account.id, &sender) == 0)
    {
        printf("Sender account not found.\n");
        return 0;
    }

    printf("\n=== TRANSFER FUNDS ===\n");
    printf("Your Balance: %.2lf\n", sender.balance);

    /* Take receiver ID */
    printf("Enter receiver account ID: ");
    scanf("%d", &receiver_id);

    /* Check own account */
    if (receiver_id == sender.id)
    {
        printf("Cannot transfer money to yourself.\n");
        return 0;
    }

    /* Find receiver account */
    if (find_account(receiver_id, &receiver) == 0)
    {
        printf("Receiver account not found.\n");
        return 0;
    }

    /* Take transfer amount */
    printf("Enter transfer amount: ");
    scanf("%lf", &amount);

    /* Check amount */
    if (amount <= 0)
    {
        printf("Invalid amount.\n");
        return 0;
    }

    /* Set transfer limit */
    if (sender.tier == PREMIUM)
    {
        transfer_limit = PREMIUM_TRANSFER_LIMIT;
    }
    else
    {
        transfer_limit = NORMAL_TRANSFER_LIMIT;
    }

    /* Check transfer limit */
    if (amount > transfer_limit)
    {
        printf("Transfer limit exceeded.\n");
        return 0;
    }

    /* Check sender balance */
    if (amount > sender.balance)
    {
        printf("Insufficient balance.\n");
        return 0;
    }

    /* Set receiver balance limit */
    if (receiver.tier == PREMIUM)
    {
        max_receiver_balance = PREMIUM_MAX_BALANCE;
    }
    else
    {
        max_receiver_balance = NORMAL_MAX_BALANCE;
    }

    /* Check receiver balance */
    if (receiver.balance + amount > max_receiver_balance)
    {
        printf("Receiver balance limit exceeded.\n");
        return 0;
    }

    /* Update both balances */
    sender.balance = sender.balance - amount;
    receiver.balance = receiver.balance + amount;

    /* Update sender balance */
    if (update_balance(sender.id, sender.balance) == 0)
    {
        printf("Sender balance update failed.\n");
        return 0;
    }

    /* Update receiver balance */
    if (update_balance(receiver.id, receiver.balance) == 0)
    {
        printf("Receiver balance update failed.\n");
        return 0;
    }

    /* Update session balance */
    s->account.balance = sender.balance;

    /* Save sender transaction */
    record_txn(sender.id,
               TXN_TRANSFER_OUT,
               amount,
               receiver.id,
               sender.balance,
               "Transfer_sent");

    /* Save receiver transaction */
    record_txn(receiver.id,
               TXN_TRANSFER_IN,
               amount,
               sender.id,
               receiver.balance,
               "Transfer_received");

    printf("Transfer successful.\n");
    printf("Your New Balance: %.2lf\n", sender.balance);

    return 1;
}


/* Show transaction history */

int view_history(const Session *s)
{
    FILE *fp;

    int account_id;
    int type;
    int related_id;

    double amount;
    double balance;

    char note[50];

    int found = 0;

    /* Open transaction file */
    fp = fopen("transactions.txt", "r");

    if (fp == NULL)
    {
        printf("No transaction history found.\n");
        return 0;
    }

    printf("\n=== TRANSACTION HISTORY ===\n");

    /* Read transactions */
    while (fscanf(fp, "%d %d %lf %lf %d %s",
                  &account_id,
                  &type,
                  &amount,
                  &balance,
                  &related_id,
                  note) == 6)
    {
        /* Show current user's transactions */
        if (account_id == s->account.id)
        {
            printf("\nAmount: %.2lf\n", amount);
            printf("Balance After: %.2lf\n", balance);
            printf("Related Account: %d\n", related_id);
            printf("Note: %s\n", note);

            found = 1;
        }
    }

    fclose(fp);

    if (found == 0)
    {
        printf("No transactions found.\n");
    }

    return found;
}


/* Export account statement */

int export_statement(const Session *s)
{
    FILE *fp;
    FILE *out;

    int account_id;
    int type;
    int related_id;

    double amount;
    double balance;

    char note[50];
    char filename[64];

    int found = 0;

    /* Open transaction file */
    fp = fopen("transactions.txt", "r");

    if (fp == NULL)
    {
        printf("No transaction history found.\n");
        return 0;
    }

    /* Create statement file name */
    sprintf(filename, "statement_%d.txt", s->account.id);

    /* Create statement file */
    out = fopen(filename, "w");

    if (out == NULL)
    {
        fclose(fp);
        printf("Could not create statement file.\n");
        return 0;
    }

    /* Write statement heading */
    fprintf(out, "ACCOUNT STATEMENT\n");
    fprintf(out, "Account ID: %d\n", s->account.id);
    fprintf(out, "Name: %s\n\n", s->account.name);

    /* Read transactions */
    while (fscanf(fp, "%d %d %lf %lf %d %s",
                  &account_id,
                  &type,
                  &amount,
                  &balance,
                  &related_id,
                  note) == 6)
    {
        /* Write user's transactions */
        if (account_id == s->account.id)
        {
            fprintf(out, "Amount: %.2lf\n", amount);
            fprintf(out, "Balance After: %.2lf\n", balance);
            fprintf(out, "Related Account: %d\n", related_id);
            fprintf(out, "Note: %s\n", note);
            fprintf(out, "-------------------------\n");

            found = 1;
        }
    }

    fclose(fp);
    fclose(out);

    if (found == 0)
    {
        printf("No transactions found.\n");
        return 0;
    }

    printf("Statement exported successfully.\n");
    printf("Saved file: %s\n", filename);

    return 1;
}
