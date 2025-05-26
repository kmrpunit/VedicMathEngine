/**
 * enhanced_division_dispatcher.c - Enhanced division dispatcher for Vedic mathematics
 */

#include "vedicmath.h"
#include <stdlib.h>
#include <string.h>

/**
 * Enhanced division dispatcher - chooses best method automatically
 * 
 * This function analyzes the dividend and divisor to choose the most
 * appropriate division method from:
 * 1. Standard division (for simple cases)
 * 2. Paravartya Yojayet (existing method)
 * 3. Dhvajanka (new flag method)
 * 4. Nikhilam Division (new complement method)
 * 
 * @param dividend The number to be divided
 * @param divisor The number to divide by
 * @param remainder Pointer to store remainder
 * @param method_used Output parameter for method name (can be NULL)
 * @return The quotient using the best available method
 */
long vedic_divide_enhanced(long dividend, long divisor, long *remainder, const char **method_used) {
    // Handle edge cases
    if (divisor == 0) {
        if (remainder) *remainder = dividend;
        if (method_used) *method_used = "Error: Division by zero";
        return 0;
    }
    
    long abs_divisor = abs(divisor);
    int divisor_digits = count_digits(abs_divisor);
    
    // Decision logic for best method
    
    // 1. Simple cases - use standard division
    if (abs_divisor <= 10 || abs(dividend) < abs_divisor) {
        if (method_used) *method_used = "Standard Division";
        long quot = dividend / divisor;
        if (remainder) *remainder = dividend % divisor;
        return quot;
    }
    
    // 2. Check for Nikhilam suitability (divisor near power of 10)
    long nearest_base = nearest_power_of_10(abs_divisor);
    if (is_close_to_base(abs_divisor, nearest_base)) {
        double proximity = (double)abs_divisor / nearest_base;
        if (proximity >= 0.8 && proximity <= 1.2) {
            if (method_used) *method_used = "Nikhilam Division Sutra";
            return nikhilam_divide_sutra(dividend, divisor, remainder);
        }
    }
    
    // 3. Check for Dhvajanka suitability (multi-digit with clear flag)
    if (divisor_digits >= 2 && divisor_digits <= 4) {
        // Dhvajanka works well for 2-4 digit divisors
        if (method_used) *method_used = "Dhvajanka (Flag Method)";
        return dhvajanka_divide(dividend, divisor, remainder);
    }
    
    // 4. Check for Paravartya Yojayet suitability (existing method)
    if (divisor_digits == 2) {
        if (method_used) *method_used = "Paravartya Yojayet";
        return paravartya_divide(dividend, divisor, remainder);
    }
    
    // 5. Fallback to standard division
    if (method_used) *method_used = "Standard Division (Fallback)";
    long quot = dividend / divisor;
    if (remainder) *remainder = dividend % divisor;
    return quot;
}

/**
 * Utility function to test and compare all division methods
 * 
 * This function is useful for benchmarking and validation
 * 
 * @param dividend Test dividend
 * @param divisor Test divisor
 */
void test_all_division_methods(long dividend, long divisor) {
    printf("\n=== Division Method Comparison: %ld ÷ %ld ===\n", dividend, divisor);
    
    long remainder;
    const char* method_name;
    
    // Standard division
    long std_quot = dividend / divisor;
    long std_rem = dividend % divisor;
    printf("Standard:        %ld R %ld\n", std_quot, std_rem);
    
    // Enhanced dispatcher
    long enhanced_quot = vedic_divide_enhanced(dividend, divisor, &remainder, &method_name);
    printf("Enhanced (%s): %ld R %ld\n", method_name, enhanced_quot, remainder);
    
    // Individual methods
    long dhvajanka_quot = dhvajanka_divide(dividend, divisor, &remainder);
    printf("Dhvajanka:       %ld R %ld\n", dhvajanka_quot, remainder);
    
    long nikhilam_quot = nikhilam_divide_sutra(dividend, divisor, &remainder);
    printf("Nikhilam Div:    %ld R %ld\n", nikhilam_quot, remainder);
    
    if (abs(divisor) <= 100) {  // Only test Paravartya for smaller divisors
        long paravartya_quot = paravartya_divide(dividend, divisor, &remainder);
        printf("Paravartya:      %ld R %ld\n", paravartya_quot, remainder);
    }
    
    // Verification
    bool all_correct = (enhanced_quot == std_quot && 
                       dhvajanka_quot == std_quot && 
                       nikhilam_quot == std_quot);
    printf("Verification:    %s\n", all_correct ? "✓ All methods agree" : "⚠ Methods disagree");
}