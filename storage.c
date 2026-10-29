/*****************************************************
 * Filename: storage.c
 * Author: Emanuel Hernandez
 * Course: CPE 2600 - Systems Programming
 * Lab: Lab 5 - Vector Calculator
 * Description: Manages storage for up to 10 vectors. Handles adding, replacing, finding, listing access, and clearing vectors.
 *****************************************************/
#include <string.h>
#include "storage.h"

static vect vectors[MAX_VECTORS];
static int count = 0;

int findvect(const char *name, vect *found) {
    for (int i = 0; i < count; i++) {
        if (strcmp(vectors[i].name, name) == 0) {
            *found = vectors[i];
            return 1;
        }
    }

    return 0;
}

int addvect(vect value) {
    for (int i = 0; i < count; i++) {
        if (strcmp(vectors[i].name, value.name) == 0) {
            vectors[i] = value;
            return 1;
        }
    }

    if (count >= MAX_VECTORS) {
        return 0;
    }

    vectors[count++] = value;
    return 1;
}

void clearVectors(void) {
    count = 0;
}

int vectorCount(void) {
    return count;
}

int getVectorAt(int index, vect *found) {
    if (index < 0 || index >= count) {
        return 0;
    }

    *found = vectors[index];
    return 1;
}