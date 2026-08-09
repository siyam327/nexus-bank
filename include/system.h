#ifndef SYSTEM_H
#define SYSTEM_H
#include "types.h"

// reading/writing the data files
int load_accounts(Account *arr, int *count);
int save_accounts(const Account *arr, int count);
int update_account(const Account *updated);
int load_transactions(Transaction *arr, int *count);
int append_transaction(const Transaction *txn);
int load_loans(Loan *arr, int *count);
int save_loans(const Loan *arr, int count);
int update_loan(const Loan *updated);
int report_file_error(const char *what_failed);

// all print/menu formatting
int print_centered(const char *text);
int print_logo();
int print_header(const char *title);
int print_separator();
int print_double_sep();
int print_account_card(const Account *acc);
int print_account_row(const Account *acc);
int print_accounts_table_header();
int print_transaction_row(const Transaction *txn);
int print_txn_table_header();
int print_receipt(const char *type, double amount, double balance_after);
int print_loan_row(const Loan *loan);
int print_loan_table_header();
int print_loan_summary(const Loan *loan);
int print_menu_item(int num, const char *label);
int print_admin_stats(int total_accounts, int active, double total_balance,
                       int total_loans, double loan_balance);

// admin panel
int admin_dashboard();
int admin_list_accounts();
int admin_view_account();
int admin_reset_pin();
int admin_toggle_account();
int admin_view_all_loans();

#endif

