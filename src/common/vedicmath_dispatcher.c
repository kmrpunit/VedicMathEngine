/**
 * vedicmath_dispatcher.c - Central dispatcher functions for Vedic Mathematics
 * 
 * These functions analyze input characteristics and automatically select
 * the most appropriate Vedic mathematics technique for computation.
 */

 #include "vedicmath.h"
 #include <stdlib.h>  // For abs function
 
 /**
  * Vedic multiply - Central dispatcher function
  * 
  * Automatically selects the most efficient Vedic method based on input characteristics.
  * 
  * @param a First number to multiply
  * @param b Second number to multiply
  * @return The product a * b
  */
 long vedic_multiply(long a, long b) {
     // Handle trivial cases first (Vilokanam observation)
     if (a == 0 || b == 0) return 0;
     if (a == 1) return b;
     if (b == 1) return a;
     
     // Handle negative numbers
     int sign = 1;
     if (a < 0) {
         a = -a;
         sign = -sign;
     }
     if (b < 0) {
         b = -b;
         sign = -sign;
     }
     
     // For very small numbers, just use direct multiplication
     if (a < 10 && b < 10) {
         return sign * (a * b);
     }
     
     // Get digit counts for both numbers
     int digits_a = count_digits(a);
     int digits_b = count_digits(b);
     
     // Find nearest power of 10 base
     long base_a = nearest_power_of_10(a);
     long base_b = nearest_power_of_10(b);
     
     // Check if a number ends in 5
     if ((a % 10 == 5) && (b % 10 == 5) && (a == b)) {
         // Same number ending in 5, use Ekadhikena Purvena for squaring
         return sign * ekadhikena_purvena(a);
     }
     
     // Check if last digits sum to 10 and the prefixes are the same
     if (last_digits_sum_to_10(a, b) && same_prefix(a, b)) {
         return sign * antya_dasake_mul(a, b);
     }
     
     // Check if one or both numbers are close to a power of 10
     if (is_close_to_base(a, base_a) && is_close_to_base(b, base_b)) {
         // Both numbers are close to powers of 10, use Nikhilam
         return sign * nikhilam_mul(a, b);
     } else if (is_close_to_base(a, base_a) || is_close_to_base(b, base_b)) {
         // One number is close to a power of 10
         
         // If close to the same base, use Nikhilam
         if (base_a == base_b) {
             return sign * nikhilam_mul(a, b);
         }
         
         // Otherwise, decision depends on how close they are
         double ratio_a = (double)a / base_a;
         double ratio_b = (double)b / base_b;
         
         if ((ratio_a >= 0.95 && ratio_a <= 1.05) || 
             (ratio_b >= 0.95 && ratio_b <= 1.05)) {
             // Very close to a base, use Nikhilam
             return sign * nikhilam_mul(a, b);
         }
     }
     
     // For numbers with many digits, Urdhva-Tiryagbhyam is generally efficient
     if (digits_a > 2 || digits_b > 2) {
         #ifdef _OPENMP
         // Use parallel version for large numbers if OpenMP is available
         if (digits_a > 3 && digits_b > 3) {
             return sign * urdhva_mult_parallel(a, b);
         }
         #endif
         return sign * urdhva_mult(a, b);
     }
     
     // Default to standard multiplication for small numbers or when no special pattern applies
     return sign * (a * b);
 }
 
 /**
  * Vedic square - Central dispatcher function for squaring
  * 
  * Chooses the best method to square a number based on its characteristics.
  * 
  * @param n Number to square
  * @return The square of n
  */
 long vedic_square(long n) {
     // Handle trivial cases
     if (n == 0 || n == 1) return n;
     
     // Handle negative numbers (square is always positive)
     if (n < 0) n = -n;
     
     // For very small numbers, just square directly
     if (n < 10) return n * n;
     
     // If number ends in 5, use Ekadhikena Purvena
     if (n % 10 == 5) {
         return ekadhikena_purvena(n);
     }
     
     // Find the nearest power of 10 base
     long base = nearest_power_of_10(n);
     
     // If number is close to a power of 10, use Yaavadunam
     if (is_close_to_base(n, base)) {
         double ratio = (double)n / base;
         if (ratio >= 0.9 && ratio <= 1.1) {
             return yaavadunam_square(n, base);
         }
     }
     
     // Default to multiplication
     return vedic_multiply(n, n);
 }
 
 /**
  * Vedic divide - Central dispatcher function for division
  * 
  * Automatically selects the most efficient Vedic method for division.
  * 
  * @param dividend Number to be divided
  * @param divisor Number to divide by
  * @param remainder Pointer to store remainder (can be NULL if not needed)
  * @return The quotient
  */
 long vedic_divide(long dividend, long divisor, long *remainder) {
     // Handle trivial cases
     if (divisor == 0) {
         // Division by zero
         if (remainder) *remainder = dividend;
         return (dividend < 0) ? -1 : 1;  // Return a non-zero error value
     }
     
     if (divisor == 1) {
         if (remainder) *remainder = 0;
         return dividend;
     }
     
     if (divisor == -1) {
         if (remainder) *remainder = 0;
         return -dividend;
     }
     
     // For simple cases, use standard division
     if (abs(divisor) < 10 || abs(dividend) < 100) {
         long quot = dividend / divisor;
         if (remainder) *remainder = dividend % divisor;
         return quot;
     }
     
     // For 2-digit divisors, use Paravartya Yojayet
     if (abs(divisor) < 100) {
         return paravartya_divide(dividend, divisor, remainder);
     }
     
     // For now, default to standard division for larger divisors
     // A more comprehensive library would implement additional Vedic techniques
     long quot = dividend / divisor;
     if (remainder) *remainder = dividend % divisor;
     return quot;
 }

 /**
 * DISPATCHER DEBUG: Add logging to see what's actually happening
 * 
 * Add this to src/common/vedicmath_dispatcher.c to debug
 * why Vedic methods aren't being selected
 */

