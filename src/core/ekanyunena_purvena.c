/**
 * ekanyunena_purvena.c - Implementation of "By one less than the previous one" sutra
 * 
 * This sutra provides a fast method for multiplying a number by 9, 99, 999, etc.,
 * or for finding complements.
 */

 #include "vedicmath.h"

 /**
  * Ekanyunena Purvena - "By one less than the previous one"
  * 
  * Purpose: Fast multiplication by 9, 99, 999, etc. (series of 9s)
  * When to use: When multiplying by 9, 99, 999, etc.
  * 
  * Core logic: For N × 9: take one less than N for the left part,
  * and the complement to 10 for the right part.
  * For N × 99: take one less than N for the left part,
  * and the complement to 100 for the right part.
  * 
  * Example: 47 × 9: 47-1=46 and complement to 10 = 10-7 = 3 → 423
  * 
  * @param multiplicand Number to multiply
  * @param all9s_multiplier Multiplier consisting of all 9s (9, 99, 999, etc.)
  * @return The product
  */
 long ekanyunena_purvena_mul(long multiplicand, long all9s_multiplier) {
     // Handle negative numbers
     int sign = 1;
     if (multiplicand < 0) {
         multiplicand = -multiplicand;
         sign = -sign;
     }
     
     // Count number of 9s in multiplier
     int count_9s = 0;
     long temp = all9s_multiplier;
     while (temp > 0) {
         if (temp % 10 != 9) {
             // Not a valid all-9s multiplier
             return sign * multiplicand * all9s_multiplier;
         }
         count_9s++;
         temp /= 10;
     }
     
     // Calculate the base (10^count_9s)
     long base = 1;
     for (int i = 0; i < count_9s; i++) {
         base *= 10;
     }
     
     // For multiplying by 9, 99, 999, etc.
     // result = multiplicand * (base - 1) = multiplicand * base - multiplicand
     
     // Left part: one less than the multiplicand
     long left_part = multiplicand - 1;
     
     // Right part: complement of the multiplicand to the base
     long right_part = base - multiplicand;
     
     // Format right part to have exactly count_9s digits
     int right_digits = count_digits(right_part);
     
     // If right_part has fewer digits than count_9s, pad with leading zeros
     if (right_digits < count_9s) {
         long result = left_part * base + right_part;
         return sign * result;
     } else {
         // Handle cases where the complement needs more digits
         return sign * (multiplicand * all9s_multiplier);
     }
 }
 
 /**
  * Calculate the complement of a number with respect to a power of 10
  * 
  * For example, complement of 47 with respect to 100 is 53
  * 
  * @param n Number to find complement of
  * @param base Power of 10 to find complement with respect to
  * @return The complement
  */
 long ekanyunena_complement(long n, long base) {
     if (n >= base) {
         // Cannot calculate complement if n is greater than or equal to base
         return -1;
     }
     
     return base - n;
 }
 
 /**
  * Multiply a number by 9, 99, 999, etc. faster than standard multiplication
  * 
  * @param n Number to multiply
  * @param count_9s Number of 9s in the multiplier (1 for 9, 2 for 99, etc.)
  * @return The product
  */
 long multiply_by_all_9s(long n, int count_9s) {
     // Generate the all-9s multiplier
     long multiplier = 0;
     for (int i = 0; i < count_9s; i++) {
         multiplier = multiplier * 10 + 9;
     }
     
     return ekanyunena_purvena_mul(n, multiplier);
 }