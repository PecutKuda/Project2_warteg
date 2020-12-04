#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const unsigned int MAX_ELEMENT = 26;
unsigned int totalCust=0;
int choice;

typedef struct ascii
{
	char str[255];
};
ascii s[255];

unsigned long DJB2(char *str) 
{
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
  {
    hash = ((hash << 5) + hash) + c;
  }
  return hash % MAX_ELEMENT;
}

struct dish
{
    char name[255];
    int price;
    int quantity;
    dish *next, *prev;
}*headDish = NULL, *tailDish = NULL, *currDish = NULL;

struct customer
{
    char name[255];
    customer *next, *prev;
}*headCust[MAX_ELEMENT], *currCust, *tailCust[MAX_ELEMENT];

struct orders
{
    char custName[255];
    char dishName[255];
    int qty;
    int price;
    orders *next, *prev;
}*headOrder[MAX_ELEMENT], *currOrder, *tailOrder[MAX_ELEMENT];

orders* createneworders(char* custName, char* dishName, int qty, int price)
{
    orders* node = (orders*)malloc(sizeof(orders));
    strcpy(node->custName, custName);
    strcpy(node->dishName, dishName);
    node->qty = qty;
    node->price = price;
    node->next = node->prev = NULL;
    return node;
}

dish* createnewdish(char* name, int price, int quantity)
{
    dish* node = (dish*)malloc(sizeof(dish));
    strcpy(node->name, name);
    node->price = price;
    node->quantity = quantity;
    node->next = node->prev = NULL;
    return node;
}

customer* createnewcust(char* name)
{
    customer* node = (customer*)malloc(sizeof(customer));
    strcpy(node->name, name);
    node->next = node->prev = NULL;
    return node;
}

void insertCust(char *str) 
{
  int index = DJB2(str);
  if(headCust[index]) 
  { 
    customer *temp = createnewcust(str);
    tailCust[index]->next = temp;
    tailCust[index] = temp;
  } 
  else headCust[index] = tailCust[index] = createnewcust(str);
}

void insertOrder(char* custName, char* dishName, int qty, int price) 
{
  int index = DJB2(custName);
  if(headOrder[index]) 
  { 
    orders *temp = createneworders(custName, dishName, qty, price);
    tailOrder[index]->next = temp;
    tailOrder[index] = temp;
  } 
  else headOrder[index] = tailOrder[index] = createneworders(custName, dishName, qty, price);
}

void pushTailDish(char* name, int price, int quantity)
{
    dish* temp = createnewdish(name, price, quantity);
    if (!headDish) headDish = tailDish = temp;
    else
    {
        tailDish->next = temp;
        temp->prev = tailDish;
        tailDish = temp;
    }
}

void popHeadDish()
{
    if (!headDish) return;
    else if (headDish == tailDish) 
    {
        headDish = tailDish = NULL;
        free(headDish);
    }
    else
    {
        dish *newHead = headDish->next; 
        headDish->next = newHead->next = NULL; 
        free(headDish);
        headDish = newHead;
    }
}

void popTailDish()
{
    if (!headDish) return;
    else if (headDish == tailDish)
    {
        headDish = tailDish = NULL;
        free(headDish);
    }
    else 
    {
       dish* newTail = tailDish->prev;
       tailDish->prev = newTail->next = NULL;
       free(tailDish);
       tailDish = newTail;  
    }
}

void popMidDish(char* name, int price, int quantity)
{
    if(!headDish) return;
    else if(strcmp(headDish->name, name) == 0) 
    {
        popHeadDish();
    }
    else if(strcmp(tailDish->name, name) == 0) 
    {
        popTailDish();
    } 
    else 
    { 
        currDish = headDish;
        while(currDish && strcmp(currDish->name, name) != 0) 
        {
            currDish = currDish->next;
        }
        currDish->prev->next = currDish->next; 
        currDish->next->prev = currDish->prev;
        currDish->prev = currDish->next = NULL;
        free(currDish);
    }
}

