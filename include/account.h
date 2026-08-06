#ifndef ACCOUNT_H
#define ACCOUNT_H


struct account
{
    int id;
    char name[30];
    char type[20];
    char pin[20];
    char userType[20];
    float balance;
    int active;
};

int create_account();
int find_account();
int view_account();
int update_balance(int searchID, float new_balance);
int close_account();

#endif

