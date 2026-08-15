/*
utils.c (shared input helpers, id generators, misc)
Owner: Arpita Malakar
Id:2026-2-60-553
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include "../include/types.h"
#include "../include/account.h"
#include "../include/system.h"

// reads an int and keeps asking until it's actually in range
int get_int(const char *prompt, int min, int max) {
    int val;
    char buf[64];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        if (sscanf(buf, "%d", &val) == 1 && val >= min && val <= max) return val;
        printf("  Please enter a number between %d and %d.\n", min, max);
    }
}

double get_double(const char *prompt, double min, double max) {
    double val;
    char buf[64];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        if (sscanf(buf, "%lf", &val) == 1 && val >= min && val <= max) return val;
        printf("  Enter a value between %.2f and %.2f.\n", min, max);
    }
}

int get_string(const char *prompt, char *buf, int maxlen) {
    printf("%s", prompt);
    if (fgets(buf, maxlen, stdin))
        buf[strcspn(buf, "\n")] = '\0';  // fgets keeps the newline, chop it off
    return 0;
}

// simple PIN input, just a normal visible text field, kept plain on purpose
int get_pin(const char *prompt, char *buf) {
    char line[32];
    printf("%s", prompt);
    if (!fgets(line, sizeof(line), stdin)) { buf[0] = '\0'; return 0; }
    line[strcspn(line, "\n")] = '\0';
    strncpy(buf, line, MAX_PIN_LEN - 1);
    buf[MAX_PIN_LEN - 1] = '\0';
    return 0;
}

// same idea, just a longer field for the admin password
int get_masked(const char *prompt, char *buf, int maxlen) {
    char line[64];
    printf("%s", prompt);
    if (!fgets(line, sizeof(line), stdin)) { buf[0] = '\0'; return 0; }
    line[strcspn(line, "\n")] = '\0';
    strncpy(buf, line, maxlen - 1);
    buf[maxlen - 1] = '\0';
    return 0;
}

int trim(char *s) {
    char *end;
    while (isspace((unsigned char)*s)) s++;
    if (*s == '\0') return 0;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return 0;
}

int is_digits_only(const char *s) {
    if (!s || *s == '\0') return 0;
    while (*s) { if (!isdigit((unsigned char)*s)) return 0; s++; }
    return 1;
}

int get_current_datetime(char *buf) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, MAX_DATE_LEN, "%Y-%m-%d %H:%M:%S", tm_info);
    return 0;
}

// looks at every existing account and returns max id + 1
int generate_account_id() {
    Account arr[MAX_ACCOUNTS];
    int count = 0, i, max_id = STARTING_ACCOUNT_ID - 1;
    load_accounts(arr, &count);
    for (i = 0; i < count; i++) if (arr[i].id > max_id) max_id = arr[i].id;
    return max_id + 1;
}

int generate_txn_id() {
    Transaction arr[MAX_TRANSACTIONS];
    int count = 0, i, max_id = 0;
    load_transactions(arr, &count);
    for (i = 0; i < count; i++) if (arr[i].id > max_id) max_id = arr[i].id;
    return max_id + 1;
}

int generate_loan_id() {
    Loan arr[MAX_LOANS];
    int count = 0, i, max_id = 0;
    load_loans(arr, &count);
    for (i = 0; i < count; i++) if (arr[i].id > max_id) max_id = arr[i].id;
    return max_id + 1;
}

int clear_screen() {
    printf("\n\n");   // simple: just add some blank space instead of a real clear
    return 0;
}

int press_enter() {
    int c;
    printf("\n  Press ENTER to continue...");
    while ((c = getchar()) != '\n' && c != EOF);
    return 0;
}

int make_data_dir() {
    mkdir(DATA_DIR, 0755);
    return 0;
}

