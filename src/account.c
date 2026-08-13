/*
 account.c (account management)
 Owner: Jeetu
 Contains: create/find/view/close account, update balance.
 */
#include <stdio.h>
#include <string.h>
#include "../include/types.h"
#include "../include/account.h"
#include "../include/system.h"

int find_account(int id, Account *out) {
    Account arr[MAX_ACCOUNTS];
    int count = 0, i;
    load_accounts(arr, &count);
    for (i = 0; i < count; i++) {
        if (arr[i].id == id) { *out = arr[i]; return 1; }
    }
    return 0;
}

int create_account() {
    Account arr[MAX_ACCOUNTS];
    int count = 0;
    Account new_acc;
    char pin2[MAX_PIN_LEN];
    int choice;

    print_header("OPEN NEW ACCOUNT");
    memset(&new_acc, 0, sizeof(Account));

    get_string(" Full name : ", new_acc.name, MAX_NAME_LEN);
    if (strlen(new_acc.name) < 2) { print_error("Name is too short."); press_enter(); return 1; }

    printf("\n Account type:\n");
    print_menu_item(1, "Savings (3.5%% interest/year)");
    print_menu_item(2, "Checking (1.0%% interest/year)");
    printf("\n");
    choice = get_int(" Choose [1-2]: ", 1, 2);
    new_acc.type = (choice == 1) ? SAVINGS : CHECKING;
    new_acc.interest_rate = (new_acc.type == SAVINGS) ? SAVINGS_RATE : CHECKING_RATE;

    printf("\n Customer tier:\n");
    print_menu_item(1, "Normal (balance up to $100,000, $20,000/day withdraw limit)");
    print_menu_item(2, "Premium (balance up to $1,000,000, $100,000/day withdraw limit)");
    printf("\n");
    choice = get_int(" Choose [1-2]: ", 1, 2);
    new_acc.tier = (choice == 1) ? NORMAL : PREMIUM;

    printf("\n");
    get_pin(" Set a 6-digit PIN : ", new_acc.pin);
    if (strlen(new_acc.pin) != 6 || !is_digits_only(new_acc.pin)) {
        print_error("PIN must be exactly 6 digits."); press_enter(); return 1;
    }
    get_pin(" Confirm PIN : ", pin2);
    if (strcmp(new_acc.pin, pin2) != 0) {
        print_error("PINs don't match. Account not created."); press_enter(); return 1;
    }

    printf("\n");
    new_acc.balance = get_double(" Initial deposit ($): ", 0.0, max_balance_for(new_acc.tier));
    new_acc.id = generate_account_id();
    new_acc.is_active = 1;
    new_acc.withdrawn_today = 0.0;
    get_current_datetime(new_acc.created_at);
    strncpy(new_acc.last_withdraw_date, new_acc.created_at, MAX_DATE_LEN - 1);

    load_accounts(arr, &count);
    if (count >= MAX_ACCOUNTS) { print_error("System limit reached."); press_enter(); return 1; }
    arr[count++] = new_acc;
    if (!save_accounts(arr, count)) {
        print_error("Could not save the new account. Please try again.");
        press_enter();
        return 1;
    }

    printf("\n" " Account created successfully!\n" );
    printf(" Your Account ID: " "%d\n", new_acc.id);
    printf(" Keep this number safe, you'll need it to log in.\n\n");
    print_account_card(&new_acc);
    press_enter();
    return 0;
}

int view_my_account(const Session *s) {
    Account fresh;
    print_header("MY ACCOUNT");
    if (!find_account(s->account.id, &fresh)) { print_error("Could not load account data."); press_enter(); return 1; }
    print_account_card(&fresh);
    press_enter();
    return 0;
}

// doesn't actually delete anything, just flips is_active off. transactions
// still need the account record to exist so we can't just remove it
int delete_my_account(Session *s) {
    char pin[MAX_PIN_LEN], confirm[8];
    Account fresh;

    print_header("CLOSE ACCOUNT");
    print_warn("This will permanently deactivate your account.");
    printf(" You must have a zero balance before closing.\n\n");

    if (!find_account(s->account.id, &fresh)) { print_error("Account not found."); press_enter(); return 1; }
    if (fresh.balance > 0.01) { print_error("Please withdraw all funds before closing."); press_enter(); return 1; }

    get_pin(" Confirm PIN to proceed : ", pin);
    if (strcmp(s->account.pin, pin) != 0) { print_error("Wrong PIN. Cancelled."); press_enter(); return 1; }

    get_string(" Type YES to confirm : ", confirm, sizeof(confirm));
    if (strcmp(confirm, "YES") != 0) { print_info("Cancelled. No changes made."); press_enter(); return 1; }

    fresh.is_active = 0;
    if (!update_account(&fresh)) {
        print_error("Could not close the account -- please try again.");
        press_enter();
        return 1;
    }
    print_success("Account closed. Thank you for banking with us.");
    memset(s, 0, sizeof(Session));
    press_enter();
    return 0;
}

int update_balance(int account_id, double new_balance) {
    Account acc;
    if (!find_account(account_id, &acc)) return 0;
    acc.balance = new_balance;
    return update_account(&acc);
}

double max_balance_for(CustomerType tier) {
    return (tier == PREMIUM) ? PREMIUM_MAX_BALANCE : NORMAL_MAX_BALANCE;
}

double daily_withdraw_limit_for(CustomerType tier) {
    return (tier == PREMIUM) ? PREMIUM_DAILY_WITHDRAW : NORMAL_DAILY_WITHDRAW;
}

double transfer_limit_for(CustomerType tier) {
    return (tier == PREMIUM) ? PREMIUM_TRANSFER_LIMIT : NORMAL_TRANSFER_LIMIT;
}
