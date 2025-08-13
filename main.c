#include <stdio.h>
#include <string.h>

struct user 
{
    int user_id;
    char user_name[200];    
    float amount;
} user_database[1001];

struct expense
{
    int expense_id;
    float expense;
    struct user *paid;
    struct user *owe[4];
} expense_database[1001];

int no_user = 0;
int no_expense = 0;

struct user * binary_search(int userID)
{
    int left = 0, right = no_user, middle;
    while (left <= right)
    {
        middle = (left + right)/2;
        if (user_database[middle].user_id  == userID)
            return &user_database[middle];
        else if (userID > user_database[middle].user_id)
            left = middle + 1;
        else right = middle - 1;
    }
    return NULL;
}


void AddUser();
void AddExpense();
void AmountOwed(int userID);
void Amount_to_pay(int userID);
void User_balances(struct user user1);
void Settle_up(int userID1, int userID2);
void DeleteUser(int userID);
void DeleteExpense();

  
void main()
{
    int option = 0;
    do
    {
        printf("Enter what you want to do :\n1.Add User\n2.Add Expense\n3.Amount owed to a user\n4.Amount owed by a user\n5.Display all user balances\n6.Settle owe\n7.Delete User\n8.Delete Expense\n9.Quit\n");
        scanf("%d", &option);
        if (option == 1)
        {
            if (no_user < 1000)
                AddUser();
            else
                printf("Overflow\n");
            printf("\n");
        }
        else if (option == 2)
        {
            if (no_expense < 1000)
                AddExpense();
            else
                printf("Overflow\n");
            printf("\n");
        }
        else if (option == 3)
        {
            int ID;
            printf("Enter ID to check : ");
            scanf("%d", &ID);
            AmountOwed(ID);
            printf("\n");
        }
        else if (option == 4)
        {
            int ID;
            printf("Enter ID to check : ");
            scanf("%d", &ID);
            Amount_to_pay(ID);
            printf("\n");
        }
        else if (option == 5)
        {
            for (int i = 1; i <= no_user; i++)
                User_balances(user_database[i]);
        }
        else if (option == 6)
        {
            int uID, oID;
            printf("Enter UserID who paid : ");
            scanf("%d", &uID);
            printf("Enter UserID who owes : ");
            scanf("%d", &oID);
            Settle_up(uID, oID);
            printf("\n");
        }
        else if (option == 7)
        {
            int ID;
            printf("Enter ID to check : ");
            scanf("%d", &ID);
            DeleteUser(ID);
            printf("\n");
        }
        else if (option == 8)
        {
            DeleteExpense();
            printf("\n");
        }
        else if (option != 9)
            printf("Enter valid input, Try again\n\n");
    } while (option != 9);
}


void AddUser()
{
    int id = 0;
    char name[200];
    float spent;
    do{
        printf("Enter UserID: ");
        scanf("%d", &id);
    } while ((id < 1 || id >= 1000) || binary_search(id) != NULL);
    printf("Enter first name: ");
    scanf("%s", name);
    printf("Enter amount spent: ");
    scanf("%f", &spent);
    int flag = 1;
    for (int i = no_user; i >= 0 && flag; i--)
    {
        if (user_database[i].user_id > id)
        {
            user_database[i+1] = user_database[i];
        }
        else
        {
            flag = 0;
            user_database[i+1].user_id = id;
            strcpy(user_database[i+1].user_name, name);
            user_database[i+1].amount = spent;
        }
    }
    no_user++;
    printf("User added!\n");
    // for (int i = 1; i <= no_user; i++)
    // {
    //     printf("%d \t %s \t %f\n", user_database[i].user_id, user_database[i].user_name, user_database[i].amount);
    // }
}


