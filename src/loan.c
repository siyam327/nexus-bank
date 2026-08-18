/* loan.c (loans + savings interest) 
Owner: Hasan Ali 
ID:2026-2-60-026 */
#include <stdio.h>
#include <math.h>

#define ACTIVE 0
#define CLOSED 1
#define DELINQUENT 2

#define SAVINGS_RATE 0.035
#define CHECKING_RATE 0.020


double calc_monthly_payment(double principal, double annual_rate, int months);

double calculate_loan_monthly_interest(double remaining_balance,double annual_rate);

double calculate_principal_payment(double monthly_payment,double interest);

double calculate_remaining_balance(double remaining_balance,double principal_payment);

double calculate_total_payment(double monthly_payment,int months);

double calculate_total_interest(double monthly_payment,int months,double principal);

int validate_loan(double principal,double annual_rate,int months);

double get_interest_rate(int account_type);

double calc_interest(double balance,double annual_rate,int days);

double calculate_account_monthly_interest(double balance,double annual_rate);

double calculate_interest_amount(double balance,double annual_rate);

double calculate_new_balance(double balance,double interest);

double calculate_yearly_interest(double balance,double annual_rate);


//FILE HANDLING FUNCTIONS//

int save_loan_data(double principal,
                   double annual_rate,
                   int months,
                   double monthly_payment,
                   double total_payment,
                   double total_interest,
                   double remaining_balance,
                   int status)
{
    FILE *file;

    file = fopen("data/loans.txt", "a");

    if (file == NULL)
    {
        printf("Error: Could not open loan file.\n");
        return 0;
    }

    fprintf(file, "Loan Amount: %.2f\n", principal);
    fprintf(file, "Annual Rate: %.2f%%\n", annual_rate * 100);
    fprintf(file, "Loan Term: %d months\n", months);
    fprintf(file, "Monthly EMI: %.2f\n", monthly_payment);
    fprintf(file, "Total Payment: %.2f\n", total_payment);
    fprintf(file, "Total Interest: %.2f\n", total_interest);
    fprintf(file, "Remaining Balance: %.2f\n", remaining_balance);

    if (status == ACTIVE)
        fprintf(file, "Loan Status: ACTIVE\n");
    else if (status == CLOSED)
        fprintf(file, "Loan Status: CLOSED\n");
    else
        fprintf(file, "Loan Status: DELINQUENT\n");

    fprintf(file, "--------------------------------------------\n");

    fclose(file);

    return 1;
}


int save_interest_data(double savings_balance,
                       double savings_interest,
                       double savings_new,
                       double savings_yearly,
                       double checking_balance,
                       double checking_interest,
                       double checking_new,
                       double checking_yearly)
{
    FILE *file;

    file = fopen("data/interest.txt", "a");

    if (file == NULL)
    {
        printf("Error: Could not open interest file.\n");
        return 0;
    }

    fprintf(file, "Savings Account\n");
    fprintf(file, "Balance: %.2f\n", savings_balance);
    fprintf(file, "Monthly Interest: %.2f\n", savings_interest);
    fprintf(file, "New Balance: %.2f\n", savings_new);
    fprintf(file, "Yearly Interest: %.2f\n", savings_yearly);

    fprintf(file, "\nChecking Account\n");
    fprintf(file, "Balance: %.2f\n", checking_balance);
    fprintf(file, "Monthly Interest: %.2f\n", checking_interest);
    fprintf(file, "New Balance: %.2f\n", checking_new);
    fprintf(file, "Yearly Interest: %.2f\n", checking_yearly);

    fprintf(file, "============================================\n");

    fclose(file);

    return 1;
}


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

