/**
 * vedicmath_optimized.h - Performance-optimized dynamic type operations
 * 
 * This file defines highly optimized versions of the dynamic type operations,
 * focusing on reducing overhead from conditional branches and unnecessary checks.
 */

 #ifndef VEDICMATH_OPTIMIZED_H
 #define VEDICMATH_OPTIMIZED_H
 
 #include "vedicmath_types.h"
 
 /**
  * Optimized dynamic multiplication using function lookup tables and fast paths
  * 
  * @param a First operand
  * @param b Second operand
  * @return The product a * b as a VedicValue
  */
 inline VedicValue vedic_optimized_multiply(VedicValue a, VedicValue b);
 
 /**
  * Optimized dynamic addition
  * 
  * @param a First operand
  * @param b Second operand
  * @return The sum a + b as a VedicValue
  */
 inline VedicValue vedic_optimized_add(VedicValue a, VedicValue b);
 
 /**
  * Optimized dynamic subtraction
  * 
  * @param a First operand
  * @param b Second operand
  * @return The difference a - b as a VedicValue
  */
 inline VedicValue vedic_optimized_subtract(VedicValue a, VedicValue b);
 
 /**
  * Optimized dynamic division
  * 
  * @param a Dividend
  * @param b Divisor
  * @return The quotient a / b as a VedicValue
  */
 inline VedicValue vedic_optimized_divide(VedicValue a, VedicValue b);
 
 /**
  * Optimized dynamic modulo
  * 
  * @param a Dividend
  * @param b Divisor
  * @return The remainder a % b as a VedicValue
  */
 inline VedicValue vedic_optimized_modulo(VedicValue a, VedicValue b);
 
 /**
  * Optimized dynamic power operation
  * 
  * @param a Base
  * @param b Exponent
  * @return a^b as a VedicValue
  */
 inline VedicValue vedic_optimized_power(VedicValue a, VedicValue b);
 
 /**
  * Optimized evaluation of a simple expression
  * Uses lookup tables, fast paths, and result caching
  * 
  * @param expression The expression to evaluate
  * @return The result as a VedicValue
  */
 VedicValue vedic_optimized_evaluate(const char* expression);
 
 /**
  * Optimized batch multiplication of arrays
  * Uses SIMD operations when available
  * 
  * @param results Array to store results
  * @param a Array of first operands
  * @param b Array of second operands
  * @param count Number of elements to process
  */
 void vedic_optimized_multiply_batch(VedicValue* results, 
                                    const VedicValue* a, 
                                    const VedicValue* b, 
                                    size_t count);
 
 /**
  * Optimized batch expression evaluation
  * 
  * @param results Array to store results
  * @param expressions Array of expressions to evaluate
  * @param count Number of expressions
  */
 void vedic_optimized_evaluate_batch(VedicValue* results,
                                    const char** expressions,
                                    size_t count);
 
 /**
  * Initialize the optimization tables
  * Should be called once at program startup
  */
 void vedic_optimized_init(void);
 
 /**
  * Cleanup optimization resources
  * Should be called at program termination
  */
 void vedic_optimized_cleanup(void);
 
 #endif /* VEDICMATH_OPTIMIZED_H */