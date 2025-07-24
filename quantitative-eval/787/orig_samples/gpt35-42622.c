//FormAI DATASET v1.0 Category: Database Indexing System ; Style: scalable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 100

// Define a structure to store data for each entry in the database
typedef struct {
    int id;
    char name[50];
    char address[100];
    double salary;
} Employee;

// Define a structure to store index information for each record in the database
typedef struct {
    int id;
    long position;
} IndexEntry;

// Define a structure to store the overall index for the database
typedef struct {
    int num_entries;
    IndexEntry entries[MAX_ENTRIES];
} Index;

// Define a function to read an Employee record from a file
void read_employee(FILE* fp, Employee* employee) {
    fscanf(fp, "%d %s %s %lf", &employee->id, employee->name, employee->address, &employee->salary);
}

// Define a function to write an Employee record to a file
void write_employee(FILE* fp, const Employee* employee) {
    fprintf(fp, "%d %s %s %lf\n", employee->id, employee->name, employee->address, employee->salary);
}

// Define a function to add an entry to the index
void add_index_entry(Index* index, int id, long position) {
    if (index->num_entries < MAX_ENTRIES) {
        index->entries[index->num_entries].id = id;
        index->entries[index->num_entries].position = position;
        index->num_entries++;
    }
}

// Define a function to read an index entry from a file
void read_index_entry(FILE* fp, IndexEntry* entry) {
    fscanf(fp, "%d %ld", &entry->id, &entry->position);
}

// Define a function to write an index entry to a file
void write_index_entry(FILE* fp, const IndexEntry* entry) {
    fprintf(fp, "%d %ld\n", entry->id, entry->position);
}

// Define a function to create the index for the database
void create_index(const char* filename, Index* index) {
    FILE* fp = fopen(filename, "r");
    if (fp) {
        // Read each employee record from the file
        Employee employee;
        long position = 0;
        while (!feof(fp)) {
            // Read the next employee record
            read_employee(fp, &employee);
            // Add an index entry for this record
            add_index_entry(index, employee.id, position);
            // Increment the file position to the next record 
            position = ftell(fp);
        }
        fclose(fp);
    }
}

// Define a function to print an employee record
void print_employee(const Employee* employee) {
    printf("%d %s %s %lf\n", employee->id, employee->name, employee->address, employee->salary);
}

// Define a function to retrieve an employee record by id
void get_employee_by_id(const char* filename, const Index* index, int id, Employee* employee) {
    FILE* fp = fopen(filename, "r");
    if (fp) {
        // Binary search the index for the record with the given id
        int min = 0;
        int max = index->num_entries - 1;
        while (min <= max) {
            int mid = (min + max) / 2;
            if (index->entries[mid].id == id) {
                // Seek to the position of the record in the file
                fseek(fp, index->entries[mid].position, SEEK_SET);
                // Read the record from the file
                read_employee(fp, employee);
                break;
            } else if (index->entries[mid].id < id) {
                min = mid + 1;
            } else {
                max = mid - 1;
            }
        }
        fclose(fp);
    }
}

int main() {
    Index index;
    create_index("employees.txt", &index);
    Employee employee;
    get_employee_by_id("employees.txt", &index, 2, &employee);
    print_employee(&employee);
    return 0;
}