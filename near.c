#include <stdio.h>
#include <string.h>

#define MAX_N 100
#define MAX_M 100
#define ALPHABET_SIZE 256

char input[MAX_N][MAX_M + 1];
char output[MAX_M + 1];
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
void greedy_closest_string() {    //greedy since ndi nmn ineexhaust lahat para makita lahat ng results, most frequent per col, brute force backtracking wit pruning
    //applicable dito dfs
    //complexity of (O (n * m))
    printf("Step-by-step character selection:\n\n");

    for (int i = 0; i < m; i++) {
        int freq[ALPHABET_SIZE] = {0};

        // Count frequency of each character in column i
        for (int j = 0; j < n; j++) {
            freq[(unsigned char)input[j][i]]++;
        }

        // Display frequencies for the column
        printf("Column %d:\n", i);
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            if (freq[c] > 0)
                printf("  '%c': %d\n", c, freq[c]);
        }

        // Select the most frequent character
        int max_freq = 0;
        char best_char = input[0][i];  // default choice
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            if (freq[c] > max_freq) {
                max_freq = freq[c];
                best_char = (char)c;
            }
        }

        output[i] = best_char;

        printf("  → Chosen character: '%c'\n\n", best_char);
    }
    output[m] = '\0'; // Null-terminate the resulting string
}

int main() {
    //mas okay ito if mag reread sa file, gawin ko later
    n = 5;  //input strings to process
    m = 10;   //string lenght
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

    // Run the greedy heuristic
    greedy_closest_string();

    // Output final result
    printf("Final Heuristic Closest String: %s\n", output);

    // Show distances to each input string
    int max_dist = 0;
    printf("\nHamming Distances to Input Strings:\n");
    for (int i = 0; i < n; i++) {
        int dist = hamming_distance(output, input[i]);
        if (dist > max_dist) max_dist = dist;
        printf("  %s -> %d\n", input[i], dist);
    }

    printf("\nMaximum Hamming Distance: %d\n", max_dist);

    return 0;
}
