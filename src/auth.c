/*
 auth.c (login, logout, pin management)
 Owner: Arpita Malakar
 Id:2026-2-60-553
*/
#include <stdio.h>
#include <string.h>
#include "../include/types.h"
#include "../include/account.h"
#include "../include/system.h"

int verify_pin(const char *stored, const char *entered) {
    return strcmp(stored, entered) == 0;
}

int login_user(Session *s) {
    int id;
    char pin[MAX_PIN_LEN], buf[32];
    Account acc;

    print_header("USER LOGIN");
    printf(" Account ID : ");
    fflush(stdout);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    if (sscanf(buf, "%d", &id) != 1) { print_error("Invalid account ID."); press_enter(); return 0; }
    if (!find_account(id, &acc)) { print_error("Account not found."); press_enter(); return 0; }
    if (!acc.is_active) { print_error("This account is inactive. Contact admin."); press_enter(); return 0; }

    get_pin(" Enter PIN : ", pin);
    if (!verify_pin(acc.pin, pin)) {
        // wrong pin, count the attempt and lock the account after 3
        acc.failed_attempts++;
        if (acc.failed_attempts >= 3) {
            acc.is_active = 0;
            if (!update_account(&acc)) print_warn("Could not save the account lockout -- please tell an admin.");
            print_error("Too many wrong attempts. Your account is now LOCKED.");
            printf( " Please visit an admin to unlock your account.\n" );
        } else {
            if (!update_account(&acc)) print_warn("Could not save the failed-attempt count.");
            printf( " [!] " "Incorrect PIN. %d attempt(s) remaining before lockout.\n",
                   3 - acc.failed_attempts);
        }
        press_enter();
        return 0;
    }

    if (acc.failed_attempts > 0) {
        acc.failed_attempts = 0;
        if (!update_account(&acc)) print_warn("Could not reset the failed-attempt count.");
    }
    s->logged_in = 1;
    s->is_admin = 0;
    s->account = acc;
    print_success("Login successful. Welcome back!");
    press_enter();
    return 1;
}

int login_admin(Session *s) {
    char pass[MAX_PASS_LEN];
    print_header("ADMIN LOGIN");
    get_masked(" Admin password : ", pass, MAX_PASS_LEN);
    if (strcmp(pass, ADMIN_PASSWORD) != 0) { print_error("Wrong password."); press_enter(); return 0; }
    s->logged_in = 1;
    s->is_admin = 1;
    memset(&s->account, 0, sizeof(Account));
    print_success("Admin access granted.");
    press_enter();
    return 1;
}

int logout_session(Session *s) {
    memset(s, 0, sizeof(Session));
    print_success("You have been logged out.");
    press_enter();
    return 0;
}

int change_pin(Session *s) {
    char old_pin[MAX_PIN_LEN], new_pin[MAX_PIN_LEN], confirm[MAX_PIN_LEN];
    print_header("CHANGE PIN");
    get_pin(" Current PIN : ", old_pin);
    if (!verify_pin(s->account.pin, old_pin)) { print_error("Incorrect current PIN."); press_enter(); return 1; }
    get_pin(" New PIN (6 digits) : ", new_pin);
    if (strlen(new_pin) != 6 || !is_digits_only(new_pin)) { print_error("PIN must be exactly 6 digits."); press_enter(); return 1; }
    if (strcmp(new_pin, old_pin) == 0) { print_warn("New PIN can't be the same as old one."); press_enter(); return 1; }
    get_pin(" Confirm new PIN : ", confirm);
    if (strcmp(new_pin, confirm) != 0) { print_error("PINs don't match."); press_enter(); return 1; }
    strncpy(s->account.pin, new_pin, MAX_PIN_LEN);
    if (update_account(&s->account)) print_success("PIN changed successfully.");
    else print_error("Failed to save new PIN.");
    press_enter();
    return 0;
}
