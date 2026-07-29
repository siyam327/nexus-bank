# 🏦 NEXUS BANK — Advanced Core Bank Management System

> **CSE103 Structured Programming Project**  
> 👥 **Group Project** · 5 Members Team · 💻 **Cross-Platform Compatibility** (macOS, Linux, Windows)

---

## 📊 Architectural Feature Blueprint

| Module | Feature Set | Technical Details |
| :--- | :--- | :--- |
| 👤 **User Accounts** | Account Lifecycle | Dynamic CRUD engine to create, view, or securely terminate Savings/Checking accounts. |
| 🔒 **Authentication** | High-Security Access | Secure session login requiring a 6-digit PIN with console input masking. |
| 💵 **Cash Desk** | Deposits & Withdrawals | Algorithmic ledger balancing paired with automated transaction receipt printing. |
| 💸 **Fund Transfers** | Peer-to-Peer Routing | Double-leg transaction entry ensuring absolute financial tracking between any two accounts. |
| 📝 **Ledger Logs** | Transaction History | Tracks and prints the final 30 entries instantly utilizing a reverse chronological order layout. |
| 📈 **Credit Desk** | Loan Processing Engine | Interactive loan processing with dynamic monthly EMI calculators and debt payoff tools. |
| 🛡️ **Admin Suite** | System Override Panel | Password-protected root environment to view, freeze, reset, or override client database indexes. |
| 💾 **Data Layer** | Flat-File Persistence | Automates system states out to standard readable `.txt` flat files locally inside a `data/` map. |

---

## ⚙️ Compilation & Local Deployment

### 🍏 Mac & 🐧 Linux Environment
```bash
# Enter project workspace directory
cd c-bank-system

# Compile codebase via native compiler rule
make

# Boot up program binary
./bank
```

### 🪟 Windows Native (MinGW Suite)
```bash
# Enter project workspace directory
cd c-bank-system

# Compile code with terminal make variant
mingw32-make

# Boot up local executable
bank.exe
```

> **📋 Prerequisites:** Native installation of `gcc` required. On native Windows, utilize [MinGW-w64](https://mingw-w64.org) distributions or route execution via a Windows Subsystem for Linux (WSL) instance.

---

## 🛠️ Project Directory Tree

```text
c-bank-system/
├── include/             ← Combined Header Library Filepath (.h)
│   ├── config.h         ← Global constants, path mappings, operational ceilings
│   ├── types.h          ← Global data model structures (Accounts, Ledgers, Sessions)
│   ├── color.h          ← Cross-platform ANSI terminal coloring macros
│   ├── utils.h          ← Automated entry parsing handles & generation tooling
│   ├── display.h        ← UI layout formatting tables and visual statement generation
│   ├── file_io.h        ← System input/output disk persistence streams
│   ├── auth.h           ← Login sequence execution wrappers & state handles
│   ├── account.h        ← Core operational client accounting procedures
│   ├── transaction.h    ← Balance manipulation handlers and history tracking
│   ├── loan.h           ← Multi-period compound financing calculators
│   ├── interest.h       ← Local automated checking/savings rate evaluations
│   └── admin.h          ← Root management control loops
│
├── src/                 ← Pure Source Code Filepath (.c)
│   ├── main.c           ← Application gateway, main menu orchestration loops
│   ├── utils.c          ← Input buffers, automated date tracking tools
│   ├── display.c        ← Terminal banners, user tables, aesthetic visual separation 
│   ├── file_io.c        ← Disk file indexing pipelines
│   ├── auth.c           ← Entry validation parsing arrays
│   ├── account.c        ← Direct profile manipulation steps
│   ├── transaction.c    ← Core operational debit and credit functions
│   ├── loan.c           ← Computational loan formulas
│   ├── interest.c       ← Monthly balance compounding applications
│   └── admin.c          ← Root interface utilities
│
├── data/                ← Flat File Database (Built on first runtime)
│   ├── accounts.txt     ← Delimiter-separated active registry log
│   ├── transactions.txt ← System audit trails
│   └── loans.txt        ← Active balance financing files
│
├── Makefile             ← Global dynamic GNU compiler ruleset file
└── README.md            ← Project overview page
```

---

## 👥 Engineering Team & Work Allocation

| Full Name | Student ID | Core File Map | Feature Responsibility Domain |
| :--- | :--- | :--- | :--- |
| **Ariful Islam Siyam** | `2026-2-60-019` | `main.c`, `admin.c`, `display.c`, `file_io.c` | **Project Lead:** Menu flow loop orchestration, admin overrides, flat-file persistence, and UI layout. |
| **Teammate 2 Name** | `12345679` | `auth.c`, `utils.c` | **Auth Module:** Session login loop, 6-digit PIN tracking, console input masking, and global formatting utilities. |
| **Teammate 3 Name** | `12345680` | `account.c` | **Account Registry:** Account profile lifecycle hooks, full directory CRUD operations, and core profile setups. |
| **Teammate 4 Name** | `12345681` | `transaction.c` | **Cash Desk System:** Deposits, withdrawals, peer-to-peer balance transfer rules, and transaction history tracking. |
| **Hasan Ali** | `2026-2-60-026` | `loan.c`, `interest.c` | **Credit Desk System:** Multi-period compound financing models, loan EMI math, and automated monthly balance updates. |


## 💿 Internal Flat-File Data Formats

Data fields are segmented sequentially utilizing standard string pipes (`|`) inside the `data/` repository pathing:

### `data/accounts.txt`
```text
id|name|pin|type|balance|interest_rate|is_active|created_at
10001|Alice Smith|123456|0|5000.00|0.0350|1|2024-01-15 10:30:00
```

### `data/transactions.txt`
```text
id|account_id|type|amount|related_id|balance_after|date|note
1|10001|0|500.00|0|5500.00|2024-01-15 11:00:00|Cash deposit
```

### `data/loans.txt`
```text
id|account_id|principal|interest_rate|term_months|monthly_payment|remaining_balance|months_paid|status|start_date
1|10001|10000.00|0.0800|12|869.88|9130.12|1|0|2024-01-15 12:00:00
```

> **📌 Global Enumeration Maps:**
> * **Transaction Variants:** `0` Deposit | `1` Withdrawal | `2` Transfer-Out | `3` Transfer-In | `4` Interest-Credit | `5` Loan-Credit | `6` Loan-Repay
> * **Account Variants:** `0` Savings | `1` Checking  
> * **Loan Statuses:** `0` Active Account | `1` Term Closed | `2` Delinquent Account

---

## 🌐 Agile Group Git Workflow

When introducing new functionalities or merging localized modules, execute your version tracking workflow strictly through localized environment branches:

```bash
# 👑 Project Lead Initialization Steps
git init
git add .
git commit -m "Setup: Initialize base architecture file trees"
git remote add origin https://github.com/siyam327/nexus-bank.git
git push -u origin main

# 👥 Core Teammate Local Configuration Setup
git clone https://github.com/siyam327/nexus-bank.git
git checkout -b feature/your-assigned-module

# 📦 Module Integration Step
git add src/your-file.c
git commit -m "Feature: Integrate completed system modules"
git push origin feature/your-assigned-module
```

---

## 🔒 Master System Configurations

Modify key application variables securely via `include/config.h`:

* **Administrative Login Key:** Hardcoded defaults map to root values (`ADMIN@123`).
* **Interest Baseline Settings:** Adjustable constants controlling checking (`CHECKING_RATE`) and savings (`SAVINGS_RATE`) profiles.
* **Credit Constraints:** Global financial parameters restricting total lending allocations (`MAX_LOAN_AMOUNT`).
