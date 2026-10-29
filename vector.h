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

typedef struct {
    char name[NAME_SIZE];
    double x;
    double y;
    double z;
} vect;

vect add(vect a, vect b);
vect subtract(vect a, vect b);
vect multiply(vect a, double scalar);
double dotProduct(vect a, vect b);
vect crossProduct(vect a, vect b);

#endif