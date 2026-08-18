/*
utils.c (shared input helpers, id generators, misc)
Owner: Arpita Malakar
Id:2026-2-60-553
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include "../include/types.h"
#include "../include/account.h"
#include "../include/system.h"

// 1. Read an integer in range
int get_int(const char *prompt, int min, int max) {
    int val;
    char buf[64];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) && sscanf(buf, "%d", &val) == 1) {
            if (val >= min && val <= max) return val;
        }
        printf("Enter a number between %d and %d.\n", min, max);
    }
}

// 2. Read a decimal in range
double get_double(const char *prompt, double min, double max) {
    double val;
    char buf[64];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) && sscanf(buf, "%lf", &val) == 1) {
            if (val >= min && val <= max) return val;
        }
        printf("Enter an amount between %.2f and %.2f.\n", min, max);
    }
}

// 3. Read text input safely
int get_string(const char *prompt, char *buffer, int maxlen) {
    printf("%s", prompt);
    if (fgets(buffer, maxlen, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline
        return 1;
    }
    buffer[0] = '\0';
    return 0;
}

// 4. Input wrappers for PIN and Password
int get_pin(const char *prompt, char *buf) {
    return get_string(prompt, buf, MAX_PIN_LEN);
}

int get_masked(const char *prompt, char *buf, int maxlen) {
    return get_string(prompt, buf, maxlen);
}

// 5. Remove leading and trailing spaces
int trim(char *str) {
    int start = 0, end = strlen(str) - 1;

    while (isspace((unsigned char)str[start])) start++;
    while (end >= start && isspace((unsigned char)str[end])) end--;

    int i;
    for (i = start; i <= end; i++) {
        str[i - start] = str[i];
    }
    str[i - start] = '\0';
    return strlen(str);
}

// 6. Check if string is only numbers
int is_digits_only(const char *str) {
    if (!str || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) return 0;
    }
    return 1;
}

// 7. Get formatted date and time
int get_current_datetime(char *buffer) {
    time_t now = time(NULL);
    struct tm *info = localtime(&now);
    if (!info) return 0;
    strftime(buffer, MAX_DATE_LEN, "%Y-%m-%d %H:%M:%S", info);
    return 1;
}

// 8. Generate Auto-Increment IDs
int generate_account_id() {
    Account arr[MAX_ACCOUNTS];
    int count = 0, max_id = STARTING_ACCOUNT_ID - 1;
    load_accounts(arr, &count);
    for (int i = 0; i < count; i++) {
        if (arr[i].id > max_id) max_id = arr[i].id;
    }
    return max_id + 1;
}

int generate_txn_id() {
    Transaction arr[MAX_TRANSACTIONS];
    int count = 0, max_id = 0;
    load_transactions(arr, &count);
    for (int i = 0; i < count; i++) {
        if (arr[i].id > max_id) max_id = arr[i].id;
    }
    return max_id + 1;
}

int generate_loan_id() {
    Loan arr[MAX_LOANS];
    int count = 0, max_id = 0;
    load_loans(arr, &count);
    for (int i = 0; i < count; i++) {
        if (arr[i].id > max_id) max_id = arr[i].id;
    }
    return max_id + 1;
}

// 9. System Screen Utilities
int clear_screen() {
    printf("\n\n");
    return 1;
}

int press_enter() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
    return 1;
}

int make_data_dir() {
    mkdir(DATA_DIR, 0755);
    return 0;
}
