/**
 * enhanced_division_dispatcher.c - Enhanced Division Dispatcher with Refined Logic
 *
 * ANALYSIS OF TEST RESULTS:
 * ✓ All mathematical results are correct
 * ✓ Nikhilam selection working perfectly
 * ✓ Standard division selection working
 * ⚠ Need to refine Dhvajanka vs Paravartya selection
 * ⚠ Need better fallback logic for complex cases
 */

#include "vedicmath.h"
#include <stdlib.h>
#include <string.h>

/**
 * IMPROVED: Enhanced division dispatcher with refined selection logic
 *
 * PRIORITY ORDER (from most to least specific):
 * 1. Standard Division - Simple cases (single digit, edge cases)
 * 2. Nikhilam Division - Divisors near powers of 10 (highest specificity)
 * 3. Paravartya Yojayet - 2-digit divisors (existing proven method)
 * 4. Dhvajanka - Multi-digit with clear patterns (broader applicability)
 * 5. Standard Fallback - Complex cases
 *
 * @param dividend The number to be divided
 * @param divisor The number to divide by
 * @param remainder Pointer to store remainder
 * @param method_used Output parameter for method name (can be NULL)
 * @return The quotient using the best available method
 */
long vedic_divide_enhanced(long dividend, long divisor, long *remainder, const char **method_used)
{
    // Handle edge cases
    if (divisor == 0)
    {
        if (remainder)
            *remainder = dividend;
        if (method_used)
            *method_used = "Error: Division by zero";
        return 0;
    }

    long abs_dividend = abs(dividend);
    long abs_divisor = abs(divisor);
    int divisor_digits = count_digits(abs_divisor);

    // === PRIORITY 1: Simple cases - use standard division ===
    if (abs_divisor <= 10 || abs_dividend < abs_divisor)
    {
        if (method_used)
            *method_used = "Standard Division";
        long quot = dividend / divisor;
        if (remainder)
            *remainder = dividend % divisor;
        return quot;
    }

    // === PRIORITY 2: Nikhilam suitability (highest specificity) ===
    long nearest_base = nearest_power_of_10(abs_divisor);
    if (is_close_to_base(abs_divisor, nearest_base))
    {
        double proximity_ratio = (double)abs_divisor / nearest_base;

        // Tighter criteria for Nikhilam: divisor must be very close to power of 10
        if ((proximity_ratio >= 0.85 && proximity_ratio <= 1.15) &&
            (nearest_base == 10 || nearest_base == 100 || nearest_base == 1000 || nearest_base == 10000))
        {

            // Additional check: ensure the complement is small enough to be beneficial
            long complement = abs(abs_divisor - nearest_base);
            if (complement <= nearest_base / 10)
            { // Complement should be ≤ 10% of base
                if (method_used)
                    *method_used = "Nikhilam Division Sutra";
                return nikhilam_divide_sutra(dividend, divisor, remainder);
            }
        }
    }

    // === PRIORITY 3: Paravartya Yojayet (proven 2-digit method) ===
    if (divisor_digits == 2)
    {
        // Check if it's a "nice" 2-digit divisor (not too close to power of 10)
        bool near_power_of_10 = (abs_divisor >= 95 && abs_divisor <= 105);

        if (!near_power_of_10)
        {
            // Prefer Paravartya for regular 2-digit divisors
            if (method_used)
                *method_used = "Paravartya Yojayet";
            return paravartya_divide(dividend, divisor, remainder);
        }
    }

    // === PRIORITY 4: Dhvajanka suitability (broader patterns) ===
    if (divisor_digits >= 2 && divisor_digits <= 4)
    {
        // Check if divisor has a clear "flag" pattern
        bool suitable_for_dhvajanka = false;

        if (divisor_digits == 2)
        {
            // For 2-digit: use if not already handled by Paravartya/Nikhilam
            suitable_for_dhvajanka = true;
        }
        else if (divisor_digits == 3)
        {
            // For 3-digit: check if it has a dominant leading digit
            long leading_digit = abs_divisor / 100;
            long remaining = abs_divisor % 100;

            // Good for Dhvajanka if leading digit is significant relative to remainder
            suitable_for_dhvajanka = (leading_digit >= 2 && remaining < leading_digit * 50);
        }
        else if (divisor_digits == 4)
        {
            // For 4-digit: more conservative - only if very clear pattern
            long leading_two = abs_divisor / 100;
            long remaining = abs_divisor % 100;

            suitable_for_dhvajanka = (leading_two >= 10 && remaining < leading_two * 5);
        }

        if (suitable_for_dhvajanka)
        {
            if (method_used)
                *method_used = "Dhvajanka (Flag Method)";
            return dhvajanka_divide(dividend, divisor, remainder);
        }
    }

    // === PRIORITY 5: Standard division fallback ===
    if (method_used)
        *method_used = "Standard Division (Fallback)";
    long quot = dividend / divisor;
    if (remainder)
        *remainder = dividend % divisor;
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