#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX_N 100
#define MAX_M 100
#define ALPHABET_SIZE 256
#define OPTION 1  // Set to 1 for greedy, 0 for DFS

char input[MAX_N][MAX_M + 1];
char output[MAX_M + 1];
char best_string[MAX_M + 1];
int best_cost = INT_MAX;
int n, m;

// Hamming distance between two strings
int hamming_distance(const char *a, const char *b) {
    int dist = 0;
    for (int i = 0; i < m; i++) {
        if (a[i] != b[i]) dist++;
    }
    return dist;
}

// Greedy approach with step-by-step output
void greedy_closest_string() {
    printf("Step-by-step character selection (Greedy):\n\n");

    for (int i = 0; i < m; i++) {
        int freq[ALPHABET_SIZE] = {0};

        for (int j = 0; j < n; j++) {
            freq[(unsigned char)input[j][i]]++;
        }

        printf("Column %d:\n", i);
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            if (freq[c] > 0)
                printf("  '%c': %d\n", c, freq[c]);
        }

        int max_freq = 0;
        char best_char = input[0][i];
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            if (freq[c] > max_freq) {
                max_freq = freq[c];
                best_char = (char)c;
            }
        }

        output[i] = best_char;
        printf("  → Chosen character: '%c'\n\n", best_char);
    }
    output[m] = '\0';
}

int max_hamming(const char *candidate) {
    int max_dist = 0;
    for (int i = 0; i < n; i++) {
        int dist = hamming_distance(candidate, input[i]);
        if (dist > max_dist)
            max_dist = dist;
    }
    return max_dist;
}

// DFS for brute-force closest string
void dfs(char *current, int pos) {
    if (pos == m) {
        int cost = max_hamming(current);
        printf("Tested: %s | Max Hamming: %d\n", current, cost);
        if (cost < best_cost) {
            best_cost = cost;
            strcpy(best_string, current);
        }
        return;
    }

    int used[ALPHABET_SIZE] = {0};
    for (int i = 0; i < n; i++) {
        char c = input[i][pos];
        if (!used[(unsigned char)c]) {
            used[(unsigned char)c] = 1;
            current[pos] = c;
            current[pos + 1] = '\0';
            dfs(current, pos + 1);
        }
    }
}

int main() {
    n = 5;
    m = 10;
    strcpy(input[0], "GATTACBGDC");
    strcpy(input[1], "GACTACERSA");
    strcpy(input[2], "GCTTGCHBML");
    strcpy(input[3], "GATAGCFDRF");
    strcpy(input[4], "GATTACBGMF");

    printf("Input Strings:\n");
    for (int i = 0; i < n; i++) {
        printf("  %s\n", input[i]);
    }
    printf("\n");

    if (OPTION == 1) {
        greedy_closest_string();
        printf("Final Heuristic Closest String: %s\n", output);

        int max_dist = 0;
        printf("\nHamming Distances to Input Strings:\n");
        for (int i = 0; i < n; i++) {
            int dist = hamming_distance(output, input[i]);
            if (dist > max_dist) max_dist = dist;
            printf("  %s -> %d\n", input[i], dist);
        }
        printf("\nMaximum Hamming Distance: %d\n", max_dist);

    } else {
        char current[MAX_M + 1] = {0};
        printf("Running Brute-force DFS with Debugging...\n\n");
        dfs(current, 0);
        printf("\nBest Brute-force String: %s\n", best_string);
        printf("Minimum Max Hamming Distance: %d\n", best_cost);

        printf("\nHamming Distances to Input Strings:\n");
        for (int i = 0; i < n; i++) {
            int dist = hamming_distance(best_string, input[i]);
            printf("  %s -> %d\n", input[i], dist);
        }
    }

    return 0;
}
