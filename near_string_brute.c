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

char *best_string;
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

    best_string = (char*)malloc((string_length + 1) * sizeof(char));

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
 * @brief Function to calculate the Hamming distance between two strings.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return The Hamming distance between the two strings.
 */
int hamming_distance(const char *str1, const char *str2) {
    int dist = 0;
    int difference;

    for (int i = 0; i < string_length; i++) {
        if (str1[i] != str2[i]) {
            if (str1[i] != str2[i]) dist++;
        };
    };

    // if (verbose) printf("Hamming distance between %s and %s: %d\n", str1, str2, dist);

    return dist;
};

/**
 * @brief Function to calculate the average Hamming distance of a candidate string from all input strings.
 * @param candidate The candidate string.
 * @return The average Hamming distance.
 */
int max_hamming(const char *candidate) {
    int max_dist = 0, dist;
    for (int i = 0; i < num_strings; i++) {
        dist = hamming_distance(candidate, input[i]);
        if (dist > max_dist) {
            max_dist = dist;
        };
    };
    return max_dist;
};

/**
 * @brief Function to perform a depth-first search (DFS) to find the closest string.
 * @param current The current string being constructed.
 * @param pos The current position in the string.
 */
void dfs(char *current, int pos) {
    if (pos == string_length) {
        int cost = max_hamming(current);
        if (verbose) printf("\t%s -> %d", current, cost);
        if (cost < best_cost) {
            best_cost = cost;
            strcpy(best_string, current);
        };
        if (verbose) printf(" : %s\n", best_string);
        else printf("\n");
        return;
    };

    int used[ALPHABET_SIZE] = {0};
    for (int i = 0; i < num_strings; i++) {
        char c = input[i][pos];
        if (!used[(unsigned char)c]) {
            used[(unsigned char)c] = 1;
            current[pos] = c;
            current[pos + 1] = '\0';
            dfs(current, pos + 1);
        };
    };
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

    // Allocate memory for the current string
    char *current = (char*)malloc((string_length + 1) * sizeof(char));
    if (!current) {
        perror("Failed to allocate memory for current string");
        return 1;
    };

    // Start timer
    struct timespec time_before, time_after;

    clock_gettime(CLOCK_MONOTONIC, &time_before);
    // Start the DFS to find the closest string
    if (verbose) {
        printf("\nStarting DFS...\n");
        printf("Hamming Distances (<string> -> <distance> : <best_string (after comparison)>)\n");
    }

    dfs(current, 0);
    clock_gettime(CLOCK_MONOTONIC, &time_after);

    // Calculate elapsed time
    double elapsed_time = (time_after.tv_sec - time_before.tv_sec) + (time_after.tv_nsec - time_before.tv_nsec) / 1e9;

    save_time(elapsed_time);
    printf("Time elapsed: %.6f seconds\n", elapsed_time);

    // Output the best string found
    printf("Best string found: %s\n", best_string);
    printf("Best cost: %d\n", best_cost);

    free(current);
    free(input);

    return 0;

};