int main()
{
    //LOAN MODULE//

    double principal = 10000.0;
    double annual_rate = 0.08;
    int months = 12;

    double monthly_payment;
    double monthly_interest;
    double principal_payment;
    double remaining_balance;

    double total_payment;
    double total_interest;

    int payment_month = 0;
    int status = ACTIVE;

    if (!validate_loan(principal, annual_rate, months))
    {
        printf("Invalid loan information.\n");
        return 1;
    }

    monthly_payment = calc_monthly_payment(
        principal,
        annual_rate,
        months
    );

    total_payment = calculate_total_payment(
        monthly_payment,
        months
    );

    total_interest = calculate_total_interest(
        monthly_payment,
        months,
        principal
    );

    remaining_balance = principal;

    printf("\n============================================\n");
    printf("             NEXUS BANK - LOAN\n");
    printf("============================================\n");

    printf("Loan Amount       : %.2f\n", principal);
    printf("Annual Rate       : %.2f%%\n", annual_rate * 100);
    printf("Loan Term         : %d months\n", months);
    printf("Monthly EMI       : %.2f\n", monthly_payment);
    printf("Total Payment     : %.2f\n", total_payment);
    printf("Total Interest    : %.2f\n", total_interest);

    printf("============================================\n");
    printf("REPAYMENT SCHEDULE\n");
    printf("============================================\n");
    printf("%-8s %-12s %-12s %-12s\n",
           "Month",
           "EMI",
           "Interest",
           "Balance");
    while (payment_month < months &&
           remaining_balance > 0.0)
    {
        payment_month++;
        monthly_interest =
            calculate_loan_monthly_interest(remaining_balance,annual_rate);
        principal_payment =calculate_principal_payment(monthly_payment,monthly_interest);
        if (principal_payment > remaining_balance)
        {
            principal_payment = remaining_balance;
        }
        remaining_balance =
            calculate_remaining_balance(remaining_balance,principal_payment);
        printf("%-8d %-12.2f %-12.2f %-12.2f\n",
               payment_month,
               monthly_payment,
               monthly_interest,
               remaining_balance);
    }
    if (remaining_balance <= 0.01)
    {
        remaining_balance = 0.0;
        status = CLOSED;
    }
    printf("Loan Status : ");
    if (status == ACTIVE)
        printf("ACTIVE\n");
    else if (status == CLOSED)
        printf("CLOSED\n");
    else
        printf("DELINQUENT\n");
   //SAVE LOAN DATA//
    save_loan_data(
        principal,
        annual_rate,
        months,
        monthly_payment,
        total_payment,
        total_interest,
        remaining_balance,
        status
    );
    //INTEREST MODULE //
    double savings_balance = 10000.0;
    double checking_balance = 10000.0;
    double savings_rate = get_interest_rate(0);
    double checking_rate = get_interest_rate(1);
    double savings_interest =
        calculate_interest_amount(savings_balance,savings_rate);
    double checking_interest =calculate_interest_amount(checking_balance,checking_rate);
    double savings_new =calculate_new_balance(savings_balance,savings_interest);
    double checking_new =calculate_new_balance( checking_balance,checking_interest);
    double savings_yearly =calculate_yearly_interest( savings_balance,savings_rate);
    double checking_yearly =calculate_yearly_interest(
            checking_balance,
            checking_rate);
    printf("\n============================================\n");
    printf("          NEXUS BANK - INTEREST\n");
    printf("============================================\n");

    printf("\nSavings Account\n");
    printf("Balance          : %.2f\n", savings_balance);
    printf("Monthly Interest : %.2f\n", savings_interest);
    printf("New Balance      : %.2f\n", savings_new);
    printf("Yearly Interest  : %.2f\n", savings_yearly);

    printf("\nChecking Account\n");
    printf("Balance          : %.2f\n", checking_balance);
    printf("Monthly Interest : %.2f\n", checking_interest);
    printf("New Balance      : %.2f\n", checking_new);
    printf("Yearly Interest  : %.2f\n", checking_yearly);

    // SAVE INTEREST DATA //
    save_interest_data(
        savings_balance,
        savings_interest,
        savings_new,
        savings_yearly,
        checking_balance,
        checking_interest,
        checking_new,
        checking_yearly
    );
    return 0;
}
