#ifndef TYPES_H
#define TYPES_H
// config
#define BANK_NAME            "NEXUS BANK"
#define BANK_VERSION         "1.0.0"
#define ADMIN_PASSWORD       "6767"

#define MAX_ACCOUNTS         1000
#define MAX_TRANSACTIONS     10000
#define MAX_LOANS            500
#define STARTING_ACCOUNT_ID  10001

#define MAX_NAME_LEN         50
#define MAX_PIN_LEN          7 
#define MAX_DATE_LEN         25
#define MAX_NOTE_LEN         100
#define MAX_PASS_LEN         32

#define SAVINGS_RATE         0.035
#define CHECKING_RATE        0.010
#define DEFAULT_LOAN_RATE    0.08

#define MIN_LOAN_AMOUNT      500.0
#define MAX_LOAN_AMOUNT      100000.0
#define MIN_LOAN_MONTHS      3
#define MAX_LOAN_MONTHS      60

#define DATA_DIR             "data"
#define ACCOUNTS_FILE        "data/accounts.txt"
#define TRANSACTIONS_FILE    "data/transactions.txt"
#define LOANS_FILE           "data/loans.txt"

// colors, just ansi escape codes
#define RESET    "\x1b[0m"
#define BOLD     "\x1b[1m"
#define DIM      "\x1b[2m"
#define RED      "\x1b[31m"
#define GREEN    "\x1b[32m"
#define YELLOW   "\x1b[33m"
#define BLUE     "\x1b[34m"
#define MAGENTA  "\x1b[35m"
#define CYAN     "\x1b[36m"
#define WHITE    "\x1b[37m"

#define print_success(msg) printf(GREEN  BOLD "  [OK] " RESET GREEN  msg RESET "\n")
#define print_error(msg)   printf(RED    BOLD "  [!] "  RESET RED    msg RESET "\n")
#define print_warn(msg)    printf(YELLOW BOLD "  [~] "  RESET YELLOW msg RESET "\n")
#define print_info(msg)    printf(CYAN   BOLD "  [i] "  RESET CYAN   msg RESET "\n")

// locked structs, ask in group chat before changing anything here

typedef enum { SAVINGS = 0, CHECKING = 1 } AccountType;

typedef enum {
    TXN_DEPOSIT = 0, TXN_WITHDRAW = 1,
    TXN_TRANSFER_OUT = 2, TXN_TRANSFER_IN = 3,
    TXN_INTEREST = 4, TXN_LOAN_IN = 5, TXN_LOAN_PAY = 6
} TxnType;

typedef enum { LOAN_ACTIVE = 0, LOAN_PAID = 1, LOAN_DEFAULTED = 2 } LoanStatus;

typedef struct {
    int         id;
    char        name[MAX_NAME_LEN];
    char        pin[MAX_PIN_LEN];
    AccountType type;
    double      balance;
    double      interest_rate;
    int         is_active;
    int         failed_attempts;
    char        created_at[MAX_DATE_LEN];
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
