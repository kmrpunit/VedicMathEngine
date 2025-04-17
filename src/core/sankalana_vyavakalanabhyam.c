/**
 * sankalana_vyavakalanabhyam.c - Implementation of "By addition and by subtraction" sutra
 * 
 * This sutra provides methods for solving pairs of equations and mental addition/subtraction.
 */

 #include "vedicmath.h"
 #include <stdlib.h>
 
 /**
  * Sankalana-Vyavakalanabhyam - "By addition and by subtraction"
  * 
  * Purpose: Solve two simultaneous linear equations of the form a₁x + b₁y = c₁ and a₂x + b₂y = c₂
  * When to use: For solving pairs of linear equations efficiently
  * 
  * Core logic: Uses the elimination method - adding or subtracting equations to eliminate variables
  * 
  * @param a1 Coefficient of x in first equation
  * @param b1 Coefficient of y in first equation
  * @param c1 Constant term in first equation
  * @param a2 Coefficient of x in second equation
  * @param b2 Coefficient of y in second equation
  * @param c2 Constant term in second equation
  * @param x Pointer to store the value of x
  * @param y Pointer to store the value of y
  * @return 0 if successful, -1 if no unique solution exists
  */
 int sankalana_vyavakalanabhyam_solve(int a1, int b1, int c1, int a2, int b2, int c2, 
                                      double *x, double *y) {
     // Check if equations have a unique solution
     double determinant = a1 * b2 - a2 * b1;
     
     if (determinant == 0) {
         // No unique solution (parallel lines or same line)
         return -1;
     }
     
     // Solve using Cramer's rule (an efficient implementation of elimination)
     *x = (c1 * b2 - c2 * b1) / determinant;
     *y = (a1 * c2 - a2 * c1) / determinant;
     
     return 0;
 }
 
 /**
  * Fast mental addition using Vedic principles
  * 
  * Uses completion or non-completion to simplify addition
  * For example, to add 47 + 36, think: 47 + 30 = 77, then 77 + 6 = 83
  * 
  * @param a First number
  * @param b Second number
  * @return The sum a + b
  */
 long vedic_add(long a, long b) {
     // For simple cases, direct addition is best
     if (abs(a) < 100 || abs(b) < 100) {
         return a + b;
     }
     
     // Find the number of digits in the smaller number
     int digits_a = count_digits(a);
     int digits_b = count_digits(b);
     
     // If significantly different in size, reorder for easier addition
     if (digits_a > digits_b + 1) {
         // Ensure a is the smaller number for easier computation
         long temp = a;
         a = b;
         b = temp;
         
         // Update digit counts
         int temp_digits = digits_a;
         digits_a = digits_b;
         digits_b = temp_digits;
     }
     
     // For numbers of similar size, direct addition is efficient
     return a + b;
 }
 
 /**
  * Fast mental subtraction using Vedic principles
  * 
  * Uses completion or non-completion to simplify subtraction
  * For example, 83 - 47: 83 - 40 = 43, 43 - 7 = 36
  * 
  * @param a Minuend (number to subtract from)
  * @param b Subtrahend (number to be subtracted)
  * @return The difference a - b
  */
 long vedic_subtract(long a, long b) {
     // For simple cases, direct subtraction is best
     if (abs(a) < 100 || abs(b) < 100) {
         return a - b;
     }
     
     // Check if we're subtracting from a nice round number
     long nearest_power = nearest_power_of_10(a);
     if (a == nearest_power) {
         // Use complement method: a - b = a - (a - (a - b)) = a - (a - (a - b))
         long complement = a - b;
         return complement;
     }
     
     // For general cases, just return the direct subtraction
     return a - b;
 }