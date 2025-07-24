//FormAI DATASET v1.0 Category: Classical Circuit Simulator ; Style: single-threaded
#include <stdio.h>
#include <stdlib.h>
#define MAXGATES 10
#define MAXWIRES 30

typedef struct Gate {
    int input[2];
    int output;
    char op;
} Gate;

typedef struct Circuit {
    int num_wires;
    int num_gates;
    int gates_wired[MAXGATES][2];
    Gate gates[MAXGATES];
} Circuit;

int evaluate(Circuit *c, int *vals)
{
    int i, j, k, out, in[2], res[MAXGATES];
    for (i = 0; i < c->num_gates; i++) {
        in[0] = in[1] = -1;
        for (j = 0; j < c->num_gates; j++) {
            for (k = 0; k < 2; k++) {
                if (c->gates_wired[j][k] == i) {
                    in[k] = j;
                }
            }
        }
        if (in[0] == -1 || in[1] == -1) {
            out = vals[c->gates[i].input[0]];
        } else {
            out = c->gates[in[0]].output;
            switch (c->gates[i].op) {
                case '&':
                    out &= c->gates[in[1]].output; break;
                case '|':
                    out |= c->gates[in[1]].output; break;
                case '^':
                    out ^= c->gates[in[1]].output; break;
            }
        }
        res[i] = out;
    }
    for (i = 0; i < c->num_gates; i++) {
        c->gates[i].output = res[i];
    }
    return c->gates[c->num_gates - 1].output;
}

int main()
{
    Circuit c;
    c.num_wires = 3;
    c.num_gates = 2;
    c.gates[0].input[0] = 0;
    c.gates[0].input[1] = 1;
    c.gates[0].output = -1;
    c.gates[0].op = '&';
    c.gates[1].input[0] = 1;
    c.gates[1].input[1] = 2;
    c.gates[1].output = -1;
    c.gates[1].op = '|';
    c.gates_wired[0][0] = 1;
    c.gates_wired[0][1] = -1;
    c.gates_wired[1][0] = -1;
    c.gates_wired[1][1] = -1;
    int vals[MAXWIRES] = {1, 0, 1};
    printf("The result of evaluating the circuit is %d\n", evaluate(&c, vals));
    return 0;
}