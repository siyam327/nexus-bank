/*
 admin.c (admin panel)
 Owner: Ariful Islam Siyam
 Id:2026-2-60-019
*/
#include <stdio.h>
#include <string.h>
#include "../include/types.h"
#include "../include/account.h"
#include "../include/system.h"

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

int admin_list_accounts() {
    Account arr[MAX_ACCOUNTS];
    int count = 0, i;
    print_header("ALL ACCOUNTS");
    load_accounts(arr, &count);
    if (count == 0) { printf( " No accounts yet.\n" ); press_enter(); return 1; }
    print_accounts_table_header();
    for (i = 0; i < count; i++) print_account_row(&arr[i]);
    print_separator();
    printf( " Total: %d account(s)\n", count);
    press_enter();
    return 0;
}

int admin_view_account() {
    int id;
    Account acc;
    Transaction txns[MAX_TRANSACTIONS];
    Loan loans[MAX_LOANS];
    int txn_count = 0, loan_count = 0, i, found;

    print_header("VIEW ACCOUNT DETAILS");
    id = get_int(" Enter account ID: ", STARTING_ACCOUNT_ID, 99999);
    if (!find_account(id, &acc)) { print_error("Account not found."); press_enter(); return 1; }
    print_account_card(&acc);

    load_transactions(txns, &txn_count);
    printf("\n Recent Transactions:\n");
    print_txn_table_header();
    found = 0;
    for (i = txn_count - 1; i >= 0 && found < 10; i--)
        if (txns[i].account_id == id) { print_transaction_row(&txns[i]); found++; }
    if (found == 0) printf( " No transactions.\n" );

    load_loans(loans, &loan_count);
    printf("\n Loans:\n");
    print_loan_table_header();
    found = 0;
    for (i = 0; i < loan_count; i++)
        if (loans[i].account_id == id) { print_loan_row(&loans[i]); found++; }
    if (found == 0) printf( " No loans.\n" );
    print_separator();
    press_enter();
    return 0;
}

int admin_reset_pin() {
    int id;
    Account acc;
    char confirm[8];
    print_header(" ACCOUNT PIN");
    id = get_int(" Enter account ID: ", STARTING_ACCOUNT_ID, 99999);
    if (!find_account(id, &acc)) { print_error("Account not found."); press_enter(); return 1; }
    printf(" Account holder : " "%s\n\n", acc.name);
    print_warn("This will reset their PIN to 000000.");
    get_string(" Type YES to confirm: ", confirm, sizeof(confirm));
    if (strcmp(confirm, "YES") != 0) { print_info("Cancelled."); press_enter(); return 1; }
    strncpy(acc.pin, "000000", MAX_PIN_LEN);
    acc.failed_attempts = 0;
    acc.is_active = 1;
    if (!update_account(&acc)) {
        print_error("Could not save the PIN reset -- please try again.");
        press_enter();
        return 1;
    }
    print_success("PIN reset to 000000. Account unlocked. Notify the account holder.");
    press_enter();
    return 0;
}

int admin_toggle_account() {
    int id;
    Account acc;
    char confirm[8];
    print_header("TOGGLE ACCOUNT STATUS");
    id = get_int(" Enter account ID: ", STARTING_ACCOUNT_ID, 99999);
    if (!find_account(id, &acc)) { print_error("Account not found."); press_enter(); return 1; }
    printf(" Account holder : " "%s\n", acc.name);
    printf(" Current status : %s\n\n", acc.is_active ? "Active" : "Inactive" );
    printf(" Type YES to %s: ", acc.is_active ? "deactivate" : "activate");
    fflush(stdout);
    if (!fgets(confirm, sizeof(confirm), stdin)) { press_enter(); return 1; }
    confirm[strcspn(confirm, "\n")] = '\0';
    if (strcmp(confirm, "YES") != 0) { print_info("Cancelled."); press_enter(); return 1; }
    acc.is_active = !acc.is_active;
    if (acc.is_active) acc.failed_attempts = 0;
    if (!update_account(&acc)) {
        print_error("Could not save the status change -- please try again.");
        press_enter();
        return 1;
    }
    printf( " Account %s successfully.\n", acc.is_active ? "activated" : "deactivated");
    press_enter();
    return 0;
}

int admin_view_all_loans() {
    Loan arr[MAX_LOANS];
    int count = 0, i;
    print_header("ALL LOANS");
    load_loans(arr, &count);
    if (count == 0) { printf( " No loans in the system.\n" ); press_enter(); return 1; }
    print_loan_table_header();
    for (i = 0; i < count; i++) print_loan_row(&arr[i]);
    print_separator();
    printf( " Total: %d loan(s)\n", count);
    press_enter();
    return 0;
}

