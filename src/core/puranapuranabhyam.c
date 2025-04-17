/**
 * puranapuranabhyam.c - Implementation of "By the completion or non-completion" sutra
 * 
 * This sutra provides methods for calculations by completing to a round figure
 * or working with the deficit.
 */

 #include "vedicmath.h"
 #include <stdlib.h>
 
 /**
  * Puranapuranabhyam - "By the completion or non-completion"
  * 
  * Purpose: Simplified calculation by rounding up or down to a convenient base
  * When to use: For addition, subtraction, or multiplication where rounding
  * to a convenient number simplifies the calculation
  * 
  * Core logic: Round to a convenient number, perform the calculation,
  * then adjust for the difference.
  * 
  * @param a First number
  * @param b Second number
  * @param operation 0 for addition, 1 for subtraction, 2 for multiplication
  * @return The result of the operation
  */
 long puranapuranabhyam_calc(long a, long b, int operation) {
     // For very small numbers, use direct calculation
     if (abs(a) < 10 || abs(b) < 10) {
         switch (operation) {
             case 0: return a + b;
             case 1: return a - b;
             case 2: return a * b;
             default: return 0;
         }
     }
     
     // Find nearest power of 10 for first number
     long base_a = nearest_power_of_10(a);
     
     // Calculate how much to add to reach the base (completion)
     long diff_a = base_a - a;
     
     // Calculate result based on operation
     switch (operation) {
         case 0: // Addition
             // Example: 97 + 35 = 100 + 35 - 3 = 132
             if (abs(diff_a) < 10) {
                 // Complete to base_a then adjust
                 return base_a + b - diff_a;
             }
             return a + b; // Direct addition if not close to a base
             
         case 1: // Subtraction
             // Example: 97 - 35 = 100 - 35 - 3 = 62
             if (abs(diff_a) < 10) {
                 // Complete to base_a then adjust
                 return base_a - b - diff_a;
             }
             return a - b; // Direct subtraction if not close to a base
             
         case 2: // Multiplication
             // Example: 97 * 5 = (100 - 3) * 5 = 500 - 15 = 485
             if (abs(diff_a) < 10) {
                 // Use non-completion: (base_a - diff_a) * b = base_a * b - diff_a * b
                 return base_a * b - diff_a * b;
             }
             return a * b; // Direct multiplication if not close to a base
             
         default:
             return 0;
     }
 }
 
 /**
  * Special case of Puranapuranabhyam for multiplication near a base
  * 
  * When numbers are close to powers of 10, this method is more efficient
  * 
  * @param a First number
  * @param b Second number
  * @return The product a * b
  */
 long puranapuranabhyam_mul(long a, long b) {
     // For small numbers, direct multiplication is best
     if (abs(a) < 10 || abs(b) < 10) {
         return a * b;
     }
     
     // Find nearest power of 10 for both numbers
     long base_a = nearest_power_of_10(a);
     long base_b = nearest_power_of_10(b);
     
     // Calculate deficiencies or excesses
     long diff_a = base_a - a;
     long diff_b = base_b - b;
     
     // Check if both are close to their bases
     if (abs(diff_a) < base_a / 10 && abs(diff_b) < base_b / 10) {
         // Both close to bases, use the distributive property
         // (base_a - diff_a) * (base_b - diff_b) = 
         // base_a * base_b - base_a * diff_b - base_b * diff_a + diff_a * diff_b
         return base_a * base_b - base_a * diff_b - base_b * diff_a + diff_a * diff_b;
     }
     
     // If only one is close to a base, use simpler formula
     if (abs(diff_a) < base_a / 10) {
         return base_a * b - diff_a * b;
     }
     
     if (abs(diff_b) < base_b / 10) {
         return a * base_b - a * diff_b;
     }
     
     // Otherwise, direct multiplication
     return a * b;
 }
 
 /**
  * Simplify addition by using the completion method
  * 
  * @param a First number
  * @param b Second number
  * @return The sum a + b
  */
 long puranapuranabhyam_add(long a, long b) {
     return puranapuranabhyam_calc(a, b, 0);
 }
 
 /**
  * Simplify subtraction by using the completion method
  * 
  * @param a Minuend (number to subtract from)
  * @param b Subtrahend (number to be subtracted)
  * @return The difference a - b
  */
 long puranapuranabhyam_subtract(long a, long b) {
     return puranapuranabhyam_calc(a, b, 1);
 }