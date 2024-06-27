#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define struct for Aadhar information
typedef struct aadhar_tag {
    char full_name[100];
    char address[1000];
    long long int aadhar_no;

    int pan_count;
    bool multiple_banks; // Under different PAN
    bool has_lpg;
    bool is_inconsistent;
    double account_balance;
    struct aadhar_tag* next;
} Aadhar_Node;

// Define struct for PAN information
typedef struct pan_tag {
    char full_name[100];
    char address[1000];
    char PAN_number[10];
    int bank_count;
    Aadhar_Node* aadhar_info;
    struct pan_tag* next;
} PAN_Node;

// Define struct for bank account information
typedef struct bank_account_tag {
    char full_name[100];
    char bank_name[100];
    int account_number; // 8 digits
    double deposit;
    PAN_Node* PAN_info;
    struct bank_account_tag* next;
} BankAcc_Node;

// Define struct for LPG subsidy information
typedef struct lpg_tag {
    char full_name[100];
    BankAcc_Node* bank_account;
    struct lpg_tag* next;
} LPG_Node;

// Function to insert Aadhar information into linked list
Aadhar_Node* aadhar_insert(char* name, char* address, long long int aadhar_no, Aadhar_Node* ptr) {
    Aadhar_Node* new_node = (Aadhar_Node*)malloc(sizeof(Aadhar_Node));

    // Copy data to new node
    strcpy(new_node->full_name, name);
    strcpy(new_node->address, address);
    new_node->pan_count = 0;
    new_node->multiple_banks = false;
    new_node->has_lpg = false;
    new_node->account_balance = 0;
    new_node->aadhar_no = aadhar_no;
    new_node->is_inconsistent = false;

    // Link new node to the list
    new_node->next = ptr;
    return new_node;
}

