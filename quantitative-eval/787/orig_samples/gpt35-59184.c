//FormAI DATASET v1.0 Category: Network Topology Mapper ; Style: cheerful
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100 // Maximum number of nodes in the network
#define MAX_EDGES 500 // Maximum number of edges in the network

struct node {
    char* name; // Node name
    int id; // Node ID
};

struct edge {
    int from; // ID of the node the edge is coming from
    int to; // ID of the node the edge is going to
    int weight; // Weight of the edge
};

struct graph {
    struct node nodes[MAX_NODES];
    struct edge edges[MAX_EDGES];
    int num_nodes;
    int num_edges;
};

void add_node(struct graph* g, char* name) {
    if (g->num_nodes < MAX_NODES) {
        struct node n;
        n.name = strdup(name);
        n.id = g->num_nodes;
        g->nodes[g->num_nodes++] = n;
    }
    else {
        printf("Error: Maximum number of nodes reached.\n");
    }
}

void add_edge(struct graph* g, int from, int to, int weight) {
    if (g->num_edges < MAX_EDGES) {
        struct edge e;
        e.from = from;
        e.to = to;
        e.weight = weight;
        g->edges[g->num_edges++] = e;
    }
    else {
        printf("Error: Maximum number of edges reached.\n");
    }
}

void print_graph(struct graph* g) {
    printf("Nodes:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        printf("%d: %s\n", g->nodes[i].id, g->nodes[i].name);
    }

    printf("Edges:\n");
    for (int i = 0; i < g->num_edges; i++) {
        printf("%d -> %d (%d)\n", g->edges[i].from, g->edges[i].to, g->edges[i].weight);
    }
}

int main() {
    // Sample graph
    struct graph g;
    g.num_nodes = 0;
    g.num_edges = 0;

    // Add nodes
    add_node(&g, "Server1");
    add_node(&g, "Server2");
    add_node(&g, "Switch1");
    add_node(&g, "Switch2");
    add_node(&g, "Router");

    // Add edges
    add_edge(&g, 0, 2, 10); // Server1 -> Switch1
    add_edge(&g, 1, 2, 15); // Server2 -> Switch1
    add_edge(&g, 2, 3, 5);  // Switch1 -> Switch2
    add_edge(&g, 3, 4, 20); // Switch2 -> Router

    // Print the graph
    print_graph(&g);

    return 0;
}