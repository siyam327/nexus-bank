/*
  file_io.c (reading/writing pipe-delimited data files)
  Owner: Siyam
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../include/types.h"
#include "../include/system.h"

int report_file_error(const char *what_failed) {
    fprintf(stderr, "[FILE ERROR] %s: %s (errno %d)\n", what_failed, strerror(errno), errno);
    return 0;
}

int load_accounts(Account *arr, int *count) {
    FILE *fp;
    char line[512];
    *count = 0;

    errno = 0;
    fp = fopen(ACCOUNTS_FILE, "r");
    if (!fp) {
        if (errno == ENOENT) return 1;
        return report_file_error("opening accounts file");
    }

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        if (*count >= MAX_ACCOUNTS) break;

        Account *a = &arr[*count];
        int type, tier, active, failed = 0;
        double withdrawn = 0.0;
        char last_withdraw[MAX_DATE_LEN] = "";

        int n = sscanf(line, "%d|%49[^|]|%6[^|]|%d|%d|%lf|%lf|%d|%d|%lf|%24[^|]|%24[^\n]",
                       &a->id, a->name, a->pin, &type, &tier,
                       &a->balance, &a->interest_rate, &active, &failed,
                       &withdrawn, last_withdraw, a->created_at);

        if (n < 9) continue;

        a->type = (AccountType)type;
        a->tier = (n >= 12) ? (CustomerType)tier : NORMAL;
        a->is_active = active;
        a->failed_attempts = failed;
        a->withdrawn_today = (n >= 12) ? withdrawn : 0.0;

        const char *src_date = (n >= 12) ? last_withdraw : a->created_at;
        strncpy(a->last_withdraw_date, src_date, MAX_DATE_LEN - 1);
        a->last_withdraw_date[MAX_DATE_LEN - 1] = '\0';

        (*count)++;
    }

    fclose(fp);
    return 1;
}

int save_accounts(const Account *arr, int count) {
    errno = 0;
    FILE *fp = fopen(ACCOUNTS_FILE, "w");
    if (!fp) return report_file_error("writing accounts file");

    fprintf(fp, "# id|name|pin|type|tier|balance|interest_rate|is_active|failed_attempts|withdrawn_today|last_withdraw_date|created_at\n");
    for (int i = 0; i < count; i++) {
        const Account *a = &arr[i];
        fprintf(fp, "%d|%s|%s|%d|%d|%.2f|%.4f|%d|%d|%.2f|%s|%s\n",
                a->id, a->name, a->pin, (int)a->type, (int)a->tier,
                a->balance, a->interest_rate, a->is_active,
                a->failed_attempts, a->withdrawn_today,
                a->last_withdraw_date, a->created_at);
    }

    fclose(fp);
    return 1;
}

int update_account(const Account *updated) {
    Account arr[MAX_ACCOUNTS];
    int count = 0;

    if (!load_accounts(arr, &count)) return 0;

    for (int i = 0; i < count; i++) {
        if (arr[i].id == updated->id) {
            arr[i] = *updated;
            return save_accounts(arr, count);
        }
    }
    return 0;
}

int load_transactions(Transaction *arr, int *count) {
    FILE *fp;
    char line[512];
    *count = 0;

    errno = 0;
    fp = fopen(TRANSACTIONS_FILE, "r");
    if (!fp) {
        if (errno == ENOENT) return 1;
        return report_file_error("opening transactions file");
    }

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        if (*count >= MAX_TRANSACTIONS) break;

        Transaction *t = &arr[*count];
        int type;
        char note[MAX_NOTE_LEN] = "";

        int n = sscanf(line, "%d|%d|%d|%lf|%d|%lf|%24[^|]|%99[^\n]",
                       &t->id, &t->account_id, &type, &t->amount,
                       &t->related_id, &t->balance_after, t->date, note);

        if (n < 7) continue;

        t->type = (TxnType)type;
        strncpy(t->note, note, MAX_NOTE_LEN - 1);
        t->note[MAX_NOTE_LEN - 1] = '\0';

        (*count)++;
    }

    fclose(fp);
    return 1;
}

int append_transaction(const Transaction *txn) {
    errno = 0;
    FILE *fp = fopen(TRANSACTIONS_FILE, "r");

    if (!fp) {
        if (errno != ENOENT) return report_file_error("checking transactions file");

        errno = 0;
        fp = fopen(TRANSACTIONS_FILE, "w");
        if (!fp) return report_file_error("creating transactions file");

        fprintf(fp, "# id|account_id|type|amount|related_id|balance_after|date|note\n");
        fclose(fp);
    } else {
        fclose(fp);
    }

    errno = 0;
    fp = fopen(TRANSACTIONS_FILE, "a");
    if (!fp) return report_file_error("appending to transactions file");

    fprintf(fp, "%d|%d|%d|%.2f|%d|%.2f|%s|%s\n",
            txn->id, txn->account_id, (int)txn->type, txn->amount,
            txn->related_id, txn->balance_after, txn->date, txn->note);

    fclose(fp);
    return 1;
}

int load_loans(Loan *arr, int *count) {
    FILE *fp;
    char line[512];
    *count = 0;

    errno = 0;
    fp = fopen(LOANS_FILE, "r");
    if (!fp) {
        if (errno == ENOENT) return 1;
        return report_file_error("opening loans file");
    }

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        if (*count >= MAX_LOANS) break;

        Loan *l = &arr[*count];
        int status;

        int n = sscanf(line, "%d|%d|%lf|%lf|%d|%lf|%lf|%d|%d|%24[^\n]",
                       &l->id, &l->account_id, &l->principal,
                       &l->interest_rate, &l->term_months,
                       &l->monthly_payment, &l->remaining_balance,
                       &l->months_paid, &status, l->start_date);

        if (n < 9) continue;

        l->status = (LoanStatus)status;
        (*count)++;
    }

    fclose(fp);
    return 1;
}

int save_loans(const Loan *arr, int count) {
    errno = 0;
    FILE *fp = fopen(LOANS_FILE, "w");
    if (!fp) return report_file_error("writing loans file");

    fprintf(fp, "# id|account_id|principal|interest_rate|term_months|monthly_payment|remaining_balance|months_paid|status|start_date\n");
    for (int i = 0; i < count; i++) {
        const Loan *l = &arr[i];
        fprintf(fp, "%d|%d|%.2f|%.4f|%d|%.2f|%.2f|%d|%d|%s\n",
                l->id, l->account_id, l->principal, l->interest_rate,
                l->term_months, l->monthly_payment, l->remaining_balance,
                l->months_paid, (int)l->status, l->start_date);
    }

    fclose(fp);
    return 1;
}

int update_loan(const Loan *updated) {
    Loan arr[MAX_LOANS];
    int count = 0;

    if (!load_loans(arr, &count)) return 0;

    for (int i = 0; i < count; i++) {
        if (arr[i].id == updated->id) {
            arr[i] = *updated;
            return save_loans(arr, count);
        }
    }
    return 0;
}
