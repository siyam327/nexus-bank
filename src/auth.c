#include <stdio.h>
#include <string.h>

#define PIN_LEN 4
int verify_pin(const char *stored_pin,const char *entered_pin){
    if(strcmp(stored_pin, entered_pin)==0){
     return 1;
    }
    return 0;
}

int login_user(){
    char entered_pin[20];
    printf("Enter your pin:");
    scanf("%s", entered_pin);
    if(verify_pin(correct_pin,entered_pin)){
    printf("login successful:\n");
    return 1;
    }else {
    printf("login failed:\n");
    return 0;
    }
}

int logout_user(){
    printf("youe have logout:\n")
    return 1;
}
