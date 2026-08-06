# Nexus Bank

CSE103 Structured Programming group project. A console bank management system written in C — accounts, deposits/withdrawals, transfers, loans, interest, and an admin panel. No database, just plain text files.

5-person team project.

---

## Features

- Open a Savings or Checking account, log in with a 6-digit PIN
- Deposit, withdraw, transfer between accounts
- Two customer tiers (Normal / Premium) with different balance caps, daily withdrawal limits, and per-transfer limits
- Transaction history (last 30 shown, full history exportable to a text file)
- Loans: apply, calculate monthly payment (standard amortization), make payments, track payoff
- Interest calculation for savings accounts
- Admin panel: list accounts, view account details, reset a locked PIN, activate/deactivate accounts, view all loans

Every function returns an `int` status code (no `void` functions in the project) and file errors are reported using `errno`/`strerror`.

---

## Build and run

### Mac / Linux
```bash
cd c-bank-system
make
./bank
```

### Windows (MinGW)
```bash
cd c-bank-system
mingw32-make
bank.exe
```

Needs `gcc`. On Windows, install [MinGW-w64](https://mingw-w64.org) or just use WSL.

---

## Project structure

```
c-bank-system/
├── include/
│   ├── types.h        shared structs, enums, constants
│   ├── account.h      input helpers, login/PIN, account create/find/view/close, tier limits
│   ├── banking.h      deposit/withdraw/transfer, loans, interest
│   └── system.h       file I/O, all display/print functions, admin panel
│
├── src/
│   ├── main.c          menu loop, ties everything together
│   ├── utils.c         input helpers, id generators
│   ├── auth.c          login, logout, pin management
│   ├── account.c       account create/find/view/close
│   ├── transaction.c   deposit, withdraw, transfer, history
│   ├── loan.c           loan application and repayment
│   ├── interest.c      savings interest
│   ├── file_io.c       reading/writing the data files
│   ├── display.c       print/menu formatting
│   └── admin.c          admin panel
│
├── data/                created automatically on first run
│   ├── accounts.txt
│   ├── transactions.txt
│   └── loans.txt
│
└── Makefile
```

We started with one header per file (10 total) but merged them down to 4, grouped by what they actually do rather than one per person.

---

## Team

| Name | Student ID | Files | Owns |
|---|---|---|---|
| Ariful Islam Siyam (lead) | 2026-2-60-019 | `main.c`, `admin.c`, `display.c`, `file_io.c` | menu loop, admin panel, all display/print code, file storage |
| Arpita Malakar | 2026-2-60-553 | `auth.c`, `utils.c` | login/PIN handling, shared input helpers |
| Md. Mobin Hossain Jitu | 2026-2-60-538 | `account.c` | account create/find/view/close |
| Busrat Jahan | 2026-2-60-032 | `transaction.c` | deposit, withdraw, transfer, history |
| Hasan Ali | 2026-2-60-026 | `loan.c`, `interest.c` | loan math, interest calculation |

---

## Data file formats

Plain pipe-delimited text, one record per line. First line of each file is a `#` comment header.

**data/accounts.txt**
```
id|name|pin|type|tier|balance|interest_rate|is_active|failed_attempts|withdrawn_today|last_withdraw_date|created_at
10001|Alice Smith|123456|0|0|5000.00|0.0350|1|0|0.00|2026-01-15 10:30:00|2026-01-15 10:30:00
```

**data/transactions.txt**
```
id|account_id|type|amount|related_id|balance_after|date|note
1|10001|0|500.00|0|5500.00|2026-01-15 11:00:00|Cash deposit
```

**data/loans.txt**
```
id|account_id|principal|interest_rate|term_months|monthly_payment|remaining_balance|months_paid|status|start_date
1|10001|10000.00|0.0800|12|869.88|9130.12|1|0|2026-01-15 12:00:00
```

**Enum values:**
- Transaction type: `0` deposit, `1` withdraw, `2` transfer out, `3` transfer in, `4` interest, `5` loan credit, `6` loan payment
- Account type: `0` Savings, `1` Checking
- Customer tier: `0` Normal, `1` Premium
- Loan status: `0` Active, `1` Paid, `2` Defaulted

---

## Git workflow

```bash
git clone https://github.com/siyam327/nexus-bank.git
git checkout -b feature/your-module
# ... work ...
git add src/your-file.c
git commit -m "[module] short description"
git push origin feature/your-module
```
Open a PR, don't push straight to main.

---

## Config

Everything adjustable lives in `include/types.h`:
- Admin password (`ADMIN_PASSWORD`)
- Interest rates (`SAVINGS_RATE`, `CHECKING_RATE`)
- Loan limits (`MIN_LOAN_AMOUNT`, `MAX_LOAN_AMOUNT`, term limits)
- Customer tier limits (`NORMAL_MAX_BALANCE`, `PREMIUM_DAILY_WITHDRAW`, etc.)
