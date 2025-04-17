/**
 * nikhilam_navatashcaramam.c - Implementation of "All from 9 and the last from 10" sutra
 * 
 * This sutra provides efficient methods for multiplication, especially for numbers
 * close to a power of 10 base.
 */

 #include "vedicmath.h"
 #include <stdlib.h>  // For abs function
 
 /**
  * Determine the best base for Nikhilam multiplication
  * 
  * @param a First number
  * @param b Second number
  * @return The best power of 10 base
  */
 static long nikhilam_best_base(long a, long b) {
     long base_a = nearest_power_of_10(a);
     long base_b = nearest_power_of_10(b);
     
     // If both numbers are closer to the same base, use that
     if (base_a == base_b) {
         return base_a;
     }
     
     // If bases are different, choose the one that minimizes the deviations
     long dev_a_to_base_a = labs(a - base_a);
     long dev_b_to_base_a = labs(b - base_a);
     long dev_a_to_base_b = labs(a - base_b);
     long dev_b_to_base_b = labs(b - base_b);
     
     // Choose base that minimizes total deviation
     if (dev_a_to_base_a + dev_b_to_base_a <= dev_a_to_base_b + dev_b_to_base_b) {
         return base_a;
     } else {
         return base_b;
     }
 }
 
 /**
  * Format the right part to have the correct number of digits based on the base
  * 
  * @param right_part The right part of the result
  * @param base The base used for calculation
  * @return The formatted right part
  */
 static long format_right_part(long right_part, long base) {
     int base_digits = count_digits(base) - 1;
     
     // If right_part has fewer digits than needed, pad with zeros
     int right_digits = count_digits(right_part);
     
     // If right_part has more digits than the base, we need to carry
     if (right_digits > base_digits) {
         long carry = right_part / base;
         right_part %= base;
         return right_part + (carry * base);
     }
     
     return right_part;
 }
 
 /**
  * Nikhilam multiplication - Type 1: Both numbers below base
  * 
  * @param a First number (below base)
  * @param b Second number (below base)
  * @param base The power of 10 base
  * @return The product a * b
  */
 static long nikhilam_below_base(long a, long b, long base) {
     // Calculate deficiencies
     long deficiency_a = base - a;
     long deficiency_b = base - b;
     
     // Left part: a - deficiency_b or b - deficiency_a (they're the same)
     long left_part = a - deficiency_b;  // Equivalent to: base - deficiency_a - deficiency_b
     
     // Right part: deficiency_a * deficiency_b
     long right_part = deficiency_a * deficiency_b;
     
     // Format right part to have correct number of digits
     int base_digits = count_digits(base) - 1;
     
     // Combine parts
     return combine_parts(left_part, right_part, base_digits);
 }
 
 /**
  * Nikhilam multiplication - Type 2: Both numbers above base
  * 
  * @param a First number (above base)
  * @param b Second number (above base)
  * @param base The power of 10 base
  * @return The product a * b
  */
 static long nikhilam_above_base(long a, long b, long base) {
     // Calculate excesses
     long excess_a = a - base;
     long excess_b = b - base;
     
     // Left part: a + excess_b or b + excess_a (they're the same)
     long left_part = a + excess_b;  // Equivalent to: base + excess_a + excess_b
     
     // Right part: excess_a * excess_b
     long right_part = excess_a * excess_b;
     
     // Format right part to have correct number of digits
     int base_digits = count_digits(base) - 1;
     
     // Combine parts
     return combine_parts(left_part, right_part, base_digits);
 }
 
 /**
  * Nikhilam multiplication - Type 3: One number above base, one below
  * 
  * @param a First number
  * @param b Second number
  * @param base The power of 10 base
  * @return The product a * b
  */
 static long nikhilam_mixed_base(long a, long b, long base) {
     // Ensure a is above base and b is below
     if (a < base && b > base) {
         long temp = a;
         a = b;
         b = temp;
     }
     
     // Calculate excess and deficiency
     long excess_a = a - base;
     long deficiency_b = base - b;
     
     // Left part: a - deficiency_b or base + excess_a - deficiency_b
     long left_part = a - deficiency_b;
     
     // Right part: excess_a * deficiency_b (with negative sign)
     long right_part = excess_a * deficiency_b;
     
     // Since one number is above and one below, the right part is negative
     // This means we need to subtract it from the base
     long formatted_right = 0;
     int base_digits = count_digits(base) - 1;
     
     // Calculate power of 10 for the right part
     long power = 1;
     for (int i = 0; i < base_digits; i++) {
         power *= 10;
     }
     
     // If right_part is smaller than power, we need to subtract from power
     if (right_part < power) {
         formatted_right = power - right_part;
         left_part--;  // Borrow 1 from left part
     } else {
         // This case is more complex - we'd need to handle multi-digit borrowing
         // For simplicity, let's just do standard multiplication
         return a * b;
     }
     
     // Combine parts
     return combine_parts(left_part, formatted_right, base_digits);
 }
 
 /**
  * Nikhilam Navatashcaramam Dashatah - "All from 9 and the last from 10"
  * 
  * Purpose: Efficient multiplication of numbers near a power of 10.
  * When to use: For multiplying numbers close to a common base (10, 100, 1000, etc.)
  * 
  * @param a First number to multiply
  * @param b Second number to multiply
  * @return The product a * b
  */
 long nikhilam_mul(long a, long b) {
     // For very small numbers or numbers far from a base, use regular multiplication
     if (a < 10 || b < 10) {
         return a * b;
     }
     
     // Find the best base for calculation
     long base = nikhilam_best_base(a, b);
     
     // Check if both numbers are close enough to the base
     if (!is_close_to_base(a, base) || !is_close_to_base(b, base)) {
         return a * b;  // Regular multiplication if not close to base
     }
     
     // Determine which type of Nikhilam to use
     if (a < base && b < base) {
         // Type 1: Both numbers below base
         return nikhilam_below_base(a, b, base);
     } else if (a > base && b > base) {
         // Type 2: Both numbers above base
         return nikhilam_above_base(a, b, base);
     } else {
         // Type 3: One number above base, one below
         return nikhilam_mixed_base(a, b, base);
     }
 }