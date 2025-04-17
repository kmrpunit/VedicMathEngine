/**
 * anurupyena.c - Implementation of "Proportionately" sub-sutra
 * 
 * This sub-sutra helps adjust calculations when numbers are proportionally near a base.
 */

 #include "vedicmath.h"

 /**
  * Anurupyena - "Proportionately"
  * 
  * Purpose: Adjust calculations proportionately when numbers aren't
  * directly near an easy base but are proportionally near a factor of a base.
  * 
  * When to use: When numbers are around a multiple of a power of 10 (like 500, 250).
  * 
  * Core logic: Scale down proportionately, use another sutra, then scale back up.
  * 
  * @param a First number
  * @param b Second number
  * @param scale Factor to scale the base by
  * @return The product a * b
  */
 long anurupyena_mul(long a, long b, int scale) {
     // For small numbers, direct multiplication is fine
     if (a < 100 || b < 100) {
         return a * b;
     }
     
     // Check if scale makes sense
     if (scale <= 0) {
         return a * b;  // Invalid scale, use direct multiplication
     }
     
     // Check if numbers are proportionate to the scale
     double a_scaled = (double)a / scale;
     double b_scaled = (double)b / scale;
     
     // If they're whole numbers after scaling, it's perfect for Anurupyena
     if (a_scaled == (long)a_scaled && b_scaled == (long)b_scaled) {
         // Scale down, multiply, then scale back up
         long result_scaled = vedic_multiply((long)a_scaled, (long)b_scaled);
         return result_scaled * scale * scale;
     }
     
     // If they're very close to whole numbers after scaling
     if (a_scaled - (long)a_scaled < 0.1 && b_scaled - (long)b_scaled < 0.1) {
         // Scale down, multiply, then scale back up with adjustment
         long a_approx = (long)(a_scaled + 0.5);
         long b_approx = (long)(b_scaled + 0.5);
         long result_approx = vedic_multiply(a_approx, b_approx);
         
         // Adjust for approximation error
         double error_factor = (a_scaled * b_scaled) / (a_approx * b_approx);
         return (long)(result_approx * scale * scale * error_factor);
     }
     
     // If proportional scaling doesn't work well, use direct multiplication
     return a * b;
 }
 
 /**
  * Find the best scaling factor for two numbers
  * 
  * @param a First number
  * @param b Second number
  * @return The suggested scaling factor
  */
 int anurupyena_best_scale(long a, long b) {
     // Find the nearest power of 10 for both numbers
     long base_a = nearest_power_of_10(a);
     long base_b = nearest_power_of_10(b);
     
     // If they're near the same base, try factors of that base
     if (base_a == base_b) {
         long base = base_a;
         int common_factors[] = {2, 4, 5, 8, 10, 20, 25, 50};
         int best_factor = 1;
         double best_error = 1000000.0;  // Large initial error
         
         for (int i = 0; i < sizeof(common_factors) / sizeof(int); i++) {
             int factor = common_factors[i];
             double a_scaled = (double)a / factor;
             double b_scaled = (double)b / factor;
             
             // Calculate how close these are to whole numbers
             double error_a = a_scaled - (long)a_scaled;
             double error_b = b_scaled - (long)b_scaled;
             
             if (error_a < 0) error_a = 1 + error_a;
             if (error_b < 0) error_b = 1 + error_b;
             
             double total_error = error_a + error_b;
             
             if (total_error < best_error) {
                 best_error = total_error;
                 best_factor = factor;
             }
         }
         
         // If the error is small enough, suggest this factor
         if (best_error < 0.2) {
             return best_factor;
         }
     }
     
     // No good scaling factor found
     return 1;
 }
 
 /**
  * Scale up or down a number by a factor
  * 
  * @param n Number to scale
  * @param scale_factor Factor to scale by
  * @param scale_up 1 to scale up, 0 to scale down
  * @return The scaled number
  */
 double anurupyena_scale(long n, int scale_factor, int scale_up) {
     if (scale_up) {
         return n * scale_factor;
     } else {
         return (double)n / scale_factor;
     }
 }