//Falcon-180B DATASET v1.0 Category: Stock market tracker ; Style: puzzling
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX 1000

struct company{
    char name[20];
    char symbol[10];
    float price;
    int shares;
};

void add_company(struct company companies[], int n){
    printf("Enter company name: ");
    scanf("%s", companies[n].name);
    printf("Enter company symbol: ");
    scanf("%s", companies[n].symbol);
    printf("Enter company price per share: ");
    scanf("%f", &companies[n].price);
    printf("Enter number of shares: ");
    scanf("%d", &companies[n].shares);
}

void display_companies(struct company companies[], int n){
    printf("\nCompany\tSymbol\tPrice per share\tShares\n");
    for(int i=0; i<n; i++){
        printf("%s\t%s\t%.2f\t%d\n", companies[i].name, companies[i].symbol, companies[i].price, companies[i].shares);
    }
}

void buy_shares(struct company companies[], int n, char symbol[]){
    int found = 0;
    for(int i=0; i<n; i++){
        if(strcmp(companies[i].symbol, symbol) == 0){
            printf("\nEnter number of shares to buy: ");
            scanf("%d", &companies[i].shares);
            found = 1;
            break;
        }
    }
    if(found == 0){
        printf("\nCompany not found!");
    }
}

void sell_shares(struct company companies[], int n, char symbol[]){
    int found = 0;
    for(int i=0; i<n; i++){
        if(strcmp(companies[i].symbol, symbol) == 0){
            printf("\nEnter number of shares to sell: ");
            scanf("%d", &companies[i].shares);
            found = 1;
            break;
        }
    }
    if(found == 0){
        printf("\nCompany not found!");
    }
}

void main(){
    struct company companies[MAX];
    int n = 0;

    printf("\nWelcome to the Stock Market Tracker!\n");

    while(n<MAX){
        printf("\nDo you want to add a company? (y/n): ");
        char choice;
        scanf("%c", &choice);

        if(choice == 'y' || choice == 'Y'){
            add_company(companies, n);
            n++;
        }
        else{
            break;
        }
    }

    while(1){
        system("clear");
        printf("\nCompany\tSymbol\tPrice per share\tShares\n");
        display_companies(companies, n);

        printf("\nWhat do you want to do?\n1. Buy shares\n2. Sell shares\n3. Exit\n");
        int choice;
        scanf("%d", &choice);

        switch(choice){
            case 1:
                printf("\nEnter company symbol: ");
                char symbol[10];
                scanf("%s", symbol);
                buy_shares(companies, n, symbol);
                break;

            case 2:
                printf("\nEnter company symbol: ");
                scanf("%s", symbol);
                sell_shares(companies, n, symbol);
                break;

            case 3:
                exit(0);

            default:
                printf("\nInvalid choice!");
        }
    }
}