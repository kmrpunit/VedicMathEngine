/**
 * antyayordasake.c - Implementation of "Last totaling 10" sutra
 * 
 * This sutra provides a fast method for multiplying two numbers whose last digits sum to 10
 * and whose prefixes are the same.
 */

 #include "vedicmath.h"

 /**
  * Antyayordasake - "Last totaling 10"
  * 
  * Purpose: Multiply two numbers whose last digits sum to 10 and prefixes are equal.
  * When to use: For pairs like 47×43, 26×24, etc.
  * 
  * Core logic: If numbers are mA and mB with A+B=10, result = m(m+1) || (A×B)
  * Example: 47×43: m=4, A=7, B=3, result = 4×5 || 7×3 = 20||21 = 2021
  * 
  * @param a First number
  * @param b Second number
  * @return The product a * b
  */
 int antya_dasake_mul(int a, int b) {
     // Check if last digits sum to 10
     if ((a % 10 + b % 10) != 10) {
         return a * b;  // Use regular multiplication
     }
     
     // Get prefixes
     int prefix_a = a / 10;
     int prefix_b = b / 10;
     
     // Check if prefixes are the same
     if (prefix_a != prefix_b) {
         return a * b;  // Use regular multiplication
     }
     
     // Calculate left part: prefix * (prefix + 1)
     int left_part = prefix_a * (prefix_a + 1);
     
     // Calculate right part: product of last digits
     int right_part = (a % 10) * (b % 10);
     
     // Format the right part to have at least 2 digits
     int right_digits = (right_part < 10) ? 2 : count_digits(right_part);
     
     // Combine parts
     int multiplier = 1;
     for (int i = 0; i < right_digits; i++) {
         multiplier *= 10;
     }
     
     return left_part * multiplier + right_part;
 }