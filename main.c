/*****************************************************
 * Filename: main.c
 * Author: Emanuel Hernandez
 * Course: CPE 2600 - Systems Programming
 * Lab: Lab 5 - Vector Calculator
 * Description: Main entry point for the MiniMAT calculator. Starts the program and handles command line arguments.
 *****************************************************/

#include <stdio.h>
#include <string.h>
#include "interface.h"

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        showHelp();
        return 0;
    }

    if (argc != 1) {
        printf("Unknown option. Run ./minimat -h for help.\n");
        return 1;
    }

    runCalculator();
    return 0;
}