#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MAX_N 100 // Maximum number of input strings
#define MAX_M 100 // Maximum length of each input string
#define ALPHABET_SIZE 256 // Size of the character set

int best_cost = INT_MAX;

int num_strings, string_length;

char *output;
char **input;

int verbose = 0;

/**
 * @brief Function to print the strings.
 * @param strings The array of strings to be printed.
 * @param count The number of strings to be printed.
 */
void print_strings(char **strings, int count) {
    printf("Input strings:\n");
    for (int i = 0; i < count; i++) {
        printf("\t%s\n", strings[i]);
    };
};

/**
 * @brief Function to read the configuration file and populate the input strings.
 * @param filename The name of the configuration file.
 * @return A pointer to the array of input strings.
 */
void *read_config_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open config file");
        return NULL;
    };

    fscanf(fp, "%d", &num_strings);
    fscanf(fp, "%d", &string_length);

    if (num_strings <= 0 || string_length <= 0) {
        printf("Invalid number of strings or string length\n");
        fclose(fp);
        return NULL;
    };

    if (num_strings > MAX_N || string_length > MAX_M) {
        printf("Number of strings or string length exceeds maximum limit [100]\n");
        fclose(fp);
        return NULL;
    };

    output = (char*)malloc((string_length + 1) * sizeof(char));

    // Allocate memory for input strings
    input = (char **)malloc(num_strings * sizeof(char *));
    if (!input) {
        perror("Failed to allocate memory for input strings");
        fclose(fp);
        return NULL;
    };

    // Allocate memory for each string
    for (int i = 0; i < num_strings; i++) {
        input[i] = (char*)malloc((string_length + 1) * sizeof(char));
        if (!input[i]) {
            perror("Failed to allocate memory for input string");
            fclose(fp);
            return NULL;
        };
    };

    // Read the input strings from the file
    for (int i = 0; i < num_strings; i++) {
        fscanf(fp, "%s", input[i]);
    };

    fclose(fp);
    return input;
};

/**
 * @brief Function that performs a hybrid greedy search to find the closest string.
 *
 */
void greedy_closest_string() {
    if (verbose) printf("Step-by-step character selection (Hybrid Greedy):\n\n");
    int candidate_count;

    // Iterate over the characters of the strings
    for (int i = 0; i < string_length; i++) {
        int freq[ALPHABET_SIZE] = {0};
        char candidates[ALPHABET_SIZE];
        candidate_count = 0;

        // Step 1: Count frequency of each character in this column
        for (int j = 0; j < num_strings; j++) {
            unsigned char c = input[j][i];
            if (freq[c] == 0) {
                candidates[candidate_count++] = c;
            }
            freq[c]++;
        };

        if (verbose) printf("Column %d:\n", i);
        for (int k = 0; k < candidate_count; k++) {
            char c = candidates[k];
            int estimated_max_hamming = num_strings - freq[(unsigned char)c];
            if (verbose) printf("\t'%c': freq = %d, est max Hamming = %d\n", c, freq[(unsigned char)c], estimated_max_hamming);
        };

        // Step 2–3: Select candidates with lowest estimated max Hamming
        int min_est_hamming = INT_MAX;
        char tied_chars[ALPHABET_SIZE];
        int tie_count = 0;

        for (int k = 0; k < candidate_count; k++) {
            char c = candidates[k];
            int est = num_strings - freq[(unsigned char)c];
            if (est < min_est_hamming) {
                min_est_hamming = est;
                tie_count = 0;
                tied_chars[tie_count++] = c;
            } else if (est == min_est_hamming) {
                tied_chars[tie_count++] = c;
            };
        };

        // Step 4: Tie-breaking
        char final_char = 127;  // High ASCII init
        char first_input_char = input[0][i];

        // Prefer character from first string if tied
        for (int k = 0; k < tie_count; k++) {
            if (tied_chars[k] == first_input_char) {
                final_char = tied_chars[k];
                break;
            };
        };

        // If not found, pick lexicographically smallest
        if (final_char == 127) {
            for (int k = 0; k < tie_count; k++) {
                if (tied_chars[k] < final_char) {
                    final_char = tied_chars[k];
                }
            };
        };
        output[i] = final_char;
        if (verbose) printf("  → Chosen character: '%c'\n\n", final_char);
    };
    output[string_length] = '\0';
};

/**
 * @brief Function to save the elapsed time to a file.
 * @param elapsed_time The elapsed time to be saved.
 */
void save_time(double elapsed_time) {
    FILE *fp = fopen("runtimes.txt", "a");
    if (!fp) {
        perror("Failed to open time file");
        return;
    };

    fprintf(fp, "%d,%d,%.6f\n", num_strings, string_length, elapsed_time);
    fclose(fp);
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <config_file> [-v]\n", argv[0]);
        return 1;
    };

    if (argc == 3) {
        char *arg = argv[2];
        if (strcmp(arg, "-v") == 0) {
            verbose = 1;
        } else {
            printf("Invalid argument: %s\n", arg);
            return 1;
        };
    }

    char *filename = argv[1];

    void *input = read_config_file(filename);

    if (input == NULL) {
        printf("Error reading config file\n");
        return 1;
    };

    if (verbose) print_strings(input, num_strings);

    // Start timer
    struct timespec time_before, time_after;

    clock_gettime(CLOCK_MONOTONIC, &time_before);
    // Start the DFS to find the closest string
    if (verbose) {
        printf("\nStarting Greedy Approach...\n");
    }

    greedy_closest_string();
    clock_gettime(CLOCK_MONOTONIC, &time_after);

    // Calculate elapsed time
    double elapsed_time = (time_after.tv_sec - time_before.tv_sec) + (time_after.tv_nsec - time_before.tv_nsec) / 1e9;

    save_time(elapsed_time);
    printf("Time elapsed: %.6f seconds\n", elapsed_time);

    // Output the best string found
    printf("Closest string found: %s\n", output);

    free(input);

    return 0;

};