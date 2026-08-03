#include <stdio.h>
#include <string.h>
#include "../include/types.h"
#include "../include/admin.h"
#include "../include/account.h"
#include "../include/utils.h"
#include "../include/file_io.h"
#include "../include/display.h"

int admin_dashboard() {
    Account accounts[MAX_ACCOUNTS];
    Loan loans[MAX_LOANS];
    int acc_count = 0, loan_count = 0, active = 0, active_loans = 0, i;
    double total_balance = 0.0, loan_balance = 0.0;
    load_accounts(accounts, &acc_count);
    load_loans(loans, &loan_count);
    for (i = 0; i < acc_count; i++) if (accounts[i].is_active) { active++; total_balance += accounts[i].balance; }
    for (i = 0; i < loan_count; i++) if (loans[i].status == LOAN_ACTIVE) { active_loans++; loan_balance += loans[i].remaining_balance; }
    print_admin_stats(acc_count, active, total_balance, active_loans, loan_balance);
    return 0;
}

int admin_list_accounts() { //syntax fixed
    Account arr[MAX_ACCOUNTS];
    int count = 0, i;
    print_header("ALL ACCOUNTS");
    load_accounts(arr, &count);
    if (count == 0) { printf(DIM "  No accounts yet.\n" RESET); press_enter(); return 1; }
    print_accounts_table_header();
    for (i = 0; i < count; i++) print_account_row(&arr[i]);
    print_separator();
    printf(DIM "  Total: %d account(s)\n" RESET, count);
    press_enter(); //added this
    return 0;
}

int admin_view_account() {
    int id;
    Account acc;
    Transaction txns[MAX_TRANSACTIONS];
    Loan loans[MAX_LOANS];
    int txn_count = 0, loan_count = 0, i, found;

    print_header("VIEW ACCOUNT DETAILS");
    id = get_int("  Enter account ID: ", STARTING_ACCOUNT_ID, 99999);
    if (!find_account(id, &acc)) { print_error("Account not found."); press_enter(); return 1; }
    print_account_card(&acc);

    load_transactions(txns, &txn_count);
    printf("\n  Recent Transactions:\n");
    print_txn_table_header();
    found = 0;
    for (i = txn_count - 1; i >= 0 && found < 10; i--)
        if (txns[i].account_id == id) { print_transaction_row(&txns[i]); found++; }
    if (found == 0) printf(DIM "  No transactions.\n" RESET);

    load_loans(loans, &loan_count);
    printf("\n  Loans:\n");
    print_loan_table_header();
    found = 0;
    for (i = 0; i < loan_count; i++)
        if (loans[i].account_id == id) { print_loan_row(&loans[i]); found++; }
    if (found == 0) printf(DIM "  No loans.\n" RESET);
    press_enter();
    return 0;
}
