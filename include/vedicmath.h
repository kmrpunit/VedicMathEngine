/**
 * vedicmath.h - Header file for Vedic Mathematics Library
 * 
 * A modular C library implementing Vedic Mathematics techniques for efficient computation.
 * 
 * This library provides implementations of all 16 sutras and 13 sub-sutras of Vedic Mathematics,
 * organized as separate functions with a central dispatcher to automatically select the
 * most efficient method based on input characteristics.
 */

 #ifndef VEDICMATH_H
 #define VEDICMATH_H
 
 #include <stdbool.h>
 #include <math.h>
 #include <stdlib.h>
 #include "vedicmath_platform.h"


 // include/vedicmath.h (add these declarations)

/**
 * Check if a number is close to a base (power of 10)
 * 
 * @param n The number to check
 * @param base The base to compare against
 * @return true if n is close to base, false otherwise
 */
VEDICMATH_API bool is_close_to_base(long n, long base);

/**
 * Combine two parts into a number with concatenation
 * For example, combine_parts(12, 34, 2) = 1234
 * 
 * @param left Left part of the number
 * @param right Right part of the number
 * @param right_digits Number of digits to use for the right part
 * @return The combined number
 */
VEDICMATH_API long combine_parts(long left, long right, int right_digits);
 
 /**
  * Ekadhikena Purvena - "By one more than the previous one"
  * 
  * Purpose: Fast squaring of numbers ending in 5.
  * When to use: For squaring any number that ends in 5.
  * 
  * Core logic: If N ends in 5, split N = m*10 + 5. The result N^2 = m * (m+1) appended with 25.
  * Example: N=25 -> m=2, m*(m+1)=2*3=6, append 25 => 625.
  * 
  * @param n Number ending in 5 to be squared
  * @return The square of n
  */
 long ekadhikena_purvena(long n);
 
 /**
  * Nikhilam Navatashcaramam Dashatah - "All from 9 and the last from 10"
  * 
  * Purpose: Efficient multiplication of numbers near a power of 10.
  * When to use: For multiplying numbers close to a common base (10, 100, 1000, etc.)
  * 
  * Core logic: Uses the deviation from base for fast multiplication.
  * Example: 98 × 96 (near 100): Left part = 98-4 = 94, Right part = 4×4 = 16, Result = 9416
  * 
  * @param a First number to multiply
  * @param b Second number to multiply
  * @return The product a * b
  */
 long nikhilam_mul(long a, long b);
 
 /**
  * Urdhva-Tiryagbhyam - "Vertically and crosswise"
  * 
  * Purpose: General multiplication method using vertical and crosswise pattern.
  * When to use: For any multiplication when no special pattern applies.
  * 
  * Core logic: Every digit of the first number is multiplied with every digit of the second
  * in a structured way from left to right.
  * 
  * @param a First number to multiply
  * @param b Second number to multiply
  * @return The product a * b
  */
 long urdhva_mult(long a, long b);
 
 /**
  * Paravartya Yojayet - "Transpose and adjust"
  * 
  * Purpose: Division using the transpose and adjust method.
  * When to use: For division by numbers not near a convenient base.
  * 
  * @param dividend The number to be divided
  * @param divisor The number to divide by
  * @param remainder Pointer to store the remainder (can be NULL if not needed)
  * @return The quotient
  */
 long paravartya_divide(long dividend, long divisor, long *remainder);
 
 /**
  * Yaavadunam - "Whatever the extent of its deficiency"
  * 
  * Purpose: Fast squaring of numbers just below a power of 10.
  * When to use: For squaring numbers like 98, 997, etc. that are just below a base.
  * 
  * Core logic: For N = B - d, compute N^2 = (N - d) || d^2 where || is concatenation.
  * Example: 98^2 (base 100, d=2): 98-2=96, 2^2=4, result = 9604
  * 
  * @param n Number to square
  * @param base The base (power of 10) that n is close to
  * @return The square of n
  */
 long yaavadunam_square(long n, long base);
 
 /**
  * Antyayordasake - "Last totaling 10"
  * 
  * Purpose: Multiply two numbers whose last digits sum to 10 and prefixes are equal.
  * When to use: For pairs like 47×43, 26×24, etc.
  * 
  * Core logic: If numbers are mA and mB with A+B=10, result = m(m+1) || (A×B)
  * Example: 47×43: m=4, A=7, B=3, result = 4×5 || 7×3 = 20||21 = 2021
  * 
  * @param a First number
  * @param b Second number
  * @return The product a * b
  */
 int antya_dasake_mul(int a, int b);
 
 /**
  * Vedic multiply - Central dispatcher function
  * 
  * Automatically selects the most efficient Vedic method based on input characteristics.
  * 
  * @param a First number to multiply
  * @param b Second number to multiply
  * @return The product a * b
  */
 long vedic_multiply(long a, long b);
 
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
 long vedic_divide(long dividend, long divisor, long *remainder);
 
 /**
  * Vedic square - Central dispatcher function for squaring
  * 
  * Chooses the best method to square a number based on its characteristics.
  * 
  * @param n Number to square
  * @return The square of n
  */
 long vedic_square(long n);
 
 /**
  * Count the number of digits in a number
  * 
  * @param n The number
  * @return The number of digits
  */
 int count_digits(long n);
 
 /**
  * Find the nearest power of 10 to a number
  * 
  * @param n The number
  * @return The nearest power of 10
  */
 long nearest_power_of_10(long n);
 
 /**
  * Check if the last digits of two numbers sum to 10
  * 
  * @param a First number
  * @param b Second number
  * @return true if the last digits sum to 10, false otherwise
  */
 bool last_digits_sum_to_10(long a, long b);
 
 /**
  * Check if two numbers have the same prefix (all digits except the last)
  * 
  * @param a First number
  * @param b Second number
  * @return true if prefixes are the same, false otherwise
  */
 bool same_prefix(long a, long b);
 
 /**
  * Sankalana-Vyavakalanabhyam - "By addition and by subtraction"
  * 
  * Purpose: Solve two simultaneous linear equations of the form a₁x + b₁y = c₁ and a₂x + b₂y = c₂
  * When to use: For solving pairs of linear equations efficiently
  * 
  * @param a1 Coefficient of x in first equation
  * @param b1 Coefficient of y in first equation
  * @param c1 Constant term in first equation
  * @param a2 Coefficient of x in second equation
  * @param b2 Coefficient of y in second equation
  * @param c2 Constant term in second equation
  * @param x Pointer to store the value of x
  * @param y Pointer to store the value of y
  * @return 0 if successful, -1 if no unique solution exists
  */
 int sankalana_vyavakalanabhyam_solve(int a1, int b1, int c1, int a2, int b2, int c2,
                                      double *x, double *y);
 
 /**
  * Fast mental addition using Vedic principles
  * 
  * @param a First number
  * @param b Second number
  * @return The sum a + b
  */
 long vedic_add(long a, long b);
 
 /**
  * Fast mental subtraction using Vedic principles
  * 
  * @param a Minuend (number to subtract from)
  * @param b Subtrahend (number to be subtracted)
  * @return The difference a - b
  */
 long vedic_subtract(long a, long b);
 
 /**
  * Shunyam Saamyasamuccaye - "When the sum is the same, that sum is zero"
  * 
  * Purpose: Solve equations where equal expressions appear on both sides
  * When to use: For algebraic equations where the same term appears on both sides
  * 
  * @param coefficients Array of polynomial coefficients (highest degree first)
  * @param degree The degree of the polynomial
  * @param root Pointer to store the found root
  * @return 0 if successful, -1 if no zero-sum solution found
  */
 int shunyam_samuccaye(double *coefficients, int degree, double *root);
 
 /**
  * Check if a polynomial has a root at x=value
  * 
  * @param coefficients Array of polynomial coefficients (highest degree first)
  * @param degree The degree of the polynomial
  * @param value The value to check
  * @return true if value is a root, false otherwise
  */
 bool is_polynomial_root(double *coefficients, int degree, double value);
 
 /**
  * Ekanyunena Purvena - "By one less than the previous one"
  * 
  * Purpose: Fast multiplication by 9, 99, 999, etc. (series of 9s)
  * When to use: When multiplying by 9, 99, 999, etc.
  * 
  * @param multiplicand Number to multiply
  * @param all9s_multiplier Multiplier consisting of all 9s (9, 99, 999, etc.)
  * @return The product
  */
 long ekanyunena_purvena_mul(long multiplicand, long all9s_multiplier);
 
 /**
  * Calculate the complement of a number with respect to a power of 10
  * 
  * @param n Number to find complement of
  * @param base Power of 10 to find complement with respect to
  * @return The complement
  */
 long ekanyunena_complement(long n, long base);
 
 /**
  * Multiply a number by 9, 99, 999, etc. faster than standard multiplication
  * 
  * @param n Number to multiply
  * @param count_9s Number of 9s in the multiplier (1 for 9, 2 for 99, etc.)
  * @return The product
  */
 long multiply_by_all_9s(long n, int count_9s);
 
 /**
  * Puranapuranabhyam - "By the completion or non-completion"
  * 
  * Purpose: Simplified calculation by rounding up or down to a convenient base
  * When to use: For addition, subtraction, or multiplication where rounding
  * to a convenient number simplifies the calculation
  * 
  * @param a First number
  * @param b Second number
  * @param operation 0 for addition, 1 for subtraction, 2 for multiplication
  * @return The result of the operation
  */
 long puranapuranabhyam_calc(long a, long b, int operation);
 
 /**
  * Simplify addition by using the completion method
  * 
  * @param a First number
  * @param b Second number
  * @return The sum a + b
  */
 long puranapuranabhyam_add(long a, long b);
 
 /**
  * Simplify subtraction by using the completion method
  * 
  * @param a Minuend (number to subtract from)
  * @param b Subtrahend (number to be subtracted)
  * @return The difference a - b
  */
 long puranapuranabhyam_subtract(long a, long b);
 
 /**
  * Vestanam - "By Osculation"
  * 
  * Purpose: Test if a number is divisible by certain primes by applying
  * a factor to the last digit and adding/subtracting from the rest
  * 
  * When to use: For quickly determining divisibility by primes like 7, 13, 17, etc.
  * 
  * @param number The number to check
  * @param prime The prime divisor to test
  * @return 1 if divisible, 0 if not, -1 if prime not supported
  */
 int vestanam_divisibility(long number, int prime);
 
 /**
  * Test if a number is divisible by 7 (special case of Vestanam)
  * 
  * @param number The number to check
  * @return 1 if divisible by 7, 0 if not
  */
 int is_divisible_by_7(long number);
 
 /**
  * Test if a number is divisible by 13 (special case of Vestanam)
  * 
  * @param number The number to check
  * @return 1 if divisible by 13, 0 if not
  */
 int is_divisible_by_13(long number);
 
 /**
  * Anurupyena - "Proportionately"
  * 
  * Purpose: Adjust calculations proportionately when numbers aren't
  * directly near an easy base but are proportionally near a factor of a base.
  * 
  * When to use: When numbers are around a multiple of a power of 10 (like 500, 250).
  * 
  * @param a First number
  * @param b Second number
  * @param scale Factor to scale the base by
  * @return The product a * b
  */
 long anurupyena_mul(long a, long b, int scale);
 
 /**
  * Find the best scaling factor for two numbers
  * 
  * @param a First number
  * @param b Second number
  * @return The suggested scaling factor
  */
 int anurupyena_best_scale(long a, long b);
 
 /**
  * Scale up or down a number by a factor
  * 
  * @param n Number to scale
  * @param scale_factor Factor to scale by
  * @param scale_up 1 to scale up, 0 to scale down
  * @return The scaled number
  */
 double anurupyena_scale(long n, int scale_factor, int scale_up);
 
 /**
  * Addition operator using Vedic principles when appropriate
  * 
  * @param a First number
  * @param b Second number
  * @return The sum a + b
  */
 long vedic_op_add(long a, long b);
 
 /**
  * Subtraction operator using Vedic principles when appropriate
  * 
  * @param a Minuend (number to subtract from)
  * @param b Subtrahend (number to be subtracted)
  * @return The difference a - b
  */
 long vedic_op_subtract(long a, long b);
 
 /**
  * Multiplication operator using Vedic principles
  * 
  * @param a First number
  * @param b Second number
  * @return The product a * b
  */
 long vedic_op_multiply(long a, long b);
 
 /**
  * Division operator using Vedic principles
  * 
  * @param a Dividend
  * @param b Divisor
  * @return The quotient a / b
  */
 long vedic_op_divide(long a, long b);
 
 /**
  * Modulo operator using Vedic principles
  * 
  * @param a Dividend
  * @param b Divisor
  * @return The remainder a % b
  */
 long vedic_op_modulo(long a, long b);
 
 /**
  * Power operator using Vedic principles
  * 
  * @param base The base
  * @param exponent The exponent (must be >= 0)
  * @return base^exponent
  */
 long vedic_op_power(long base, int exponent);
 
 /**
  * Parse and evaluate a simple mathematical expression using Vedic methods
  * 
  * Supports basic operators: +, -, *, /, %, ^
  * 
  * @param expression The expression to evaluate (must be properly formatted)
  * @param result Pointer to store the result
  * @return 0 if successful, -1 if parsing error
  */
 int vedic_evaluate_expression(const char *expression, long *result);
 
 /**
  * Library version information
  * 
  * @return String with library version
  */
 const char* vedicmath_version(void);
 
 #endif /* VEDICMATH_H */