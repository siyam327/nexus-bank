/*
transaction.c (transactions)
Owner: Bushra
Contains: deposit, withdraw, transfer, history, statement export.
*/
#include <stdio.h>
#include <string.h>
#include "../include/types.h"
#include "../include/banking.h"
#include "../include/account.h"
#include "../include/system.h"

int record_txn(int account_id, TxnType type, double amount,
                int related_id, double balance_after, const char *note) {
    Transaction t;
    memset(&t, 0, sizeof(t));
    t.id = generate_txn_id();
    t.account_id = account_id;
    t.type = type;
    t.amount = amount;
    t.related_id = related_id;
    t.balance_after = balance_after;
    get_current_datetime(t.date);
    strncpy(t.note, note ? note : "", MAX_NOTE_LEN - 1);
    return append_transaction(&t);
}

int deposit(Session *s) {
    double amount;
    Account acc;
    print_header("DEPOSIT");
    if (!find_account(s->account.id, &acc)) { print_error("Could not load account."); press_enter(); return 1; }
    printf("  Current balance: $%.2f\n\n", acc.balance);
    amount = get_double("  Amount to deposit ($): ", 0.01, MAX_LOAN_AMOUNT);
    if (acc.balance + amount > max_balance_for(acc.tier)) {
        print_error("This would go over your account tier's max balance.");
        press_enter();
        return 1;
    }
    acc.balance += amount;
    if (!update_account(&acc)) { print_error("Could not save deposit -- balance not updated."); press_enter(); return 1; }
    s->account.balance = acc.balance;
    if (!record_txn(acc.id, TXN_DEPOSIT, amount, 0, acc.balance, "Cash deposit")) {
        print_warn("Deposit saved, but the transaction history record may not have saved.");
    }
    print_receipt("Deposit", amount, acc.balance);
    press_enter();
    return 0;
}

int withdraw(Session *s) {
    double amount;
    Account acc;
    print_header("WITHDRAW");
    if (!find_account(s->account.id, &acc)) { print_error("Could not load account."); press_enter(); return 1; }
    printf("  Current balance: $%.2f\n\n", acc.balance);
    amount = get_double("  Amount to withdraw ($): ", 0.01, acc.balance > 0 ? acc.balance : 0.01);
    if (amount > acc.balance) { print_error("Insufficient funds."); press_enter(); return 1; }

    // reset the daily counter if this is a new day (just compare the date part, "YYYY-MM-DD")
    char today[MAX_DATE_LEN];
    get_current_datetime(today);
    if (strncmp(today, acc.last_withdraw_date, 10) != 0) {
        acc.withdrawn_today = 0.0;
    }
    if (acc.withdrawn_today + amount > daily_withdraw_limit_for(acc.tier)) {
        print_error("This would go over your daily withdrawal limit for your tier.");
        press_enter();
        return 1;
    }

    acc.balance -= amount;
    acc.withdrawn_today += amount;
    strncpy(acc.last_withdraw_date, today, MAX_DATE_LEN - 1);
    if (!update_account(&acc)) { print_error("Could not save withdrawal -- balance not updated."); press_enter(); return 1; }
    s->account.balance = acc.balance;
    if (!record_txn(acc.id, TXN_WITHDRAW, amount, 0, acc.balance, "Cash withdrawal")) {
        print_warn("Withdrawal saved, but the transaction history record may not have saved.");
    }
    print_receipt("Withdrawal", amount, acc.balance);
    press_enter();
    return 0;
}

