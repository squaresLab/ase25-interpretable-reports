//Falcon-180B DATASET v1.0 Category: Database simulation ; Style: minimalist
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 1000
#define MAX_COLS 100

typedef struct {
    char* name;
    int age;
    float salary;
} Record;

typedef struct {
    char* header[MAX_COLS];
    Record rows[MAX_ROWS];
    int num_rows;
    int num_cols;
} Database;

void create_table(Database* db) {
    db->num_rows = 0;
    db->num_cols = 3;
    db->header[0] = "Name";
    db->header[1] = "Age";
    db->header[2] = "Salary";
}

void add_record(Database* db, char* name, int age, float salary) {
    if (db->num_rows >= MAX_ROWS) {
        printf("Error: Table is full.\n");
        return;
    }
    Record* new_row = &db->rows[db->num_rows];
    new_row->name = strdup(name);
    new_row->age = age;
    new_row->salary = salary;
    db->num_rows++;
}

void print_table(Database* db) {
    printf("+----+-----+--------+\n");
    for (int i = 0; i < db->num_cols; i++) {
        printf("| %s |", db->header[i]);
    }
    printf("|\n");
    for (int i = 0; i < db->num_rows; i++) {
        printf("+----+-----+--------+\n");
        printf("| %s | %d | %.2f |\n", db->rows[i].name, db->rows[i].age, db->rows[i].salary);
    }
}

int main() {
    Database db;
    create_table(&db);
    add_record(&db, "John Doe", 25, 50000);
    add_record(&db, "Jane Smith", 30, 60000);
    print_table(&db);
    return 0;
}