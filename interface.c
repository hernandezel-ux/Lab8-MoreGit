/*****************************************************
 * Filename: interface.c
 * Author: Emanuel Hernandez
 * Course: CPE 2600 - Systems Programming
 * Lab: Lab 5 - Vector Calculator
 * Description: Handles user input and calculator commands. 
 * Processes vector assignments, calculations, help, list, clear, and quit commands. Displays results and error messages.
 *****************************************************/

#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"
#include "storage.h"
#include "vector.h"

#define LINE_SIZE 512


// Display a vector
static void printVector(vect v) {
    printf("%s = %.3f  %.3f  %.3f\n",
           v.name, v.x, v.y, v.z);
}

// Remove extra spaces
static char *trim(char *text) {
    while (isspace((unsigned char)*text)) {
        text++;
    }

    char *end = text + strlen(text);

    while (end > text &&
           isspace((unsigned char)end[-1])) {
        *--end = '\0';
    }

    return text;
}

// Check if a vector name is valid
static int validName(const char *name) {
    if (!name[0] || strlen(name) >= NAME_SIZE) {
        return 0;
    }

    if (!isalpha((unsigned char)name[0]) &&
        name[0] != '_') {
        return 0;
    }

    for (const char *p = name + 1; *p; p++) {
        if (!isalnum((unsigned char)*p) &&
            *p != '_') {
            return 0;
        }
    }

    return strcmp(name, "help") != 0 &&
           strcmp(name, "list") != 0 &&
           strcmp(name, "clear") != 0 &&
           strcmp(name, "quit") != 0 &&
           strcmp(name, "load") != 0 &&
           strcmp(name, "save") != 0;
}

// Convert text into a number
static int parseNumber(const char *text, double *value) {
    char *end;

    errno = 0;
    *value = strtod(text, &end);

    return end != text &&
           *end == '\0' &&
           errno != ERANGE;
}

// Read three vector components
static int parseComponents(char *text, vect *v) {
    for (char *p = text; *p; p++) {
        if (*p == ',') {
            *p = ' ';
        }
    }

    char trailing;

    return sscanf(text, " %lf %lf %lf %c",
                  &v->x, &v->y, &v->z, &trailing) == 3;
}

// Evaluate vector mathematical operations
// Return 0 = error, 1 = vector, 2 = scalar
static int evaluate(const char *expression,
                    vect *result,
                    double *scalar) {

    char left[64], op[2], right[64], extra;

    if (sscanf(expression, " %63s %1s %63s %c",
               left, op, right, &extra) != 3) {
        puts("Invalid expression. Use spaces around operators.");
        return 0;
    }

    vect a, b;
    double number;

    // Addition, subtraction, dot, and cross product
    if (strcmp(op, "+") == 0 ||
        strcmp(op, "-") == 0 ||
        strcmp(op, ".") == 0 ||
        strcmp(op, "x") == 0) {

        if (!findvect(left, &a) ||
            !findvect(right, &b)) {
            puts("Both vectors must exist.");
            return 0;
        }

        if (*op == '+') {
            *result = add(a, b);
        } else if (*op == '-') {
            *result = subtract(a, b);
        } else if (*op == 'x') {
            *result = crossProduct(a, b);
        } else {
            *scalar = dotProduct(a, b);
            return 2;
        }

        return 1;
    }

    // Scalar multiplication
    if (strcmp(op, "*") == 0) {
        if (findvect(left, &a) &&
            parseNumber(right, &number)) {
            *result = multiply(a, number);
            return 1;
        }

        if (parseNumber(left, &number) &&
            findvect(right, &b)) {
            *result = multiply(b, number);
            return 1;
        }

        puts("Scalar multiplication needs one vector and one number.");
        return 0;
    }

    printf("Unknown operator: %s\n", op);
    return 0;
}

// Display all stored vectors
static void listVectors(void) {
    if (vectorCount() == 0) {
        puts("No vectors stored.");
        return;
    }

    for (size_t i = 0; i < vectorCount(); i++) {
        vect value;

        if (getVectorAt(i, &value)) {
            printVector(value);
        }
    }
}

// Parse one CSV row: name,x,y,z
static int parseCsvRow(char *line, vect *v) {
    char *fields[4];
    char *p = line;

    for (int i = 0; i < 4; i++) {
        fields[i] = p;
        char *comma = strchr(p, ',');

        if (i < 3) {
            if (!comma) {
                return 0;
            }

            *comma = '\0';
            p = comma + 1;
        } else if (comma) {
            return 0;
        }

        fields[i] = trim(fields[i]);
    }

    if (!validName(fields[0])) {
        return 0;
    }

    if (!parseNumber(fields[1], &v->x) ||
        !parseNumber(fields[2], &v->y) ||
        !parseNumber(fields[3], &v->z)) {
        return 0;
    }

    strcpy(v->name, fields[0]);
    return 1;
}