// the tricky one: check everything first, THEN move money, so a failed
// transfer never leaves money floating in neither account
int transfer(Session *s) {
    int dest_id;
    double amount;
    Account from, to;
    char note_from[MAX_NOTE_LEN], note_to[MAX_NOTE_LEN];
    char buf[32];

    print_header("TRANSFER FUNDS");
    if (!find_account(s->account.id, &from)) { print_error("Could not load your account."); press_enter(); return 1; }
    printf("  Your balance: $%.2f\n\n", from.balance);
    printf("  Destination account ID: ");
    fflush(stdout);
    if (!fgets(buf, sizeof(buf), stdin) || sscanf(buf, "%d", &dest_id) != 1) { print_error("Invalid account ID."); press_enter(); return 1; }
    if (dest_id == from.id) { print_error("You can't transfer to your own account."); press_enter(); return 1; }
    if (!find_account(dest_id, &to)) { print_error("Destination account not found."); press_enter(); return 1; }
    if (!to.is_active) { print_error("Destination account is inactive."); press_enter(); return 1; }
    printf("  Sending to   : %s (#%d)\n\n", to.name, to.id);
    amount = get_double("  Amount to transfer ($): ", 0.01, from.balance > 0 ? from.balance : 0.01);
    if (amount > from.balance) { print_error("Insufficient funds."); press_enter(); return 1; }
    if (amount > transfer_limit_for(from.tier)) {
        print_error("This is over your account tier's per-transfer limit.");
        press_enter();
        return 1;
    }
    if (to.balance + amount > max_balance_for(to.tier)) {
        print_error("This would put the destination account over its tier's max balance.");
        press_enter();
        return 1;
    }

    // everything above was checked before we touch any balance, so by this
    // point the transfer should go through, but we still check each save
    from.balance -= amount;
    to.balance   += amount;

    if (!update_account(&from)) {
        print_error("Could not save transfer -- your balance was not touched.");
        press_enter();
        return 1;
    }
    if (!update_account(&to)) {
        print_error("Could not save transfer to the destination account. Please contact an admin.");
        press_enter();
        return 1;
    }
    s->account.balance = from.balance;

    snprintf(note_from, sizeof(note_from), "Transfer to #%d (%s)", to.id, to.name);
    snprintf(note_to,   sizeof(note_to),   "Transfer from #%d (%s)", from.id, from.name);
    if (!record_txn(from.id, TXN_TRANSFER_OUT, amount, to.id,   from.balance, note_from) ||
        !record_txn(to.id,   TXN_TRANSFER_IN,  amount, from.id, to.balance,   note_to)) {
        print_warn("Money was transferred, but the transaction history record may not have saved.");
    }

    print_receipt("Transfer Out", amount, from.balance);
    printf("  Sent to: %s (#%d)\n\n", to.name, to.id);
    press_enter();
    return 0;
}

int view_history(const Session *s) {
    Transaction arr[MAX_TRANSACTIONS];
    int count = 0, found = 0, i;
    print_header("TRANSACTION HISTORY");
    load_transactions(arr, &count);
    print_txn_table_header();
    for (i = count - 1; i >= 0; i--) {
        if (arr[i].account_id == s->account.id) { print_transaction_row(&arr[i]); found++; }
        if (found >= 30) break;
    }
    if (found == 0) printf("  No transactions yet.\n");
    else { print_separator(); printf("  Showing last %d transaction(s).\n", found); }
    press_enter();
    return 0;
}

int export_statement(const Session *s) {
    Transaction arr[MAX_TRANSACTIONS];
    int count = 0, found = 0, i;
    char filename[64];
    FILE *fp;

    print_header("EXPORT ACCOUNT STATEMENT");
    snprintf(filename, sizeof(filename), "statement_%d.txt", s->account.id);
    fp = fopen(filename, "w");
    if (!fp) { print_error("Could not create statement file."); press_enter(); return 1; }

    fprintf(fp, "NEXUS BANK - Account Statement\n");
    fprintf(fp, "Account ID  : %d\n", s->account.id);
    fprintf(fp, "Holder Name : %s\n", s->account.name);
    fprintf(fp, "Balance     : $%.2f\n\n", s->account.balance);
    fprintf(fp, "%-5s  %-20s  %-14s  %-14s  %-22s\n", "ID", "Type", "Amount", "Balance After", "Date");

    load_transactions(arr, &count);
    for (i = 0; i < count; i++) {
        if (arr[i].account_id != s->account.id) continue;
        const char *type_str;
        switch (arr[i].type) {
            case TXN_DEPOSIT:      type_str = "Deposit";      break;
            case TXN_WITHDRAW:     type_str = "Withdrawal";   break;
            case TXN_TRANSFER_OUT: type_str = "Transfer Out"; break;
            case TXN_TRANSFER_IN:  type_str = "Transfer In";  break;
            case TXN_INTEREST:     type_str = "Interest";     break;
            case TXN_LOAN_IN:      type_str = "Loan Credit";  break;
            case TXN_LOAN_PAY:     type_str = "Loan Payment"; break;
            default:               type_str = "Unknown";      break;
        }
        fprintf(fp, "%-5d  %-20s  $%-13.2f  $%-13.2f  %s\n",
                arr[i].id, type_str, arr[i].amount, arr[i].balance_after, arr[i].date);
        found++;
    }
    fprintf(fp, "\nTotal transactions: %d\n", found);
    fclose(fp);

    print_success("Statement exported successfully!");
    printf("  File saved as : %s\n", filename);
    press_enter();
    return 0;
}
