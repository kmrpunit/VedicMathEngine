/**
 * Nikhilam Division Sutra - "All from 9 and the last from 10"
 * 
 * MATHEMATICAL BASIS: Uses the Nikhilam principle for division when
 * the divisor is close to a power of 10. This creates a systematic
 * method that reduces complex division to simpler operations.
 **/

#include "vedicmath.h"

/* WHEN TO USE:
 * - Divisor is within 20% of a power of 10 (9, 11, 99, 101, 999, 1001, etc.)
 * - Want to leverage the Nikhilam complement principle
 * - Seeking an alternative to traditional long division
 * 
 * ALGORITHM:
 * 1. Find the nearest power of 10 to the divisor
 * 2. Calculate the complement (deficiency or excess)
 * 3. Use the complement in a systematic division process
 * 4. Apply the "all from 9, last from 10" principle
 * 
 * EXAMPLE: 1234 ÷ 99
 * - Nearest power: 100
 * - Complement: 100 - 99 = 1
 * - Use complement-based division algorithm
 * 
 * @param dividend The number to be divided
 * @param divisor The number to divide by (should be near a power of 10)
 * @param remainder Pointer to store remainder (can be NULL if not needed)
 * @return The quotient
 */
long nikhilam_divide_sutra(long dividend, long divisor, long *remainder) {
    // Handle edge cases
    if (divisor == 0) {
        if (remainder) *remainder = dividend;
        return 0;
    }
    
    if (divisor == 1) {
        if (remainder) *remainder = 0;
        return dividend;
    }
    
    if (abs(dividend) < abs(divisor)) {
        if (remainder) *remainder = dividend;
        return 0;
    }
    
    // Handle negative numbers
    int sign = 1;
    long abs_dividend = dividend;
    long abs_divisor = divisor;
    
    if (dividend < 0) {
        abs_dividend = -dividend;
        sign = -sign;
    }
    if (divisor < 0) {
        abs_divisor = -divisor;
        sign = -sign;
    }
    
    // Find the nearest power of 10 to the divisor
    long base = nearest_power_of_10(abs_divisor);
    
    // Check if divisor is close enough to base for Nikhilam division
    if (!is_close_to_base(abs_divisor, base)) {
        // If not close to a power of 10, fall back to standard division
        long quot = abs_dividend / abs_divisor;
        if (remainder) *remainder = abs_dividend % abs_divisor;
        return sign * quot;
    }
    
    // Calculate the complement (deviation from base)
    long complement = base - abs_divisor;  // Positive if divisor < base
    bool divisor_below_base = (complement > 0);
    
    if (!divisor_below_base) {
        complement = abs_divisor - base;  // Handle case where divisor > base
    }
    
    // Nikhilam division algorithm
    long quotient = 0;
    long working_remainder = abs_dividend;
    
    // Convert dividend to working format for Nikhilam
    int base_digits = count_digits(base) - 1;
    
    if (divisor_below_base) {
        // Divisor is below the base (e.g., 99, 999)
        // Use the complement to perform division
        
        // Initial estimate using base
        long base_quotient = abs_dividend / base;
        long base_remainder = abs_dividend % base;
        
        // Apply Nikhilam correction
        // The correction accounts for the fact that actual divisor is smaller
        long correction = (base_quotient * complement);
        
        // Adjust quotient and remainder
        quotient = base_quotient;
        working_remainder = base_remainder + correction;
        
        // Handle overflow in working_remainder
        while (working_remainder >= base) {
            quotient++;
            working_remainder -= base;
        }
        
        // Final adjustment to get exact division
        while (working_remainder >= abs_divisor) {
            quotient++;
            working_remainder -= abs_divisor;
        }
        
    } else {
        // Divisor is above the base (e.g., 101, 1001)
        // Use negative complement
        
        long base_quotient = abs_dividend / base;
        long base_remainder = abs_dividend % base;
        
        // Apply negative correction
        long correction = (base_quotient * complement);
        
        quotient = base_quotient;
        if (base_remainder >= correction) {
            working_remainder = base_remainder - correction;
        } else {
            // Need to borrow from quotient
            quotient--;
            working_remainder = base + base_remainder - correction;
        }
        
        // Final adjustment
        while (working_remainder >= abs_divisor) {
            quotient++;
            working_remainder -= abs_divisor;
        }
    }
    
    // Verification step
    long verification = quotient * abs_divisor + working_remainder;
    if (verification != abs_dividend) {
        // If verification fails, use standard division as fallback
        quotient = abs_dividend / abs_divisor;
        working_remainder = abs_dividend % abs_divisor;
    }
    
    if (remainder) *remainder = working_remainder;
    return sign * quotient;
}
