//GEMINI-pro DATASET v1.0 Category: Educational ; Style: shape shifting
#include <stdio.h>
#include <stdlib.h>

// Define a macro to simplify printing
#define PRINTF(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

// Define a struct to represent a shape
typedef struct shape {
    char *name;
    int num_sides;
    int *sides;
} shape_t;

// Create a new shape
shape_t *create_shape(char *name, int num_sides, int *sides) {
    shape_t *shape = malloc(sizeof(shape_t));
    shape->name = name;
    shape->num_sides = num_sides;
    shape->sides = sides;
    return shape;
}

// Free the memory allocated for a shape
void free_shape(shape_t *shape) {
    free(shape->name);
    free(shape->sides);
    free(shape);
}

// Print the details of a shape
void print_shape(shape_t *shape) {
    PRINTF("Shape: %s", shape->name);
    PRINTF("Number of sides: %d", shape->num_sides);
    for (int i = 0; i < shape->num_sides; i++) {
        PRINTF("Side %d: %d", i + 1, shape->sides[i]);
    }
}

// Create a rectangle
shape_t *create_rectangle(char *name, int width, int height) {
    int sides[] = {width, height, width, height};
    return create_shape(name, 4, sides);
}

// Create a circle
shape_t *create_circle(char *name, int radius) {
    int sides[] = {radius};
    return create_shape(name, 1, sides);
}

// Create a triangle
shape_t *create_triangle(char *name, int side1, int side2, int side3) {
    int sides[] = {side1, side2, side3};
    return create_shape(name, 3, sides);
}

// Main function
int main() {
    // Create a rectangle
    shape_t *rectangle = create_rectangle("Rectangle", 5, 3);

    // Create a circle
    shape_t *circle = create_circle("Circle", 4);

    // Create a triangle
    shape_t *triangle = create_triangle("Triangle", 3, 4, 5);

    // Print the details of each shape
    print_shape(rectangle);
    print_shape(circle);
    print_shape(triangle);

    // Free the memory allocated for each shape
    free_shape(rectangle);
    free_shape(circle);
    free_shape(triangle);

    return 0;
}