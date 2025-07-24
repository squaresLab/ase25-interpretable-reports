//FormAI DATASET v1.0 Category: Digital Auction System ; Style: thoughtful
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BIDDERS 10
#define MAX_ITEMS 5

struct Bidder {
    int id;
    char* name;
    float balance;
};

struct Item {
    int id;
    char* name;
    float starting_price;
    struct Bidder* highest_bidder;
    float highest_bid;
};

struct Auction {
    int number_of_items;
    int number_of_bidders;
    struct Item items[MAX_ITEMS];
    struct Bidder bidders[MAX_BIDDERS];
};

// Initializes all the items and bidders in the auction
void initialize_auction(struct Auction* auction) {
    auction->number_of_items = MAX_ITEMS;
    auction->number_of_bidders = MAX_BIDDERS;

    // Creating the items
    for (int i = 0; i < auction->number_of_items; i++) {
        auction->items[i].id = i;
        auction->items[i].name = "Item Name";
        auction->items[i].starting_price = 10.00;
        auction->items[i].highest_bidder = NULL;
        auction->items[i].highest_bid = 0.00;
    }

    // Creating the bidders
    for (int i = 0; i < auction->number_of_bidders; i++) {
        char* name = (char*)calloc(10, sizeof(char));
        sprintf(name, "Bidder #%d", i+1);

        auction->bidders[i].id = i;
        auction->bidders[i].name = name;
        auction->bidders[i].balance = 50.00;
    }
}

// Shows the current state of the auction
void show_auction(struct Auction* auction) {
    printf("AUCTION\n");

    printf("Items:\n");
    for (int i = 0; i < auction->number_of_items; i++) {
        printf("Item #%d: %s\n", auction->items[i].id, auction->items[i].name);

        if (auction->items[i].highest_bidder == NULL) {
            printf("Starting price: $%.2f\n", auction->items[i].starting_price);
        } else {
            printf("Highest bid: $%.2f by %s\n", auction->items[i].highest_bid, auction->items[i].highest_bidder->name);
        }

        printf("------------------------\n");
    }

    printf("Bidders:\n");
    for (int i = 0; i < auction->number_of_bidders; i++) {
        printf("%s - Balance: $%.2f\n", auction->bidders[i].name, auction->bidders[i].balance);
    }
}

// Get the bidder with the given id
struct Bidder* get_bidder(struct Auction* auction, int id) {
    for (int i = 0; i < auction->number_of_bidders; i++) {
        if (auction->bidders[i].id == id) {
            return &auction->bidders[i];
        }
    }

    return NULL;
}

// Get the item with the given id
struct Item* get_item(struct Auction* auction, int id) {
    for (int i = 0; i < auction->number_of_items; i++) {
        if (auction->items[i].id == id) {
            return &auction->items[i];
        }
    }

    return NULL;
}

// Run the auction
void run_auction(struct Auction* auction) {
    int number_of_bids = 0;

    // Randomly select a bidder and an item, and have the bidder bid a random amount
    while (number_of_bids < 20) {
        int bidder_id = rand() % auction->number_of_bidders;
        int item_id = rand() % auction->number_of_items;
        float bid_amount = ((float)(rand() % 100)) + 10.00;

        struct Bidder* bidder = get_bidder(auction, bidder_id);
        struct Item* item = get_item(auction, item_id);

        if (bidder == NULL || item == NULL) {
            continue;
        }

        // If the bidder doesn't have enough balance, skip the bid
        if (bid_amount > bidder->balance) {
            continue;
        }

        // If the bid is lower than the current highest bid, skip the bid
        if (bid_amount <= item->highest_bid) {
            continue;
        }

        // Update the highest bid for the item
        item->highest_bidder = bidder;
        item->highest_bid = bid_amount;

        // Deduct the bid amount from the bidder's balance
        bidder->balance -= bid_amount;

        number_of_bids++;
    }
}

// Main function
int main() {
    srand(time(0));

    struct Auction auction;

    initialize_auction(&auction);
    show_auction(&auction);

    run_auction(&auction);
    printf("\nAuction has ended.\n");
    show_auction(&auction);

    return 0;
}