void popHeadCust(int index)
{
    if (!headCust[index]) return;
    else if (headCust[index] == tailCust[index]) 
    {
        headCust[index] = tailCust[index] = NULL;
        free(headCust[index]);
    }
    else
    {
        customer *newHead = headCust[index]->next; 
        headCust[index]->next = newHead->next = NULL; 
        free(headCust[index]);
        headCust[index] = newHead;
    }
}

void popTailCust(int index)
{
    if (!headCust[index]) return;
    else if (headCust[index] == tailCust[index])
    {
        headCust[index] = tailCust[index] = NULL;
        free(headCust[index]);
    }
    else 
    {
       customer* newTail = tailCust[index]->prev;
       tailCust[index]->prev = newTail->next = NULL;
       free(tailCust[index]);
       tailCust[index] = newTail;  
    }
}

void popMidCust(int index)
{
    char custName[255];
   strcpy(custName, headCust[index]->name);
   if(!headCust[index]) return;
   else if(strcmp(headCust[index]->name, custName) == 0) 
   {
       popHeadCust(index);
   }
   else if(strcmp(tailCust[index]->name, custName) == 0) 
   {
       popTailCust(index);
   } 
   else 
   { 
       currCust = headCust[index];
       while(currCust && strcmp(currCust->name, custName) != 0) 
       {
           currCust = currCust->next;
       }
       currCust->prev->next = currCust->next; 
       currCust->next->prev = currCust->prev;
       currCust->prev = currCust->next = NULL;
       free(currCust);
   }
}

void popHeadOrder(int index)
{
    if (!headOrder[index]) return;
    else if (headOrder[index] == tailOrder[index]) 
    {
        headOrder[index] = tailOrder[index] = NULL;
        free(headOrder[index]);
    }
    else
    {
        orders *newHead = headOrder[index]->next; 
        headOrder[index]->next = newHead->next = NULL; 
        free(headOrder[index]);
        headOrder[index] = newHead;
    }
}

void popTailOrder(int index)
{
    if (!headOrder[index]) return;
    else if (headOrder[index] == tailOrder[index])
    {
        headOrder[index] = tailOrder[index] = NULL;
        free(headOrder[index]);
    }
    else 
    {
       orders* newTail = tailOrder[index]->prev;
       tailOrder[index]->prev = newTail->next = NULL;
       free(tailOrder[index]);
       tailOrder[index] = newTail;  
    }
}

void popMidOrder(int index)
{
    char custname[255];
   strcpy(custname, headOrder[index]->custName);
   if(!headOrder[index]) return;
   else if(strcmp(headOrder[index]->custName, custname) == 0) 
   {
       popHeadOrder(index);
   }
   else if(strcmp(tailOrder[index]->custName, custname) == 0) 
   {
       popTailOrder(index);
   } 
   else 
   { 
       currOrder = headOrder[index];
       while(currOrder && strcmp(currOrder->custName, custname) != 0) 
       {
           currOrder = currOrder->next;
       }
       currOrder->prev->next = currOrder->next; 
       currOrder->next->prev = currOrder->prev;
       currOrder->prev = currOrder->next = NULL;
       free(currOrder);
   }
}



void mainmenu();
void addDish();
void removeDish();
void addCustomer();
void searchCustomer();
void viewWarteg();
void order();
void payment();
void exit();

int main()
{
    //validasi input choice main menu
    do
    {
        system("cls");
        mainmenu();
    } while (choice < 1 || choice > 9);
    while (1)
    {
	    if (choice == 1) addDish();
	    else if (choice == 2) removeDish();
	    else if (choice == 3) addCustomer();
	    else if (choice == 4) searchCustomer();
	    else if (choice == 5) viewWarteg();
	    else if (choice == 6) order();
	    else if (choice == 7) payment();
	    else if (choice == 8)
	    {
            exit();
            break;
	    }
	}

return 0;}

void mainmenu()
{
    time_t t;
    time(&t);
    printf("System: ");
	#ifdef _WIN32
		printf("Windows\n");
	#elif __linux__
		printf("Linux\n");
	#elif TARGET_OS_MAC
		printf("MacOS\n");
	#elif _APPLE_
		printf("Apple OS\n");
	#elif __ANDROID__
		printf("Android\n");
	#endif
    printf("%s\n", ctime(&t));
    puts("1. Add Dish");
    puts("2. Remove Dish");
    puts("3. Add Customer");
    puts("4. Search Customer");
    puts("5. View Warteg");
    puts("6. Order");
    puts("7. Payment");
    puts("8. Exit Warteg");
    printf(">> ");
    scanf("%d", &choice); getchar();
}

