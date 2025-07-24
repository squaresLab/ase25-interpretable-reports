//Falcon-180B DATASET v1.0 Category: Natural Language Date Converter ; Style: secure
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_STRING_LENGTH 100

struct tm parse_date(char* date_string);
int is_valid_date(struct tm date);
char* format_date(struct tm date, char* format_string);

int main() {
    char input_date[MAX_STRING_LENGTH];
    char output_date[MAX_STRING_LENGTH];
    char format_string[MAX_STRING_LENGTH];

    printf("Enter a date (YYYY-MM-DD): ");
    scanf("%s", input_date);

    printf("Enter an output format (YYYY-MM-DD): ");
    scanf("%s", format_string);

    struct tm date = parse_date(input_date);

    if (!is_valid_date(date)) {
        printf("Invalid date.\n");
        return 1;
    }

    strcpy(output_date, format_date(date, format_string));
    printf("Output date: %s\n", output_date);

    return 0;
}

struct tm parse_date(char* date_string) {
    struct tm date = {0};
    int year, month, day;

    if (sscanf(date_string, "%d-%d-%d", &year, &month, &day)!= 3) {
        return date;
    }

    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;

    return date;
}

int is_valid_date(struct tm date) {
    time_t timestamp = mktime(&date);

    if (timestamp == -1) {
        return 0;
    }

    return 1;
}

char* format_date(struct tm date, char* format_string) {
    char output_date[MAX_STRING_LENGTH] = {0};
    strftime(output_date, MAX_STRING_LENGTH, format_string, &date);

    return output_date;
}