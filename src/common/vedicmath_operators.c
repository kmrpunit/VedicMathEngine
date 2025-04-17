/**
 * vedicmath_operators.c - Support for standard mathematical operators
 * 
 * This file implements support for standard mathematical operators
 * (+, -, *, /, %) using Vedic mathematics techniques when appropriate.
 */

 #include "../../include/vedicmath.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>

 /**
  * Addition operator using Vedic principles when appropriate
  * 
  * Calls the appropriate Vedic addition function based on input characteristics
  * 
  * @param a First number
  * @param b Second number
  * @return The sum a + b
  */
 long vedic_op_add(long a, long b) {
     // For small numbers, direct addition is fastest
     if (abs(a) < 100 || abs(b) < 100) {
         return a + b;
     }
     
     // Check if a is close to a round number for completion-based addition
     long base_a = nearest_power_of_10(a);
     if (abs(a - base_a) < 10) {
         return puranapuranabhyam_add(a, b);
     }
     
     // Check if b is close to a round number
     long base_b = nearest_power_of_10(b);
     if (abs(b - base_b) < 10) {
         return puranapuranabhyam_add(b, a);  // Switch order for better calculation
     }
     
     // Default to standard addition
     return vedic_add(a, b);
 }
 
 /**
  * Subtraction operator using Vedic principles when appropriate
  * 
  * Calls the appropriate Vedic subtraction function based on input characteristics
  * 
  * @param a First number (minuend)
  * @param b Second number (subtrahend)
  * @return The difference a - b
  */
 long vedic_op_subtract(long a, long b) {
     // For small numbers, direct subtraction is fastest
     if (abs(a) < 100 || abs(b) < 100) {
         return a - b;
     }
     
     // Check if a is a round number for complement-based subtraction
     long base_a = nearest_power_of_10(a);
     if (a == base_a) {
         return ekanyunena_complement(b, a);
     }
     
     // Check if a is close to a round number
     if (abs(a - base_a) < 10) {
         return puranapuranabhyam_subtract(a, b);
     }
     
     // Default to standard subtraction
     return vedic_subtract(a, b);
 }
 
 /**
  * Multiplication operator using Vedic principles
  * 
  * Calls the central dispatcher to select the best multiplication method
  * 
  * @param a First number
  * @param b Second number
  * @return The product a * b
  */
 long vedic_op_multiply(long a, long b) {
     return vedic_multiply(a, b);
 }
 
 /**
  * Division operator using Vedic principles
  * 
  * Calls the central dispatcher to select the best division method
  * 
  * @param a Dividend
  * @param b Divisor
  * @return The quotient a / b
  */
 long vedic_op_divide(long a, long b) {
     if (b == 0) {
         // Division by zero
         return (a < 0) ? -1 : 1;  // Return a non-zero error value
     }
     
     return vedic_divide(a, b, NULL);
 }
 
 /**
  * Modulo operator using Vedic principles
  * 
  * Calls the central dispatcher for division and returns the remainder
  * 
  * @param a Dividend
  * @param b Divisor
  * @return The remainder a % b
  */
 long vedic_op_modulo(long a, long b) {
     if (b == 0) {
         // Modulo by zero
         return a;  // Return the dividend as the remainder
     }
     
     long remainder;
     vedic_divide(a, b, &remainder);
     return remainder;
 }
 
 /**
  * Power operator using Vedic principles
  * 
  * Uses Vedic squaring for powers of 2, and combines with multiplication for other powers
  * 
  * @param base The base
  * @param exponent The exponent (must be >= 0)
  * @return base^exponent
  */
 long vedic_op_power(long base, int exponent) {
     // Handle special cases
     if (exponent < 0) {
         return 0;  // Integer division would be 0 for fractional results
     }
     
     if (exponent == 0) {
         return 1;  // Any number to the power of 0 is 1
     }
     
     if (exponent == 1) {
         return base;  // Any number to the power of 1 is itself
     }
     
     if (exponent == 2) {
         return vedic_square(base);  // Use Vedic squaring
     }
     
     // For higher powers, use binary exponentiation with Vedic multiplication
     long result = 1;
     long power = base;
     
     while (exponent > 0) {
         if (exponent % 2 == 1) {
             result = vedic_multiply(result, power);
         }
         power = vedic_square(power);
         exponent /= 2;
     }
     
     return result;
 }
 
 /**
  * Parse and evaluate a simple mathematical expression using Vedic methods
  * 
  * Supports basic operators: +, -, *, /, %, ^
  * 
  * @param expression The expression to evaluate (must be properly formatted)
  * @param result Pointer to store the result
  * @return 0 if successful, -1 if parsing error
  */
 int vedic_evaluate_expression(const char *expression, long *result) {
     // This is a simplified parser for basic expressions
     // It doesn't handle parentheses or operator precedence
     // For a real-world application, a proper expression parser would be needed
     
     // Simple implementation for basic expressions like "12 + 34"
     long operand1 = 0, operand2 = 0;
     char operator = '\0';
     int parsed = sscanf(expression, "%ld %c %ld", &operand1, &operator, &operand2);
     
     if (parsed != 3) {
         return -1;  // Parsing error
     }
     
     // Apply the appropriate Vedic operation
     switch (operator) {
         case '+':
             *result = vedic_op_add(operand1, operand2);
             break;
             
         case '-':
             *result = vedic_op_subtract(operand1, operand2);
             break;
             
         case '*':
             *result = vedic_op_multiply(operand1, operand2);
             break;
             
         case '/':
             *result = vedic_op_divide(operand1, operand2);
             break;
             
         case '%':
             *result = vedic_op_modulo(operand1, operand2);
             break;
             
         case '^':
             *result = vedic_op_power(operand1, (int)operand2);
             break;
             
         default:
             return -1;  // Unsupported operator
     }
     
     return 0;  // Success
 }