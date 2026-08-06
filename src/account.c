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
