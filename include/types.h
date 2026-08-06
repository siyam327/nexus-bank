#ifndef TYPES_H
#define TYPES_H

// basic config
#define BANK_NAME            "NEXUS BANK"
#define BANK_VERSION         "1.0.0"
#define ADMIN_PASSWORD       "6767"

#define MAX_ACCOUNTS         1000
#define MAX_TRANSACTIONS     10000
#define MAX_LOANS            500
#define STARTING_ACCOUNT_ID  10001

#define MAX_NAME_LEN         50
#define MAX_PIN_LEN          7      // 6 digits + null terminator, don't go over this
#define MAX_DATE_LEN         25
#define MAX_NOTE_LEN         100
#define MAX_PASS_LEN         32

#define SAVINGS_RATE         0.035
#define CHECKING_RATE        0.010
#define DEFAULT_LOAN_RATE    0.08

// customer tiers: separate from account type (savings/checking)
// account type affects interest rate, customer tier affects spending limits
#define NORMAL_MAX_BALANCE        100000.0
#define NORMAL_DAILY_WITHDRAW     20000.0
#define NORMAL_TRANSFER_LIMIT     10000.0

#define PREMIUM_MAX_BALANCE       1000000.0
#define PREMIUM_DAILY_WITHDRAW    100000.0
#define PREMIUM_TRANSFER_LIMIT    100000.0

#define MIN_LOAN_AMOUNT      500.0
#define MAX_LOAN_AMOUNT      100000.0
#define MIN_LOAN_MONTHS      3
#define MAX_LOAN_MONTHS      60

#define DATA_DIR             "data"
#define ACCOUNTS_FILE        "data/accounts.txt"
#define TRANSACTIONS_FILE    "data/transactions.txt"
#define LOANS_FILE           "data/loans.txt"

#define print_success(msg) printf("  [OK] " msg "\n")
#define print_error(msg)   printf("  [!] " msg "\n")
#define print_warn(msg)    printf("  [~] " msg "\n")
#define print_info(msg)    printf("  [i] " msg "\n")

// locked structs, ask in group chat before changing anything here

typedef enum { SAVINGS = 0, CHECKING = 1 } AccountType;
typedef enum { NORMAL = 0, PREMIUM = 1 } CustomerType;

typedef enum {
    TXN_DEPOSIT = 0, TXN_WITHDRAW = 1,
    TXN_TRANSFER_OUT = 2, TXN_TRANSFER_IN = 3,
    TXN_INTEREST = 4, TXN_LOAN_IN = 5, TXN_LOAN_PAY = 6
} TxnType;

typedef enum { LOAN_ACTIVE = 0, LOAN_PAID = 1, LOAN_DEFAULTED = 2 } LoanStatus;

typedef struct {
    int          id;
    char         name[MAX_NAME_LEN];
    char         pin[MAX_PIN_LEN];
    AccountType  type;
    CustomerType tier;
    double       balance;
    double       interest_rate;
    int          is_active;
    int          failed_attempts;
    double       withdrawn_today;
    char         last_withdraw_date[MAX_DATE_LEN];
    char         created_at[MAX_DATE_LEN];
} Account;

typedef struct {
    int     id;
    int     account_id;
    TxnType type;
    double  amount;
    int     related_id;
    double  balance_after;
    char    date[MAX_DATE_LEN];
    char    note[MAX_NOTE_LEN];
} Transaction;

typedef struct {
    int        id;
    int        account_id;
    double     principal;
    double     interest_rate;
    int        term_months;
    double     monthly_payment;
    double     remaining_balance;
    int        months_paid;
    LoanStatus status;
    char       start_date[MAX_DATE_LEN];
} Loan;

typedef struct {
    int     logged_in;
    int     is_admin;
    Account account;
} Session;

#endif
