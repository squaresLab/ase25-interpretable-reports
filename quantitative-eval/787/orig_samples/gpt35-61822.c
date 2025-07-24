//FormAI DATASET v1.0 Category: Hotel Management System ; Style: unmistakable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS 100
#define MAX_CUSTOMERS 200

struct Customer {
    char name[50];
    int age;
    char address[100];
    char phone_number[20];
    int room_number;
};

struct Room {
    int room_number;
    int capacity;
    int price;
    int is_available;
    struct Customer *customer;
};

struct Hotel {
    struct Room rooms[MAX_ROOMS];
    struct Customer customers[MAX_CUSTOMERS];
    int num_rooms;
    int num_customers;
};

void initializeRooms(struct Hotel *hotel) {
    int i;
    for(i = 0; i < MAX_ROOMS; i++) {
        hotel->rooms[i].room_number = i + 1;
        hotel->rooms[i].capacity = (i % 4) + 1;
        hotel->rooms[i].price = (i % 4) * 50 + 100;
        hotel->rooms[i].is_available = 1;
        hotel->rooms[i].customer = NULL;
    }
    hotel->num_rooms = MAX_ROOMS;
}

int findCustomer(struct Hotel *hotel, char *name) {
    int i;
    for(i = 0; i < hotel->num_customers; i++) {
        if(strcmp(hotel->customers[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

struct Customer* addCustomer(struct Hotel *hotel) {
    struct Customer customer;
    printf("Enter customer's name: ");
    scanf("%s", customer.name);
    printf("Enter customer's age: ");
    scanf("%d", &customer.age);
    printf("Enter customer's address: ");
    scanf("%s", customer.address);
    printf("Enter customer's phone number: ");
    scanf("%s", customer.phone_number);
    printf("Enter customer's room number: ");
    scanf("%d", &customer.room_number);
    if(customer.room_number < 1 || customer.room_number > hotel->num_rooms) {
        printf("Invalid room number!\n");
        return NULL;
    }
    if(hotel->rooms[customer.room_number-1].is_available == 0) {
        printf("Room is not available!\n");
        return NULL;
    }
    hotel->rooms[customer.room_number-1].is_available = 0;
    struct Customer *new_customer = &hotel->customers[hotel->num_customers];
    strcpy(new_customer->name, customer.name);
    new_customer->age = customer.age;
    strcpy(new_customer->address, customer.address);
    strcpy(new_customer->phone_number, customer.phone_number);
    new_customer->room_number = customer.room_number;
    hotel->rooms[customer.room_number-1].customer = new_customer;
    hotel->num_customers++;
    printf("Customer added successfully.\n");
    return new_customer;
}

void printCustomer(struct Customer *customer) {
    printf("Name: %s\n", customer->name);
    printf("Age: %d\n", customer->age);
    printf("Address: %s\n", customer->address);
    printf("Phone Number: %s\n", customer->phone_number);
    printf("Room Number: %d\n", customer->room_number);
}

void searchCustomer(struct Hotel *hotel) {
    char search_name[50];
    printf("Enter name to search: ");
    scanf("%s", search_name);
    int i = findCustomer(hotel, search_name);
    if(i == -1) {
        printf("Customer not found!\n");
        return;
    }
    printCustomer(&hotel->customers[i]);
}

void printRooms(struct Hotel *hotel) {
    int i;
    printf("Room\tCapacity\tPrice\tStatus\n");
    for(i = 0; i < hotel->num_rooms; i++) {
        printf("%d\t%d\t\t%d\t", hotel->rooms[i].room_number, hotel->rooms[i].capacity, hotel->rooms[i].price);
        if(hotel->rooms[i].is_available == 1) {
            printf("Available\n");
        } else {
            printf("Occupied by %s\n", hotel->rooms[i].customer->name);
        }
    }
}

void checkout(struct Hotel *hotel) {
    int room_number;
    printf("Enter room number: ");
    scanf("%d", &room_number);
    if(room_number < 1 || room_number > hotel->num_rooms) {
        printf("Invalid room number!\n");
        return;
    }
    if(hotel->rooms[room_number-1].is_available == 1) {
        printf("Room is not occupied!\n");
        return;
    }
    struct Customer *customer = hotel->rooms[room_number-1].customer;
    hotel->rooms[room_number-1].is_available = 1;
    hotel->rooms[room_number-1].customer = NULL;
    printf("Customer %s has checked out of room %d.\n", customer->name, room_number);
}

int main() {
    struct Hotel hotel;
    initializeRooms(&hotel);
    while(1) {
        printf("\nAvailable options:\n1. Add customer\n2. Search customer\n3. Print available rooms\n4. Checkout\n5. Exit\n");
        int option;
        printf("Enter option: ");
        scanf("%d", &option);
        switch(option) {
            case 1:
                addCustomer(&hotel);
                break;
            case 2:
                searchCustomer(&hotel);
                break;
            case 3:
                printRooms(&hotel);
                break;
            case 4:
                checkout(&hotel);
                break;
            case 5:
                printf("Thank you for using the hotel management system.\n");
                exit(0);
                break;
            default:
                printf("Invalid option!\n");
        }
    }
    return 0;
}