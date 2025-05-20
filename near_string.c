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
    printf("Step-by-step character selection (Hybrid Greedy):\n\n");

    for (int i = 0; i < m; i++) {
        int freq[ALPHABET_SIZE] = {0};
        char candidates[ALPHABET_SIZE];
        int candidate_count = 0;

        // Step 1: Count frequency of each character in this column
        for (int j = 0; j < n; j++) {
            unsigned char c = input[j][i];
            if (freq[c] == 0) {
                candidates[candidate_count++] = c;
            }
            freq[c]++;
        }

        printf("Column %d:\n", i);
        for (int k = 0; k < candidate_count; k++) {
            char c = candidates[k];
            int estimated_max_hamming = n - freq[(unsigned char)c];
            printf("  '%c': freq = %d, est max Hamming = %d\n", c, freq[(unsigned char)c], estimated_max_hamming);
        }

        // Step 2–3: Select candidates with lowest estimated max Hamming
        int min_est_hamming = INT_MAX;
        char tied_chars[ALPHABET_SIZE];
        int tie_count = 0;

        for (int k = 0; k < candidate_count; k++) {
            char c = candidates[k];
            int est = n - freq[(unsigned char)c];
            if (est < min_est_hamming) {
                min_est_hamming = est;
                tie_count = 0;
                tied_chars[tie_count++] = c;
            } else if (est == min_est_hamming) {
                tied_chars[tie_count++] = c;
            }
        }

        // Step 4: Tie-breaking
        char final_char = 127;  // High ASCII init
        char first_input_char = input[0][i];

        // Prefer character from first string if tied
        for (int k = 0; k < tie_count; k++) {
            if (tied_chars[k] == first_input_char) {
                final_char = tied_chars[k];
                break;
            }
        }

        // If not found, pick lexicographically smallest
        if (final_char == 127) {
            for (int k = 0; k < tie_count; k++) {
                if (tied_chars[k] < final_char) {
                    final_char = tied_chars[k];
                }
            }
        }

        output[i] = final_char;
        printf("  → Chosen character: '%c'\n\n", final_char);
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

void read_config_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open config file");
        return;
    }

    fscanf(fp, "%d", &n);
    fscanf(fp, "%d", &m);
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%s", input[i]);
    }

    fclose(fp);
}

int main() {
    read_config_file("config.txt");

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
