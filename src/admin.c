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

nt admin_list_accounts() {
    Account arr[MAX_ACCOUNTS];
    int count = 0, i;
    print_header("ALL ACCOUNTS");
    load_accounts(arr, &count);
    if (count == 0) { printf(DIM "  No accounts yet.\n" RESET); press_enter(); return 1; }
    print_accounts_table_header();
    for (i = 0; i < count; i++) print_account_row(&arr[i]);
    print_separator();
    printf(DIM "  Total: %d account(s)\n" RESET, count);
    return 0;
}
