//FormAI DATASET v1.0 Category: Clustering Algorithm Implementation ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define DIMENSIONS 2
#define MAX_ITEMS 1000

struct item {
    int id;
    double coords[DIMENSIONS];
};

struct cluster {
    int count;
    struct item centroid;
    struct item items[MAX_ITEMS];
};

void init_clusters(struct cluster *clusters, int num_clusters) {
    for (int i = 0; i < num_clusters; i++) {
        clusters[i].count = 0;
        for (int j = 0; j < DIMENSIONS; j++) {
            clusters[i].centroid.coords[j] = 0.0;
        }
    }
}

double distance(struct item a, struct item b) {
    double sum_squares = 0.0;
    for (int i = 0; i < DIMENSIONS; i++) {
        double diff = a.coords[i] - b.coords[i];
        sum_squares += diff * diff;
    }
    return sqrt(sum_squares);
}

int nearest_cluster(struct item data, struct cluster *clusters, int num_clusters) {
    int nearest_index = 0;
    double nearest_distance = distance(data, clusters[0].centroid);
    for (int i = 1; i < num_clusters; i++) {
        double current_distance = distance(data, clusters[i].centroid);
        if (current_distance < nearest_distance) {
            nearest_index = i;
            nearest_distance = current_distance;
        }
    }
    return nearest_index;
}

void assign_to_clusters(struct item *data, int num_items, struct cluster *clusters, int num_clusters) {
    for (int i = 0; i < num_items; i++) {
        int cluster_index = nearest_cluster(data[i], clusters, num_clusters);
        clusters[cluster_index].items[clusters[cluster_index].count] = data[i];
        clusters[cluster_index].count++;
    }
}

void calculate_centroids(struct cluster *clusters, int num_clusters) {
    for (int i = 0; i < num_clusters; i++) {
        for (int j = 0; j < DIMENSIONS; j++) {
            double sum_coords = 0.0;
            for (int k = 0; k < clusters[i].count; k++) {
                sum_coords += clusters[i].items[k].coords[j];
            }
            clusters[i].centroid.coords[j] = sum_coords / clusters[i].count;
        }
    }
}

void print_cluster(struct cluster cl) {
    printf("Cluster with centroid at (");
    for (int i = 0; i < DIMENSIONS; i++) {
        printf("%g", cl.centroid.coords[i]);
        if (i < DIMENSIONS - 1) {
            putchar(',');
        }
    }
    printf(") contains %d items:\n", cl.count);
    for (int i = 0; i < cl.count; i++) {
        printf("    Item %d at (", cl.items[i].id);
        for (int j = 0; j < DIMENSIONS; j++) {
            printf("%g", cl.items[i].coords[j]);
            if (j < DIMENSIONS - 1) {
                putchar(',');
            }
        }
        putchar(')');
        putchar('\n');
    }
}

void print_clusters(struct cluster *clusters, int num_clusters) {
    for (int i = 0; i < num_clusters; i++) {
        print_cluster(clusters[i]);
    }
}

void randomize_data(struct item *data, int num_items) {
    srand(time(NULL));
    for (int i = 0; i < num_items; i++) {
        data[i].id = i;
        for (int j = 0; j < DIMENSIONS; j++) {
            data[i].coords[j] = rand() / (double)RAND_MAX;
        }
    }
}

void cluster_data(struct item *data, int num_items, struct cluster *clusters, int num_clusters) {
    init_clusters(clusters, num_clusters);
    randomize_data(data, num_items);
    assign_to_clusters(data, num_items, clusters, num_clusters);
    calculate_centroids(clusters, num_clusters);
}

int main() {
    int num_items = 100;
    int num_clusters = 3;
    struct item data[num_items];
    struct cluster clusters[num_clusters];
    cluster_data(data, num_items, clusters, num_clusters);
    print_clusters(clusters, num_clusters);
    return 0;
}