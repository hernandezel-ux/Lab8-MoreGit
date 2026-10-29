/*****************************************************
 * Filename: storage.h
 * Author: Emanuel Hernandez
 * Course: CPE 2600 - Systems Programming
 * Lab: Lab 5 - Vector Calculator
 * Description: Declares functions used to store, retrieve, and manage vectors in memory.
 *****************************************************/

#ifndef STORAGE_H
#define STORAGE_H

#include <stddef.h>
#include "vector.h"


int findvect(const char *name, vect *found);
int addvect(vect value);
void clearVectors(void);
size_t vectorCount(void);
int getVectorAt(size_t index, vect *found);

#endif