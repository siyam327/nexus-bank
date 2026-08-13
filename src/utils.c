#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define Line_size 256

int get_int(const char *prompt, int min, int max)
{
    char line[Line_size];
    int value;
    int ok;

    while(1)
    {
        printf("%s", prompt);
        fgets(line, Line_size, stdin);
        sscanf(line, "%d", &value);

        if (ok != 1)
        {
            printf("Invalid input. Please enter a whole number \n");
        }
        if (value < min || value > max)
        {
            printf("Please enter a number between %d and %d \n", min, max);
        }
            return value;
    }
}
double get_double(const char *prompt, double min, double max)
{
    char line[Line_size];
    double value;
    int ok;

    while(1)
    {
        printf("%s", prompt);
        fgets(line, Line_size, stdin);
        sscanf(line, "%lf", &value);

        if (ok != 1)
        {
            printf("Invalid input. Please enter a decimal number \n");
        }
        if (value < min || value > max)
        {
            printf("Please enter an amount between %2f and %2f \n", min, max);
        }
        return value;
    }
}
int get_string(const char *prompt, char *buffer, int maxlen)
{
    printf("%s", prompt);

    if (fgets(buffer, maxlen, stdin) == NULL)
    {
        buffer[0] = '\0';
        return 0;
    }
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
    return 1;
}
int trim(char *str)
{
    int len = strlen(str);
    int start = 0;

    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n')
    {
        start++;
    }
    int end = len - 1;
    while (end >= start &&
          (str[end] == ' ' || str[end] == '\t' || str[end] == '\n'))
          {
              end--;
          }
    char temp[Line_size];
    int j = 0;
    for (int i = start; i <= end; i++)
    {
        temp[j] = str[i];
        j++;
    }
    temp[j] = '\0';
    strcpy(str, temp);
    return j;
}
int is_digits_only(const char *str)
{
    if (str[0] == '\0')
    {
        return 0;
    }
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}
int get_current_datetime(char *buffer, int size)
{
    time_t now = time(NULL);
    struct tm *info = localtime(&now);

    if (info == NULL)
    {
        return 0;
    }
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", info);
    return 1;
}
int clear_screen(){
    printf("\033[H\033[2J");
    return 1;
}
int press_enter()
{
    printf("\nPress Enter to continue ");
    int ch;
    do {
        ch = getchar();
    }while (ch != '\n' && ch != EOF);
    return 1;
}
int main()
{
    char name[Line_size];
    char datetime[Line_size];

    int age = get_int("Enter your age: ", 1, 120);
    printf("Your age is: %d\n", age);

    double amount = get_double("Enter an amount: ", 0, 100000);
    printf("Your amount is: %.2f\n", amount);

    get_string("Enter your name: ", name, Line_size);
    trim(name);
    printf("Hello, %s!\n", name);

    if (is_digits_only(name))
    {
        printf("That name is only digits.\n");
    }else{
        printf("That name is NOT only digits.\n");
    }
    get_current_datetime(datetime, Line_size);
    printf("Current date/time: %s\n", datetime);

    printf("Screen cleared. Program finished.\n");
    return 0;
}


