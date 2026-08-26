/*
 account.c (account management)
 Owner: Jeetu
 Contains: create/find/view/close account, update balance.
 */
#include <stdio.h>
#include <string.h>
#include "account.h"
#include "system.h"

int create_account()
{
    Account acc;
    Account accounts[MAX_ACCOUNTS];
    int maxID=STARTING_ACCOUNT_ID-1;
    int choice;
    int option;
    char confirm_pin[MAX_PIN_LEN];
    int i;
    int count=0;

    if(load_accounts(accounts, &count)==0)
    {
        printf("File error\n");
        return 0;
    }

    for(i=0; i<count; i++)
    {
        if(accounts[i].id>maxID)
        {
            maxID=accounts[i].id;
        }
    }
    acc.id=maxID+1;
     printf("Enter account holders name:");
    fgets(acc.name,MAX_NAME_LEN,stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0';

     if(strlen(acc.name)<2)
     {
         printf("Invalid name");
        return 0;
     }

    printf("Account type:\n");
    printf("1.Savings\n");
    printf("2.Checking\n");
    printf("Enter a number:");
    scanf("%d",&choice);

    if(choice==1)
    {
        acc.type=SAVINGS;
    }
    else if(choice==2)
    {
        acc.type=CHECKING;
    }
    else
    {
        printf("Invalid choice");
        return 0;
    }
    if(acc.type==SAVINGS)
    {
        acc.interest_rate=SAVINGS_RATE;
    }
    else
    {
        acc.interest_rate=CHECKING_RATE;
    }
    printf("User type:\n");
    printf("1.Premium\n");
    printf("2.Normal\n");

    printf("Enter a number:");
    scanf("%d",&option);
    if(option==1)
    {
        acc.tier=PREMIUM;
    }
    else if(option==2)
    {
        acc.tier=NORMAL;
    }
    else
    {
        printf("Invalid choice");
        return 0;
    }

     printf("Enter a 6 digit pin:");
     scanf("%6s",acc.pin);

     if(strlen(acc.pin)!=6)
    {
        printf("PIN must be in 6 digits\n");
        return 0;
    }

    for(i=0;i<6;i++)
    {
        if(acc.pin[i]<'0'||acc.pin[i]>'9')
        {
            printf("PIN must contain only numbers\n");
            return 0;
        }
    }
       printf("Confirm PIN:");
       scanf("%6s",confirm_pin);

       if(strcmp(acc.pin,confirm_pin)!=0)
       {
          printf("PIN does not match");
          return 0;
       }
        printf("Enter initial deposit:");
        scanf("%lf",&acc.balance);

        if(acc.balance<0)
       {
          printf("Invalid deposit amount\n");
          return 0;
       }

        if(acc.tier==PREMIUM && acc.balance<10000)
       {
          printf("Premium users must deposit at least ten thousand taka\n");
          return 0;
       }

       acc.is_active=1;
       acc.failed_attempts=0;
       acc.withdrawn_today=0.0;
       get_current_datetime(acc.created_at);
       strncpy(acc.last_withdraw_date, acc.created_at, MAX_DATE_LEN - 1);
       acc.last_withdraw_date[MAX_DATE_LEN - 1] = '\0';

       if(count>=MAX_ACCOUNTS)
       {
          printf("Account limit reached\n");
          return 0;
       }
        accounts[count]=acc;
        count++;

        if(save_accounts(accounts,count)==0)
        {
          printf("File error\n");
          return 0;
        }

         printf("Account created successfully\n");
         printf("Account ID:%d\n",acc.id);
         printf("Name:");
         fputs(acc.name,stdout);

         if(acc.type==SAVINGS)
         printf("Account type:Savings\n");
         else
         printf("Account type:Checking\n");

         if(acc.tier==PREMIUM)
         printf("User type:Premium\n");
         else
         printf("User type: Normal\n");

         printf("Balance:%.2f\n",acc.balance);

         return 1;
}








    int find_account(int searchID, Account *out)
    {
        Account acc;
        Account accounts[MAX_ACCOUNTS];
        int count=0;
        int found=0;

        if(load_accounts(accounts, &count)==0)
        {
            printf("File not found\n");
            return 0;
        }

        for(int i=0; i<count; i++)
        {
            if(accounts[i].id==searchID && accounts[i].is_active==1)
        {
            acc=accounts[i];
            *out=acc;
            found=1;
            printf("Account found\n");
            break;
        }
        }
        if(found==0)
        {
            printf("Account not found\n");
        }
            return found;
    }






    int view_my_account(const Session *s)
    {
        Account acc;
        Account accounts[MAX_ACCOUNTS];
        int searchID=s->account.id;
        int count=0;
        int found=0;
/*
        printf("Enter ID:");
        scanf("%d",&searchID);

*/      if(load_accounts(accounts, &count)==0)
    {
        printf("File not found\n");
        return 0;
    }

    for(int i=0; i<count; i++)
    {
        if(accounts[i].id==searchID && accounts[i].is_active==1)
        {
            acc=accounts[i];
            found=1;


                printf("Account found\n");
                printf("Account ID:%d\n",acc.id);
                printf("Name:");
                fputs(acc.name,stdout);
                if(acc.type==SAVINGS)
                {
                    printf("Account type: Savings\n");
                }
                else
                {
                   printf("Account type: Checking\n");
                }
                if(acc.tier==PREMIUM)
                {
                    printf("User type: Premium\n");
                }
                else
                {
                     printf("User type: Normal\n");
                }
                printf("Balance:%.2f\n",acc.balance);

                break;
            }

        }

        if(found==0)
        {
            printf("Account not found\n");
        }
        return 1;

    }

    int update_balance(int searchID, double new_balance)   //bushrats part
    {

        Account acc;
        Account accounts[MAX_ACCOUNTS];
        int found=0;
        int count=0;

         if(load_accounts(accounts, &count)==0)
         {
           return 0;
         }

         for(int i=0; i<count; i++)
         {
             if(accounts[i].id==searchID && accounts[i].is_active==1)
             {
               acc=accounts[i];

               found=1;
               acc.balance=new_balance;

               accounts[i]=acc;

               break;
             }
         }
         if(found==1)
         {
             if(save_accounts(accounts, count)==0)
           {
             return 0;
           }

             return 1;
         }
         else
         {
             return 0;
         }
    }





        int delete_my_account(Session *s)
        {
            Account acc;
            Account accounts[MAX_ACCOUNTS];
            int searchID=s->account.id;
            char confirm[10];
            int count=0;
           // char pin[20];

            if(load_accounts(accounts, &count)==0)
            {
                printf("File error\n");
                return 0;
            }

            for(int i=0; i<count; i++)
            {
                if(accounts[i].id==searchID)
                {
                    acc=accounts[i];

                    if(acc.is_active==0)
                    {
                        printf("This account is already closed\n");
                        return 0;
                    }

                    if(acc.balance!=0)
                    {
                        printf("Account cannot be closed\n");
                        printf("Balance must be 0 before closing an account\n");
                        return 0;
                    }

            /*
            printf("Enter PIN: ");
            scanf("%s", pin);
            */

                    if(strcmp(acc.pin, s->account.pin)!=0)
                    {
                        printf("PIN does not match\n");
                        return 0;
                    }

                        printf("Are you sure you want to close this account?\n");
                        printf("Type YES to confirm: ");
                        scanf("%s", confirm);

                    if(strcmp(confirm, "YES")!=0)
                    {
                        printf("Account closure cancelled.\n");
                        return 0;
                    }

                    acc.is_active=0;
                    accounts[i]=acc;

                    if(save_accounts(accounts, count)==0)
                    {
                        printf("File error\n");
                        return 0;
                    }

                        printf("Account closed successfully.\n");
                        return 1;
                }
            }
                printf("Account not found.\n");
                return 0;
        }


