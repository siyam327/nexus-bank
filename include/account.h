#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "types.h"

// input helpers and misc utility functions
int    get_int(const char *prompt, int min, int max);
double get_double(const char *prompt, double min, double max);
int    get_string(const char *prompt, char *buf, int maxlen);
int    get_pin(const char *prompt, char *buf);
int    get_masked(const char *prompt, char *buf, int maxlen);
int    trim(char *s);
int    is_digits_only(const char *s);
int    get_current_datetime(char *buf);
int    generate_account_id();
int    generate_txn_id();
int    generate_loan_id();
int    clear_screen();
int    press_enter();
int    make_data_dir();

// login, logout, pin management
int verify_pin(const char *stored, const char *entered);
int login_user(Session *s);
int login_admin(Session *s);
int logout_session(Session *s);
int change_pin(Session *s);

// account create/find/view/close
int find_account(int id, Account *out);
int create_account();
int view_my_account(const Session *s);
int delete_my_account(Session *s);
int update_balance(int account_id,double new_balance);

//customer tier limits
double max_balance_for(CustomerTypetier);
double daily_withdraw_limit_for(CustomerTypetier);
double trsnsfer_limit_for(CustomerTypetier);

#endif

