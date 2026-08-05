#ifndef LOAN_INTEREST_H
#define LOAN_INTEREST_H

/* ================= LOAN FUNCTIONS ================= */

double calc_monthly_payment(double principal,
                            double annual_rate,
                            int months);

double calculate_monthly_interest(double remaining_balance,
                                  double annual_rate);

double calculate_principal_payment(double monthly_payment,
                                   double interest);

double calculate_remaining_balance(double remaining_balance,
                                   double principal_payment);

double calculate_total_payment(double monthly_payment,
                               int months);

double calculate_total_interest(double monthly_payment,
                                int months,
                                double principal);

int validate_loan(double principal,
                  double annual_rate,
                  int months);


/* ================= INTEREST FUNCTIONS ================= */

double get_interest_rate(int account_type);

double calc_interest(double balance,
                     double annual_rate,
                     int days);

double calculate_interest_amount(double balance,
                                 double annual_rate);

double calculate_new_balance(double balance,
                             double interest);

double calculate_yearly_interest(double balance,
                                 double annual_rate);

#endif