// Load vectors from a CSV file
static void loadVectors(const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        perror("Cannot open file for loading");
        return;
    }

    vect *incoming = NULL;
    size_t length = 0;
    size_t capacity = 0;
    size_t lineNo = 0;
    int good = 1;

    char line[LINE_SIZE];

    while (fgets(line, sizeof(line), fp)) {
        lineNo++;

        // Reject lines that exceed the buffer
        if (!strchr(line, '\n') && !feof(fp)) {
            fprintf(stderr, "CSV line %zu is too long.\n",
                    lineNo);
            good = 0;
            break;
        }

        char *row = trim(line);

        // Skip blank lines
        if (*row == '\0') {
            continue;
        }

        vect value = {0};

        if (!parseCsvRow(row, &value)) {
            fprintf(stderr,
                    "Invalid CSV data on line %zu.\n",
                    lineNo);
            good = 0;
            break;
        }

        // Expand temporary storage if necessary
        if (length == capacity) {
            size_t next = capacity == 0 ? 8 : capacity * 2;

            if (next < capacity ||
                next > SIZE_MAX / sizeof(*incoming)) {
                good = 0;
                break;
            }

            vect *grown = realloc(
                incoming,
                next * sizeof(*incoming)
            );

            if (!grown) {
                good = 0;
                break;
            }

            incoming = grown;
            capacity = next;
        }

        incoming[length++] = value;
    }

    if (ferror(fp)) {
        perror("Error reading CSV");
        good = 0;
    }

    if (fclose(fp) != 0) {
        perror("Error closing CSV");
        good = 0;
    }

    // Merge loaded vectors with existing vectors
    if (good) {
        for (size_t i = 0; i < length; i++) {
            if (!addvect(incoming[i])) {
                fprintf(stderr,
                        "Out of memory while loading CSV.\n");
                good = 0;
                break;
            }
        }
    }

    if (good) {
        printf("Loaded %zu rows from %s (%zu stored vectors).\n",
               length, filename, vectorCount());
    } else {
        puts("Load failed.");
    }

    free(incoming);
}

// Save all stored vectors to a CSV file
static void saveVectors(const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (!fp) {
        perror("Cannot open file for saving");
        return;
    }

    int good = 1;

    for (size_t i = 0; i < vectorCount(); i++) {
        vect v;

        if (getVectorAt(i, &v) &&
            fprintf(fp, "%s,%.17g,%.17g,%.17g\n",
                    v.name, v.x, v.y, v.z) < 0) {
            good = 0;
            break;
        }
    }

    if (fclose(fp) != 0) {
        good = 0;
    }

    if (good) {
        printf("Saved %zu vectors to %s.\n",
               vectorCount(), filename);
    } else {
        fprintf(stderr, "Error writing CSV to %s.\n",
                filename);
    }
}

// Process user commands
static void processCommand(char *line, int *running) {
    char *command = trim(line);

    if (!*command) {
        return;
    }

    // Quit command
    if (strcmp(command, "quit") == 0) {
        *running = 0;
        return;
    }

    // Help command
    if (strcmp(command, "help") == 0) {
        showHelp();
        return;
    }

    // Clear command
    if (strcmp(command, "clear") == 0) {
        clearVectors();
        puts("Memory cleared and freed.");
        return;
    }

    // List command
    if (strcmp(command, "list") == 0) {
        listVectors();
        return;
    }

    // Load and save commands
    if (strncmp(command, "load ", 5) == 0 ||
        strncmp(command, "save ", 5) == 0) {

        int loading = command[0] == 'l';
        char *filename = trim(command + 5);

        if (!*filename ||
            strpbrk(filename, " \t\r\n")) {
            puts("Filename is required and cannot contain spaces.");
            return;
        }

        if (loading) {
            loadVectors(filename);
        } else {
            saveVectors(filename);
        }

        return;
    }

    // Vector assignment
    char *equals = strchr(command, '=');

    if (equals) {
        *equals = '\0';

        char *name = trim(command);
        char *rhs = trim(equals + 1);

        if (!validName(name)) {
            puts("Invalid or reserved vector name.");
            return;
        }

        vect result = {0};
        double scalar = 0;
        char numeric[LINE_SIZE];

        snprintf(numeric, sizeof(numeric), "%s", rhs);

        if (!parseComponents(numeric, &result)) {
            int type = evaluate(rhs, &result, &scalar);

            if (type == 0) {
                return;
            }

            // Dot product produces a scalar
            if (type == 2) {
                printf("Dot product = %.3f; scalar cannot "
                       "be stored as a vector.\n", scalar);
                return;
            }
        }

        strcpy(result.name, name);

        if (!addvect(result)) {
            puts("Out of memory: cannot store vector.");
            return;
        }

        printVector(result);
        return;
    }

    // Display one vector
    if (!strchr(command, ' ') &&
        !strchr(command, '\t')) {

        vect value;

        if (findvect(command, &value)) {
            printVector(value);
        } else {
            printf("Vector '%s' does not exist.\n", command);
        }

        return;
    }

    // Calculate without assigning
    vect result = {0};
    double scalar = 0;

    int type = evaluate(command, &result, &scalar);

    if (type == 2) {
        printf("ans = %.3f\n", scalar);
    }

    if (type == 1) {
        strcpy(result.name, "ans");
        printVector(result);
    }
}

// Display the help menu
void showHelp(void) {
    puts("MiniMAT Vector Calculator");
    puts("  help             Show this help");
    puts("  a = 1 2 3        Create or replace vector");
    puts("  a = 1, 2, 3     Commas also work");
    puts("  a                Display vector");
    puts("  a + b            Vector addition");
    puts("  a - b            Vector subtraction");
    puts("  a * 2            Scalar multiplication");
    puts("  2 * a            Scalar multiplication");
    puts("  a . b            Dot product");
    puts("  a x b            Cross product");
    puts("  c = a x b        Save cross product");
    puts("  list             List stored vectors");
    puts("  clear            Free vector memory");
    puts("  load <filename>  Load CSV vectors");
    puts("  save <filename>  Save vectors to CSV");
    puts("  quit             Exit calculator");
    puts("Use spaces around arithmetic operators.");
    puts("CSV format: name,x,y,z (no header).");
}

// Main calculator loop
void runCalculator(void) {
    char line[LINE_SIZE];
    int running = 1;

    while (running) {
        printf("minimat> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        // Check for input exceeding buffer size
        if (!strchr(line, '\n') && !feof(stdin)) {
            int ch;

            while ((ch = getchar()) != '\n' &&
                   ch != EOF) {
            }

            puts("Input too long.");
            continue;
        }

        processCommand(line, &running);
    }
}
