/*
 display.c (all print/menu formatting)
 Owner: Siyam
 */
#include <stdio.h>
#include <string.h>
#include "../include/types.h"
#include "../include/system.h"
#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

// centers against the terminal's actual current width so the menu
// looks right no matter how wide/narrow the window is. falls back
// to 80 columns if the width can't be detected (e.g. piped output)
static int get_screen_width() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return 80;
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0)
        return w.ws_col;
    return 80;
#endif
}

static const char *account_type_str(AccountType t) { return t == SAVINGS ? "Savings" : "Checking"; }

static const char *txn_type_str(TxnType t) {
    switch (t) {
        case TXN_DEPOSIT:      return "Deposit";
        case TXN_WITHDRAW:     return "Withdrawal";
        case TXN_TRANSFER_OUT: return "Transfer Out";
        case TXN_TRANSFER_IN:  return "Transfer In";
        case TXN_INTEREST:     return "Interest";
        case TXN_LOAN_IN:      return "Loan Credit";
        case TXN_LOAN_PAY:     return "Loan Payment";
        default:               return "Unknown";
    }
}

static const char *loan_status_str(LoanStatus s) {
    switch (s) {
        case LOAN_ACTIVE:    return "Active";
        case LOAN_PAID:      return "Paid Off";
        case LOAN_DEFAULTED: return "Defaulted";
        default:             return "Unknown";
    }
}

int print_centered(const char *text) {
    int len = strlen(text);
    int pad = (get_screen_width() - len) / 2;
    if (pad < 0) pad = 0;
    printf("%*s%s\n", pad, "", text);
    return 0;
}

int print_logo() {
    char version_line[40];
    printf("\n");
    print_centered("------------------------------------");
    print_centered("N E X U S   B A N K");
    snprintf(version_line, sizeof(version_line), "version %s", BANK_VERSION);
    print_centered(version_line);
    print_centered("------------------------------------");
    printf("\n");
    return 0;
}

int print_header(const char *title) {
    char line[80];
    snprintf(line, sizeof(line), "== %s ==", title);
    printf("\n");
    print_centered(line);
    printf("\n");
    return 0;
}

int print_separator() {
    print_centered("------------------------------------------------------------------------");
    return 0;
}

int print_double_sep() {
    print_centered("============================================================================");
    return 0;
}

int print_account_card(const Account *acc) {
    print_double_sep();
    printf("  Account #%d\n", acc->id);
    printf("  Holder       : %s\n", acc->name);
    printf("  Type         : %s\n", account_type_str(acc->type));
    printf("  Tier         : %s\n", acc->tier == PREMIUM ? "Premium" : "Normal");
    printf("  Balance      : $%.2f\n", acc->balance);
    printf("  Interest Rate: %.1f%% per year\n", acc->interest_rate * 100.0);
    printf("  Status       : %s\n", acc->is_active ? "Active" : "Inactive");
    printf("  Opened On    : %s\n", acc->created_at);
    print_double_sep();
    return 0;
}

int print_accounts_table_header() {
    print_separator();
    printf("  %-7s %-20s %-10s %-14s %-8s %-8s\n",
           "ID", "Name", "Type", "Balance", "Rate", "Status");
    print_separator();
    return 0;
}

int print_account_row(const Account *acc) {
    printf("  %-7d %-20s %-10s $%-13.2f %-7.1f%% %s\n",
           acc->id, acc->name, account_type_str(acc->type),
           acc->balance, acc->interest_rate * 100.0,
           acc->is_active ? "Active" : "Inactive");
    return 0;
}

int print_txn_table_header() {
    print_separator();
    printf("  %-5s %-20s %-14s %-14s %-22s\n",
           "ID", "Type", "Amount", "Balance After", "Date");
    print_separator();
    return 0;
}

int print_transaction_row(const Transaction *txn) {
    printf("  %-5d %-20s $%-13.2f $%-13.2f %-22s\n",
           txn->id, txn_type_str(txn->type),
           txn->amount, txn->balance_after, txn->date);
    return 0;
}

int print_receipt(const char *type, double amount, double balance_after) {
    printf("\n");
    print_double_sep();
    print_centered("RECEIPT");
    print_separator();
    printf("  Transaction  : %s\n", type);
    printf("  Amount       : $%.2f\n", amount);
    printf("  New Balance  : $%.2f\n", balance_after);
    printf("  Bank         : %s\n", BANK_NAME);
    print_double_sep();
    printf("\n");
    return 0;
}

int print_loan_table_header() {
    print_separator();
    printf("  %-5s %-12s %-12s %-8s %-14s %-10s\n",
           "ID", "Principal", "Remaining", "Months", "Monthly Pmt", "Status");
    print_separator();
    return 0;
}

int print_loan_row(const Loan *loan) {
    printf("  %-5d $%-11.2f $%-11.2f %-8d $%-13.2f %s\n",
           loan->id, loan->principal, loan->remaining_balance,
           loan->term_months - loan->months_paid,
           loan->monthly_payment, loan_status_str(loan->status));
    return 0;
}

int print_loan_summary(const Loan *loan) {
    print_double_sep();
    printf("  Loan #%d\n", loan->id);
    printf("  Principal      : $%.2f\n", loan->principal);
    printf("  Interest Rate  : %.1f%% per year\n", loan->interest_rate * 100.0);
    printf("  Term           : %d months\n", loan->term_months);
    printf("  Monthly Payment: $%.2f\n", loan->monthly_payment);
    printf("  Months Paid    : %d / %d\n", loan->months_paid, loan->term_months);
    printf("  Remaining      : $%.2f\n", loan->remaining_balance);
    printf("  Status         : %s\n", loan_status_str(loan->status));
    print_double_sep();
    return 0;
}

int print_menu_item(int num, const char *label) {
    char line[80];
    snprintf(line, sizeof(line), "[%d] %s", num, label);
    print_centered(line);
    return 0;
}

int print_admin_stats(int total_accounts, int active, double total_balance,
                       int total_loans, double loan_balance) {
    print_header("SYSTEM OVERVIEW");
    printf("  Total Accounts  : %d  (%d active, %d inactive)\n",
           total_accounts, active, total_accounts - active);
    printf("  Total Deposits  : $%.2f\n", total_balance);
    printf("  Total Loans     : %d\n", total_loans);
    printf("  Loan Exposure   : $%.2f\n", loan_balance);
    printf("\n");
    return 0;
}
