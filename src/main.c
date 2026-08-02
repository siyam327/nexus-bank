/*
main.c  the top-level menu loop tying every module together
 Owner: Ariful Islam Siyam
 Id:2026-2-60-019
*/

#include <stdio.h>
#include <string.h>
#include "../include/types.h"
#include "../include/utils.h"
#include "../include/display.h"
#include "../include/account.h"
#include "../include/transaction.h"
#include "../include/loan.h"
#include "../include/interest.h"
#include "../include/auth.h"
#include "../include/admin.h"

#ifdef _WIN32
  #include <windows.h>
  #define ENABLE_COLORS() do { \
      HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); \
      DWORD mode = 0; \
      if (GetConsoleMode(h, &mode)) \
          SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING); \
  } while (0)
#else
  #define ENABLE_COLORS() ((void)0)
#endif

static int user_menu(Session *s) {
    int choice;
    // issue fixed: Using 'while' loop instead of 'if'
    while (s->logged_in) {
        clear_screen();
        print_logo();
        print_centered_line(CYAN "Welcome, " BOLD "%s" RESET CYAN " | Account #%d | Balance: " GREEN BOLD "$%.2f" RESET,
               s->account.name, s->account.id, s->account.balance);
        printf("\n");
        print_separator();
        print_menu_item(1,  "View my account");
        print_menu_item(2,  "Change PIN");
        print_menu_item(4,  "Deposit");
        print_menu_item(5,  "Withdraw");
        print_menu_item(0,  "Logout");
        print_separator();

        choice = get_int("\n  Your choice: ", 0, 5);
        clear_screen();
        switch (choice) {
            case 1:  view_my_account(s);   break;
            case 2:  change_pin(s);        break;
            case 4:  deposit(s);           break;
            case 5:  withdraw(s);          break;
            case 0:  logout_session(s);    break;
        }

        if (s->logged_in) {
            Account fresh;
            if (find_account(s->account.id, &fresh)) s->account.balance = fresh.balance;
        }
    }
    return 0;
}

int main() {
    Session session;
    
    int choice;
    ENABLE_COLORS();
    make_data_dir();

    while (1) {
        clear_screen();
        print_logo();
        print_separator();
        print_menu_item(1, "User Login");
        print_menu_item(0, "Exit");
        print_separator();

        choice = get_int("\n  Your choice: ", 0, 1);
        switch (choice) {
            case 1: clear_screen(); if (login_user(&session)) user_menu(&session); break;
            case 0:
                clear_screen();
                printf("\n" CYAN "  Goodbye!\n\n" RESET);
                return 0;
        }
    }
}
