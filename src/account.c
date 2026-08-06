#include <stdio.h>
#include <string.h>
#include "account.h"

int create_account()
{
    struct account acc;
    FILE *fp;
    int maxID=1000;
    int choice;
    int option;
    char confirm_pin[20]; // Fixed string buffer overflow bug
    int i;

    fp= fopen("account.dat", "rb");
    if(fp==NULL)
    {
        acc.id=1001;
    }
    else
    {
        while(fread(&acc,sizeof(acc),1,fp))
       {
           if(acc.id>maxID)
           {
               maxID=acc.id;
           }
       }
        acc.id=maxID+1;
        fclose(fp);
    }

    printf("Enter account holders name:");
    getchar();
    fgets(acc.name,30,stdin);

    if(strlen(acc.name)<2){
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
        strcpy(acc.type,"Savings");
    }
    else if(choice==2)
    {
        strcpy(acc.type,"Checking");
    }
    else
    {
        printf("Invalid choice");
    }

    printf("User type:\n");
    printf("1.Premium\n");
    printf("2.Normal\n");

    printf("Enter a number:");
    scanf("%d",&option);
    if(option==1)
    {
        strcpy(acc.userType,"Premium");
    }
    else if(option==2)
    {
        strcpy(acc.userType,"Normal");
    }
    else
    {
        printf("Invalid choice");
    }

    printf("Enter a 6 digit pin:");
    scanf("%s",acc.pin);
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
    scanf("%s",confirm_pin);

    if(strcmp(acc.pin,confirm_pin)!=0)
    {
        printf("PIN does not match");
        return 0;
    }

    printf("Enter initial deposit:");
    scanf("%f",&acc.balance);
    if(acc.balance<0)
    {
        printf("Invalid deposit amount\n");
        return 0;
    }
    if(strcmp(acc.userType,"Premium")==0 && acc.balance<10000)
    {
        printf("Premium users must deposit at least ten thousand taka\n ");
        return 0;
    }

    acc.active=1;

    fp=fopen("account.dat","ab");
    if(fp==NULL)
    {
        printf("File error");
        return 0;
    }

    fwrite(&acc,sizeof(acc),1,fp);
    fclose(fp);

    printf("Account created successfully\n");
    printf("Account ID:%d\n",acc.id);
    printf("Name:");
    fputs(acc.name,stdout);
    printf("Account type:%s\n",acc.type);
    printf("User type:%s\n",acc.userType);
    printf("Balance:%.2f\n",acc.balance);

    return 1;
}

int find_account()
{
    struct account acc;
    int searchID;
    FILE *fp;
    int found=0;

    printf("Enter ID:");
    scanf("%d",&searchID);

    fp=fopen("account.dat", "rb");
    if(fp==NULL)
    {
        printf("File not found\n");
        return 0;
    }

    while(fread(&acc,sizeof(struct account),1,fp)==1)
    {
        if(acc.id==searchID && acc.active==1)
        {
            found=1;
            printf("Account found\n");
            break;
        }
    }
    fclose(fp);

    if(found==0)
    {
        printf("Account not found\n");
    }
    return 1;
}

int view_account()
{
    struct account acc;
    int searchID;
    FILE *fp;
    int found=0;

    printf("Enter ID:");
    scanf("%d",&searchID);

    fp=fopen("account.dat", "rb");
    if(fp==NULL)
    {
        printf("File not found\n");
        return 0;
    }

    while(fread(&acc,sizeof(struct account),1,fp)==1)
    {
        if(acc.id==searchID && acc.active==1)
        {
            found=1;
            printf("Account found\n");
            printf("Account ID:%d\n",acc.id);
            printf("Name:");
            fputs(acc.name,stdout);
            printf("Account type:%s\n",acc.type);
            printf("User type:%s\n",acc.userType);
            printf("Balance:%.2f\n",acc.balance);
            break;
        }
    }
    fclose(fp);

    if(found==0)
    {
        printf("Account not found\n");
    }
    return 1;
}

int update_balance(int searchID, float new_balance)
{
    FILE *fp;
    FILE *temp;
    struct account acc;
    int found=0;

    fp=fopen("account.dat","rb");
    if(fp==NULL)
    {
        return 0;
    }
    temp=fopen("temp.dat","wb");
    if(temp==NULL)
    {
        fclose(fp);
        return 0;
    }

    while(fread(&acc,sizeof(struct account),1,fp)==1)
    {
        if(acc.id==searchID && acc.active==1)
        {
            found=1;
            acc.balance=new_balance;
        }
        fwrite(&acc,sizeof(struct account),1,temp);
    }
    fclose(fp);
    fclose(temp);

    if(found==1)
    {
        remove("account.dat");
        rename("temp.dat","account.dat");
        return 1;
    }
    else
    {
        remove("temp.dat");
        return 0;
    }
} // FIXED BUG: Missing closing brace added here. Otherwise close_account was nesting inside.

int close_account() // FIXED TYPO: Added missing function parentheses block ()
{
    struct account acc;
    FILE *fp;
    FILE *temp;
    int searchID;
    int found = 0;

    fp=fopen("account.dat","rb");
    if(fp==NULL)
    {
        printf("File error\n"); // FIXED TYPO: Added missing quotes "" and proper backslash \n
        return 0;
    }

    printf("Enter ID:");
    scanf("%d",&searchID);

    // Phase 1: First scan to validate account constraints before destructive write operations
    while(fread(&acc,sizeof(acc),1,fp))
    {
        if(acc.id==searchID)
        {
            if(acc.active==0)
            {
                printf("This account is already closed\n");
                fclose(fp);
                return 0;
            }
            if(acc.balance!=0)
            {
                printf("Account cannot be closed\n");
                printf("Balance must be 0 before closing an account\n");
                fclose(fp);
                return 0;
            }
            found = 1;
        }
    }
    fclose(fp);

    if(!found) {
        printf("Account not found\n");
        return 0;
    }

    // Phase 2: ADDED LOGIC Database Persistence. Rewriting binary records to explicitly commit acc.active = 0
    fp = fopen("account.dat", "rb");
    temp = fopen("temp.dat", "wb");
    if(fp == NULL || temp == NULL) {
        if(fp) fclose(fp);
        if(temp) fclose(temp);
        return 0;
    }
    
    while(fread(&acc, sizeof(struct account), 1, fp) == 1)
    {
        if(acc.id == searchID)
        {
            acc.active = 0; // The missing file handling operation that updates state permanently
        }
        fwrite(&acc, sizeof(struct account), 1, temp);
    }
    fclose(fp);
    fclose(temp);

    remove("account.dat");
    rename("temp.dat", "account.dat");

    printf("Account closed successfully!\n");
    return 1;
}
