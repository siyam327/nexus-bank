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
    char confirm_pin[20];
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
