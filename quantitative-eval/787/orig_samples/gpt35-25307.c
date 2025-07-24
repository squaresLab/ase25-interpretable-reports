//FormAI DATASET v1.0 Category: Graph Coloring Problem ; Style: calm
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

/* Each vertex is represented by a color */
typedef enum {
    RED,
    GREEN,
    BLUE,
    YELLOW
} vertex_color;

/* Each vertex in the graph has a unique name */
typedef struct {
    char name;
    vertex_color color;
    int num_adj_vertices; /* number of adjacent vertices */
    int adj_vertices[MAX_VERTICES]; /* indices of adjacent vertices */
} vertex;

/* Initialize the vertices in the input graph */
void init_vertices(vertex vertices[], int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        printf("Enter the name of vertex %d: ", i+1);
        scanf(" %c", &vertices[i].name);
        vertices[i].color = RED; /* set default color to red */
        vertices[i].num_adj_vertices = 0; /* initialize adjacents to 0 */
    }
}

/* Add an edge between two vertices */
void add_edge(vertex vertices[], int u, int v) {
    vertices[u].adj_vertices[vertices[u].num_adj_vertices++] = v;
    vertices[v].adj_vertices[vertices[v].num_adj_vertices++] = u;
}

/* Check if assigning a given color to a vertex is valid */
int is_valid_color(vertex v, vertex_color color) {
    for (int i = 0; i < v.num_adj_vertices; i++) {
        if (v.adj_vertices[i] != -1 && v.color == color) {
            return 0; /* color already used by adjacent vertex */
        }
    }
    return 1;
}

/* Assign colors to each vertex in the graph */
void color_graph(vertex vertices[], int num_vertices) {
    int i, j;
    for (i = 0; i < num_vertices; i++) {
        for (j = 0; j < 4; j++) {
            if (is_valid_color(vertices[i], j)) {
                /* assign color and move on to next vertex */
                vertices[i].color = j;
                break;
            }
        }
        if (j == 4) { /* couldn't find a valid color */
            printf("Error: no color available for vertex %c\n", vertices[i].name);
            exit(1);
        }
    }
}

/* Print the colors of each vertex */
void print_colors(vertex vertices[], int num_vertices) {
    printf("The colors assigned to each vertex are:\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("%c: ", vertices[i].name);
        switch (vertices[i].color) {
            case RED:
                printf("red\n");
                break;
            case GREEN:
                printf("green\n");
                break;
            case BLUE:
                printf("blue\n");
                break;
            case YELLOW:
                printf("yellow\n");
                break;
        }
    }
}

int main() {
    int num_vertices, num_edges, u, v;
    printf("Enter the number of vertices: ");
    scanf("%d", &num_vertices);
    vertex vertices[num_vertices];
    init_vertices(vertices, num_vertices);
    printf("Enter the number of edges: ");
    scanf("%d", &num_edges);
    for (int i = 0; i < num_edges; i++) {
        printf("Enter the indices of edge %d: ", i+1);
        scanf("%d %d", &u, &v);
        add_edge(vertices, u, v);
    }
    color_graph(vertices, num_vertices);
    print_colors(vertices, num_vertices);
    
    return 0;
}