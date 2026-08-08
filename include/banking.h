#ifndef BANKING_H
#define BANKING_H
#include "types.h"

// deposit, withdraw, transfer, history
int record_txn(int account_id, TxnType type, double amount,
                int related_id, double balance_after, const char *note);
int deposit(Session *s);
int withdraw(Session *s);
int transfer(Session *s);
int view_history(const Session *s);
int export_statement(const Session *s);

// loans
double calc_monthly_payment(double principal, double annual_rate, int months);
int    apply_for_loan(Session *s);
int    make_loan_payment(Session *s);
int    view_my_loans(const Session *s);

// interest
double calc_interest(double balance, double annual_rate, int days);
int    apply_interest(Session *s);

#endif
