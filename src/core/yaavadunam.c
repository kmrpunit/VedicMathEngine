/**
 * yaavadunam.c - Implementation of "Whatever the extent of its deficiency" sutra
 * 
 * This sutra provides a fast method for squaring numbers that are close to a power of 10.
 */

 #include "vedicmath.h"

 /**
  * Yaavadunam - "Whatever the extent of its deficiency"
  * 
  * Purpose: Fast squaring of numbers just below or above a power of 10.
  * When to use: For squaring numbers like 98, 997, etc. that are close to a base.
  * 
  * Core logic: 
  * - For N = B - d (below base): N^2 = (N - d) || d^2
  * - For N = B + d (above base): N^2 = (N + d) || d^2
  * 
  * Example: 98^2 (base 100, d=2): 98-2=96, 2^2=4, result = 9604
  * 
  * @param n Number to square
  * @param base The base (power of 10) that n is close to
  * @return The square of n
  */
 long yaavadunam_square(long n, long base) {
     // Calculate deficiency or excess
     long d = base - n;  // Deficiency (if positive), excess (if negative)
     
     // Determine left part
     long left_part;
     if (d > 0) {
         // Number is below base
         left_part = n - d;  // Equivalent to base - 2*d
     } else {
         // Number is above base
         left_part = n + (-d);  // Equivalent to base + 2*d
     }
     
     // Right part is square of deficiency or excess
     long right_part = d * d;
     
     // Format right part to have correct number of digits
     int base_digits = count_digits(base) - 1;
     
     // Combine parts
     return combine_parts(left_part, right_part, base_digits);
 }
 
 /**
  * Helper function to determine the best base for Yaavadunam
  * 
  * @param n Number to find base for
  * @return The best power of 10 base for Yaavadunam
  */
 long yaavadunam_best_base(long n) {
     int digits = count_digits(n);
     
     // Calculate the two nearest powers of 10
     long lower_base = 1;
     for (int i = 1; i < digits; i++) {
         lower_base *= 10;
     }
     
     long upper_base = lower_base * 10;
     
     // See which one is closer
     if (n - lower_base < upper_base - n) {
         return lower_base;
     } else {
         return upper_base;
     }
 }