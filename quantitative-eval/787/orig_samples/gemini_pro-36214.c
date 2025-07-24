//GEMINI-pro DATASET v1.0 Category: Electronic Voting System ; Style: irregular
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CANDIDATES 10
#define MAX_VOTERS 100

struct candidate {
    int id;
    char *name;
    int num_votes;
};

struct voter {
    int id;
    char *name;
    bool has_voted;
};

struct election {
    struct candidate candidates[MAX_CANDIDATES];
    int num_candidates;
    struct voter voters[MAX_VOTERS];
    int num_voters;
};

void create_election(struct election *election) {
    // Create the candidates
    election->num_candidates = 0;
    for (int i = 0; i < MAX_CANDIDATES; i++) {
        election->candidates[i].id = i + 1;
        election->candidates[i].name = malloc(100);
        election->candidates[i].num_votes = 0;
    }

    // Create the voters
    election->num_voters = 0;
    for (int i = 0; i < MAX_VOTERS; i++) {
        election->voters[i].id = i + 1;
        election->voters[i].name = malloc(100);
        election->voters[i].has_voted = false;
    }
}

void destroy_election(struct election *election) {
    // Free the memory allocated for the candidates
    for (int i = 0; i < MAX_CANDIDATES; i++) {
        free(election->candidates[i].name);
    }

    // Free the memory allocated for the voters
    for (int i = 0; i < MAX_VOTERS; i++) {
        free(election->voters[i].name);
    }
}

void print_election(struct election *election) {
    // Print the candidates
    printf("Candidates:\n");
    for (int i = 0; i < election->num_candidates; i++) {
        printf("%d. %s\n", election->candidates[i].id, election->candidates[i].name);
    }

    // Print the voters
    printf("\nVoters:\n");
    for (int i = 0; i < election->num_voters; i++) {
        printf("%d. %s\n", election->voters[i].id, election->voters[i].name);
    }
}

void vote(struct election *election, int voter_id, int candidate_id) {
    // Get the voter
    struct voter *voter = NULL;
    for (int i = 0; i < election->num_voters; i++) {
        if (election->voters[i].id == voter_id) {
            voter = &election->voters[i];
            break;
        }
    }

    // Check if the voter has already voted
    if (voter->has_voted) {
        printf("Error: Voter has already voted.\n");
        return;
    }

    // Get the candidate
    struct candidate *candidate = NULL;
    for (int i = 0; i < election->num_candidates; i++) {
        if (election->candidates[i].id == candidate_id) {
            candidate = &election->candidates[i];
            break;
        }
    }

    // Increment the candidate's vote count
    candidate->num_votes++;

    // Mark the voter as having voted
    voter->has_voted = true;
}

void get_winner(struct election *election) {
    // Find the candidate with the most votes
    int max_votes = 0;
    struct candidate *winner = NULL;
    for (int i = 0; i < election->num_candidates; i++) {
        if (election->candidates[i].num_votes > max_votes) {
            max_votes = election->candidates[i].num_votes;
            winner = &election->candidates[i];
        }
    }

    // Print the winner
    printf("The winner is: %s\n", winner->name);
}
int main() {
    // Create the election
    struct election election;
    create_election(&election);

    // Print the election
    print_election(&election);

    // Vote
    vote(&election, 1, 1);
    vote(&election, 2, 2);
    vote(&election, 3, 3);

    // Get the winner
    get_winner(&election);

    // Destroy the election
    destroy_election(&election);

    return 0;
}