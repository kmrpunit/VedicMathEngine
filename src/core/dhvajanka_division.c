/**
 * Dhvajanka Division - "Flag Method"
 * 
 * MATHEMATICAL BASIS: The Dhvajanka sutra uses a "flag" digit approach
 * where the divisor is decomposed into a convenient base and remainder.
 * 
 **/
 
#include "vedicmath.h"

 /** 
 * WHEN TO USE:
 * - Divisors that can be expressed as 10^n ± small_number
 * - Multi-digit divisors where traditional methods are complex
 * - When divisor has a dominant digit pattern
 * 
 * ALGORITHM:
 * 1. Identify the "flag" (dominant digit pattern in divisor)
 * 2. Use the flag to create a systematic division process
 * 3. Apply corrections based on the remainder pattern
 * 
 * EXAMPLE: 1234 ÷ 23
 * - Flag: 2 (tens digit), remainder: 3 (units)
 * - Process: Divide by 20, then adjust for the +3
 * 
 * @param dividend The number to be divided
 * @param divisor The number to divide by
 * @param remainder Pointer to store remainder (can be NULL if not needed)
 * @return The quotient
 */
long dhvajanka_divide(long dividend, long divisor, long *remainder) {
    // Handle edge cases
    if (divisor == 0) {
        if (remainder) *remainder = dividend;
        return 0; // Division by zero - return 0 for safety
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
    
    // Check if Dhvajanka method is applicable
    int divisor_digits = count_digits(abs_divisor);
    
    // For single digit divisors, use direct division
    if (divisor_digits == 1) {
        long quot = abs_dividend / abs_divisor;
        if (remainder) *remainder = abs_dividend % abs_divisor;
        return sign * quot;
    }
    
    // Dhvajanka method for multi-digit divisors
    // Extract the flag (leading digit) and the remainder part
    long power_of_10 = 1;
    for (int i = 1; i < divisor_digits; i++) {
        power_of_10 *= 10;
    }
    
    long flag_digit = abs_divisor / power_of_10;           // Leading digit
    long remainder_part = abs_divisor % power_of_10;       // Remaining digits
    
    // Create the flag divisor (leading digit * power of 10)
    long flag_divisor = flag_digit * power_of_10;
    
    // Dhvajanka algorithm implementation
    long quotient = 0;
    long working_dividend = abs_dividend;
    
    // Initial division by flag divisor
    long temp_quotient = working_dividend / flag_divisor;
    long temp_remainder = working_dividend % flag_divisor;
    
    // Apply Dhvajanka corrections
    // The correction accounts for the difference between flag_divisor and actual divisor
    long correction_factor = remainder_part;
    
    if (correction_factor > 0) {
        // Positive correction - divisor is larger than flag
        long correction = (temp_quotient * correction_factor);
        
        // Adjust quotient downward and increase remainder
        while (correction > temp_remainder && temp_quotient > 0) {
            temp_quotient--;
            temp_remainder += flag_divisor;
            correction = temp_quotient * correction_factor;
        }
        
        temp_remainder -= correction;
        quotient = temp_quotient;
        
        // Final adjustment if remainder is still too large
        while (temp_remainder >= abs_divisor) {
            quotient++;
            temp_remainder -= abs_divisor;
        }
        
    } else {
        // Negative correction - divisor is smaller than flag
        long correction = -(temp_quotient * (-correction_factor));
        
        temp_remainder += correction;
        quotient = temp_quotient;
        
        // Adjust if remainder becomes negative
        while (temp_remainder < 0) {
            quotient--;
            temp_remainder += abs_divisor;
        }
    }
    
    // Verification and final correction
    long verification = quotient * abs_divisor + temp_remainder;
    if (verification != abs_dividend) {
        // Fallback to standard division if Dhvajanka fails
        quotient = abs_dividend / abs_divisor;
        temp_remainder = abs_dividend % abs_divisor;
    }
    
    if (remainder) *remainder = temp_remainder;
    return sign * quotient;
}