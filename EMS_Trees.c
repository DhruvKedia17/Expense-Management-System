#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct aadhar {
    char name[100];
    char address[1000];
    long long int aadhar_no;
} aadhar;

typedef struct pan {
    char name[100];
    char address[1000];
    char PAN_no[10];
    aadhar* aadhar_info; 
} pan;

typedef struct bank{
    char name[100];
    pan* PAN_info; 
    char bankName[100];
    int account_no; // 8 digits
    double deposit;
} bank;

typedef struct lpg {
    char name[100];
    bank* bankacc;
    bool availedLPG;
} lpg;

typedef union person {
    aadhar* aadharPtr;
    pan* panPtr;
    bank* bankPtr;
    lpg* lpgPtr;
} person;

typedef struct TreeNode
{
    long long int key;
    person data;
    int height;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct LLNode {
    person data;
    struct LLNode* next;
} LLNode;


LLNode* insertLL(LLNode* head, person new_data) {
    LLNode* nptr = (LLNode*)malloc(sizeof(LLNode));
    nptr -> data = new_data;
    nptr -> next = head;
    return nptr;
}


LLNode* mergeLL(LLNode* L1, LLNode* L2) {
    if (L1 == NULL) {
        return L2;
    }
    LLNode* tail = L1;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = L2;
    return L1;
}

int height(TreeNode* root) {
    int h = 0;
    if (root != NULL) {
        h = root->height;
    }
    return h;
}


void updateHeight(TreeNode* root){
    int lh, rh;

    if (root != NULL) {
        lh = height(root->left);
        rh = height(root->right);

        if (lh > rh) 
            root->height = lh + 1;
        else 
            root->height = rh + 1;
    }
}


int balanceFactor(TreeNode* root){
    int balance = 0, lh, rh;

    if (root != NULL) {
        lh = height(root->left);
        rh = height(root->right);
        balance = rh - lh;
    }
    return balance;
}


TreeNode* rotateLeft(TreeNode* root){
    TreeNode* retVal = NULL;

    if (root != NULL){
        retVal = root->right;
        root->right = retVal->left;
        retVal->left = root;
        updateHeight(root);
    }
    return retVal;
}


TreeNode* rotateRight(TreeNode* root){
    TreeNode* retVal = NULL;

    if (root != NULL){
        retVal = root->left;
        root->left = retVal->right;
        retVal->right = root;
        updateHeight(root);
    }
    return retVal;
}


TreeNode* insertTree(TreeNode* root, long long int new_key, person data_ptr){
    if (root == NULL){
        root = (TreeNode*)malloc(sizeof(TreeNode));
        root -> key = new_key;
        root -> data = data_ptr;
        root -> left = root -> right = NULL;
        root -> height = 1;
    }
    else{
        if (new_key < root -> key)
            root -> left = insertTree(root -> left, new_key, data_ptr);
        else if (new_key > root -> key)
            root -> right = insertTree(root -> right, new_key, data_ptr);
        else{
            // printf("Duplicate\n")
            return root;
        }

        int balance = balanceFactor(root);
        if (balance > 1 && new_key > root->right->key)
            root = rotateLeft(root);
        else if (balance > 1 && new_key < root->right->key){
            root -> right = rotateRight(root -> right);
            root = rotateLeft(root);
        }
        else if (balance < -1 && new_key < root->left->key)
            root = rotateRight(root);
        else if (balance < -1 && new_key > root->left->key){
            root -> left = rotateLeft(root->left);
            root = rotateRight(root);
        }
        updateHeight(root);
    }
    return root;
}


TreeNode* searchAVL(TreeNode* root, long long int key) {
    TreeNode* retVal = NULL;
    if (root != NULL) {
        if (root->key == key) retVal = root;
        else if (root->key < key) retVal = searchAVL(root->right, key);
        else if (root->key > key) retVal = searchAVL(root->left, key);
    }
    return retVal;
}


TreeNode* insertAadhar(char* name, char* address, long long int aadhar_no, TreeNode* aadhar_tree) {
    aadhar* new = (aadhar*)malloc(sizeof(aadhar));

    strcpy(new->name, name);
    strcpy(new->address, address);
    new->aadhar_no = aadhar_no;

    person aadhar_data;
    aadhar_data.aadharPtr = new;

    return insertTree(aadhar_tree, aadhar_no, aadhar_data);
}


aadhar* search_aadhar(long long int aadhar, TreeNode* aadhar_tree) {
    TreeNode* tptr = searchAVL(aadhar_tree, aadhar);
    return tptr->data.aadharPtr;
}


long long int PANkey(char* PAN_no) {
    long long int key = 0;
    for (int i = 0; i < 9; i++) {
        key = key * 100 + (PAN_no[i] - '0');
    }
    return key;
}


TreeNode* insertPAN(TreeNode* pan_tree, char* name, char* address, char* PAN_no, long long int aadhar, TreeNode* aadhar_tree) {
    pan* new = (pan*)malloc(sizeof(pan));
    new->aadhar_info = search_aadhar(aadhar, aadhar_tree);

    if (new->aadhar_info == NULL){
        printf("Corresponding aadhar number not found\n");
        free(new);
        return pan_tree;
    }

    strcpy(new->name, name);
    strcpy(new->address, address);
    strcpy(new->PAN_no, PAN_no);
    
    person pan_data;
    pan_data.panPtr = new;

    return insertTree(pan_tree, PANkey(PAN_no), pan_data);
}


pan* search_PAN(TreeNode* pan_tree, char* PAN) {
    TreeNode* tptr = searchAVL(pan_tree, PANkey(PAN));
    return tptr->data.panPtr;
}


TreeNode* insertBank(TreeNode* bank_tree, char* name, char* PAN, TreeNode* pan_tree, char* bankName, int account_no, double deposit) {
    bank* new = (bank*)malloc(sizeof(bank));

    strcpy(new->name, name);
    new->PAN_info = search_PAN(pan_tree, PAN);

    if (new->PAN_info == NULL) {
        printf("Corresponding PAN number not found\n");
        free(new);
        return bank_tree;
    }

    strcpy(new->bankName, bankName);
    new->account_no = account_no;
    new->deposit = deposit; 

    person bank_data;
    bank_data.bankPtr = new;

    return insertTree(bank_tree, account_no, bank_data);
}


bank* search_bank(TreeNode* bank_tree, int accNum) {
    TreeNode* tptr = searchAVL(bank_tree, accNum);
    return tptr->data.bankPtr;
}


TreeNode* insertLPG(TreeNode* lpg_tree, char* name, int acc, TreeNode* bank_tree, bool hasAvailed){
    lpg* new = (lpg*)malloc(sizeof(lpg));

    strcpy(new->name, name);
    new->availedLPG = hasAvailed;
    new->bankacc = search_bank(bank_tree, acc);

    if (new->bankacc == NULL){
        printf("Corresponding bank account not found\n");
        free(new);
        return lpg_tree;
    }
    
    person lpg_data;
    lpg_data.lpgPtr = new;

    return insertTree(lpg_tree, acc, lpg_data);
}


TreeNode* read_aadhar() {
    FILE*file = fopen("aadhar_list.csv", "r");
    if (!file) {
        printf("Error opening file \n");
        return NULL;
    }

    TreeNode* aadhar_tree = NULL;

    char line[100];
    char*token;
    char*name,*address,*aadhar_number;
    long long int aadhar_num;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, ",");
        name = token;
        token = strtok(NULL, ",");
        address = token;
        token = strtok(NULL, ",");
        aadhar_number = token;

        if (aadhar_number != NULL) {
            aadhar_number[strcspn(aadhar_number, "\n")] = 0;
        }

        aadhar_num = atoll(aadhar_number);

        aadhar_tree = insertAadhar(name, address, aadhar_num, aadhar_tree);
    }

    fclose(file);

    return aadhar_tree;
}


