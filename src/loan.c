#include <stdio.h>
#include <math.h>
#include "../include/loan.h"



#define ACTIVE 0
#define CLOSED 1
#define DELINQUENT 2



double calc_monthly_payment(double principal, double annual_rate, int months);

double calculate_monthly_interest(double remaining_balance,
                                  double annual_rate);

double calculate_principal_payment(double monthly_payment,
                                   double interest);

double calculate_remaining_balance(double remaining_balance,
                                   double principal_payment);

double calculate_total_payment(double monthly_payment, int months);

double calculate_total_interest(double monthly_payment,
                                int months,
                                double principal);

int validate_loan(double principal, double annual_rate, int months);



double calc_monthly_payment(double principal, double annual_rate, int months)
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




double calculate_monthly_interest(double remaining_balance,
                                  double annual_rate)
{
    double monthly_rate;

    if (remaining_balance <= 0.0 || annual_rate < 0.0)
    {
        return 0.0;
    }

    monthly_rate = annual_rate / 12.0;

    return remaining_balance * monthly_rate;
}




double calculate_principal_payment(double monthly_payment,
                                   double interest)
{
    double principal_payment;

    principal_payment = monthly_payment - interest;

    if (principal_payment < 0.0)
    {
        principal_payment = 0.0;
    }

    return principal_payment;
}




double calculate_remaining_balance(double remaining_balance,
                                   double principal_payment)
{
    double new_balance;

    new_balance = remaining_balance - principal_payment;

    if (new_balance < 0.0)
    {
        new_balance = 0.0;
    }

    return new_balance;
}





double calculate_total_payment(double monthly_payment, int months)
{
    return monthly_payment * months;
}



double calculate_total_interest(double monthly_payment,
                                int months,
                                double principal)
{
    double total_payment;

    total_payment = calculate_total_payment(monthly_payment, months);

    return total_payment - principal;
}




int validate_loan(double principal, double annual_rate, int months)
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



int main()
{
    double principal;
    double annual_rate;
    int months;

    double monthly_payment;
    double monthly_interest;
    double principal_payment;
    double remaining_balance;

    double total_payment;
    double total_interest;

    int payment_month;
    int status;



    principal = 10000.0;
    annual_rate = 0.08;
    months = 12;

   

    if (validate_loan(principal, annual_rate, months) == 0)
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

    payment_month = 0;

    status = ACTIVE;



    printf("\n");
    printf("============================================\n");
    printf("             NEXUS BANK - LOAN              \n");
    printf("============================================\n");

    printf("Loan Amount       : %.2f\n", principal);
    printf("Annual Rate       : %.2f%%\n", annual_rate * 100);
    printf("Loan Term         : %d months\n", months);
    printf("Monthly EMI       : %.2f\n", monthly_payment);
    printf("Total Payment     : %.2f\n", total_payment);
    printf("Total Interest    : %.2f\n", total_interest);

    printf("============================================\n");
    printf("           REPAYMENT SCHEDULE               \n");
    printf("============================================\n");

    printf("%-8s %-12s %-12s %-12s\n",
           "Month",
           "EMI",
           "Interest",
           "Balance");

    printf("--------------------------------------------\n");



    while (payment_month < months && remaining_balance > 0.0)
    {
        payment_month++;

        /* Calculate current month's interest */

        monthly_interest = calculate_monthly_interest(
            remaining_balance,
            annual_rate
        );

        /* Calculate principal portion of EMI */

        principal_payment = calculate_principal_payment(
            monthly_payment,
            monthly_interest
        );

        /* Prevent overpayment in final month */

        if (principal_payment > remaining_balance)
        {
            principal_payment = remaining_balance;
        }

       

        remaining_balance = calculate_remaining_balance(
            remaining_balance,
            principal_payment
        );

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

    printf("--------------------------------------------\n");

    printf("Months Paid       : %d\n", payment_month);
    printf("Remaining Balance : %.2f\n", remaining_balance);

    if (status == ACTIVE)
    {
        printf("Loan Status       : ACTIVE\n");
    }
    else if (status == CLOSED)
    {
        printf("Loan Status       : CLOSED\n");
    }
    else
    {
        printf("Loan Status       : DELINQUENT\n");
    }

    printf("============================================\n");

    return 0;
}