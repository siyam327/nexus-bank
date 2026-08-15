/*
 loan.c (loans + savings interest)
 Owner: Hasan Ali
 ID:2026-2-60-026
*/
#include <string.h>
#include <math.h>
#include "../include/types.h"
#include "../include/account.h"
#include "../include/banking.h"
#include "../include/system.h"

// loan section

/* internal math helpers, broken into small steps on purpose so
   each part of the amortization can be tested/read on its own */

// standard loan amortization formula
// r = monthly rate (annual rate / 12), n = number of months
// M = P * r * (1+r)^n / ((1+r)^n - 1)
double calc_monthly_payment(double principal, double annual_rate, int months) {
    double r = annual_rate / 12.0;
    if (r < 1e-9) return principal / months; // prevent divide by zero if rate is ever 0
    double factor = pow(1.0 + r, months);
    return principal * r * factor / (factor - 1.0);
}

// how much of this month's payment is interest, based on the CURRENT
// remaining balance (not the original principal)
static double calculate_loan_monthly_interest(double remaining_balance, double annual_rate) {
    if (remaining_balance <= 0.0 || annual_rate < 0.0) return 0.0;
    double monthly_rate = annual_rate / 12.0;
    return remaining_balance * monthly_rate;
}

// whatever's left of the payment after interest goes toward principal
static double calculate_principal_payment(double monthly_payment, double interest) {
    double principal_payment = monthly_payment - interest;
    if (principal_payment < 0.0) principal_payment = 0.0;
    return principal_payment;
}

static double calculate_remaining_balance(double remaining_balance, double principal_payment) {
    double new_balance = remaining_balance - principal_payment;
    if (new_balance < 0.0) new_balance = 0.0;
    return new_balance;
}

// sanity checks loan terms before we even calculate a payment plan
static int validate_loan(double principal, double annual_rate, int months) {
    if (principal <= 0.0) return 0;
    if (annual_rate < 0.0) return 0;
    if (months <= 0) return 0;
    return 1;
}

