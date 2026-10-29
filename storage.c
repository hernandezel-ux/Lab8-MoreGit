/*****************************************************
 * Filename: storage.c
 * Author: Emanuel Hernandez
 * Course: CPE 2600 - Systems Programming
 * Lab: Lab 5 - Vector Calculator
 * Description: Manages storage for up to 10 vectors. Handles adding, replacing, finding, listing access, and clearing vectors.
 *****************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "storage.h"

// Dynamically allocated array of vectors
static vect *vectors = NULL;
static size_t count = 0;
static size_t capacity = 0;

// Dynamically allocated array of vectors
int findvect(const char *name, vect *found) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(vectors[i].name, name) == 0) {
            *found = vectors[i];
            return 1;
        }
    }

    return 0;
}

// Add a new vector or replace an existing one
int addvect(vect value) {

    // Check if the vector already exists
    for (size_t i = 0; i < count; i++) {
        if (strcmp(vectors[i].name, value.name) == 0) {
            vectors[i] = value;
            return 1;
        }
    }

    // Expand the array if it is full
    if (count == capacity) {
        size_t newCapacity;

        if (capacity == 0) {
            newCapacity = 4;
        } else {
            newCapacity = capacity * 2;
        }

        // Prevent allocation size overflow
        if (newCapacity < capacity ||
            newCapacity > SIZE_MAX / sizeof(*vectors)) {
            return 0;
        }

        // Allocate or expand memory
        vect *grown = realloc(
            vectors,
            newCapacity * sizeof(*vectors)
        );

        if (grown == NULL) {
            return 0;
        }

        vectors = grown;
        capacity = newCapacity;
    }

    // Store the new vector
    vectors[count] = value;
    count++;

    return 1;
}

// Free all allocated memory
void clearVectors(void) {
    free(vectors);

    vectors = NULL;
    count = 0;
    capacity = 0;
}

// Return the number of stored vectors
size_t vectorCount(void) {
    return count;
}

// Retrieve a vector at a specific index
int getVectorAt(size_t index, vect *found) {
    if (index >= count) {
        return 0;
    }

    *found = vectors[index];
    return 1;
}