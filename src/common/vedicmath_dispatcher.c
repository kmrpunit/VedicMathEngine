/**
 * vedicmath_dispatcher.c - Central dispatcher functions for Vedic Mathematics
 *
 * These functions analyze input characteristics and automatically select
 * the most appropriate Vedic mathematics technique for computation.
 */

#include "vedicmath.h"
#include <stdlib.h> // For abs function
#include "dispatch_decision.h"
#include "vedic_core.h"
#include "dispatch_mixed_mode.h" // The mode switcher

/**
 * Check if a number ends in 5
 *
 * @param n Number to check
 * @return 1 if it ends in 5, 0 otherwise  
 */
DispatchDecision dispatch_multiply_decision(int64_t a, int64_t b)
{
    DispatchDecision decision;

    if (ends_in_5(a) && a == b)
    {
        decision.result = square_ekadhikena(a);
        decision.method_used = "Ekadhikena Purvena";
        decision.reason = "Both numbers same and end in 5";
        decision.mode = "vedic";
        decision.time_taken_ns = 0; // Optional: benchmark it
        return decision;
    }

    if (is_near_power_of_10(a, b))
    {
        decision.result = multiply_nikhilam(a, b);
        decision.method_used = "Nikhilam";
        decision.reason = "Both near base power of 10";
        decision.mode = "vedic";
        return decision;
    }
    
    if (last_digits_sum_to_10(a, b) && same_prefix(a, b))
    {
        decision.result = antya_dasake_mul(a, b);
        decision.method_used = "Antya Dasake";
        decision.reason = "Last digits sum to 10 and prefixes are same";
        decision.mode = "vedic";
        return decision;
    }

    // Default fallback
    decision.result = dispatch_multiply(a, b); // Uses optimized/dynamic
    decision.method_used = "Urdhva-Tiryagbhyam";
    decision.reason = "General multiplication";
    decision.mode = is_using_optimized_mode() ? "optimized" : "dynamic";
    return decision;
}




/**
 * Vedic multiply - Central dispatcher function
 *
 * Automatically selects the most efficient Vedic method based on input characteristics.
 *
 * @param a First number to multiply
 * @param b Second number to multiply
 * @return The product a * b
 */
long vedic_multiply(long a, long b)
{
    // Handle trivial cases first (Vilokanam observation)
    if (a == 0 || b == 0)
        return 0;
    if (a == 1)
        return b;
    if (b == 1)
        return a;

    // Handle negative numbers
    int sign = 1;
    if (a < 0)
    {
        a = -a;
        sign = -sign;
    }
    if (b < 0)
    {
        b = -b;
        sign = -sign;
    }

    // For very small numbers, just use direct multiplication
    if (a < 10 && b < 10)
    {
        return sign * (a * b);
    }

    // Get digit counts for both numbers
    int digits_a = count_digits(a);
    int digits_b = count_digits(b);

    // Find nearest power of 10 base
    long base_a = nearest_power_of_10(a);
    long base_b = nearest_power_of_10(b);

    // Check if a number ends in 5
    if ((a % 10 == 5) && (b % 10 == 5) && (a == b))
    {
        // Same number ending in 5, use Ekadhikena Purvena for squaring
        return sign * ekadhikena_purvena(a);
    }

    // Check if last digits sum to 10 and the prefixes are the same
    if (last_digits_sum_to_10(a, b) && same_prefix(a, b))
    {
        return sign * antya_dasake_mul(a, b);
    }

    // Check if one or both numbers are close to a power of 10
    if (is_close_to_base(a, base_a) && is_close_to_base(b, base_b))
    {
        // Both numbers are close to powers of 10, use Nikhilam
        return sign * nikhilam_mul(a, b);
    }
    else if (is_close_to_base(a, base_a) || is_close_to_base(b, base_b))
    {
        // One number is close to a power of 10

        // If close to the same base, use Nikhilam
        if (base_a == base_b)
        {
            return sign * nikhilam_mul(a, b);
        }

        // Otherwise, decision depends on how close they are
        double ratio_a = (double)a / base_a;
        double ratio_b = (double)b / base_b;

        if ((ratio_a >= 0.95 && ratio_a <= 1.05) ||
            (ratio_b >= 0.95 && ratio_b <= 1.05))
        {
            // Very close to a base, use Nikhilam
            return sign * nikhilam_mul(a, b);
        }
    }

    // For numbers with many digits, Urdhva-Tiryagbhyam is generally efficient
    if (digits_a > 2 || digits_b > 2)
    {
#ifdef _OPENMP
        // Use parallel version for large numbers if OpenMP is available
        if (digits_a > 3 && digits_b > 3)
        {
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
long vedic_square(long n)
{
    // Handle trivial cases
    if (n == 0 || n == 1)
        return n;

    // Handle negative numbers (square is always positive)
    if (n < 0)
        n = -n;

    // For very small numbers, just square directly
    if (n < 10)
        return n * n;

    // If number ends in 5, use Ekadhikena Purvena
    if (n % 10 == 5)
    {
        return ekadhikena_purvena(n);
    }

    // Find the nearest power of 10 base
    long base = nearest_power_of_10(n);

    // If number is close to a power of 10, use Yaavadunam
    if (is_close_to_base(n, base))
    {
        double ratio = (double)n / base;
        if (ratio >= 0.9 && ratio <= 1.1)
        {
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
long vedic_divide(long dividend, long divisor, long *remainder)
{
    // Handle trivial cases
    if (divisor == 0)
    {
        // Division by zero
        if (remainder)
            *remainder = dividend;
        return (dividend < 0) ? -1 : 1; // Return a non-zero error value
    }

    if (divisor == 1)
    {
        if (remainder)
            *remainder = 0;
        return dividend;
    }

    if (divisor == -1)
    {
        if (remainder)
            *remainder = 0;
        return -dividend;
    }

    // For simple cases, use standard division
    if (abs(divisor) < 10 || abs(dividend) < 100)
    {
        long quot = dividend / divisor;
        if (remainder)
            *remainder = dividend % divisor;
        return quot;
    }

    // For 2-digit divisors, use Paravartya Yojayet
    if (abs(divisor) < 100)
    {
        return paravartya_divide(dividend, divisor, remainder);
    }

    // For now, default to standard division for larger divisors
    // A more comprehensive library would implement additional Vedic techniques
    long quot = dividend / divisor;
    if (remainder)
        *remainder = dividend % divisor;
    return quot;
}