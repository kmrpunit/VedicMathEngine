/**
 * vedicmath_dynamic.h - Dynamic type operations for Vedic Mathematics Library
 * 
 * This file defines functions that work with the dynamic type system to
 * automatically select the appropriate implementation based on data types.
 */

 #ifndef VEDICMATH_DYNAMIC_H
 #define VEDICMATH_DYNAMIC_H
 
 #include "vedicmath_types.h"
 
 /**
  * Perform dynamic multiplication using the appropriate Vedic technique
  * 
  * This function automatically selects the best implementation based on 
  * the types of the operands.
  * 
  * @param a First operand
  * @param b Second operand
  * @return The product a * b as a VedicValue
  */
 VedicValue vedic_dynamic_multiply(VedicValue a, VedicValue b);
 
 /**
  * Perform dynamic squaring using the appropriate Vedic technique
  * 
  * @param a The number to square
  * @return The square of a as a VedicValue
  */
 VedicValue vedic_dynamic_square(VedicValue a);
 
 /**
  * Perform dynamic division using the appropriate Vedic technique
  * 
  * @param a Dividend
  * @param b Divisor
  * @return The quotient a / b as a VedicValue
  */
 VedicValue vedic_dynamic_divide(VedicValue a, VedicValue b);
 
 /**
  * Perform dynamic addition using the appropriate Vedic technique
  * 
  * @param a First operand
  * @param b Second operand
  * @return The sum a + b as a VedicValue
  */
 VedicValue vedic_dynamic_add(VedicValue a, VedicValue b);
 
 /**
  * Perform dynamic subtraction using the appropriate Vedic technique
  * 
  * @param a First operand
  * @param b Second operand
  * @return The difference a - b as a VedicValue
  */
 VedicValue vedic_dynamic_subtract(VedicValue a, VedicValue b);
 
 /**
  * Perform dynamic modulo operation using the appropriate Vedic technique
  * 
  * @param a Dividend
  * @param b Divisor
  * @return The remainder a % b as a VedicValue
  */
 VedicValue vedic_dynamic_modulo(VedicValue a, VedicValue b);
 
 /**
  * Perform a dynamic operation based on operator type
  * 
  * @param a First operand
  * @param b Second operand
  * @param op The operation to perform
  * @return The result as a VedicValue
  */
 VedicValue vedic_dynamic_operation(VedicValue a, VedicValue b, VedicOperation op);
 
 /**
  * Parse and evaluate an expression with dynamic types
  * 
  * @param expression The expression to evaluate
  * @return The result as a VedicValue
  */
 VedicValue vedic_dynamic_evaluate(const char* expression);
 
 /**
  * Create a VedicValue from a number in a string
  * 
  * @param number_str String representation of the number
  * @return VedicValue containing the parsed number
  */
 VedicValue vedic_dynamic_from_string(const char* number_str);
 
 /**
  * Type-specific multiplication for int32
  * 
  * @param a First operand
  * @param b Second operand
  * @return The product a * b
  */
 int32_t vedic_multiply_i32(int32_t a, int32_t b);
 
 /**
  * Type-specific multiplication for int64
  * 
  * @param a First operand
  * @param b Second operand
  * @return The product a * b
  */
 int64_t vedic_multiply_i64(int64_t a, int64_t b);
 
 /**
  * Type-specific multiplication for float
  * 
  * @param a First operand
  * @param b Second operand
  * @return The product a * b
  */
 float vedic_multiply_f32(float a, float b);
 
 /**
  * Type-specific multiplication for double
  * 
  * @param a First operand
  * @param b Second operand
  * @return The product a * b
  */
 double vedic_multiply_f64(double a, double b);
 
 /**
  * Type-specific squaring for int32
  * 
  * @param a The number to square
  * @return The square of a
  */
 int32_t vedic_square_i32(int32_t a);
 
 /**
  * Type-specific squaring for int64
  * 
  * @param a The number to square
  * @return The square of a
  */
 int64_t vedic_square_i64(int64_t a);
 
 /**
  * Type-specific squaring for float
  * 
  * @param a The number to square
  * @return The square of a
  */
 float vedic_square_f32(float a);
 
 /**
  * Type-specific squaring for double
  * 
  * @param a The number to square
  * @return The square of a
  */
 double vedic_square_f64(double a);
 
 #endif /* VEDICMATH_DYNAMIC_H */