int apply_for_loan(Session *s) {
    Loan arr[MAX_LOANS];
    int count = 0, i;
    Account acc;
    Loan new_loan;

    print_header("APPLY FOR LOAN");
    if (!find_account(s->account.id, &acc)) { print_error("Could not load account."); press_enter(); return 1; }

    load_loans(arr, &count);
    for (i = 0; i < count; i++) {
        if (arr[i].account_id == s->account.id && arr[i].status == LOAN_ACTIVE) {
            print_warn("You already have an active loan. Pay it off first."); press_enter(); return 1;
        }
    }

    printf(" Interest rate : %.1f%% per year\n", DEFAULT_LOAN_RATE * 100.0);
    printf(" Loan range : $%.0f - $%.0f\n", MIN_LOAN_AMOUNT, MAX_LOAN_AMOUNT);
    printf(" Term range : %d - %d months\n\n", MIN_LOAN_MONTHS, MAX_LOAN_MONTHS);

    memset(&new_loan, 0, sizeof(Loan));
    new_loan.principal = get_double(" Loan amount ($) : ", MIN_LOAN_AMOUNT, MAX_LOAN_AMOUNT);
    new_loan.term_months = get_int(" Repayment term (months): ", MIN_LOAN_MONTHS, MAX_LOAN_MONTHS);
    new_loan.interest_rate = DEFAULT_LOAN_RATE;

    // get_int/get_double already enforce the min/max ranges above, but we
    // still run validate_loan() as an explicit final check before doing
    // any math with the numbers
    if (!validate_loan(new_loan.principal, new_loan.interest_rate, new_loan.term_months)) {
        print_error("Invalid loan information.");
        press_enter();
        return 1;
    }

    new_loan.monthly_payment = calc_monthly_payment(new_loan.principal, new_loan.interest_rate, new_loan.term_months);

    printf("\n"); print_separator();
    printf(" Loan amount : $%.2f\n", new_loan.principal);
    printf(" Monthly payment : $%.2f\n", new_loan.monthly_payment);
    printf(" Total repayment : $%.2f\n", new_loan.monthly_payment * new_loan.term_months);
    printf(" Total interest : $%.2f\n", new_loan.monthly_payment * new_loan.term_months - new_loan.principal);
    print_separator();

    // show the first few rows of the repayment schedule so the user can
    // see how the interest/principal split changes over time
    printf("\n Repayment schedule (first %d months):\n", new_loan.term_months < 6 ? new_loan.term_months : 6);
    printf(" %-8s %-12s %-12s %-12s\n", "Month", "Payment", "Interest", "Balance");
    double preview_balance = new_loan.principal;
    for (i = 1; i <= new_loan.term_months && i <= 6; i++) {
        double interest = calculate_loan_monthly_interest(preview_balance, new_loan.interest_rate);
        double principal_paid = calculate_principal_payment(new_loan.monthly_payment, interest);
        if (principal_paid > preview_balance) principal_paid = preview_balance;
        preview_balance = calculate_remaining_balance(preview_balance, principal_paid);
        printf(" %-8d $%-11.2f $%-11.2f $%-11.2f\n", i, new_loan.monthly_payment, interest, preview_balance);
    }
    if (new_loan.term_months > 6) printf(" ... (%d more months)\n", new_loan.term_months - 6);
    printf("\n");

    char confirm[8];
    get_string(" Confirm loan? (YES/no): ", confirm, sizeof(confirm));
    if (strcmp(confirm, "YES") != 0) { print_info("Loan application cancelled."); press_enter(); return 1; }

    new_loan.id = generate_loan_id();
    new_loan.account_id = s->account.id;
    new_loan.remaining_balance = new_loan.principal;
    new_loan.months_paid = 0;
    new_loan.status = LOAN_ACTIVE;
    get_current_datetime(new_loan.start_date);
    arr[count++] = new_loan;
    if (!save_loans(arr, count)) {
        print_error("Could not save the loan -- please try again.");
        press_enter();
        return 1;
    }

    acc.balance += new_loan.principal;
    if (!update_account(&acc)) {
        print_error("CRITICAL: loan was saved but crediting your account failed. Contact an admin.");
        press_enter();
        return 1;
    }
    s->account.balance = acc.balance;

    char note[MAX_NOTE_LEN];
    snprintf(note, sizeof(note), "Loan #%d disbursed", new_loan.id);
    if (!record_txn(acc.id, TXN_LOAN_IN, new_loan.principal, 0, acc.balance, note)) {
        print_warn("Loan was disbursed, but the transaction history record may not have saved.");
    }

    print_success("Loan approved and credited to your account!");
    print_loan_summary(&new_loan);
    press_enter();
    return 0;
}

