//FormAI DATASET v1.0 Category: Database Indexing System ; Style: single-threaded
#include <stdio.h>
#include <stdlib.h>

#define MAX_ENTRIES 1000      // Maximum number of entries in the database
#define MAX_INDEX 26          // Maximum number of index entries (A-Z)

struct entry {
    int id;
    char name[50];
    int age;
};

struct index_entry {
    char letter;             // The letter that the entry is indexed by
    struct entry *entries[10];  // Array of pointers to entries
    int count;               // Number of entries currently in the index
};

struct database {
    struct entry entries[MAX_ENTRIES];      // Array of entries in the database
    struct index_entry index[MAX_INDEX];    // Array of index entries
    int count;                              // Number of entries currently in the database
};

void initialize_database(struct database *db) {
    int i, j;
    for(i = 0; i < MAX_INDEX; i++) {
        db->index[i].letter = 'A' + i;
        db->index[i].count = 0;
        for(j = 0; j < 10; j++) {
            db->index[i].entries[j] = NULL;
        }
    }
    db->count = 0;
}

void add_entry(struct database *db, int id, char *name, int age) {
    int index = name[0] - 'A';
    if(db->index[index].count < 10) {
        struct entry *e = (struct entry*)malloc(sizeof(struct entry));
        e->id = id;
        sprintf(e->name, "%s", name);
        e->age = age;
        
        db->index[index].entries[db->index[index].count] = e;
        db->index[index].count++;
        db->entries[db->count] = *e;
        db->count++;
    }
}

void print_entries(struct database *db) {
    int i;
    for(i = 0; i < db->count; i++) {
        printf("ID: %d, Name: %s, Age: %d\n", db->entries[i].id, db->entries[i].name, db->entries[i].age);
    }
}

int main() {
    struct database db;
    initialize_database(&db);
    
    add_entry(&db, 1, "Alice", 25);
    add_entry(&db, 2, "Bob", 30);
    add_entry(&db, 3, "Charlie", 20);
    add_entry(&db, 4, "Dave", 35);
    
    print_entries(&db);
    
    return 0;
}