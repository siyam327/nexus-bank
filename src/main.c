#include <stdio.h>

#include "cashdesk.h"


int main()
{
    Customer customers[MAX_CUSTOMERS];

    int customerCount;

    int choice;

    int id;


    /* Array */

    int menuNumbers[9] =
    {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };


    /* Multidimensional Array */

    char menuNames[9][40] =
    {
        "Create Customer",
        "Deposit Money",
        "Withdraw Money",
        "Peer-to-Peer Transfer",
        "Show Customer Information",
        "Show Customer Statement",
        "Show All Transaction History",
        "Reset Daily Withdrawal Limit",
        "Exit"
    };


    /* Pointer */

    Customer *customerPointer;


    customerCount =
        loadCustomers(
            customers
        );


    /* Pointer Assignment */

    customerPointer =
        customers;


    printf(
        "\n============================================\n"
    );

    printf(
        "          CASH DESK SYSTEM\n"
    );

    printf(
        "============================================\n"
    );


    /* Nested Loop */

    {
        int i;
        int j;

        for (
            i = 0;
            i < 1;
            i++
        )
        {
            for (
                j = 0;
                j < 1;
                j++
            )
            {
                printf(
                    "Welcome to Cash Desk System!\n"
                );
            }
        }
    }


    do
    {
        printf(
            "\n============== MAIN MENU ==============\n"
        );


        /* Loop + Multidimensional Array */

        {
            int i;

            for (
                i = 0;
                i < 9;
                i++
            )
            {
                printf(
                    "%d. %s\n",
                    menuNumbers[i],
                    menuNames[i]
                );
            }
        }


        printf(
            "========================================\n"
        );


        printf(
            "Enter your choice: "
        );


        scanf(
            "%d",
            &choice
        );


        /* Switch */

        switch (choice)
        {
            case 1:

                customerCount =
                    createCustomer(
                        customers,
                        customerCount
                    );


                saveCustomers(
                    customers,
                    customerCount
                );


                break;


            case 2:

                if (
                    depositMoney(
                        customers,
                        customerCount
                    )
                )
                {
                    saveCustomers(
                        customers,
                        customerCount
                    );
                }


                break;


            case 3:

                if (
                    withdrawMoney(
                        customers,
                        customerCount
                    )
                )
                {
                    saveCustomers(
                        customers,
                        customerCount
                    );
                }


                break;


            case 4:

                if (
                    transferMoney(
                        customers,
                        customerCount
                    )
                )
                {
                    saveCustomers(
                        customers,
                        customerCount
                    );
                }


                break;


            case 5:

                showCustomer(
                    customers,
                    customerCount
                );


                break;


            case 6:

                printf(
                    "\nEnter Customer ID: "
                );


                scanf(
                    "%d",
                    &id
                );


                showStatement(
                    id
                );


                break;


            case 7:

                showTransactionHistory();


                break;


            case 8:

                resetDailyWithdrawals(
                    customers,
                    customerCount
                );


                saveCustomers(
                    customers,
                    customerCount
                );


                printf(
                    "\nDaily withdrawal reset successful.\n"
                );


                break;


            case 9:

                saveCustomers(
                    customers,
                    customerCount
                );


                printf(
                    "\nData saved successfully.\n"
                );


                printf(
                    "Thank you for using Cash Desk System!\n"
                );


                break;


            default:

                printf(
                    "\nInvalid choice!\n"
                );
        }


    }
    while (
        choice != 9
    );


    return 0;
}
