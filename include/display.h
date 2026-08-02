#ifndef DISPLAY_H
#define DISPLAY_H
#include "types.h"
#include <stdarg.h>

int get_term_width();
int print_centered(const char *text);
int print_centered_line(const char *fmt, ...);

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

#endif
