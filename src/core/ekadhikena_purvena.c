/**
 * ekadhikena_purvena.c - Implementation of the "By one more than the previous one" sutra
 * 
 * This sutra provides a fast method for squaring numbers ending in 5.
 */

 #include "vedicmath.h"

 /**
  * Ekadhikena Purvena - "By one more than the previous one"
  * 
  * Purpose: Fast squaring of numbers ending in 5
  * When to use: For squaring any number that ends in 5
  * 
  * Core logic: If N ends in 5, split N = m*10 + 5. The result N^2 = m * (m+1) appended with 25.
  * Example: N=25 -> m=2, m*(m+1)=2*3=6, append 25 => 625.
  * 
  * @param n Number ending in 5 to be squared
  * @return The square of n
  */
 long ekadhikena_purvena(long n) {
     // If number doesn't end in 5, return normal square
     if (n % 10 != 5) {
         return n * n;
     }
     
     // Extract the prefix (all digits except the last 5)
     long prefix = n / 10;
     
     // Calculate left part: prefix * (prefix + 1)
     long left_part = prefix * (prefix + 1);
     
     // Right part is always 25 for numbers ending in 5
     long right_part = 25;
     
     // Combine parts: left_part || 25
     return left_part * 100 + right_part;
 }
 
 /**
  * Ekadhikena Purvena for different numbers - "By one more than the previous one"
  * 
  * An extension of the squaring method for multiplying numbers where:
  * - They share the same prefix
  * - Their last digits sum to 10
  * 
  * Example: 47×43 (share prefix 4, last digits 7+3=10)
  * Result = 4×5 || 7×3 = 20 || 21 = 2021
  * 
  * @param a First number
  * @param b Second number
  * @return The product a * b
  */
 long ekadhikena_purvena_general(long a, long b) {
     // Check if last digits sum to 10
     if ((a % 10 + b % 10) != 10) {
         return a * b;  // Use regular multiplication
     }
     
     // Get prefixes
     long prefix_a = a / 10;
     long prefix_b = b / 10;
     
     // Check if prefixes are the same
     if (prefix_a != prefix_b) {
         return a * b;  // Use regular multiplication
     }
     
     // Calculate left part: prefix * (prefix + 1)
     long left_part = prefix_a * (prefix_a + 1);
     
     // Calculate right part: product of last digits
     long right_part = (a % 10) * (b % 10);
     
     // Format the right part to have at least 2 digits
     int right_digits = (right_part < 10) ? 2 : count_digits(right_part);
     
     // Combine parts
     long multiplier = 1;
     for (int i = 0; i < right_digits; i++) {
         multiplier *= 10;
     }
     
     return left_part * multiplier + right_part;
 }