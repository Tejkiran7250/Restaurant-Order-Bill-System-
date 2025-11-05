#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct OrderItem {
    char itemName[50];
    int quantity;
    double price;
    struct OrderItem* next;
} OrderItem;

typedef struct Bill {
    double totalAmount;
    OrderItem* order_top;
} Bill;

Bill* createBill();
void displayMenu();
void addItem(Bill* myBill);
void undoLastItem(Bill* myBill);
void viewLastItem(Bill* myBill);
void viewFullBill(Bill* myBill);
void cleanup(Bill* myBill);
void printMainMenu();
int getValidIntInput();

int main() {
    Bill* myBill = createBill();
    int choice = 0;

    printf("Welcome to the Restaurant Billing System!\n");
    printf("Created by P Karthik & A Tej Kiran Reddy\n");

    while (1) {
        printMainMenu();
        choice = getValidIntInput();

        switch (choice) {
            case 1:
                addItem(myBill);
                break;
            case 2:
                viewFullBill(myBill);
                break;
            case 3:
                viewLastItem(myBill);
                break;
            case 4:
                undoLastItem(myBill);
                break;
            case 5:
                printf("Thank you for using the system.\n");
                cleanup(myBill);
                return 0;
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
        printf("\n");
    }

    return 0; 
}

void printMainMenu() {
    printf("----------------------------------------\n");
    printf("Main Menu:\n");
    printf("1. Add Item to Bill\n");
    printf("2. View Full Bill\n");
    printf("3. View Last Item Added\n");
    printf("4. Remove Last Item (Undo)\n");
    printf("5. Exit\n");
    printf("----------------------------------------\n");
    printf("Enter your choice: ");
}

int getValidIntInput() {
    int value;
    char buffer[100]; 

    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (sscanf(buffer, "%d", &value) == 1) {
            return value;
        }
    }
    return -1;
}

Bill* createBill() {
    Bill* myBill = (Bill*)malloc(sizeof(Bill));
    if (myBill == NULL) {
        printf("FATAL ERROR: Could not allocate memory for the bill.\n");
        exit(1); 
    }
    myBill->totalAmount = 0.0;
    myBill->order_top = NULL;
    return myBill;
}

void displayMenu() {
    printf("--- Restaurant Menu ---\n");
    printf("1. Pizza    - ₹350.00\n");
    printf("2. Burger   - ₹150.00\n");
    printf("3. Pasta    - ₹250.00\n");
    printf("4. Coke     - ₹60.00\n");
    printf("5. Sandwich - ₹120.00\n");
    printf("-----------------------\n");
}

void addItem(Bill* myBill) {
    displayMenu();
    int choice = 0;
    int quantity = 0;
    char itemName[50];
    double unitPrice = 0.0;
    double itemTotalPrice = 0.0;

    printf("Enter item number: ");
    choice = getValidIntInput();

    printf("Enter quantity: ");
    quantity = getValidIntInput();

    if (quantity <= 0) {
        printf("Invalid quantity. Must be 1 or more. Returning to menu.\n");
        return;
    }

    switch (choice) {
        case 1:
            strcpy(itemName, "Pizza");
            unitPrice = 350.00;
            break;
        case 2:
            strcpy(itemName, "Burger");
            unitPrice = 150.00;
            break;
        case 3:
            strcpy(itemName, "Pasta");
            unitPrice = 250.00;
            break;
        case 4:
            strcpy(itemName, "Coke");
            unitPrice = 60.00;
            break;
        case 5:
            strcpy(itemName, "Sandwich");
            unitPrice = 120.00;
            break;
        default:
            printf("Invalid item choice. Returning to menu.\n");
            return;
    }

    itemTotalPrice = unitPrice * quantity;

    OrderItem* newNode = (OrderItem*)malloc(sizeof(OrderItem));
    if (newNode == NULL) {
        printf("ERROR: Could not allocate memory for new item.\n");
        return; 
    }

    strcpy(newNode->itemName, itemName);
    newNode->quantity = quantity;
    newNode->price = itemTotalPrice;

    newNode->next = myBill->order_top;
    myBill->order_top = newNode;

    myBill->totalAmount += itemTotalPrice;

    printf("\nSUCCESS: Added %d x %s.\n", quantity, itemName);
    printf("New Total Amount: ₹%.2f\n", myBill->totalAmount);
}

void undoLastItem(Bill* myBill) {
    if (myBill->order_top == NULL) {
        printf("No items in the bill to undo.\n");
        return;
    }

    OrderItem* nodeToFree = myBill->order_top;

    double removedPrice = nodeToFree->price;
    char removedName[50];
    strcpy(removedName, nodeToFree->itemName);
    int removedQty = nodeToFree->quantity;

    myBill->order_top = nodeToFree->next;

    myBill->totalAmount -= removedPrice;

    free(nodeToFree);

    printf("\nUNDO: Removed %d x %s (worth ₹%.2f).\n", removedQty, removedName, removedPrice);
    printf("New Total Amount: ₹%.2f\n", myBill->totalAmount);
}

void viewLastItem(Bill* myBill) {
    if (myBill->order_top == NULL) {
        printf("No items in the bill yet.\n");
        return;
    }

    OrderItem* topItem = myBill->order_top;
    printf("--- Last Item Added ---\n");
    printf("Item:     %s\n", topItem->itemName);
    printf("Quantity: %d\n", topItem->quantity);
    printf("Price:    ₹%.2f\n", topItem->price);
}

void viewFullBill(Bill* myBill) {
    OrderItem* current = myBill->order_top;

    printf("\n========== FULL BILL ==========\n");

    if (current == NULL) {
        printf("The bill is currently empty.\n");
    } else {
        while (current != NULL) {
            printf("Item: %-10s | Qty: %-3d | Price: ₹%.2f\n",
                   current->itemName, current->quantity, current->price);
            current = current->next;
        }
    }

    printf("===============================\n");
    printf("FINAL TOTAL: ₹%.2f\n", myBill->totalAmount);
    printf("===============================\n");
}

void cleanup(Bill* myBill) {
    printf("Cleaning up order history...\n");
    OrderItem* current = myBill->order_top;
    OrderItem* tempNode;

    while (current != NULL) {
        tempNode = current;
        current = current->next;
        free(tempNode);
    }

    free(myBill);
    printf("Cleanup complete. Goodbye!\n");
}

