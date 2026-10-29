/*****************************************************
 * Filename: main.c
 * Author: Emanuel Hernandez
 * Course: CPE 2600 - Systems Programming
 * Lab: Lab 5 - Vector Calculator
 * Description: Main entry point for the MiniMAT calculator. Starts the program and handles command line arguments.
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"
#include "storage.h"

int main(int argc, char *argv[]) {
    // Register function to free memory when exiting
    if (atexit(clearVectors) != 0) {
        fprintf(stderr, "Unable to register memory cleanup.\n");
        return 1;
    }

    // Display help with -h argument
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        showHelp();
        return 0;
    }

    // Check for invalid command-line arguments
    if (argc != 1) {
        fprintf(stderr, "Usage: %s [-h]\n", argv[0]);
        return 1;
    }

    //Start
    runCalculator();

    return 0;
}