int make_loan_payment(Session *s) {
    Loan arr[MAX_LOANS];
    int count = 0, i, active_idx = -1;
    Account acc;

    print_header("LOAN REPAYMENT");
    load_loans(arr, &count);
    for (i = 0; i < count; i++) {
        if (arr[i].account_id == s->account.id && arr[i].status == LOAN_ACTIVE) { active_idx = i; break; }
    }
    if (active_idx == -1) { print_info("You have no active loans."); press_enter(); return 1; }

    Loan *loan = &arr[active_idx];
    print_loan_summary(loan);
    if (!find_account(s->account.id, &acc)) { print_error("Could not load account."); press_enter(); return 1; }
    printf(" Your balance: $%.2f\n\n", acc.balance);

    if (acc.balance < loan->monthly_payment) {
        print_error("Insufficient funds.");
        printf(" You need $%.2f but only have $%.2f.\n", loan->monthly_payment, acc.balance);
        press_enter(); return 1;
    }

    // recompute this month's interest/principal split off the CURRENT
    // remaining balance, rather than just subtracting a flat amount
    // this is what actually makes it an amortization schedule
    double interest_this_month = calculate_loan_monthly_interest(loan->remaining_balance, loan->interest_rate);
    double principal_this_month = calculate_principal_payment(loan->monthly_payment, interest_this_month);

    double payment = loan->monthly_payment;
    if (principal_this_month > loan->remaining_balance) {
        // last payment might be smaller than the usual monthly amount
        principal_this_month = loan->remaining_balance;
        payment = principal_this_month + interest_this_month;
    }

    acc.balance -= payment;
    loan->remaining_balance = calculate_remaining_balance(loan->remaining_balance, principal_this_month);
    loan->months_paid++;
    if (loan->remaining_balance < 0.01) { loan->remaining_balance = 0.0; loan->status = LOAN_PAID; }

    if (!update_account(&acc)) {
        print_error("Could not save the payment -- please try again.");
        press_enter();
        return 1;
    }
    if (!save_loans(arr, count)) {
        // account was already debited, put the money back since the loan
        // record itself never actually got the payment
        acc.balance += payment;
        if (update_account(&acc)) {
            print_error("Payment failed to save on the loan -- your balance was restored, nothing was paid.");
        } else {
            print_error("CRITICAL: payment failed partway through AND the rollback also failed. Contact an admin.");
        }
        press_enter();
        return 1;
    }
    s->account.balance = acc.balance;

    char note[MAX_NOTE_LEN];
    snprintf(note, sizeof(note), "Loan #%d payment %d/%d", loan->id, loan->months_paid, loan->term_months);
    if (!record_txn(acc.id, TXN_LOAN_PAY, payment, 0, acc.balance, note)) {
        print_warn("Payment succeeded, but the transaction history record may not have saved.");
    }
    print_receipt("Loan Repayment", payment, acc.balance);
    printf(" Of that payment: $%.2f was interest, $%.2f went to principal.\n", interest_this_month, principal_this_month);

    if (loan->status == LOAN_PAID)
        printf(" Loan #%d fully paid off!\n\n", loan->id);
    else
        printf(" Remaining: $%.2f | Payments left: %d\n\n",
               loan->remaining_balance, loan->term_months - loan->months_paid);
    press_enter();
    return 0;
}

int view_my_loans(const Session *s) {
    Loan arr[MAX_LOANS];
    int count = 0, found = 0, i;
    print_header("MY LOANS");
    load_loans(arr, &count);
    print_loan_table_header();
    for (i = 0; i < count; i++) {
        if (arr[i].account_id == s->account.id) { print_loan_row(&arr[i]); found++; }
    }
    if (found == 0) printf(" No loans on record.\n");
    else print_separator();
    press_enter();
    return 0;
}

// interest section

// simple interest, not compound: balance * rate * (days / 365)
double calc_interest(double balance, double annual_rate, int days) {
    return balance * (annual_rate / 365.0) * days;
}

int apply_interest(Session *s) {
    Account acc;
    int days;
    double earned;
    char note[MAX_NOTE_LEN];

    print_header("APPLY INTEREST");
    if (!find_account(s->account.id, &acc)) { print_error("Could not load account."); press_enter(); return 1; }
    if (acc.type != SAVINGS) { print_warn("Interest is only applied to Savings accounts."); press_enter(); return 1; }

    printf(" Account type : Savings\n");
    printf(" Current balance: $%.2f\n", acc.balance);
    printf(" Annual rate : %.1f%%\n\n", acc.interest_rate * 100.0);
    days = get_int(" Days to calculate interest for (1-365): ", 1, 365);
    earned = calc_interest(acc.balance, acc.interest_rate, days);

    printf("\n"); print_separator();
    printf(" Days : %d\n", days);
    printf(" Interest earned: $%.2f\n", earned);
    printf(" New balance : $%.2f\n", acc.balance + earned);
    print_separator();

    char confirm[8];
    get_string("\n Credit interest to account? (YES/no): ", confirm, sizeof(confirm));
    if (strcmp(confirm, "YES") != 0) { print_info("Cancelled."); press_enter(); return 1; }

    acc.balance += earned;
    if (!update_account(&acc)) {
        print_error("Could not save the interest credit -- please try again.");
        press_enter();
        return 1;
    }
    s->account.balance = acc.balance;
    snprintf(note, sizeof(note), "Interest for %d days @ %.1f%% p.a.", days, acc.interest_rate * 100.0);
    if (!record_txn(acc.id, TXN_INTEREST, earned, 0, acc.balance, note)) {
        print_warn("Interest was credited, but the transaction history record may not have saved.");
    }
    print_receipt("Interest Credit", earned, acc.balance);
    press_enter();
    return 0;
}