TreeNode* read_PAN(TreeNode* aadhar_tree) {
    FILE*file = fopen("pan_list.csv", "r");
    if (!file) {
        printf("Error opening file \n");
        return NULL;
    }

    TreeNode* pan_tree = NULL;

    char line[100];
    char*token;
    char*name,*address,*aadhar_number,*pan_number;
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

        if (aadhar_number != NULL) {
            aadhar_number[strcspn(aadhar_number, "\n")] = 0;
        }

        aadhar_num = atoll(aadhar_number);

        pan_tree = insertPAN(pan_tree, name, address, pan_number, aadhar_num, aadhar_tree);
    }

    fclose(file);

    return pan_tree;
}


TreeNode* read_bank(char* filename, TreeNode* pan_tree) {
    FILE*file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file \n");
        return NULL;
    }

    TreeNode* bank_tree = NULL;

    char line[100];
    char*token;
    char*name,*pan_number,*bankName,*acc_num,*amt;
    int account_number;
    double amount;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, ",");
        name = token;
        token = strtok(NULL, ",");
        pan_number = token;
        token = strtok(NULL, ",");
        bankName = token;
        token = strtok(NULL, ",");
        acc_num = token;
        token = strtok(NULL, ",");
        amt = token;

        if (amt != NULL) {
            amt[strcspn(amt, "\n")] = 0;
        }

        account_number = atoi(acc_num);
        amount = atof(amt);

        bank_tree = insertBank(bank_tree, name, pan_number, pan_tree, bankName, account_number, amount);
    }

    fclose(file);

    return bank_tree;
}


