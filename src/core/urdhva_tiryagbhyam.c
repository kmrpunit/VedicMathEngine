/**
 * urdhva_tiryagbhyam.c - Implementation of "Vertically and crosswise" sutra
 * 
 * This is a general-purpose multiplication algorithm that works for any
 * numbers regardless of their proximity to a base.
 */

 #include "vedicmath.h"
 #include <stdlib.h>  // For malloc/free
 
 /**
  * Extract individual digits from a number into an array
  * 
  * @param n The number to extract digits from
  * @param digits Array to store the digits (must be pre-allocated)
  * @param max_digits Maximum number of digits to extract
  * @return The actual number of digits extracted
  */
 static int extract_digits(long n, int *digits, int max_digits) {
     // Handle zero or negative numbers
     if (n == 0) {
         digits[0] = 0;
         return 1;
     }
     
     if (n < 0) {
         n = -n; // Convert to positive for digit extraction
     }
     
     // Extract digits from right to left
     int count = 0;
     while (n > 0 && count < max_digits) {
         digits[count++] = n % 10;
         n /= 10;
     }
     
     // Reverse the array to get the digits in correct order (left to right)
     for (int i = 0; i < count / 2; i++) {
         int temp = digits[i];
         digits[i] = digits[count - 1 - i];
         digits[count - 1 - i] = temp;
     }
     
     return count;
 }
 
 /**
  * Urdhva-Tiryagbhyam - "Vertically and crosswise"
  * 
  * Purpose: General multiplication method using vertical and crosswise pattern.
  * When to use: For any multiplication when no special pattern applies.
  * 
  * Core logic: Every digit of the first number is multiplied with every digit 
  * of the second in a structured way, moving from right to left.
  * 
  * @param a First number to multiply
  * @param b Second number to multiply
  * @return The product a * b
  */
 long urdhva_mult(long a, long b) {
     // For simple cases, just use direct multiplication
     if (a < 10 || b < 10) {
         return a * b;
     }
     
     // Get number of digits in each number
     int digits_a = count_digits(a);
     int digits_b = count_digits(b);
     
     // For small numbers, direct multiplication is faster
     if (digits_a <= 2 && digits_b <= 2) {
         return a * b;
     }
     
     // Extract digits into arrays
     int *a_digits = (int*)malloc(digits_a * sizeof(int));
     int *b_digits = (int*)malloc(digits_b * sizeof(int));
     
     if (!a_digits || !b_digits) {
         // Handle memory allocation failure
         if (a_digits) free(a_digits);
         if (b_digits) free(b_digits);
         return a * b;  // Fall back to direct multiplication
     }
     
     extract_digits(a, a_digits, digits_a);
     extract_digits(b, b_digits, digits_b);
     
     // Result can have at most digits_a + digits_b digits
     int result_size = digits_a + digits_b;
     int *result = (int*)calloc(result_size, sizeof(int));
     
     if (!result) {
         // Handle memory allocation failure
         free(a_digits);
         free(b_digits);
         return a * b;  // Fall back to direct multiplication
     }
     
     // Perform Urdhva-Tiryagbhyam multiplication
     // This is done by multiplying digits and adding them to the correct position
     // with carries handled appropriately
     
     // For each position in the result, go from right to left
     for (int i = 0; i < result_size; i++) {
         // For each digit in a, multiply with corresponding digit in b
         // to contribute to this position
         for (int j = 0; j <= i; j++) {
             // Check if we're within bounds of both numbers
             if (j < digits_a && (i - j) < digits_b) {
                 // Multiply digits and add to result
                 result[result_size - 1 - i] += a_digits[digits_a - 1 - j] * b_digits[digits_b - 1 - (i - j)];
             }
         }
     }
     
     // Handle carries
     for (int i = result_size - 1; i > 0; i--) {
         if (result[i] >= 10) {
             result[i-1] += result[i] / 10;
             result[i] %= 10;
         }
     }
     
     // Combine the digits to form the final result
     long final_result = 0;
     int start_pos = 0;
     
     // Skip leading zeros
     while (start_pos < result_size && result[start_pos] == 0) {
         start_pos++;
     }
     
     // Special case: if all digits are 0, return 0
     if (start_pos == result_size) {
         free(a_digits);
         free(b_digits);
         free(result);
         return 0;
     }
     
     // Combine digits
     for (int i = start_pos; i < result_size; i++) {
         final_result = final_result * 10 + result[i];
     }
     
     // Clean up
     free(a_digits);
     free(b_digits);
     free(result);
     
     return final_result;
 }
 
 #ifdef _OPENMP
 // OpenMP version for large numbers
 long urdhva_mult_parallel(long a, long b) {
     // Implementation with OpenMP for parallelization
     // This would be similar to the above but using OpenMP for
     // parallel computation of the cross-products
     
     // For now, just call the sequential version
     return urdhva_mult(a, b);
 }
 #endif