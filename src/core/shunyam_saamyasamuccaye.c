/**
 * shunyam_saamyasamuccaye.c - Implementation of "When the sum is the same, that sum is zero" sutra
 * 
 * This sutra provides methods for solving algebraic equations where equal sums appear on both sides.
 */

 #include "vedicmath.h"
 #include <stdbool.h>
 
 /**
  * Check if a polynomial has a root at x=value
  * 
  * @param coefficients Array of polynomial coefficients (highest degree first)
  * @param degree The degree of the polynomial
  * @param value The value to check
  * @return true if value is a root, false otherwise
  */
 bool is_polynomial_root(double *coefficients, int degree, double value) {
     double result = 0.0;
     double power = 1.0;
     
     // Evaluate polynomial at value using Horner's method
     for (int i = degree; i >= 0; i--) {
         result += coefficients[i] * power;
         power *= value;
     }
     
     // Check if result is close enough to zero (allow for floating point error)
     return (result < 0.000001 && result > -0.000001);
 }
 
 /**
  * Shunyam Saamyasamuccaye - "When the sum is the same, that sum is zero"
  * 
  * Purpose: Solve equations where equal expressions appear on both sides
  * When to use: For algebraic equations where the same term appears on both sides
  * 
  * Core logic: If f(x) + C = g(x) + C, then C = 0 or f(x) = g(x)
  * 
  * @param coefficients Array of polynomial coefficients (highest degree first)
  * @param degree The degree of the polynomial
  * @param root Pointer to store the found root
  * @return 0 if successful, -1 if no zero-sum solution found
  */
 int shunyam_samuccaye(double *coefficients, int degree, double *root) {
     // Check if there's a root at x=0 (constant term is zero)
     if (degree > 0 && coefficients[0] == 0.0) {
         *root = 0.0;
         return 0;
     }
     
     // Check if there's a root at x=1 (sum of coefficients is zero)
     double sum = 0.0;
     for (int i = 0; i <= degree; i++) {
         sum += coefficients[i];
     }
     
     if (sum == 0.0) {
         *root = 1.0;
         return 0;
     }
     
     // Try simple integer roots between -10 and 10
     for (int i = -10; i <= 10; i++) {
         if (i == 0) continue; // Already checked above
         
         if (is_polynomial_root(coefficients, degree, (double)i)) {
             *root = (double)i;
             return 0;
         }
     }
     
     // No simple root found
     return -1;
 }
 
 /**
  * Apply the Shunyam principle to simplify a rational expression
  * When both numerator and denominator have a common factor,
  * that factor can be canceled out.
  * 
  * @param num_coeffs Numerator polynomial coefficients
  * @param num_degree Numerator polynomial degree
  * @param den_coeffs Denominator polynomial coefficients
  * @param den_degree Denominator polynomial degree
  * @param common_factor Pointer to store the common factor (x - root)
  * @return 0 if a common factor found, -1 otherwise
  */
 int shunyam_simplify_rational(double *num_coeffs, int num_degree,
                              double *den_coeffs, int den_degree,
                              double *common_factor) {
     // Check for common roots in numerator and denominator
     double num_root, den_root;
     bool num_has_root = (shunyam_samuccaye(num_coeffs, num_degree, &num_root) == 0);
     bool den_has_root = (shunyam_samuccaye(den_coeffs, den_degree, &den_root) == 0);
     
     if (num_has_root && den_has_root && num_root == den_root) {
         *common_factor = num_root;
         return 0;
     }
     
     return -1;
 }