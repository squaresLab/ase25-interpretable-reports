//FormAI DATASET v1.0 Category: Text to ASCII art generator ; Style: shape shifting
#include <stdio.h>

void dots(int num) {
    for (int i = 0; i < num; i++) {
        printf(".");
    }
}

void line(int num) {
    printf("+");
    dots(num);
    printf("+\n");
}

void rect(int width, int height) {
    line(width);
    for (int i = 0; i < height; i++) {
        printf("|");
        dots(width);
        printf("|\n");
    }
    line(width);
}

void triangle(int size) {
    for (int i = 1; i <= size; i++) {
        dots(size-i);
        printf("/");
        dots(i*2-2);
        printf("\\\n");
    }
    line(size*2);
}

void diamond(int size) {
    for (int i = 1; i <= size-1; i++) {
        dots(size-i);
        printf("/");
        dots(i*2-2);
        printf("\\\n");
    }
    line(size*2);
    for (int j = size-1; j > 0; j--) {
        dots(size-j);
        printf("\\");
        dots(j*2-2);
        printf("/\n");
    }
    line(size*2);
}

int main() {
    int choice;

    printf("Choose a shape:\n");
    printf("1. Rectangle\n");
    printf("2. Triangle\n");
    printf("3. Diamond\n");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            printf("Enter width and height of rectangle:\n");
            int width, height;
            scanf("%d %d", &width, &height);
            rect(width, height);
            break;
        case 2:
            printf("Enter size of triangle:\n");
            int size;
            scanf("%d", &size);
            triangle(size);
            break;
        case 3:
            printf("Enter size of diamond:\n");
            scanf("%d", &size);
            diamond(size);
            break;
        default:
            printf("Invalid choice.");
    }

    return 0;
}