TreeNode* read_LPG(TreeNode* bank_tree) {
    FILE*file = fopen("lpg_list.csv", "r");
    if (!file) {
        printf("Error opening file \n");
        return NULL;
    }

    TreeNode* lpg_tree = NULL;

    char line[100];
    char*token;
    char*name,*acc_num,*lpg;
    int account_number;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, ",");
        name = token;
        token = strtok(NULL, ",");
        acc_num = token;
        token = strtok(NULL, ",");
        lpg = token;

        if (lpg != NULL) {
            lpg[strcspn(lpg, "\n")] = 0;
        }

        account_number = atoi(acc_num);
        
        if (strcmp(lpg, "YES") == 0) {
            lpg_tree = insertLPG(lpg_tree, name, account_number, bank_tree, true);
        }
        else {
            lpg_tree = insertLPG(lpg_tree, name, account_number, bank_tree, false);
        }
    }

    fclose(file);

    return lpg_tree;
}


LLNode* search_PAN_aadhar(TreeNode* pan_tree, long long int aadhar) {
    LLNode* retptr = NULL;
    if (pan_tree != NULL) {
        if (pan_tree->data.panPtr->aadhar_info->aadhar_no == aadhar) {
            retptr = insertLL(retptr, pan_tree->data);
        }
        retptr = mergeLL(retptr, search_PAN_aadhar(pan_tree->left, aadhar));
        retptr = mergeLL(retptr, search_PAN_aadhar(pan_tree->right, aadhar));
    }
    return retptr;
}


void print_no_pan(TreeNode* aadhar_tree, TreeNode* pan_tree) {
    if (aadhar_tree != NULL) {
        aadhar* ptr = aadhar_tree->data.aadharPtr;
        if (search_PAN_aadhar(pan_tree, ptr->aadhar_no) == NULL) {
            printf("\nName : %s\nAddress : %s\nAadhar Number : %lld\n", ptr->name, ptr->address, ptr->aadhar_no);
        }
        print_no_pan(aadhar_tree->left, pan_tree);
        print_no_pan(aadhar_tree->right, pan_tree);
    }
}


