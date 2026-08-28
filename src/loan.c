/* loan.c (loans + savings interest)
Owner: Hasan Ali
ID:2026-2-60-026 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/types.h"
#include "../include/system.h"
#include "../include/account.h"
#include "../include/banking.h"

//LOAN FUNCTIONS//

double calc_monthly_payment(double principal,double annual_rate,int months)
{
    double monthly_rate;
    double power;
    double numerator;
    double denominator;

    if (principal <= 0.0 || annual_rate < 0.0 || months <= 0)
    {
        return 0.0;
    }

    if (annual_rate == 0.0)
    {
        return principal / months;
    }

    monthly_rate = annual_rate / 12.0;

    power = pow(1.0 + monthly_rate, months);

    numerator = principal * monthly_rate * power;

    denominator = power - 1.0;

    return numerator / denominator;
}


double calculate_loan_monthly_interest(double remaining_balance, double annual_rate)
{
    double monthly_rate;

    if (remaining_balance <= 0.0 || annual_rate < 0.0)
    {
        return 0.0;
    }

    monthly_rate = annual_rate / 12.0;

    return remaining_balance * monthly_rate;
}


double calculate_principal_payment(double monthly_payment,double interest)
{
    double principal_payment;

    principal_payment = monthly_payment - interest;

    if (principal_payment < 0.0)
    {
        principal_payment = 0.0;
    }

    return principal_payment;
}


double calculate_remaining_balance(double remaining_balance,double principal_payment)
{
    double new_balance;

    new_balance = remaining_balance - principal_payment;

    if (new_balance < 0.0)
    {
        new_balance = 0.0;
    }

    return new_balance;
}
double calculate_total_payment(double monthly_payment,int months)
{
    return monthly_payment * months;
}
double calculate_total_interest(double monthly_payment,int months,double principal)
{
    double total_payment;

    total_payment = calculate_total_payment(monthly_payment, months);

    return total_payment - principal;
}

int validate_loan(double principal,double annual_rate,int months)
{
    if (principal <= 0.0)
    {
        return 0;
    }
    if (annual_rate < 0.0)
    {
        return 0;
    }
    if (months <= 0)
    {
        return 0;
    }
    return 1;
}

//INTEREST FUNCTIONS//

double get_interest_rate(int account_type)
{
    if (account_type == 0)
    {
        return SAVINGS_RATE;
    }

    if (account_type == 1)
    {
        return CHECKING_RATE;
    }
    return 0.0;
}
double calc_interest(double balance,double annual_rate,int days)
{
    double daily_rate;
    daily_rate = annual_rate / 365.0;
    return balance * daily_rate * days;
}
double calculate_account_monthly_interest(double balance, double annual_rate)
{
    double monthly_rate;
    monthly_rate = annual_rate / 12.0;
    return balance * monthly_rate;
}
double calculate_interest_amount(double balance,double annual_rate)
{
    return calculate_account_monthly_interest(balance,annual_rate);
}
double calculate_new_balance(double balance,double interest)
{
    return balance + interest;
}
double calculate_yearly_interest(double balance,double annual_rate)
{
    return balance * annual_rate;
}

//MENU FUNCTIONS (connect the math above to the actual bank system)//

int apply_for_loan(Session *s)
{
    Loan arr[MAX_LOANS];
    int count;
    int i;
    int already_has_loan;
    Account acc;
    Loan new_loan;
    char confirm[8];
    double total_paid;
    double total_interest;

    print_header("APPLY FOR LOAN");

    if (!find_account(s->account.id, &acc))
    {
        print_error("Could not load account.");
        press_enter();
        return 1;
    }

    count = 0;
    load_loans(arr, &count);

    already_has_loan = 0;
    for (i = 0; i < count; i++)
    {
        if (arr[i].account_id == s->account.id)
        {
            if (arr[i].status == LOAN_ACTIVE)
            {
                already_has_loan = 1;
            }
        }
    }

    if (already_has_loan == 1)
    {
        print_warn("You already have an active loan. Pay it off first.");
        press_enter();
        return 1;
    }

    printf(" Interest rate : %.1f%% per year\n", DEFAULT_LOAN_RATE * 100.0);
    printf(" Loan range : $%.0f - $%.0f\n", MIN_LOAN_AMOUNT, MAX_LOAN_AMOUNT);
    printf(" Term range : %d - %d months\n\n", MIN_LOAN_MONTHS, MAX_LOAN_MONTHS);

    new_loan.principal = get_double(" Loan amount ($) : ", MIN_LOAN_AMOUNT, MAX_LOAN_AMOUNT);
    new_loan.term_months = get_int(" Repayment term (months): ", MIN_LOAN_MONTHS, MAX_LOAN_MONTHS);
    new_loan.interest_rate = DEFAULT_LOAN_RATE;

    if (!validate_loan(new_loan.principal, new_loan.interest_rate, new_loan.term_months))
    {
        print_error("Invalid loan information.");
        press_enter();
        return 1;
    }

    new_loan.monthly_payment = calc_monthly_payment(new_loan.principal, new_loan.interest_rate, new_loan.term_months);
    total_paid = calculate_total_payment(new_loan.monthly_payment, new_loan.term_months);
    total_interest = calculate_total_interest(new_loan.monthly_payment, new_loan.term_months, new_loan.principal);

    printf("\n");
    print_separator();
    printf(" Monthly payment : $%.2f\n", new_loan.monthly_payment);
    printf(" Total repayment : $%.2f\n", total_paid);
    printf(" Total interest  : $%.2f\n", total_interest);
    print_separator();

    get_string("\n Confirm loan? (YES/no): ", confirm, sizeof(confirm));
    if (strcmp(confirm, "YES") != 0)
    {
        print_info("Loan application cancelled.");
        press_enter();
        return 1;
    }

    new_loan.id = generate_loan_id();
    new_loan.account_id = s->account.id;
    new_loan.remaining_balance = new_loan.principal;
    new_loan.months_paid = 0;
    new_loan.status = LOAN_ACTIVE;
    get_current_datetime(new_loan.start_date);

    arr[count] = new_loan;
    count = count + 1;

    if (!save_loans(arr, count))
    {
        print_error("Could not save the loan. Please try again.");
        press_enter();
        return 1;
    }

    acc.balance = acc.balance + new_loan.principal;
    if (!update_account(&acc))
    {
        print_error("Loan was saved but crediting your account failed. Contact an admin.");
        press_enter();
        return 1;
    }
    s->account.balance = acc.balance;

    record_txn(acc.id, TXN_LOAN_IN, new_loan.principal, 0, acc.balance, "Loan disbursed");

    print_success("Loan approved and credited to your account!");
    print_loan_summary(&new_loan);
    press_enter();
    return 0;
}


int make_loan_payment(Session *s)
{
    Loan arr[MAX_LOANS];
    int count;
    int i;
    int active_index;
    Account acc;
    double interest_this_month;
    double principal_this_month;
    double payment;

    print_header("LOAN REPAYMENT");

    count = 0;
    load_loans(arr, &count);

    active_index = -1;
    for (i = 0; i < count; i++)
    {
        if (arr[i].account_id == s->account.id)
        {
            if (arr[i].status == LOAN_ACTIVE)
            {
                active_index = i;
            }
        }
    }

    if (active_index == -1)
    {
        print_info("You have no active loans.");
        press_enter();
        return 1;
    }

    print_loan_summary(&arr[active_index]);

    if (!find_account(s->account.id, &acc))
    {
        print_error("Could not load account.");
        press_enter();
        return 1;
    }
    printf(" Your balance: $%.2f\n\n", acc.balance);

    if (acc.balance < arr[active_index].monthly_payment)
    {
        print_error("Insufficient funds.");
        press_enter();
        return 1;
    }

    interest_this_month = calculate_loan_monthly_interest(arr[active_index].remaining_balance, arr[active_index].interest_rate);
    principal_this_month = calculate_principal_payment(arr[active_index].monthly_payment, interest_this_month);
    payment = arr[active_index].monthly_payment;

    if (principal_this_month > arr[active_index].remaining_balance)
    {
        principal_this_month = arr[active_index].remaining_balance;
        payment = principal_this_month + interest_this_month;
    }

    acc.balance = acc.balance - payment;
    arr[active_index].remaining_balance = calculate_remaining_balance(arr[active_index].remaining_balance, principal_this_month);
    arr[active_index].months_paid = arr[active_index].months_paid + 1;

    if (arr[active_index].remaining_balance < 0.01)
    {
        arr[active_index].remaining_balance = 0.0;
        arr[active_index].status = LOAN_PAID;
    }

    if (!update_account(&acc))
    {
        print_error("Could not save the payment.");
        press_enter();
        return 1;
    }

    if (!save_loans(arr, count))
    {
        acc.balance = acc.balance + payment;
        update_account(&acc);
        print_error("Payment failed to save. Your balance was restored.");
        press_enter();
        return 1;
    }
    s->account.balance = acc.balance;

    record_txn(acc.id, TXN_LOAN_PAY, payment, 0, acc.balance, "Loan payment");

    print_receipt("Loan Repayment", payment, acc.balance);

    if (arr[active_index].status == LOAN_PAID)
    {
        printf(" Loan fully paid off!\n\n");
    }
    else
    {
        printf(" Remaining: $%.2f | Payments left: %d\n\n",
               arr[active_index].remaining_balance,
               arr[active_index].term_months - arr[active_index].months_paid);
    }

    press_enter();
    return 0;
}


int view_my_loans(const Session *s)
{
    Loan arr[MAX_LOANS];
    int count;
    int found;
    int i;

    print_header("MY LOANS");

    count = 0;
    load_loans(arr, &count);
    print_loan_table_header();

    found = 0;
    for (i = 0; i < count; i++)
    {
        if (arr[i].account_id == s->account.id)
        {
            print_loan_row(&arr[i]);
            found = found + 1;
        }
    }

    if (found == 0)
    {
        printf(" No loans on record.\n");
    }
    else
    {
        print_separator();
    }

    press_enter();
    return 0;
}


int apply_interest(Session *s)
{
    Account acc;
    int days;
    double earned;
    char confirm[8];

    print_header("APPLY INTEREST");

    if (!find_account(s->account.id, &acc))
    {
        print_error("Could not load account.");
        press_enter();
        return 1;
    }

    if (acc.type != SAVINGS)
    {
        print_warn("Interest is only applied to Savings accounts.");
        press_enter();
        return 1;
    }

    printf(" Current balance: $%.2f\n", acc.balance);
    printf(" Annual rate : %.1f%%\n\n", acc.interest_rate * 100.0);

    days = get_int(" Days to calculate interest for (1-365): ", 1, 365);
    earned = calc_interest(acc.balance, acc.interest_rate, days);

    printf("\n");
    print_separator();
    printf(" Interest earned: $%.2f\n", earned);
    printf(" New balance : $%.2f\n", acc.balance + earned);
    print_separator();

    get_string("\n Credit interest to account? (YES/no): ", confirm, sizeof(confirm));
    if (strcmp(confirm, "YES") != 0)
    {
        print_info("Cancelled.");
        press_enter();
        return 1;
    }

    acc.balance = acc.balance + earned;
    if (!update_account(&acc))
    {
        print_error("Could not save the interest credit.");
        press_enter();
        return 1;
    }
    s->account.balance = acc.balance;

    record_txn(acc.id, TXN_INTEREST, earned, 0, acc.balance, "Interest credited");

    print_receipt("Interest Credit", earned, acc.balance);
    press_enter();
    return 0;
}
