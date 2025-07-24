//FormAI DATASET v1.0 Category: Network Topology Mapper ; Style: imaginative
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NODES 20        // Maximum number of nodes in the network topology
#define MAX_EDGES 40        // Maximum number of edges in the network topology 

// Definition of a node in the network topology
typedef struct Node {
    int id;             // Unique identifier of the node
    char *name;         // Name of the node
    int num_neighbors;  // Number of neighbors (i.e. adjacent nodes)
    int *neighbors;     // Array of neighbor node ids
} Node;

// Definition of an edge in the network topology
typedef struct Edge {
    int id1;            // Id of the first node connected by the edge
    int id2;            // Id of the second node connected by the edge
    int weight;         // Weight of the edge (used for distance calculations)
} Edge;

// Definition of a network topology
typedef struct Topology {
    int num_nodes;      // Number of nodes in the topology
    int num_edges;      // Number of edges in the topology
    Node nodes[MAX_NODES];  // Array of nodes in the topology
    Edge edges[MAX_EDGES];  // Array of edges in the topology
} Topology;

// Function to add a node to the topology
void add_node(Topology *topology, int id, char *name) {
    int num_nodes = topology->num_nodes;

    if (num_nodes == MAX_NODES) {
        printf("The maximum number of nodes has already been reached!");
        return;
    }

    // Check if the node already exists in the topology
    for (int i = 0; i < num_nodes; i++) {
        if (topology->nodes[i].id == id) {
            printf("A node with the same id already exists in the topology!");
            return;
        }
    }

    // Initialize the new node with the given id and name
    Node new_node;
    new_node.id = id;
    new_node.name = (char *) malloc(strlen(name) + 1);
    strcpy(new_node.name, name);
    new_node.num_neighbors = 0;
    new_node.neighbors = (int *) malloc(sizeof(int) * MAX_NODES);

    // Add the new node to the topology
    topology->nodes[num_nodes] = new_node;
    topology->num_nodes++;
}

// Function to add an edge to the topology
void add_edge(Topology *topology, int id1, int id2, int weight) {
    int num_nodes = topology->num_nodes;
    int num_edges = topology->num_edges;

    if (num_edges == MAX_EDGES) {
        printf("The maximum number of edges has already been reached!");
        return;
    }

    // Check if the nodes exist in the topology
    int index1 = -1, index2 = -1;
    for (int i = 0; i < num_nodes; i++) {
        if (topology->nodes[i].id == id1) {
            index1 = i;
        }
        if (topology->nodes[i].id == id2) {
            index2 = i;
        }
    }

    if (index1 == -1 || index2 == -1) {
        printf("One or both of the nodes does not exist in the topology!");
        return;
    }

    // Check if the edge already exists in the topology
    for (int i = 0; i < num_edges; i++) {
        if ((topology->edges[i].id1 == id1 && topology->edges[i].id2 == id2) ||
            (topology->edges[i].id1 == id2 && topology->edges[i].id2 == id1)) {
            printf("The edge already exists in the topology!");
            return;
        }
    }

    // Add the new edge to the topology
    Edge new_edge;
    new_edge.id1 = id1;
    new_edge.id2 = id2;
    new_edge.weight = weight;

    topology->edges[num_edges] = new_edge;
    topology->num_edges++;

    // Update the adjacency lists of the nodes
    topology->nodes[index1].neighbors[topology->nodes[index1].num_neighbors] = id2;
    topology->nodes[index1].num_neighbors++;
    topology->nodes[index2].neighbors[topology->nodes[index2].num_neighbors] = id1;
    topology->nodes[index2].num_neighbors++;
}

// Function to print the network topology
void print_topology(Topology *topology) {
    int num_nodes = topology->num_nodes;
    int num_edges = topology->num_edges;

    printf("Network Topology:\n\n");

    // Print the nodes
    printf("Nodes:\n");
    for (int i = 0; i < num_nodes; i++) {
        printf("%d. %s (id: %d)\n", i+1, topology->nodes[i].name, topology->nodes[i].id);
    }
    printf("\n");

    // Print the edges
    printf("Edges:\n");
    for (int i = 0; i < num_edges; i++) {
        printf("%d. %d-%d (weight: %d)\n", i+1, topology->edges[i].id1, topology->edges[i].id2, topology->edges[i].weight);
    }
    printf("\n");

    // Print the adjacency lists
    printf("Adjacency Lists:\n");
    for (int i = 0; i < num_nodes; i++) {
        printf("%d. %d (%s): ", i+1, topology->nodes[i].id, topology->nodes[i].name);
        for (int j = 0; j < topology->nodes[i].num_neighbors; j++) {
            printf("%d ", topology->nodes[i].neighbors[j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to perform a depth-first search on the network topology
void dfs(Topology *topology, int curr_node, bool *visited) {
    int num_nodes = topology->num_nodes;

    visited[curr_node] = true;
    printf("%d ", topology->nodes[curr_node].id);

    for (int i = 0; i < topology->nodes[curr_node].num_neighbors; i++) {
        int neighbor = topology->nodes[curr_node].neighbors[i];

        for (int j = 0; j < num_nodes; j++) {
            if (visited[j] == false && topology->nodes[j].id == neighbor) {
                dfs(topology, j, visited);
                break;
            }
        }
    }
}

// Function to perform a breadth-first search on the network topology
void bfs(Topology *topology, int start_node) {
    int num_nodes = topology->num_nodes;

    int queue[MAX_NODES];
    int front = 0, rear = 0;

    bool visited[MAX_NODES];
    memset(visited, false, sizeof(visited));

    // Enqueue the start node
    queue[rear++] = start_node;
    visited[start_node] = true;

    // Perform the BFS
    while (front < rear) {
        int curr_node = queue[front++];
        printf("%d ", topology->nodes[curr_node].id);

        for (int i = 0; i < topology->nodes[curr_node].num_neighbors; i++) {
            int neighbor = topology->nodes[curr_node].neighbors[i];

            for (int j = 0; j < num_nodes; j++) {
                if (visited[j] == false && topology->nodes[j].id == neighbor) {
                    queue[rear++] = j;
                    visited[j] = true;
                    break;
                }
            }
        }
    }
}

// Example usage of the network topology mapper
int main() {

    Topology topology;

    // Add some nodes to the topology
    add_node(&topology, 1, "A");
    add_node(&topology, 2, "B");
    add_node(&topology, 3, "C");

    // Add some edges to the topology
    add_edge(&topology, 1, 2, 1);
    add_edge(&topology, 1, 3, 2);
    add_edge(&topology, 2, 3, 3);

    // Print the network topology
    print_topology(&topology);

    // Perform a depth-first search
    printf("Depth-First Search (starting from node 1): ");
    bool visited[MAX_NODES];
    memset(visited, false, sizeof(visited));
    dfs(&topology, 0, visited);
    printf("\n");

    // Perform a breadth-first search
    printf("Breadth-First Search (starting from node 1): ");
    bfs(&topology, 0);
    printf("\n");

    return 0;
}