void print_multi_pan(TreeNode* aadhar_tree, TreeNode* pan_tree) {
    if (aadhar_tree != NULL) {
        aadhar* ptr = aadhar_tree->data.aadharPtr;
        LLNode* PAN_cards = search_PAN_aadhar(pan_tree, ptr->aadhar_no);
        if (PAN_cards != NULL && PAN_cards->next != NULL) {
            printf("\nName : %s\nAddress : %s\nAadhar Number : %lld\nPAN Numbers:\n", ptr->name, ptr->address, ptr->aadhar_no);
            LLNode* temp = PAN_cards;
            while (temp != NULL) {
                PAN_cards = PAN_cards->next;
                printf("%s\n", temp->data.panPtr->PAN_no);
                free(temp);
                temp = PAN_cards;
            }
        }
        print_multi_pan(aadhar_tree->left, pan_tree);
        print_multi_pan(aadhar_tree->right, pan_tree);
    }
}


bank* search_bank_pan(TreeNode* bank_tree, char* PAN_no) {
    bank* retptr = NULL;
    if (bank_tree != NULL) {
        if (strcmp(bank_tree->data.bankPtr->PAN_info->PAN_no, PAN_no) == 0) {
            retptr = bank_tree->data.bankPtr;
        }
        else {
            retptr = search_bank_pan(bank_tree->left, PAN_no);
            if (retptr == NULL) {
                retptr = search_bank_pan(bank_tree->right, PAN_no);
            }
        }
    }
    return retptr;
}


void print_multi_bank(TreeNode* aadhar_tree, TreeNode* pan_tree, TreeNode* bank_tree) {
    if (aadhar_tree != NULL) {
        aadhar* ptr = aadhar_tree->data.aadharPtr;
        LLNode* PAN_cards = search_PAN_aadhar(pan_tree, ptr->aadhar_no);
        if (PAN_cards != NULL && PAN_cards->next != NULL) {
            int bank_count = 0;
            LLNode* temp = PAN_cards;
            while (temp != NULL && bank_count < 2) {
                PAN_cards = PAN_cards->next;
                if (search_bank_pan(bank_tree, temp->data.panPtr->PAN_no) != NULL)
                    bank_count += 1;
                free(temp);
                temp = PAN_cards;
            }
            if (bank_count > 1) 
                printf("\nName : %s\nAddress : %s\nAadhar Number : %lld\n", ptr->name, ptr->address, ptr->aadhar_no);
        }
        print_multi_bank(aadhar_tree->left, pan_tree, bank_tree);
        print_multi_bank(aadhar_tree->right, pan_tree, bank_tree);
    }
}


void print_lpg(TreeNode* lpg_tree) {
    aadhar* aptr;
    pan* pptr;
    bank* bptr;
    if (lpg_tree != NULL) {
        if (lpg_tree->data.lpgPtr->availedLPG) {
            bptr = lpg_tree->data.lpgPtr->bankacc;
            pptr = bptr->PAN_info;
            aptr = pptr->aadhar_info;
            printf("\nName : %s\nAadhar Number : %lld\nPAN Number : %s\nBank : %s\nAccount Number : %d\n",
                aptr->name, aptr->aadhar_no, pptr->PAN_no, bptr->bankName, bptr->account_no);
        }
        print_lpg(lpg_tree->left);
        print_lpg(lpg_tree->right);
    }
}


double totalSaving_given_aadhar(long long int aadhar, TreeNode* bank_tree) {
    double total = 0;
    if (bank_tree != NULL) {
        if (bank_tree->data.bankPtr->PAN_info->aadhar_info->aadhar_no == aadhar) {
            total += bank_tree->data.bankPtr->deposit;
        }
        total += totalSaving_given_aadhar(aadhar, bank_tree->left);
        total += totalSaving_given_aadhar(aadhar, bank_tree->right);
    }
    return total;
}


void print_greater_X(double x, TreeNode* lpg_tree, TreeNode* bank_tree) {
    if (lpg_tree != NULL) {
        lpg* lptr = lpg_tree->data.lpgPtr;
        if (lptr->availedLPG) {
            double totalSaving = totalSaving_given_aadhar(lptr->bankacc->PAN_info->aadhar_info->aadhar_no, bank_tree);
            if (totalSaving > x)
                printf("\nName : %s\nAddress : %s\nAadhar Number : %lld\nMoney : %lf\n", lptr->name, lptr->bankacc->PAN_info->aadhar_info->address, lptr->bankacc->PAN_info->aadhar_info->aadhar_no, totalSaving);
        }
        print_greater_X(x, lpg_tree->left, bank_tree);
        print_greater_X(x, lpg_tree->right, bank_tree);
    }
}


