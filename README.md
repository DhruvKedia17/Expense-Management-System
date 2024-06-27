# Expense Management System

## Overview
This C program serves as an expense management system, allowing users to add, delete, and track expenses among multiple users. It features a user-friendly interface and efficient algorithms for user lookup.

## Features
- **User Management:**
  - Add new users with a unique User ID, name, and initial amount spent.
  - Delete existing users, ensuring all balances are settled before removal.

- **Expense Tracking:**
  - Add expenses with a unique Expense ID, amount, and the user who paid.
  - Specify users sharing the expense, dynamically updating their balances.

- **Balance Calculation:**
  - Calculate and display amounts owed and to be paid by each user.
  - Enable users to settle balances, updating amounts accordingly.

- **Efficient User Lookup:**
  - Utilize a binary search algorithm for quick and efficient user retrieval by User ID.

## Getting Started
1. **Compilation:**
   - Compile the program using a C compiler (e.g., gcc).
   ```bash
   gcc expense_management.c -o expense_management
1. **Execution:**
   - Run the compiled executable.
    ```bash
    ./expense_management
3. **Usage:**
    - Follow the on-screen prompts to perform various operations (add users, add expenses, check balances, settle amounts, etc.).

For testing, you may can use the data in input.txt by simply pasting its contents in your terminal after running the program.

## Important Notes
Ensure that the C compiler is installed on your system.
The program uses a binary search algorithm, so user IDs in the database should be sorted for efficient searching.
Validate user inputs, especially when adding expenses and users, to avoid unexpected behavior.

Contributions to the Expense Management System project are welcome! Whether it's bug fixes, feature enhancements, or documentation improvements, feel free to contribute to make this project even better.
