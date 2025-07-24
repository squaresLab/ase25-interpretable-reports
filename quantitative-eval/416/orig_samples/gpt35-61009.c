//FormAI DATASET v1.0 Category: Digital Auction System ; Style: detailed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Defining the maximum number of products and bids */
#define MAX_PRODUCTS 100
#define MAX_BIDS 1000

/* Struct for a product that will be auctioned */
typedef struct {
    int id;
    char name[100];
    char description[500];
    float startingPrice;
    float minimumBidIncrease;
    float currentPrice;
    int totalBids;
    int biddingOpen;
    int winnerId;
} Product;

/* Struct for a bid that will be placed on a product */
typedef struct {
    int id;
    int productId;
    int bidderId;
    float amount;
} Bid;

/* Keeping track of all the products and bids */
Product products[MAX_PRODUCTS];
Bid bids[MAX_BIDS];

/* The number of products and bids currently in the system */
int numProducts = 0;
int numBids = 0;

/* Function to add a new product to the system */
void addProduct() {
    /* Checking if we haven't reached the maximum number of products */
    if (numProducts == MAX_PRODUCTS) {
        printf("Maximum number of products reached.\n");
        return;
    }

    /* Creating a new product object */
    Product p;
    p.id = numProducts + 1;

    /* Collecting input from the user for the product fields */
    printf("Enter the name of the product: ");
    scanf("%s", p.name);
    printf("Enter a description for the product: ");
    scanf("%s", p.description);
    printf("Enter the starting price: ");
    scanf("%f", &p.startingPrice);
    printf("Enter the minimum bid increase: ");
    scanf("%f", &p.minimumBidIncrease);

    /* Setting the product's current price to the starting price */
    p.currentPrice = p.startingPrice;

    /* Setting the total number of bids to 0, the bidding to open, and the winner ID to 0 */
    p.totalBids = 0;
    p.biddingOpen = 1;
    p.winnerId = 0;

    /* Adding the product to the system */
    products[numProducts++] = p;

    printf("Product added successfully.\n");
}

/* Function to close the bidding on a product */
void closeBidding(int productId) {
    /* Finding the product with the given ID */
    int found = 0;
    Product *p;
    for (int i = 0; i < numProducts; i++) {
        if (products[i].id == productId) {
            found = 1;
            p = &products[i];
            break;
        }
    }

    if (!found) {
        printf("Product not found.\n");
        return;
    }

    /* Setting the bidding to closed */
    p->biddingOpen = 0;

    /* Finding the highest bidder */
    Bid *winningBid;
    float highestBid = 0;
    for (int i = 0; i < numBids; i++) {
        if (bids[i].productId == productId && bids[i].amount > highestBid) {
            highestBid = bids[i].amount;
            winningBid = &bids[i];
        }
    }

    /* Setting the winner ID */
    p->winnerId = winningBid->bidderId;

    printf("Bidding closed successfully.\n");
}

/* Function to place a bid on a product */
void placeBid(int bidderId, int productId, float amount) {
    /* Finding the product with the given ID */
    int found = 0;
    Product *p;
    for (int i = 0; i < numProducts; i++) {
        if (products[i].id == productId) {
            found = 1;
            p = &products[i];
            break;
        }
    }

    if (!found) {
        printf("Product not found.\n");
        return;
    }

    /* Checking if bidding is closed on the product */
    if (!p->biddingOpen) {
        printf("Bidding is closed on this product.\n");
        return;
    }

    /* Checking if the amount is higher than the current price + minimum bid increase */
    if (amount < p->currentPrice + p->minimumBidIncrease) {
        printf("Bid amount must be greater than the current price plus minimum bid increase.\n");
        return;
    }

    /* Creating a new bid object */
    Bid b;
    b.id = numBids + 1;
    b.productId = productId;
    b.bidderId = bidderId;
    b.amount = amount;

    /* Updating the product's current price and total bids */
    p->currentPrice = amount;
    p->totalBids++;

    /* Adding the bid to the system */
    bids[numBids++] = b;

    printf("Bid placed successfully.\n");
}

/* Function to display all the products currently in the system */
void displayProducts() {
    printf("Products currently in the system:\n");
    for (int i = 0; i < numProducts; i++) {
        printf("ID: %d\n", products[i].id);
        printf("Name: %s\n", products[i].name);
        printf("Description: %s\n", products[i].description);
        printf("Starting Price: $%.2f\n", products[i].startingPrice);
        printf("Current Price: $%.2f\n", products[i].currentPrice);
        printf("Minimum Bid Increase: $%.2f\n", products[i].minimumBidIncrease);
        printf("Total Bids: %d\n", products[i].totalBids);
        printf("Bidding Open: %s\n", products[i].biddingOpen ? "Yes" : "No");

        if (!products[i].biddingOpen) {
            printf("Winner ID: %d\n", products[i].winnerId);
        }

        printf("\n");
    }
}

int main() {
    /* A simple menu-based program to demonstrate the functions */

    int userId = 1; /* Dummy user ID for the purpose of this example */

    while (1) {
        printf("Menu:\n");
        printf("1. Add product\n");
        printf("2. Close bidding on a product\n");
        printf("3. Place a bid\n");
        printf("4. Display all products\n");
        printf("5. Exit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                printf("Enter the ID of the product to close bidding on: ");
                int productId;
                scanf("%d", &productId);
                closeBidding(productId);
                break;
            case 3:
                printf("Enter the ID of the product to place a bid on: ");
                scanf("%d", &productId);
                printf("Enter the amount of the bid: ");
                float amount;
                scanf("%f", &amount);
                placeBid(userId, productId, amount);
                break;
            case 4:
                displayProducts();
                break;
            case 5:
                return 0;
            default:
                printf("Invalid choice.\n");
        }

        /* Adding a delay of 1 second for demonstration purposes */
        sleep(1);
    }

    return 0;
}