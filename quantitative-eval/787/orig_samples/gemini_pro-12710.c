//GEMINI-pro DATASET v1.0 Category: Medical Store Management System ; Style: medieval
// Hark! Ye seekers of medicinal remedies, gather 'round!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Huzzah! Our humble apothecary, where healing herbs and elixirs reside.
typedef struct {
    char *name;
    char *description;
    int quantity;
} Medicine;

// Lo, our trusty list of medicines, ready at thy behest.
Medicine medicines[] = {
    {"Healing Salve", "A balm for thy wounds, gentle and soothing", 20},
    {"Strength Potion", "Quenches thy thirst for vigor, grants thee power", 15},
    {"Mana Elixir", "Invigorates thy mind, sharpens thy wit", 10},
    {"Antivenom", "Counteracts the venom's bite, saves thee from death's door", 5},
    {"Rejuvenation Potion", "Restores thy youth, banishes aches and pains", 3}
};

// A ledger to track our transactions, for the eyes of the realm's healers.
typedef struct {
    char *patientName;
    Medicine *medicine;
    int quantity;
    char *date;
} Transaction;

// Now, let us begin our noble quest to manage the medical store.
int main() {
    // Greet our worthy patrons.
    printf("Welcome to the Medical Store of Ye Olde Apothecary!\n");

    // Allow them to peruse our wares.
    printf("Behold, our humble offerings:\n");
    for (int i = 0; i < sizeof(medicines) / sizeof(Medicine); i++) {
        printf("%s:\n", medicines[i].name);
        printf("    %s\n", medicines[i].description);
        printf("    Quantity: %d\n\n", medicines[i].quantity);
    }

    // Lend them an ear, asking for their needs.
    char patientName[100];
    Medicine *medicine;
    int quantity;
    printf("What ails thee, good sir or madam? Enter thy name:\n");
    scanf("%s", patientName);

    // Determine the medicine they seek.
    char medicineName[100];
    printf("What medicine dost thou require? Enter its name:\n");
    scanf("%s", medicineName);

    // Search our humble abode for the desired elixir.
    for (int i = 0; i < sizeof(medicines) / sizeof(Medicine); i++) {
        if (strcmp(medicineName, medicines[i].name) == 0) {
            medicine = &medicines[i];
            break;
        }
    }

    // Alas, if the medicine is not found, we must lament.
    if (medicine == NULL) {
        printf("Alas! We regret to inform thee that we lack the medicine thou seekest.\n");
        return 0;
    }

    // Confirm the quantity they desire.
    printf("How many potions desirest thou? Enter the quantity:\n");
    scanf("%d", &quantity);

    // Check if our humble abode has enough stock.
    if (quantity > medicine->quantity) {
        printf("We apologize, but we have but %d of %s in stock.\n", medicine->quantity, medicine->name);
        return 0;
    }

    // Rejoice! We have the medicine and the quantity they desire.
    printf("Thy request has been granted, good sir or madam!\n");

    // Deduct the quantity from our stock.
    medicine->quantity -= quantity;

    // Record the transaction for posterity.
    Transaction transaction;
    transaction.patientName = malloc(strlen(patientName) + 1);
    strcpy(transaction.patientName, patientName);
    transaction.medicine = medicine;
    transaction.quantity = quantity;
    transaction.date = malloc(11);
    strcpy(transaction.date, "Ye Olde Date"); // Thou shalt fill this in later, fair scribe.

    // Bid farewell to our esteemed patron.
    printf("May thy ailments be cast away! Fare thee well!\n");

    // Release our resources, for the realm's healers await our aid.
    free(transaction.patientName);
    free(transaction.date);

    return 0;
}