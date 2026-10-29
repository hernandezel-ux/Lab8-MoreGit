/*****************************************************
 * Filename: interface.c
 * Author: Emanuel Hernandez
 * Course: CPE 2600 - Systems Programming
 * Lab: Lab 5 - Vector Calculator
 * Description: Handles user input and calculator commands. 
 * Processes vector assignments, calculations, help, list, clear, and quit commands. Displays results and error messages.
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "interface.h"
#include "storage.h"
#include "vector.h"

#define LINE_SIZE 256

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

    for (int i = 1; name[i]; i++) {
        if (!isalnum((unsigned char)name[i]) &&
            name[i] != '_') {
            return 0;
        }
    }

    return strcmp(name, "quit") != 0 &&
           strcmp(name, "clear") != 0 &&
           strcmp(name, "list") != 0;
}

// Convert text into a number
static int parseNumber(const char *text, double *number) {
    char *end;

    *number = strtod(text, &end);

    return end != text && *end == '\0';
}

// Perform vector calculations
// Returns 1 for a vector result
// Returns 2 for a scalar result (dot product)
// Returns 0 if an error occurs
static int evaluate(const char *expr,
                    vect *result,
                    double *scalarResult) {

    char left[64], right[64], op[2], extra;

    if (sscanf(expr, "%63s %1s %63s %c",
               left, op, right, &extra) != 3) {
        printf("Invalid operation.\n");
        return 0;
    }

    vect a, b;
    double n;

    // Vector addition and subtraction
    if (strcmp(op, "+") == 0 ||
        strcmp(op, "-") == 0) {

        if (!findvect(left, &a) ||
            !findvect(right, &b)) {
            printf("Both vectors must exist.\n");
            return 0;
        }

        if (op[0] == '+') {
            *result = add(a, b);
        } else {
            *result = subtract(a, b);
        }

        return 1;
    }

    // Scalar multiplication
    if (strcmp(op, "*") == 0) {
        if (findvect(left, &a) &&
            parseNumber(right, &n)) {
            *result = multiply(a, n);
            return 1;
        }

        if (parseNumber(left, &n) &&
            findvect(right, &b)) {
            *result = multiply(b, n);
            return 1;
        }

        printf("Multiplication requires one vector and one number.\n");
        return 0;
    }

    // Dot product
    if (strcmp(op, ".") == 0) {
        if (!findvect(left, &a) ||
            !findvect(right, &b)) {
            printf("Both vectors must exist for dot product.\n");
            return 0;
        }

        *scalarResult = dotProduct(a, b);
        return 2;
    }

    // Cross product
    if (strcmp(op, "x") == 0) {
        if (!findvect(left, &a) ||
            !findvect(right, &b)) {
            printf("Both vectors must exist for cross product.\n");
            return 0;
        }

        *result = crossProduct(a, b);
        return 1;
    }

    printf("Unknown operator: %s\n", op);
    return 0;
}

// List all stored vectors
static void listVectors(void) {
    if (vectorCount() == 0) {
        printf("No vectors stored.\n");
        return;
    }

    for (int i = 0; i < vectorCount(); i++) {
        vect value;

        if (getVectorAt(i, &value)) {
            printVector(value);
        }
    }
}

// Process commands entered by user
static void processCommand(char *line, int *running) {

    // Replace commas with spaces
    for (char *p = line; *p; p++) {
        if (*p == ',') {
            *p = ' ';
        }
    }

    char *command = trim(line);

    if (*command == '\0') {
        return;
    }

    // Quit command
    if (strcmp(command, "quit") == 0) {
        *running = 0;
        return;
    }

    // Clear command
    if (strcmp(command, "clear") == 0) {
        clearVectors();
        printf("Vector memory cleared.\n");
        return;
    }

    // List command
    if (strcmp(command, "list") == 0) {
        listVectors();
        return;
    }

    char *equals = strchr(command, '=');

    // Assignment
    if (equals != NULL) {
        *equals = '\0';

        char *name = trim(command);
        char *rhs = trim(equals + 1);

        if (!validName(name)) {
            printf("Invalid vector name.\n");
            return;
        }

        vect result = {0};
        double scalarResult = 0;
        double x, y, z;
        char extra;

        // Assign three numbers
        if (sscanf(rhs, "%lf %lf %lf %c",
                   &x, &y, &z, &extra) == 3) {
            result.x = x;
            result.y = y;
            result.z = z;
        } else {
            int type = evaluate(rhs, &result, &scalarResult);

            if (type == 0) {
                return;
            }

            // Dot product gives a number, not a vector
            if (type == 2) {
                printf("Dot product = %.3f\n", scalarResult);
                printf("Cannot store a scalar as a vector.\n");
                return;
            }
        }

        strcpy(result.name, name);

        if (!addvect(result)) {
            printf("Vector memory full (maximum 10 vectors).\n");
            return;
        }

        printVector(result);
        return;
    }

    // Display an existing vector
    if (strchr(command, ' ') == NULL &&
        strchr(command, '\t') == NULL) {

        vect value;

        if (!findvect(command, &value)) {
            printf("Vector '%s' does not exist.\n", command);
        } else {
            printVector(value);
        }

        return;
    }

    // Calculation without assignment
    vect result = {0};
    double scalarResult = 0;

    int type = evaluate(command, &result, &scalarResult);

    if (type == 1) {
        strcpy(result.name, "ans");
        printVector(result);
    } else if (type == 2) {
        printf("ans = %.3f\n", scalarResult);
    }
}

// Help menu
void showHelp(void) {
    printf("Usage: ./minimat [-h]\n");
    printf("Commands:\n");
    printf("  a = 1 2 3     Create a vector\n");
    printf("  a = 1, 2, 3  Commas also work\n");
    printf("  a             Display vector\n");
    printf("  a + b         Add vectors\n");
    printf("  a - b         Subtract vectors\n");
    printf("  a * 2         Scalar multiplication\n");
    printf("  2 * a         Scalar multiplication\n");
    printf("  a . b         Dot product\n");
    printf("  a x b         Cross product\n");
    printf("  c = a + b     Store calculation result\n");
    printf("  c = a x b     Store cross product\n");
    printf("  list          Display all vectors\n");
    printf("  clear         Clear vector memory\n");
    printf("  quit          Exit calculator\n");
    printf("Use spaces around operators.\n");
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

        processCommand(line, &running);
    }
}
