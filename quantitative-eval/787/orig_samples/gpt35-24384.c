//FormAI DATASET v1.0 Category: Network Topology Mapper ; Style: invasive
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct node {
    int id;
    char *name;
};

struct edge {
    int from, to;
    int weight;
};

struct graph {
    int num_nodes;
    struct node nodes[MAX_NODES];
    int num_edges;
    struct edge edges[MAX_EDGES];
};

void add_node(struct graph *g, int id, char *name) {
    struct node n;
    n.id = id;
    n.name = strdup(name);
    g->nodes[g->num_nodes++] = n;
}

void add_edge(struct graph *g, int from, int to, int weight) {
    struct edge e;
    e.from = from;
    e.to = to;
    e.weight = weight;
    g->edges[g->num_edges++] = e;
}

void print_graph(struct graph *g) {
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
    struct graph g;
    g.num_nodes = 0;
    g.num_edges = 0;

    add_node(&g, 1, "Router1");
    add_node(&g, 2, "Switch1");
    add_node(&g, 3, "Router2");
    add_node(&g, 4, "Switch2");
    add_node(&g, 5, "Router3");
    add_node(&g, 6, "Server1");
    add_node(&g, 7, "Server2");

    add_edge(&g, 1, 2, 1);
    add_edge(&g, 2, 3, 2);
    add_edge(&g, 2, 4, 4);
    add_edge(&g, 3, 5, 1);
    add_edge(&g, 4, 5, 3);
    add_edge(&g, 4, 6, 2);
    add_edge(&g, 4, 7, 1);

    print_graph(&g);

    return 0;
}