// Function to search Aadhar information in linked list
Aadhar_Node* search_aadhar(long long int aadhar, Aadhar_Node* ptr) {
    while (ptr != NULL) {
        if (ptr->aadhar_no == aadhar)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

// Function to insert PAN information into linked list
PAN_Node* PAN_insert(PAN_Node* ptr, char* name, char* address, char* PAN_no, long long int aadhar, Aadhar_Node* a_ptr) {
    PAN_Node* new_node = (PAN_Node*)malloc(sizeof(PAN_Node));
    new_node->aadhar_info = search_aadhar(aadhar, a_ptr);

    // Error handling if Aadhar information not found
    if (new_node->aadhar_info == NULL) {
        printf("Error");
        return NULL;
    }

    new_node->bank_count = 0;
    strcpy(new_node->full_name, name);
    strcpy(new_node->address, address);
    strcpy(new_node->PAN_number, PAN_no);
    (new_node->aadhar_info)->pan_count++;
    
    // Check for inconsistent data
    if (strcmp(name, new_node->aadhar_info->full_name) != 0) 
        new_node->aadhar_info->is_inconsistent = true;

    // Link new node to the list
    new_node->next = ptr;
    return new_node;
}

// Function to search PAN information in linked list
PAN_Node* search_PAN(PAN_Node* ptr, char* PAN) {
    while (ptr != NULL) {
        if (strcmp(ptr->PAN_number, PAN) == 0)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

// Function to insert bank account information into linked list
BankAcc_Node* BankAcc_insert(BankAcc_Node* ptr, char* name, char* PAN, PAN_Node* PAN_ptr, char* bank, int account_no, double deposit) {
    BankAcc_Node* new_node = (BankAcc_Node*)malloc(sizeof(BankAcc_Node));

    // Copy data to new node
    strcpy(new_node->full_name, name);
    new_node->PAN_info = search_PAN(PAN_ptr, PAN);

    // Error handling if PAN information not found
    if (new_node->PAN_info == NULL) {
        printf("Error");
        return NULL;
    }

    // Update counts and balances
    new_node->PAN_info->bank_count++;
    new_node->PAN_info->aadhar_info->multiple_banks++;
    new_node->PAN_info->aadhar_info->account_balance += deposit;

    // Check for multiple banks
    if (new_node->PAN_info->bank_count != new_node->PAN_info->aadhar_info->multiple_banks)
        new_node->PAN_info->aadhar_info->multiple_banks = true;

    // Copy bank information
    strcpy(new_node->bank_name, bank);
    new_node->account_number = account_no;
    new_node->deposit = deposit; 

    // Check for inconsistent data
    if (strcmp(name, new_node->PAN_info->full_name) != 0) 
        new_node->PAN_info->aadhar_info->is_inconsistent = true;

    // Link new node to the list
    new_node->next = ptr;
    return new_node;
}

// Function to search bank account information in linked list
BankAcc_Node* search_bank(BankAcc_Node* ptr, int acc) {
    while (ptr != NULL) {
        if (ptr->account_number == acc)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

// Function to insert LPG subsidy information into linked list
LPG_Node* LPG_insert(LPG_Node* ptr, char* name, int acc, BankAcc_Node* bank_ptr) {
    LPG_Node* new_node = (LPG_Node*)malloc(sizeof(LPG_Node));

    // Copy data to new node
    strcpy(new_node->full_name, name);
    new_node->bank_account = search_bank(bank_ptr, acc);

    // Error handling if bank account information not found
    if (new_node->bank_account == NULL){
        printf("Errorbank");
        return NULL;
    }

    // Update LPG status
    new_node->bank_account->PAN_info->aadhar_info->has_lpg = true;

    // Check for inconsistent data
    if (strcmp(name, new_node->bank_account->full_name) != 0) 
        new_node->bank_account->PAN_info->aadhar_info->is_inconsistent = true;

    // Link new node to the list
    new_node->next = ptr;
    return new_node;
}

// Function to read Aadhar data from CSV file
Aadhar_Node * read_aadhar() {
    FILE *file = fopen("aadhar_list.csv", "r");
    if (!file) {
        printf("Error opening file aadhar\n");
        return NULL;
    }

    Aadhar_Node * aadhar_list = NULL;
    char line[100];
    char *token;
    char *name, *address, *aadhar_number;
    long long int aadhar_num;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, ",");
        name = token;
        token = strtok(NULL, ",");
        address = token;
        token = strtok(NULL, ",");
        aadhar_number = token;

        // Remove newline character from Aadhar number
        if (aadhar_number != NULL) {
            aadhar_number[strcspn(aadhar_number, "\n")] = 0;
        }

        // Convert Aadhar number string to long long int
        aadhar_num = atoll(aadhar_number);

        // Insert Aadhar information into linked list
        aadhar_list = aadhar_insert(name, address, aadhar_num, aadhar_list);
    }

    fclose(file);

    return aadhar_list;
}

// Function to read PAN data from CSV file
PAN_Node * read_PAN(Aadhar_Node * aadhar_list) {
    FILE *file = fopen("pan_list.csv", "r");
    if (!file) {
        printf("Error opening file pan\n");
        return NULL;
    }

    PAN_Node * pan_list = NULL;
    char line[100];
    char *token;
    char *name, *address, *aadhar_number, *pan_number;
    long long int aadhar_num;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, ",");
        name = token;
        token = strtok(NULL, ",");
        address = token;
        token = strtok(NULL, ",");
        pan_number = token;
        token = strtok(NULL, ",");
        aadhar_number = token;

        // Remove newline character from Aadhar number
        if (aadhar_number != NULL) {
            aadhar_number[strcspn(aadhar_number, "\n")] = 0;
        }

        // Convert Aadhar number string to long long int
        aadhar_num = atoll(aadhar_number);

        // Insert PAN information into linked list
        pan_list = PAN_insert(pan_list, name, address, pan_number, aadhar_num, aadhar_list);
    }

    fclose(file);

    return pan_list;
}

// Function to read bank account data from CSV file
BankAcc_Node * read_bank(char * filename, PAN_Node * pan_list) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file bank\n");
        return NULL;
    }

    BankAcc_Node * bank_list = NULL;
    char line[100];
    char *token;
    char *name, *pan_number, *bank_name, *account_number_str, *deposit_str;
    int account_number;
    double deposit;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, ",");
        name = token;
        token = strtok(NULL, ",");
        pan_number = token;
        token = strtok(NULL, ",");
        bank_name = token;
        token = strtok(NULL, ",");
        account_number_str = token;
        token = strtok(NULL, ",");
        deposit_str = token;

        // Remove newline character from deposit amount
        if (deposit_str != NULL) {
            deposit_str[strcspn(deposit_str, "\n")] = 0;
        }

        // Convert account number string to integer
        account_number = atoi(account_number_str);

        // Convert deposit amount string to double
        deposit = atof(deposit_str);

        // Insert bank account information into linked list
        bank_list = BankAcc_insert(bank_list, name, pan_number, pan_list, bank_name, account_number, deposit);
    }

    fclose(file);

    return bank_list;
}

// Function to read LPG subsidy data from CSV file
LPG_Node * read_LPG(BankAcc_Node * bank_list) {
    FILE *file = fopen("lpg_list.csv", "r");
    if (!file) {
        printf("Error opening file LPG\n");
        return NULL;
    }

    LPG_Node * lpg_list = NULL;
    char line[100];
    char *token;
    char *name, *account_number_str, *lpg_subsidy;
    int account_number;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, ",");
        name = token;
        token = strtok(NULL, ",");
        account_number_str = token;
        token = strtok(NULL, ",");
        lpg_subsidy = token;

        // Remove newline character from LPG subsidy
        if (lpg_subsidy != NULL) {
            lpg_subsidy[strcspn(lpg_subsidy, "\n")] = 0;
        }

        // Convert account number string to integer
        account_number = atoi(account_number_str);
        
        // Insert LPG subsidy information into linked list if subsidy is present
        if (strcmp(lpg_subsidy, "YES") == 0) {
            lpg_list = LPG_insert(lpg_list, name, account_number, bank_list);
        }
    }

    fclose(file);

    return lpg_list;
}

