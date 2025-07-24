//FormAI DATASET v1.0 Category: Classical Circuit Simulator ; Style: complete
#include <stdio.h>
#include <stdlib.h>

//Define Constants
#define MAX_NODES 100
#define MAX_SOURCES 20
#define MAX_PROBES 20

//Define Structs
typedef struct circuit_node{
    int node_id;
    double voltage;
    double current;
} circuit_node;

typedef struct circuit_source{
    int source_id;
    double voltage;
    double resistance;
} circuit_source;

typedef struct circuit_probe{
    int probe_id;
    circuit_node* node;
} circuit_probe;

typedef struct circuit{
    int num_nodes;
    int num_sources;
    int num_probes;
    circuit_node nodes[MAX_NODES];
    circuit_source sources[MAX_SOURCES];
    circuit_probe probes[MAX_PROBES];
} circuit;

//Function Declarations
void print_circuit(circuit* circ);
void simulate_circuit(circuit* circ);

int main(){
    //Create Circuit
    circuit my_circuit;
    my_circuit.num_nodes = 4;
    my_circuit.num_sources = 1;
    my_circuit.num_probes = 2;

    //Set Node IDs
    my_circuit.nodes[0].node_id = 0;
    my_circuit.nodes[1].node_id = 1;
    my_circuit.nodes[2].node_id = 2;
    my_circuit.nodes[3].node_id = 3;

    //Set Source ID
    my_circuit.sources[0].source_id = 0;

    //Connect Sources to Nodes
    my_circuit.nodes[0].current = 0.0;
    my_circuit.nodes[1].current = 0.0;
    my_circuit.sources[0].voltage = 5.0;
    my_circuit.sources[0].resistance = 2.0;
    my_circuit.nodes[0].voltage = my_circuit.sources[0].voltage;
    my_circuit.nodes[1].voltage = 0.0;

    //Set Probes
    my_circuit.probes[0].probe_id = 0;
    my_circuit.probes[0].node = &my_circuit.nodes[0];
    my_circuit.probes[1].probe_id = 1;
    my_circuit.probes[1].node = &my_circuit.nodes[1];

    //Print Circuit
    print_circuit(&my_circuit);

    //Simulate Circuit
    simulate_circuit(&my_circuit);

    return 0;
}

void print_circuit(circuit* circ){
    printf("Circuit Nodes:\n");
    printf("ID\tVoltage\tCurrent\n");
    for(int i=0; i<circ->num_nodes; i++){
        printf("%d\t%.2f\t%.2f\n", circ->nodes[i].node_id,
        circ->nodes[i].voltage, circ->nodes[i].current);
    }

    printf("Circuit Sources:\n");
    printf("ID\tVoltage\tResistance\n");
    for(int i=0; i<circ->num_sources; i++){
        printf("%d\t%.2f\t%.2f\n", circ->sources[i].source_id,
        circ->sources[i].voltage, circ->sources[i].resistance);
    }

    printf("Circuit Probes:\n");
    printf("ID\tConnected to Node ID\tVoltage\n");
    for(int i=0; i<circ->num_probes; i++){
        printf("%d\t%d\t%.2f\n", circ->probes[i].probe_id, 
        circ->probes[i].node->node_id, circ->probes[i].node->voltage);
    }
}

void simulate_circuit(circuit* circ){
    //Initialize Circuit
    for(int i=0; i<circ->num_nodes; i++){
        circ->nodes[i].voltage = 0.0;
        circ->nodes[i].current = 0.0;
    }

    //Calculate Current
    double current = circ->sources[0].voltage / circ->sources[0].resistance;
    circ->nodes[0].current = -current;
    circ->nodes[1].current = current;

    //Calculate Node Voltages
    double resistance = circ->sources[0].resistance;
    double voltage_drop = current * resistance;
    circ->nodes[0].voltage = circ->sources[0].voltage;
    circ->nodes[1].voltage = circ->sources[0].voltage - voltage_drop;

    //Print Final Values
    print_circuit(circ);
}