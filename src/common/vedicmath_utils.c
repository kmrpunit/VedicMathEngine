/**
 * vedicmath_utils.c - Utility functions for Vedic Mathematics Library
 * 
 * Contains common utility functions used across different sutras.
 */
#include "../../include/vedicmath.h"
#include <stdlib.h>
#include <math.h>
 
 /**
  * Library version information
  */
 const char* vedicmath_version(void) {
     return "VedicMath Library v1.0.0";
 }
 
 /**
  * Count the number of digits in a number
  */
 int count_digits(long n) {
     if (n == 0) return 1;
     
     n = (n < 0) ? -n : n;  // Handle negative numbers
     
     int count = 0;
     while (n > 0) {
         count++;
         n /= 10;
     }
     return count;
 }
 
 /**
  * Find the nearest power of 10 to a number
  */
 long nearest_power_of_10(long n) {
     if (n <= 0) return 10;  // Default for invalid input
     
     int digits = count_digits(n);
     long base = 1;
     
     // Compute 10^(digits-1)
     for (int i = 1; i < digits; i++) {
         base *= 10;
     }
     
     // Determine if the next power of 10 is closer
     if (n > base * 5) {
         base *= 10;  // Go to the next power of 10
     }
     
     return base;
 }
 
 /**
  * Check if the last digits of two numbers sum to 10
  */
 bool last_digits_sum_to_10(long a, long b) {
     int last_a = a % 10;
     int last_b = b % 10;
     
     return (last_a + last_b) == 10;
 }
 
 /**
  * Check if two numbers have the same prefix (all digits except the last)
  */
 bool same_prefix(long a, long b) {
     return a / 10 == b / 10;
 }
 
 /**
  * Extract the prefix (all digits except the last) of a number
  */
 long get_prefix(long n) {
     return n / 10;
 }
 
 /**
  * Get the last digit of a number
  */
 int get_last_digit(long n) {
     return n % 10;
 }
 
 /**
  * Check if a number ends with digit 5
  */
 bool ends_with_5(long n) {
     return (n % 10) == 5;
 }
 
 /**
  * Calculate the deficiency of a number from a base
  * Returns negative if number exceeds base
  */
 long calculate_deficiency(long n, long base) {
     return base - n;
 }
 
 /**
  * Calculate how much a number exceeds a base
  * Returns negative if number is less than base
  */
 long calculate_excess(long n, long base) {
     return n - base;
 }
 
 /**
  * Check if a number is "close" to a base
  * A number is considered close if it's within 10% of the base
  */
 bool is_close_to_base(long n, long base) {
     double ratio = (double)n / base;
     return (ratio >= 0.9 && ratio <= 1.1);
 }
 
 /**
  * Combine two parts into a number with concatenation
  * For example, combine_parts(12, 34, 2) = 1234
  */
 long combine_parts(long left, long right, int right_digits) {
     long multiplier = 1;
     for (int i = 0; i < right_digits; i++) {
         multiplier *= 10;
     }
     
     return left * multiplier + right;
 }