// Modified vedic_multiply with debug logging
long vedic_multiply_debug(long a, long b) {
    printf("DEBUG: vedic_multiply(%ld, %ld)\n", a, b);
    
    // Handle trivial cases first
    if (a == 0 || b == 0) {
        printf("  -> Using direct (zero case)\n");
        return 0;
    }
    if (a == 1) {
        printf("  -> Using direct (unit case)\n");
        return b;
    }
    if (b == 1) {
        printf("  -> Using direct (unit case)\n");
        return a;
    }
    
    // Handle negative numbers
    int sign = 1;
    if (a < 0) { a = -a; sign = -sign; }
    if (b < 0) { b = -b; sign = -sign; }
    
    // For very small numbers, just use direct multiplication
    if (a < 10 && b < 10) {
        printf("  -> Using direct (small numbers: %ld, %ld)\n", a, b);
        return sign * (a * b);
    }
    
    // Check if a number ends in 5 (Ekadhikena Purvena)
    if ((a % 10 == 5) && (b % 10 == 5) && (a == b)) {
        printf("  -> Using Ekadhikena Purvena (%ld ends in 5)\n", a);
        return sign * ekadhikena_purvena(a);
    }
    
    // Check if last digits sum to 10 and prefixes are same (Antyayordasake)
    if (last_digits_sum_to_10(a, b) && same_prefix(a, b)) {
        printf("  -> Using Antyayordasake (%ld, %ld: last digits sum to 10)\n", a, b);
        return sign * antya_dasake_mul(a, b);
    }
    
    // Check if numbers are close to powers of 10 (Nikhilam)
    long base_a = nearest_power_of_10(a);
    long base_b = nearest_power_of_10(b);
    if (base_a == base_b && is_close_to_base(a, base_a) && is_close_to_base(b, base_b)) {
        printf("  -> Using Nikhilam (%ld, %ld near base %ld)\n", a, b, base_a);
        return sign * nikhilam_mul(a, b);
    }
    
    // Check for large numbers (Urdhva-Tiryagbhyam)
    int digits_a = count_digits(a);
    int digits_b = count_digits(b);
    if (digits_a > 2 || digits_b > 2) {
        printf("  -> Using Urdhva-Tiryagbhyam (%d, %d digits)\n", digits_a, digits_b);
        return sign * urdhva_mult(a, b);
    }
    
    // Default to standard multiplication
    printf("  -> Using standard multiplication (no pattern match)\n");
    return sign * (a * b);
}

/**
 * Test the dispatcher with known patterns
 */
void test_dispatcher_patterns() {
    printf("\n=== DISPATCHER PATTERN TESTING ===\n");
    
    // Test Ekadhikena Purvena
    printf("\nTesting Ekadhikena Purvena:\n");
    vedic_multiply_debug(25, 25);  // Should use Ekadhikena
    vedic_multiply_debug(35, 35);  // Should use Ekadhikena
    vedic_multiply_debug(45, 45);  // Should use Ekadhikena
    
    // Test Antyayordasake
    printf("\nTesting Antyayordasake:\n");
    vedic_multiply_debug(47, 43);  // 4 prefix, 7+3=10
    vedic_multiply_debug(52, 58);  // 5 prefix, 2+8=10
    vedic_multiply_debug(91, 99);  // 9 prefix, 1+9=10
    
    // Test Nikhilam
    printf("\nTesting Nikhilam:\n");
    vedic_multiply_debug(98, 97);  // Near 100
    vedic_multiply_debug(102, 103); // Near 100
    vedic_multiply_debug(995, 998); // Near 1000
    
    // Test random numbers (should use standard)
    printf("\nTesting Random Numbers:\n");
    vedic_multiply_debug(123, 456); // Should use Urdhva or standard
    vedic_multiply_debug(17, 29);   // Should use standard
}

/**
 * CRITICAL: Check if is_close_to_base() is working correctly
 */
void debug_close_to_base() {
    printf("\n=== DEBUGGING is_close_to_base() ===\n");
    
    // Test numbers near 100
    long base = 100;
    int test_numbers[] = {98, 97, 96, 95, 102, 103, 104, 105, 90, 110};
    
    for (int i = 0; i < 10; i++) {
        long n = test_numbers[i];
        bool is_close = is_close_to_base(n, base);
        double ratio = (double)n / base;
        printf("  %ld vs base %ld: is_close=%s, ratio=%.3f\n", 
               n, base, is_close ? "YES" : "NO", ratio);
    }
}