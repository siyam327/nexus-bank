#include <stdio.h>
#include <string.h>

#define PIN_LEN 4
int verify_pin(const char *stored_pin,const char *entered_pin)
{
    if(strcmp(stored_pin, entered_pin)==0){
     return 1;
    }
    return 0;
}

int login_user()
{
    char correct_pin[] = "1234";
    char entered_pin[20];

    printf("Enter your pin:");
    scanf("%s", entered_pin);

    if(verify_pin(correct_pin,entered_pin))
    {
        printf("login successful:\n");
    return 1;
    }
    else
    {
        printf("login failed:\n");
    return 0;
    }
}
int login_admin()
{
    char admin_password[] = "mypassword";
    char password[50];

    printf("enter admin password:");
    scanf("%s",password);

    if(strcmp(password,admin_password)!=0)
    {
        printf("incorrect admin passsword \n");
        return 0;
    }

    printf("admin login successful \n");
    return 1;

}

int logout_user()
{
    printf("youe have logout:\n");
    return 1;
}

int change_pin()
{
    char old_pin[20];
    char new_pin[20];
    char confirm_pin[20];

    printf("enter your current pin:");
    scanf("%s",old_pin);

    if(strcmp(old_pin,new_pin) !=0)
    {
        printf("wrong pin \n");
        return 0;
    }
    printf("enter new %d-digit pin",new_pin);
    scanf("%s",new_pin);

    if(strlen(new_pin)!=new_pin)
    {
        printf("pin must be %d-digits \n",new_pin);
        return 0;
    }
    printf("confirm new pin:");
    scanf("%s",confirm_pin);

    if(strcmp(new_pin,confirm_pin) !=0)
    {
        printf("pins do not match \n");
        return 0;
    }
    strcpy(old_pin,new_pin);
    printf("pin changed successfully \n");
    return 1;
}