void AddExpense()
{
    int exp_id = 0;
    float cost;
    int user_id;
    int no_owed;
    do{
        printf("Enter ExpenseID: ");
        scanf("%d", &exp_id);
    } while (exp_id < 1 || exp_id >= 1000);
    printf("Enter expense amount: ");
    scanf("%f", &cost);
    do
    {
        printf("Enter valid UserID of who paid: ");
        scanf("%d", &user_id);
    } while (binary_search(user_id) == NULL);
    
    printf("Enter how many more users shared expense :  ");
    scanf("%d", &no_owed);

    int flag = 1;
    for (int i = no_expense; i >= 0 && flag; i--)
    {
        if (expense_database[i].expense_id > exp_id)
        {
            expense_database[i+1] = expense_database[i];
        }
        else
        {
            flag = 0;
            expense_database[i+1].expense_id = exp_id;
            expense_database[i+1].expense = cost;
            expense_database[i+1].paid = binary_search(user_id);
            (*(expense_database[i+1].paid)).amount += cost;
            for (int j = 0; j < no_owed; j++)
            {
                int owe_id;
                do
                {
                    printf("Enter valid UserID of sharer %d: ", j+1);
                    scanf("%d", &owe_id);
                } while (binary_search(owe_id) == NULL);
                expense_database[i+1].owe[j] = binary_search(owe_id);
            }
        }
    }
    no_expense++;
    printf("Expense added!\n");
    // for (int i = 1; i <= no_expense; i++)
    // {
    //     printf("%d \t %f \t %d\t", expense_database[i].expense_id, expense_database[i].expense, (*(expense_database[i].paid)).user_id);
    //     for (int j = 0; expense_database[i].owe[j] != 0; j++)
    //         printf("%d \t", (*(expense_database[i].owe))[j].user_id);
    //     printf("\n");
    // }
}


void AmountOwed(int userID)
{
    float total_owe = 0.0, user_owe;
    printf("Ower ID\t Ower Name\t Amount Owed\n");
    for (int j = 1; j <= no_user; j++)
    {
        user_owe = 0;
        if (user_database[j].user_id != userID)
        {
            for (int i = 1; i <= no_expense; i++)
            {
                if ((*(expense_database[i].paid)).user_id == userID)
                {
                    int k, flag = 0;
                    for (k = 0; expense_database[i].owe[k] != 0; k++)
                    {
                        if ((*(expense_database[i].owe[k])).user_id == user_database[j].user_id)
                            flag = 1;
                    }
                    if (flag)
                        user_owe += (expense_database[i].expense / (k+1));
                }
            }
            total_owe += user_owe;
            printf("%d \t %s \t\t %.2f\n", user_database[j].user_id, user_database[j].user_name, user_owe);
        }
    }
    printf("Total owed to user : %.2f\n", total_owe);
}


void Amount_to_pay(int userID)
{
    float total_owed = 0.0, user_owed;
    printf("User ID\t User Name\t Amount Owed\n");
    for (int j = 1; j <= no_user; j++)
    {
        user_owed = 0;
        if (user_database[j].user_id != userID)
        {
            for (int i = 1; i <= no_expense; i++)
            {
                if ((*(expense_database[i].paid)).user_id == user_database[j].user_id) 
                {
                    int k, flag = 0;
                    for (k = 0; expense_database[i].owe[k] != 0; k++)
                    {
                        if ((*(expense_database[i].owe[k])).user_id == userID)
                            flag = 1;
                    }
                    if (flag)
                        user_owed += (expense_database[i].expense / (k+1));
                }
            }
            total_owed += user_owed;
            printf("%d \t %s \t\t %.2f\n", user_database[j].user_id, user_database[j].user_name, user_owed);
        }
    }
    printf("Total owed by user : %.2f\n", total_owed);
}


int settled(int userID)
{
    int balances = 0;
    float total_owe = 0.0, user_owe;
    for (int j = 1; j <= no_user; j++)
    {
        user_owe = 0;
        if (user_database[j].user_id != userID)
        {
            for (int i = 1; i <= no_expense; i++)
            {
                if ((*(expense_database[i].paid)).user_id == userID)
                {
                    int k, flag = 0;
                    for (k = 0; expense_database[i].owe[k] != 0; k++)
                    {
                        if ((*(expense_database[i].owe[k])).user_id == user_database[j].user_id)
                            flag = 1;
                    }
                    if (flag)
                        user_owe += (expense_database[i].expense / (k+1));
                }
            }
            total_owe += user_owe;
        }
    }
    float total_owed = 0.0, user_owed;
    for (int j = 1; j <= no_user; j++)
    {
        user_owed = 0;
        if (user_database[j].user_id != userID)
        {
            for (int i = 1; i <= no_expense; i++)
            {
                if ((*(expense_database[i].paid)).user_id == user_database[j].user_id) 
                {
                    int k, flag = 0;
                    for (k = 0; expense_database[i].owe[k] != 0; k++)
                    {
                        if ((*(expense_database[i].owe[k])).user_id == userID)
                            flag = 1;
                    }
                    if (flag)
                        user_owed += (expense_database[i].expense / (k+1));
                }
            }
            total_owed += user_owed;
        }
    }
    if (total_owe != 0 || total_owed != 0)
        balances = 1;
    return balances;
}