void addDish()
{
    system("cls");
    int nameAman=1;
    char name[255];
    int price;
    int quantity;
    do
    {
        nameAman=1;
        printf("Insert the name of the dish [Lowercase letters]: ");
        scanf("%[^\n]", name); getchar();
        for (int i=0; name[i] != '\0'; i++)
        {
            if (name[i] >= 'A' && name[i] <= 'Z') nameAman=0;
        }
    } while (nameAman == 0);
    printf("Insert the price of the dish [1000-50000]: ");
    scanf("%d", &price); getchar();
    while (price < 1000 || price > 50000)
    {
        printf("Insert the price of the dish [1000-50000]: ");
        scanf("%d", &price); getchar();
    } 

    printf("Insert the quantity of the dish [1-999]: ");
    scanf("%d", &quantity); getchar();
    while (quantity < 1 || quantity > 999) 
    {
        printf("Insert the quantity of the dish [1-999]: ");
        scanf("%d", &quantity); getchar();
    }
    pushTailDish(name, price, quantity);
    puts("The dish has been added!");
    printf("Press enter to continue...");
    getchar();
    system("cls");
    mainmenu();
}

void removeDish()
{
    if (!headDish)
    {
    	system("cls");
        printf("There's no dish for now...\n");
        printf("Please add some dish first! (Enter to the main menu)");
        getchar();
        system("cls");
        mainmenu();
    }
    else
    {	
		system("cls");
        int idx=1;
        char namaDish[255];
        printf("\t\tBude's Menu\n\n");
        printf("=======================================\n");
        printf("| No  | Name         | Quantity | Price\n");  
        currDish = headDish;
        while (currDish)
        {
            printf("|%-5d| %-13s| %-9d| %-5d\n", idx, currDish->name, currDish->quantity, currDish->price);
            currDish = currDish->next;
            idx++;
        }
        printf("=======================================\n");
        int tampungPrice, tampungQty, dishAvailable=0;
        //jika nama dish utk di delete tidak tersedia, minta nama dish terus
        while (dishAvailable == 0)
        {
        	dishAvailable=0;
        	currDish = headDish;
            printf("Insert dish's name to be deleted: ");
            scanf("%[^\n]", namaDish); getchar();
            currDish = headDish;
            while (currDish)
            {
                if (strcmp(currDish->name, namaDish) == 0)
                {
                    dishAvailable=1;
                    tampungQty = currDish->quantity;
                    tampungPrice = currDish->price;
                }
                currDish = currDish->next;
            }
        }
        if (dishAvailable == 1) popMidDish(namaDish, tampungPrice, tampungQty);
        printf("The dish has been removed!\n");
        printf("Press enter to continue...");
        getchar();
        system("cls||clear");
        mainmenu();
    }
}

void addCustomer()
{
	system("cls||clear");
    char name[255];
    int noSpace=1;
    do {
    printf("Insert the customer's name [Without space]: ");
    scanf("%[^\n]", name); getchar();
    for (int i=0; name[i] != '\0'; i++)
    {
        if (name[i] == ' ') noSpace = 0;
    }
    }while (noSpace == 0);
    insertCust(name);
    printf("Customer has been added!\n");
    printf("\nPress enter to continue..."); getchar();
    system("cls||clear");
    mainmenu();
}

void searchCustomer()
{
	system("cls||clear");
    char str[255];
    int found=0;
    int idx=0;
    printf("Insert the customer name to be searched: ");
    scanf("%s", str); getchar();
    while (idx < MAX_ELEMENT)
    {
        customer* currCust = headCust[idx];
        while (currCust)
        {
            if (strcmp(str, currCust->name) == 0)
            {
                printf("%d: %s\n", idx, currCust->name);
                found = 1;
                break;
            }
            currCust = currCust->next;
        }
		idx++;    
    }
    if (found == 0) printf("%s is not present\n", str);
    printf("Press enter to continue...");
    getchar();
    system("cls||clear");
    mainmenu();
}

