//FormAI DATASET v1.0 Category: Classical Circuit Simulator ; Style: all-encompassing
#include<stdio.h>
#include<stdlib.h>

#define MAX_NODES 1000 // Maximum number of nodes in the circuit

// Structure to represent a node in the circuit
typedef struct {
    char type; // Node type: ‘V’ for voltage source, ‘R’ for resistor, ‘C’ for capacitor
    float value; // Node value: voltage, resistance or capacitance depending on the type
    int nodes[2]; // The nodes connected to the component
} Node;

// Structure to represent the circuit
typedef struct {
    Node nodes[MAX_NODES]; // Array of all nodes in the circuit
    int numNodes; // The number of nodes in the circuit
    float voltage[MAX_NODES]; // Array to store voltage values for each node
    float matrix[MAX_NODES][MAX_NODES]; // The circuit matrix
} Circuit;

// Function to initialize the circuit
void initCircuit(Circuit *circuit) {
    circuit->numNodes = 0;
    for(int i=0; i<MAX_NODES; i++) {
        circuit->nodes[i].type = '\0';
        circuit->nodes[i].value = 0.0;
        for(int j=0; j<2; j++) {
            circuit->nodes[i].nodes[j] = -1;
        }
        circuit->voltage[i] = 0.0;
        for(int j=0; j<MAX_NODES; j++) {
            circuit->matrix[i][j] = 0.0;
        }
    }
}

// Function to add a node to the circuit
void addNode(Circuit *circuit, char type, float value, int n1, int n2) {
    Node node;
    node.type = type;
    node.value = value;
    node.nodes[0] = n1;
    node.nodes[1] = n2;

    circuit->nodes[circuit->numNodes] = node;
    circuit->numNodes++;
}

// Function to build the circuit matrix
void buildMatrix(Circuit *circuit) {
    for(int i=0; i<circuit->numNodes; i++) {
        Node node = circuit->nodes[i];

        if(node.type == 'V') {
            circuit->matrix[i][node.nodes[0]] = 1.0;
            circuit->matrix[i][node.nodes[1]] = -1.0;
            circuit->matrix[circuit->numNodes][i] = node.value;
        }
        else if(node.type == 'R') {
            circuit->matrix[i][i] = 1.0/node.value;
            circuit->matrix[i][node.nodes[0]] = -1.0/node.value;
            circuit->matrix[i][node.nodes[1]] = -1.0/node.value;
        }
        else if(node.type == 'C') {
            circuit->matrix[i][i] = node.value;
            circuit->matrix[i][node.nodes[0]] = -1.0;
            circuit->matrix[i][node.nodes[1]] = 1.0;
        }
    }
}

// Function to solve the circuit using Gauss-Jordan elimination
void solveCircuit(Circuit *circuit) {
    for(int i=0; i<circuit->numNodes-1; i++) {
        float max = circuit->matrix[i][i];
        int maxRow = i;
        for(int j=i+1; j<circuit->numNodes; j++) {
            if(circuit->matrix[j][i] > max) {
                max = circuit->matrix[j][i];
                maxRow = j;
            }
        }
        if(maxRow != i) {
            for(int k=0; k<=circuit->numNodes; k++) {
                float temp = circuit->matrix[i][k];
                circuit->matrix[i][k] = circuit->matrix[maxRow][k];
                circuit->matrix[maxRow][k] = temp;
            }
        }
        for(int j=i+1; j<circuit->numNodes; j++) {
            float scale = circuit->matrix[j][i]/circuit->matrix[i][i];
            for(int k=i; k<=circuit->numNodes; k++) {
                circuit->matrix[j][k] -= scale*circuit->matrix[i][k];
            }
        }
    }
    for(int i=circuit->numNodes-1; i>=0; i--) {
        circuit->voltage[i] = circuit->matrix[i][circuit->numNodes]/circuit->matrix[i][i];
        for(int j=i-1; j>=0; j--) {
            circuit->matrix[j][circuit->numNodes] -= circuit->matrix[j][i]*circuit->voltage[i];
            circuit->matrix[j][i] = 0.0;
        }
    }
}

int main() {
    Circuit circuit;
    initCircuit(&circuit);

    // Add nodes to the circuit
    addNode(&circuit, 'V', 12.0, 0, 1);
    addNode(&circuit, 'R', 100.0, 1, 2);
    addNode(&circuit, 'C', 0.001, 2, 3);
    addNode(&circuit, 'R', 150.0, 3, 0);

    // Build the circuit matrix
    buildMatrix(&circuit);

    // Solve the circuit and print the results
    solveCircuit(&circuit);
    printf("Voltage at node 0: %f\n", circuit.voltage[0]);
    printf("Voltage at node 1: %f\n", circuit.voltage[1]);
    printf("Voltage at node 2: %f\n", circuit.voltage[2]);
    printf("Voltage at node 3: %f\n", circuit.voltage[3]);

    return 0;
}