bool print_inconsistent_pan(long long aadhar, TreeNode* pan_tree) {
    bool isInconsistent = false;
    if (pan_tree != NULL) {
        pan* pptr = pan_tree->data.panPtr;
        if (pptr->aadhar_info->aadhar_no == aadhar) {
            if (strcmp(pptr->name, pptr->aadhar_info->name) != 0) {
                isInconsistent = true;
                printf("\nName : %s\nAddress : %s\nAadhar Number : %lld\n", pptr->aadhar_info->name, pptr->aadhar_info->address, pptr->aadhar_info->aadhar_no);
                printf("Inconsistent in PAN: %s\nPAN Name: %s\n", pptr->PAN_no, pptr->name);
            }
        }
        if (!isInconsistent) {
            isInconsistent = print_inconsistent_pan(aadhar, pan_tree->left);
        }
        if (!isInconsistent) {
            isInconsistent = print_inconsistent_pan(aadhar, pan_tree->right);
        }
    }
    return isInconsistent;
}


bool print_inconsistent_bank(long long aadhar, TreeNode* bank_tree) {
    bool isInconsistent = false;
    if (bank_tree != NULL) {
        bank* bptr = bank_tree->data.bankPtr;
        pan* pptr = bptr->PAN_info;
        if (pptr->aadhar_info->aadhar_no == aadhar) {
            if (strcmp(bptr->name, pptr->aadhar_info->name) != 0) {
                isInconsistent = true;
                printf("\nName : %s\nAddress : %s\nAadhar Number : %lld\n", pptr->aadhar_info->name, pptr->aadhar_info->address, pptr->aadhar_info->aadhar_no);
                printf("Inconsistent in Bank Account No.: %d\nBank Name: %s\n", bptr->account_no, bptr->name);
            }
        }
        if (!isInconsistent) {
            isInconsistent = print_inconsistent_bank(aadhar, bank_tree->left);
        }
        if (!isInconsistent) {
            isInconsistent = print_inconsistent_bank(aadhar, bank_tree->right);
        }
    }
    return isInconsistent;
}


bool print_inconsistent_lpg(long long aadhar, TreeNode* lpg_tree) {
    bool isInconsistent = false;
    if (lpg_tree != NULL) {
        lpg* lptr = lpg_tree->data.lpgPtr;
        bank* bptr = lptr->bankacc;
        pan* pptr = bptr->PAN_info;
        if (pptr->aadhar_info->aadhar_no == aadhar) {
            if (strcmp(lptr->name, pptr->aadhar_info->name) != 0) {
                isInconsistent = true;
                printf("\nName : %s\nAddress : %s\nAadhar Number : %lld\n", pptr->aadhar_info->name, pptr->aadhar_info->address, pptr->aadhar_info->aadhar_no);
                printf("Inconsistent in LPG with Bank Account No.: %d\nLPG Name: %s\n", bptr->account_no, lptr->name);
            }
        }
        if (!isInconsistent) {
            isInconsistent = print_inconsistent_lpg(aadhar, lpg_tree->left);
        }
        if (!isInconsistent) {
            isInconsistent = print_inconsistent_lpg(aadhar, lpg_tree->right);
        }
    }
    return isInconsistent;
}


void print_inconsistent(TreeNode* aadhar_tree, TreeNode* pan_tree, TreeNode* bank_tree, TreeNode* lpg_tree) {
    bool isInconsistent = false;
    if (aadhar_tree != NULL) {
        aadhar* aptr = aadhar_tree->data.aadharPtr;
        print_inconsistent_pan(aptr->aadhar_no, pan_tree);
        print_inconsistent_bank(aptr->aadhar_no, bank_tree);
        print_inconsistent_lpg(aptr->aadhar_no, lpg_tree);

        print_inconsistent(aadhar_tree->left, pan_tree, bank_tree, lpg_tree);
        print_inconsistent(aadhar_tree->right, pan_tree, bank_tree, lpg_tree);
    }
}


