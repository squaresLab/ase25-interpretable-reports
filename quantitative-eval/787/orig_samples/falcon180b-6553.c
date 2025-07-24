//Falcon-180B DATASET v1.0 Category: Network Topology Mapper ; Style: dynamic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_LINKS 100

//Node structure
struct node{
    int id;
    char name[20];
    int num_links;
    struct node *links[MAX_LINKS];
};

//Function to add a link to the node
void addLink(struct node *node, struct node *link){
    node->num_links++;
    node->links[node->num_links-1] = link;
}

//Function to display the topology
void displayTopology(struct node *nodes){
    int i;
    printf("Topology:\n");
    for(i=0;i<MAX_NODES;i++){
        if(nodes[i].id!= -1){
            printf("Node %d: %s\n",nodes[i].id,nodes[i].name);
            printf("Links:\n");
            int j;
            for(j=0;j<nodes[i].num_links;j++){
                printf("\t%d\n",nodes[i].links[j]->id);
            }
        }
    }
}

//Function to create a new node
struct node* createNode(int id, char *name){
    struct node *node = (struct node*)malloc(sizeof(struct node));
    node->id = id;
    strcpy(node->name,name);
    node->num_links = 0;
    int i;
    for(i=0;i<MAX_LINKS;i++){
        node->links[i] = NULL;
    }
    return node;
}

//Main function
int main(){
    struct node nodes[MAX_NODES];
    int num_nodes = 0;
    int choice;
    char name[20];
    do{
        printf("Enter 1 to add a node\n");
        printf("Enter 2 to add a link\n");
        printf("Enter 3 to display topology\n");
        printf("Enter 4 to exit\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                printf("Enter node ID: ");
                scanf("%d",&nodes[num_nodes].id);
                printf("Enter node name: ");
                scanf("%s",nodes[num_nodes].name);
                num_nodes++;
                break;
            case 2:
                printf("Enter source node ID: ");
                scanf("%d",&nodes[num_nodes-1].id);
                printf("Enter source node name: ");
                scanf("%s",nodes[num_nodes-1].name);
                printf("Enter destination node ID: ");
                scanf("%d",&nodes[num_nodes].id);
                printf("Enter destination node name: ");
                scanf("%s",nodes[num_nodes].name);
                addLink(&nodes[num_nodes-1],&nodes[num_nodes]);
                num_nodes++;
                break;
            case 3:
                displayTopology(nodes);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }while(choice!= 4);
    return 0;
}