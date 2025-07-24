//FormAI DATASET v1.0 Category: GPS Navigation Simulation ; Style: expert-level
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100 // Maximum number of Nodes
#define INF 0x3f3f3f3f // Macro for infinity

// Structure representing a node in the graph
typedef struct Node {
    int id;
    double lat;
    double lon;
} Node;

// Structure representing an edge in the graph
typedef struct Edge {
    int src;
    int dest;
    double weight;
} Edge;

// Structure representing the graph
typedef struct Graph {
    int num_nodes;
    int num_edges;
    Node nodes[MAX_NODES];
    Edge edges[MAX_NODES * MAX_NODES];
} Graph;

// Function to initialize the graph
void init_graph(Graph *g, int num_nodes, int num_edges) {
    g->num_nodes = num_nodes;
    g->num_edges = num_edges;
    for (int i = 0; i < num_nodes; i++) {
        g->nodes[i].id = i;
        g->nodes[i].lat = (double) rand() / RAND_MAX * 180.0 - 90.0;
        g->nodes[i].lon = (double) rand() / RAND_MAX * 360.0 - 180.0;
    }
    for (int i = 0; i < num_edges; i++) {
        g->edges[i].src = rand() % num_nodes;
        g->edges[i].dest = rand() % num_nodes;
        g->edges[i].weight = (double) rand() / RAND_MAX * 100000.0 + 1.0;
    }
}

// Function to print the graph
void print_graph(Graph *g) {
    printf("Nodes:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        printf("%d: (%f, %f)\n", g->nodes[i].id, g->nodes[i].lat, g->nodes[i].lon);
    }
    printf("Edges:\n");
    for (int i = 0; i < g->num_edges; i++) {
        printf("%d -> %d: %f\n", g->edges[i].src, g->edges[i].dest, g->edges[i].weight);
    }
}

// Function to find the shortest path from source node to destination node using Dijkstra's algorithm
void shortest_path(Graph *g, int src, int dest) {
    double dist[MAX_NODES];
    int prev[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[src] = 0;
    for (int i = 0; i < g->num_nodes; i++) {
        int u = -1;
        double min_dist = INF;
        for (int j = 0; j < g->num_nodes; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                u = j;
                min_dist = dist[j];
            }
        }
        if (u == -1 || u == dest) {
            break;
        }
        visited[u] = 1;
        for (int j = 0; j < g->num_edges; j++) {
            if (g->edges[j].src == u) {
                int v = g->edges[j].dest;
                double weight = g->edges[j].weight;
                double alt = dist[u] + weight;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }
    // Print the shortest path
    printf("Shortest path from Node %d to Node %d:\n", src, dest);
    if (prev[dest] == -1) {
        printf("No path found.\n");
        return;
    }
    int path[MAX_NODES];
    int path_len = 0;
    int node = dest;
    while (node != -1) {
        path[path_len++] = node;
        node = prev[node];
    }
    for (int i = path_len - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i != 0) {
            printf(" -> ");
        }
    }
    printf(", distance: %f\n", dist[dest]);
}

// Main function to run the program
int main() {
    srand(42); // Seed the random number generator
    Graph g;
    int num_nodes = 10;
    int num_edges = 20;
    init_graph(&g, num_nodes, num_edges);
    print_graph(&g);
    shortest_path(&g, 0, 5);
    shortest_path(&g, 3, 7);
    shortest_path(&g, 8, 6);
    return 0;
}