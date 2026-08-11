/* 
 * main.c  - the top level menu loop tying every module together
 * Owner: Siyam
 */
#include <stdio.h>
#include <string.h>
#include "../include/types.h"
#include "../include/account.h"
#include "../include/banking.h"
#include "../include/system.h"

static int user_menu(Session *s) {
    int choice;
    char line[160];
    while (s->logged_in) {
        clear_screen();
        print_logo();
        snprintf(line, sizeof(line), "Welcome, %s | Account #%d | Balance: $%.2f",
                 s->account.name, s->account.id, s->account.balance);
        print_centered(line);
        printf("\n");
        print_separator();
        print_centered("ACCOUNT");
        print_menu_item(1, "View my account");
        print_menu_item(2, "Change PIN");
        print_menu_item(3, "Close account");
        printf("\n");
        print_centered("TRANSACTIONS");
        print_menu_item(4, "Deposit");
        print_menu_item(5, "Withdraw");
        print_menu_item(6, "Transfer");
        print_menu_item(7, "Transaction history");
        printf("\n");
        print_centered("LOANS & INTEREST");
        print_menu_item(8, "Apply for loan");
        print_menu_item(9, "Make loan payment");
        print_menu_item(10, "View my loans");
        print_menu_item(11, "Apply interest (savings)");
        printf("\n");
        print_centered("OTHER");
        print_menu_item(12, "Export account statement");
        printf("\n");
        print_menu_item(0, "Logout");
        print_separator();

        choice = get_int("\nYour choice: ", 0, 12);
        clear_screen();
        switch (choice) {
            case 1: view_my_account(s); break;
            case 2: change_pin(s); break;
            case 3: delete_my_account(s); break;
            case 4: deposit(s); break;
            case 5: withdraw(s); break;
            case 6: transfer(s); break;
            case 7: view_history(s); break;
            case 8: apply_for_loan(s); break;
            case 9: make_loan_payment(s); break;
            case 10: view_my_loans(s); break;
            case 11: apply_interest(s); break;
            case 12: export_statement(s); break;
            case 0: logout_session(s); break;
        }
        // balance might have changed after whatever action ran, refresh it
        if (s->logged_in) {
            Account fresh;
            if (find_account(s->account.id, &fresh)) s->account.balance = fresh.balance;
        }
    }
    return 0;
}

static int admin_menu(Session *s) {
    int choice;
    while (s->logged_in && s->is_admin) {
        clear_screen();
        print_logo();
        print_centered("*** ADMIN PANEL ***");
        printf("\n");
        admin_dashboard();
        print_separator();
        print_menu_item(1, "List all accounts");
        print_menu_item(2, "View account details");
        print_menu_item(3, "Reset account PIN");
        print_menu_item(4, "Activate / deactivate account");
        print_menu_item(5, "View all loans");
        print_menu_item(0, "Logout");
        print_separator();

        choice = get_int("\nYour choice: ", 0, 5);
        clear_screen();
        switch (choice) {
            case 1: admin_list_accounts(); break;
            case 2: admin_view_account(); break;
            case 3: admin_reset_pin(); break;
            case 4: admin_toggle_account(); break;
            case 5: admin_view_all_loans(); break;
            case 0: logout_session(s); break;
        }
    }
    return 0;
}

int main() {
    Session session;
    int choice;
    make_data_dir();
    memset(&session, 0, sizeof(session));

    while (1) {
        clear_screen();
        print_logo();
        print_separator();
        print_menu_item(1, "User Login");
        print_menu_item(2, "Open New Account");
        print_menu_item(3, "Admin Login");
        print_menu_item(0, "Exit");
        print_separator();

        choice = get_int("\nYour choice: ", 0, 3);
        switch (choice) {
            case 1: clear_screen(); if (login_user(&session)) user_menu(&session); break;
            case 2: clear_screen(); create_account(); break;
            case 3: clear_screen(); if (login_admin(&session)) admin_menu(&session); break;
            case 0:
                clear_screen();
                printf("\nThank you for using %s. Goodbye!\n\n", BANK_NAME);
                return 0;
        }
    }
}
