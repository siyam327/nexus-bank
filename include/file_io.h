#ifndef FILE_IO_H
#define FILE_IO_H
#include "types.h"

int load_accounts(Account *arr, int *count);
int save_accounts(const Account *arr, int count);
int update_account(const Account *updated);
int load_transactions(Transaction *arr, int *count);
int append_transaction(const Transaction *txn);
int load_loans(Loan *arr, int *count);
int save_loans(const Loan *arr, int count);
int update_loan(const Loan *updated);

/* prints a proper system error message using errno
 * Always returns 0 (failure) so callers can write "return report_file_error(...);" */
int report_file_error(const char *what_failed);

#endif
