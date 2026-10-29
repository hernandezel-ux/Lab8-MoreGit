/*****************************************************
 * Filename: vector.h
 * Author: Emanuel Hernandez
 * Course: CPE 2600 - Systems Programming
 * Lab: Lab 5 - Vector Calculator
 * Description: Defines the vector structure and declares functions for vector mathematical operations.
 *****************************************************/

#ifndef VECTOR_H
#define VECTOR_H

#define NAME_SIZE 32

// Structure for a three-dimensional vector
typedef struct {
    char name[NAME_SIZE];
    double x;
    double y;
    double z;
} vect;
// Vector addition
vect add(vect a, vect b);
// Vector subtraction
vect subtract(vect a, vect b);
// Scalar multiplication
vect multiply(vect a, double scalar);
// Dot product
double dotProduct(vect a, vect b);
// Cross product
vect crossProduct(vect a, vect b);

#endif