int validCustomer(char* names)
{
    int found=0;
    int idx;
    for (idx=0; idx < MAX_ELEMENT; idx++)
    {
        customer* currCust = headCust[idx];
        while (currCust)
        {
            if (strcmp(names, currCust->name) == 0)
            {
                found = 1;
                break;
            }
            currCust = currCust->next;
        }    
    }
    if (found) return 1;
    else return 0;
}

void viewWarteg()
{
	system("cls||clear");
    printf("Customer's List\n\n");
    int idx;
    for (idx=0; idx < MAX_ELEMENT; idx++)
    {
    	if (headCust[idx])
    	{
	        customer* currCust = headCust[idx];
	        while (currCust)
	        {
	            printf("%d. %s\n", idx, currCust->name);
	            currCust = currCust->next;
	        } 
		}
    }
    puts("");
    printf("Press enter to continue...");
    getchar();
    system("cls||clear");
    mainmenu();
}

void order()
{
	system("cls||clear");
    char cust[255];
    int n;
    printf("Insert the customer name: ");
    scanf("%[^\n]", cust); getchar();
    int valid = validCustomer(cust);
    if (valid == 0)
    {
        printf("There's no customer with that name! (Enter to continue)");
        getchar();
        system("cls||clear");
        mainmenu();
    }
    else
    {
       int adaDish=0; 
       printf("Insert the amount of dish: ");
       scanf("%d", &n); getchar();
       for (int i=1; i<=n; i++)
       {
           	char dishname[255];
           	int len;
           	do
           	{
	           printf("[%d] Insert the dish name and quantity: ", i);
	           scanf("%[^\n]", dishname); getchar();
	           len = strlen(dishname);
       		}while (dishname[len-1] < '0' || dishname[len-1] > '9');
           	int qty = dishname[len-1] - '0'; 
           	dishname[len-3] = '\0';
           	dish* currDish = headDish;
           	while (currDish)
           	{
               if ((strcmp(currDish->name, dishname) == 0) && (qty <= currDish->quantity))
               {
                   adaDish = 1;
                   currDish->quantity -= qty;
                   insertOrder(cust, dishname, qty, currDish->price);
               }
               currDish = currDish->next;
           	}
       }
        if (adaDish == 0) 
        {	
            printf("Your input data is incorrect! (Enter to continue)");
            getchar();
            system("cls||clear");
            mainmenu();
       	}
        else
        {
            printf("Order success!\n");
            printf("Press enter to continue...");
            getchar();
            system("cls||clear");
            mainmenu();
        }
    }
}

void payment()
{
    int totalPrice=0;
    int idx;
    int index=1;
    char temp[255] = "ar";
    printf("Insert the customer index: ");
    scanf("%d", &idx); getchar();
    orders* currOrder = headOrder[idx];
    customer* currCust = headCust[idx];
    if (!currCust->name)
    {
        printf("There's no order with that index!\n");
        printf("Press enter to continue...");
        getchar();
        system("cls||clear");
        mainmenu();
    }
    else
    {
        while (currOrder)
        {
            if (strcmp(currOrder->custName, temp) != 0)
            {
                printf("\n%s\n", currOrder->custName);
                totalPrice=0;
                index=1;
            }
            printf("[%d] %s x%d\n", index, currOrder->dishName, currOrder->qty);
            totalPrice += currOrder->price;
            totalPrice *= currOrder->qty;
            strcpy(temp, currOrder->custName);
            currOrder = currOrder->next;
            index++;
        }
        printf("Total: %d\n", totalPrice);
        printf("Press enter to continue...");
        getchar();
        popMidOrder(idx);
        popMidCust(idx);
        system("cls||clear");
        mainmenu();
    }
}

void exit()
{
	system("cls||clear");
    FILE *fp = fopen("ascii.txt", "r");
    char str[2005];
    while (fscanf(fp, "%[^\n]\n", str) != EOF)
    {
    	printf("\t\t%s\n", str);	
    }
    fclose(fp);
}