void User_balances(struct user user1)
{
    printf("________________________________________________________________________________________\n\n");
    printf("Amount owed to user %d.%s by :\n", user1.user_id, user1.user_name);
    AmountOwed(user1.user_id);
    printf("\n");
    printf("Amount owed by user %d.%s to :\n", user1.user_id, user1.user_name);
    Amount_to_pay(user1.user_id);
}


void Settle_up(int userID1, int userID2)
{
    int k;
    printf("ExpenseID\t Amount Owed\n");
    for (int i = 1; i <= no_expense; i++)
    {
        if ((*(expense_database[i].paid)).user_id == userID1)
        {
            int flag = 0;
            for (k = 0; expense_database[i].owe[k] != 0; k++)
            {
                if ((*(expense_database[i].owe[k])).user_id == userID2)
                    flag = 1;
            }
            if (flag)
            {
                printf("%d\t\t%.2f\n", expense_database[i].expense_id, expense_database[i].expense/(k+1));
            }
        }
    }
    int expID;
    printf("Enter ExpenseID to settle : ");
    scanf("%d", &expID);
    int left = 0, right = no_expense, middle, found = 0;
    while (left <= right && !found)
    {
        middle = (left + right)/2;
        if (expense_database[middle].expense_id  == expID)
        {
            (*(binary_search(userID2))).amount += expense_database[middle].expense/(k+1);
            (*(binary_search(userID1))).amount -= expense_database[middle].expense/(k+1);
            expense_database[middle].expense -= expense_database[middle].expense/(k+1);
            printf("Expense settled!\n\n");
            int k;
            for (k = 0; expense_database[middle].owe[k] != 0; k++)
            {
                if (!found && (*(expense_database[middle].owe[k])).user_id == userID2)
                    found = 1;
                if (found)
                    expense_database[middle].owe[k] = expense_database[middle].owe[k+1];    
            }
        }
        else if (expID > expense_database[middle].expense_id)
            left = middle + 1;
        else right = middle - 1;
    }
}


void DeleteUser(int userID)
{
    struct user * user_id = binary_search(userID);
    if (user_id == NULL)
        printf("Try again with a valid UserID\n");
    else if (settled(userID))
    {
        printf("Please settle all below balances before deleting user\n");
        User_balances(*user_id);
    }
    else
    {
        int left = 0, right = no_user, middle, flag = 1;
        while (left <= right && flag)
        {
            middle = (left + right)/2;
            if (user_database[middle].user_id  == userID)
                flag = 0;
            else if (userID > user_database[middle].user_id)
                left = middle + 1;
            else right = middle - 1;
        }
        user_database[middle].user_id = 0;
        for (int i = middle; i < no_user; i++)
        {
            struct user temp;
            user_database[i] = temp;
            user_database[i] = user_database[i+1];
            user_database[i+1] = temp;
        }
        printf("User deleted!\n\n");
    }
    no_user -= 1;
}


void DeleteExpense()
{
    int expID, userID;
    printf("Enter expense ID to delete : ");
    scanf("%d", &expID);
    printf("Enter user ID of who made expense : ");
    scanf("%d", &userID);
    int left = 0, right = no_expense, middle, flag = 1;
    while (left <= right && flag)
    {
        middle = (left + right)/2;
        if (expense_database[middle].expense_id  == expID )
            flag = 0;
        else if (expID > expense_database[middle].expense_id)
            left = middle + 1;
        else right = middle - 1;
    }
    if (flag == 0 && (*(expense_database[middle].paid)).user_id == userID)
    {
        expense_database[middle].expense_id = 0;
        (*(expense_database[middle].paid)).amount -= expense_database[middle].expense;
        for (int i = middle; i < no_expense; i++)
        {
            struct expense temp;
            expense_database[i] = temp;
            expense_database[i] = expense_database[i+1];
            expense_database[i+1] = temp;
        }
        printf("Expense deleted!\n\n");    
        no_expense -= 1;
    }
    else
        printf("Try again with valid user and expense ID\n");
}
