/* 
  main.c (the top level menu loop tying every module together)
  Owner: Siyam
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
        print_menu_item(10, "View loan status");

        printf("\n");
        print_menu_item(0, "Logout");
        print_separator();

        choice = get_int("Select option: ", 0, 10);

        switch (choice) {
            case 1:  view_my_account(s); break;
            case 2:  change_pin(s); break;
            case 3:  delete_my_account(s); break;
            case 4:  deposit(s); break;
            case 5:  withdraw(s); break;
            case 6:  transfer(s); break;
            case 7:  view_history(s); break;
            case 8:  apply_for_loan(s); break;
            case 9:  make_loan_payment(s); break;
            case 10: view_my_loans(s); break;
            case 0:
                s->logged_in = 0;
                print_info("Logged out successfully.");
                press_enter();
                break;
            default:
                break;
        }
    }
    return 0;
}

static int admin_menu() {
    int choice;

    while (1) {
        clear_screen();
        print_logo();
        print_header("ADMINISTRATOR PANEL");

        print_menu_item(1, "System Dashboard");
        print_menu_item(2, "List All Accounts");
        print_menu_item(3, "View Account Details");
        print_menu_item(4, "Reset Account PIN");
        print_menu_item(5, "Toggle Account Status");
        print_menu_item(6, "View All Loans");
        print_menu_item(0, "Exit Admin Panel");
        print_separator();

        choice = get_int("Select option: ", 0, 6);

        switch (choice) {
            case 1: admin_dashboard(); break;
            case 2: admin_list_accounts(); break;
            case 3: admin_view_account(); break;
            case 4: admin_reset_pin(); break;
            case 5: admin_toggle_account(); break;
            case 6: admin_view_all_loans(); break;
            case 0: return 0;
            default: break;
        }
    }
}

int main() {
    Session session;
    memset(&session, 0, sizeof(Session));

    make_data_dir();

    while (1) {
        clear_screen();
        print_logo();
        print_header("MAIN MENU");

        print_menu_item(1, "Customer Login");
        print_menu_item(2, "Create New Account");
        print_menu_item(3, "Admin Login");
        print_menu_item(0, "Exit");
        print_separator();

        int choice = get_int("Select option: ", 0, 3);

        switch (choice) {
            case 1:
                if (login_user(&session)) {
                    user_menu(&session);
                }
                break;
            case 2:
                create_account();
                break;
            case 3:
                if (login_admin(&session)) {
                    admin_menu();
                }
                break;
            case 0:
                clear_screen();
                print_centered("Thank you for using Nexus Bank.");
                printf("\n");
                return 0;
            default:
                break;
        }
    }

    return 0;
}