// Function to print users with no PAN information
void print_no_pan(Aadhar_Node* ptr) {
    printf("\nUsers with no PAN information:\n\n");
    while (ptr != NULL) {
        if (ptr->pan_count == 0) {
            printf("Name: %s\nAddress: %s\nAadhar Number: %lld\n\n", ptr->full_name, ptr->address, ptr->aadhar_no);
        }
        ptr = ptr->next;
    }
}

// Function to print users with multiple PAN information
void print_multi_pan(Aadhar_Node* ptr, PAN_Node* pan_ptr) {
    printf("\nUsers with multiple PAN information:\n\n");
    while (ptr != NULL) {
        if (ptr->pan_count > 1) {
            printf("Name: %s\nAddress: %s\nAadhar Number: %lld\n", ptr->full_name, ptr->address, ptr->aadhar_no);
            int count = ptr->pan_count;
            PAN_Node* temp = pan_ptr;
            while (count) {
                if (temp->aadhar_info->aadhar_no == ptr->aadhar_no) {
                    printf("PAN %d Number: %s\n", ptr->pan_count - count + 1, temp->PAN_number);
                    count--;
                }
                temp = temp->next;
            }
            printf("\n");
        }
        ptr = ptr->next;
    }
}

// Function to print users with multiple bank accounts
void print_multi_bank(Aadhar_Node* ptr) {
    printf("\nUsers with multiple bank accounts:\n\n");
    while (ptr != NULL) {
        if (ptr->multiple_banks) {
            printf("Name: %s\nAddress: %s\nAadhar Number: %lld\n\n", ptr->full_name, ptr->address, ptr->aadhar_no);
        }
        ptr = ptr->next;
    }   
}

// Function to print users with LPG subsidy
void print_lpg(LPG_Node* ptr) {
    printf("\nUsers with LPG subsidy:\n\n");
    while (ptr != NULL) {
        printf("Name: %s\nAddress: %s\nAadhar Number: %lld\nPAN Number: %s\nBank: %s\nAccount Number: %d\n\n", ptr->full_name, ptr->bank_account->PAN_info->address, ptr->bank_account->PAN_info->aadhar_info->aadhar_no, ptr->bank_account->PAN_info->PAN_number, ptr->bank_account->bank_name, ptr->bank_account->account_number);
        ptr = ptr->next;
    }
}

// Function to print users with LPG subsidy and total balance greater than a certain amount
void print_rich(Aadhar_Node* ptr, double x) {
    printf("\nUsers with LPG subsidy and total balance greater than %.2lf:\n\n", x);
    while (ptr != NULL) {
        if (ptr->account_balance > x && ptr->has_lpg) {
            printf("Name: %s\nAddress: %s\nAadhar Number: %lld\nAccount Balance: %.2lf\n\n", ptr->full_name, ptr->address, ptr->aadhar_no, ptr->account_balance);
        }
        ptr = ptr->next;
    }
}

// Function to print inconsistent data
void print_inconsistent(Aadhar_Node * aadhar_list) {
    Aadhar_Node * aptr = aadhar_list;
    printf("\nInconsistent entries:\n\n");

    while (aptr) {
        if (aptr->is_inconsistent) {
            printf("Name: %s\nAddress: %s\nAadhar Number: %lld\n\n", aptr->full_name, aptr->address, aptr->aadhar_no);
        }
        aptr = aptr -> next;
    }
}

// Function to merge two bank lists
BankAcc_Node* bank_merge(BankAcc_Node* p1, BankAcc_Node* p2) {
    if (p1 == NULL)
        return p2;
    BankAcc_Node* tail = p1;
    while (tail->next != NULL)
        tail = tail->next;
    tail->next = p2;
    return p1;
}

// Main function
int main() {
    Aadhar_Node* aadhar_list = read_aadhar();
    PAN_Node* pan_list = read_PAN(aadhar_list);
    BankAcc_Node* bank_list = read_bank("bank_accounts_list.csv", pan_list);
    LPG_Node* lpg_list = read_LPG(bank_list);

    int input;
    do {
        // Display menu options
        printf("\nWhat to do?\n");
        printf("1. Print users with no PAN\n");
        printf("2. Print users with multiple PAN\n");
        printf("3. Print users with multiple bank accounts\n");
        printf("4. Print users with LPG subsidy\n");
        printf("5. Print users with LPG subsidy and balance greater than X\n");
        printf("6. Print inconsistent data\n");
        printf("7. Merge two bank lists\n");
        printf("8. Exit\n");
        printf("Enter input: ");
        scanf("%d", &input);

        // Perform action based on user input
        switch(input) {
            case 1:
                print_no_pan(aadhar_list);
                break;
            case 2:
                print_multi_pan(aadhar_list, pan_list);
                break;
            case 3:
                print_multi_bank(aadhar_list);
                break;
            case 4:
                print_lpg(lpg_list);
                break;
            case 5: {
                double x;
                printf("Enter X: ");
                scanf("%lf", &x);
                print_rich(aadhar_list, x);
                break;
            }
            case 6:
                print_inconsistent(aadhar_list);
                break;
            case 7:
                // Merge bank lists
                // bank_list = bank_merge(bank_list, bank_list1);
                break;
            case 8:
                // Exit program
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid input\n");
        }
    } while (input != 8);

    return 0;
}