TreeNode* bank_merge(TreeNode* bank_tree1, TreeNode* bank_tree2) {
    if (bank_tree1 == NULL) {
        return bank_tree2;
    }
    else if (bank_tree2 != NULL) {
        bank_tree1 = bank_merge(bank_tree1, bank_tree2->left);
        bank_tree1 = bank_merge(bank_tree1, bank_tree2->right);
        bank_tree1 = insertTree(bank_tree1, bank_tree2->key, bank_tree2->data);
        free(bank_tree2);
    }
    return bank_tree1;
}


void printInOrder(TreeNode* root) {
    if (root != NULL) {
        printInOrder(root->left);
        printf("%lld ", root->key);
        printInOrder(root->right);
    }
}


void print_aadhar_range(long long int aadhar1, long long int aadhar2, TreeNode* aadhar_tree) {
    if (aadhar_tree != NULL) {
        aadhar* aptr = aadhar_tree->data.aadharPtr;
        if (aptr->aadhar_no > aadhar1) {
            print_aadhar_range(aadhar1, aadhar2, aadhar_tree->left);
        }
        if (aptr->aadhar_no >= aadhar1 && aptr->aadhar_no <= aadhar2) {
            printf("\nName : %s\nAddress : %s\nAadhar Number : %lld\n", aptr->name, aptr->address, aptr->aadhar_no);
        }
        if (aptr->aadhar_no < aadhar2) {
            print_aadhar_range(aadhar1, aadhar2, aadhar_tree->right);
        }
    }
}


int main() {
    TreeNode* aadhar_tree = read_aadhar();
    TreeNode* pan_tree = read_PAN(aadhar_tree);
    TreeNode* bank_tree = read_bank("bank_accounts_list.csv", pan_tree);
    TreeNode* lpg_tree = read_LPG(bank_tree);
    int input;
    do {
        printf("\n1. Print no PAN users\n2. Print mulitple PAN users\n3. Print multiple bank account users\n4. Print users who have lpg subsidy\n5. Print users with LPG subsidy having total balance greater than X amt\n6. Print Inconsistent data\n7. Merge 2 bank lists\n8. Print Aadhar in range\n9. Exit\nEnter input : ");
        scanf("%d", &input);
        if (input == 1)
            print_no_pan(aadhar_tree, pan_tree);
        else if (input == 2)
            print_multi_pan(aadhar_tree, pan_tree);
        else if (input == 3)
            print_multi_bank(aadhar_tree, pan_tree, bank_tree);
        else if (input == 4)
            print_lpg(lpg_tree);
        else if (input ==  5) {
            double x;
            printf("X = ");
            scanf("%lf", &x);
            print_greater_X(x, lpg_tree, bank_tree);
        }
        else if (input == 6) {;
            print_inconsistent(aadhar_tree, pan_tree, bank_tree, lpg_tree);
        }
        else if (input == 7) {
            TreeNode* bank_tree1 = read_bank("bank_accounts_list1.csv", pan_tree);
            TreeNode* bank_tree2 = read_bank("bank_accounts_list2.csv", pan_tree);
            printf("\nBank tree 1: ");
            printInOrder(bank_tree1);
            printf("\nBank tree 2: ");
            printInOrder(bank_tree2);
            bank_tree1 = bank_merge(bank_tree1, bank_tree2);
            printf("\nBank tree after merging: ");
            printInOrder(bank_tree1);
            printf("\n");
        }
        else if (input == 8) {
            long long int a1, a2;
            printf("A1: ");
            scanf("%lld", &a1);
            printf("A2: ");
            scanf("%lld", &a2);
            print_aadhar_range(a1, a2, aadhar_tree);
        }
        else if (input != 9)
            printf("Enter valid input\n");
    } while (input != 9);

    return 0;
}