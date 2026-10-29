/*****************************************************
 * Filename: vector.c
 * Author: Emanuel Hernandez
 * Course: CPE 2600 - Systems Programming
 * Lab: Lab 5 - Vector Calculator
 * Description: Implements vector addition, subtraction, scalar multiplication, dot product, and cross product.
 *****************************************************/
#include "vector.h"

vect add(vect a, vect b) {
    vect result = {0};

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}

vect subtract(vect a, vect b) {
    vect result = {0};

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

vect multiply(vect a, double scalar) {
    vect result = {0};

    result.x = a.x * scalar;
    result.y = a.y * scalar;
    result.z = a.z * scalar;

    return result;
}

double dotProduct(vect a, vect b) {
    return a.x * b.x +
           a.y * b.y +
           a.z * b.z;
}

vect crossProduct(vect a, vect b) {
    vect result = {0};

    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;

    return result;
}