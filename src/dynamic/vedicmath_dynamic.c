/**
 * vedicmath_dynamic.c - Implementation of dynamic type operations
 */

 #include "vedicmath_dynamic.h"
 #include "vedicmath.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include <math.h>
 #include <limits.h>
 
 /**
  * Type-specific multiplication for int32
  */
 int32_t vedic_multiply_i32(int32_t a, int32_t b) {
     // Call the existing vedic_multiply but ensure 32-bit result
     return (int32_t)vedic_multiply((long)a, (long)b);
 }
 
 /**
  * Type-specific multiplication for int64
  */
 int64_t vedic_multiply_i64(int64_t a, int64_t b) {
     // For large values, we need to be careful about overflow
     // Check if result would fit in a long
     if ((a == 0) || (b == 0)) {
         return 0;
     }
     
     if (llabs(a) > INT64_MAX / llabs(b)) {
         // Overflow would occur - handle this case
         int sign = ((a < 0) ^ (b < 0)) ? -1 : 1;
         return (sign > 0) ? INT64_MAX : INT64_MIN;
     }
     
     // If both values fit in long, use vedic_multiply
     if (a <= LONG_MAX && a >= LONG_MIN && b <= LONG_MAX && b >= LONG_MIN) {
         return (int64_t)vedic_multiply((long)a, (long)b);
     }
     
     // Fall back to standard multiplication for larger values
     return a * b;
 }
 
 /**
  * Type-specific multiplication for float
  */
 float vedic_multiply_f32(float a, float b) {
     // For floating point, we need to handle special cases
     if (isnan(a) || isnan(b)) {
         return NAN;
     }
     
     if (isinf(a) || isinf(b)) {
         if (a == 0.0f || b == 0.0f) {
             return NAN;  // Infinity * 0 = NaN
         }
         int sign = ((a < 0.0f) ^ (b < 0.0f)) ? -1 : 1;
         return sign * INFINITY;
     }
     
     // Check if both can be converted to integers for Vedic methods
     if (a == (int32_t)a && b == (int32_t)b) {
         int32_t int_a = (int32_t)a;
         int32_t int_b = (int32_t)b;
         
         // Only use Vedic methods if both are in range for int32
         if (int_a <= INT32_MAX && int_a >= INT32_MIN && 
             int_b <= INT32_MAX && int_b >= INT32_MIN) {
             return (float)vedic_multiply_i32(int_a, int_b);
         }
     }
     
     // Otherwise use standard floating point multiplication
     return a * b;
 }
 
 /**
  * Type-specific multiplication for double
  */
 double vedic_multiply_f64(double a, double b) {
     // For floating point, we need to handle special cases
     if (isnan(a) || isnan(b)) {
         return NAN;
     }
     
     if (isinf(a) || isinf(b)) {
         if (a == 0.0 || b == 0.0) {
             return NAN;  // Infinity * 0 = NaN
         }
         int sign = ((a < 0.0) ^ (b < 0.0)) ? -1 : 1;
         return sign * INFINITY;
     }
     
     // Check if both can be converted to integers for Vedic methods
     if (a == (int64_t)a && b == (int64_t)b) {
         int64_t int_a = (int64_t)a;
         int64_t int_b = (int64_t)b;
         
         // Only use Vedic methods if both are in range for int64
         if (int_a <= INT64_MAX && int_a >= INT64_MIN && 
             int_b <= INT64_MAX && int_b >= INT64_MIN) {
             return (double)vedic_multiply_i64(int_a, int_b);
         }
     }
     
     // Otherwise use standard floating point multiplication
     return a * b;
 }
 
 /**
  * Type-specific squaring functions
  */
 int32_t vedic_square_i32(int32_t a) {
     return (int32_t)vedic_square((long)a);
 }
 
 int64_t vedic_square_i64(int64_t a) {
     // Check for potential overflow
     if (llabs(a) > (int64_t)sqrt((double)INT64_MAX)) {
         return (a > 0) ? INT64_MAX : 0; // Squared negative becomes positive
     }
     
     // Use Vedic squaring if within long range
     if (a <= LONG_MAX && a >= LONG_MIN) {
         return (int64_t)vedic_square((long)a);
     }
     
     // Fall back to standard multiplication
     return a * a;
 }
 
 float vedic_square_f32(float a) {
     if (isnan(a)) {
         return NAN;
     }
     
     if (isinf(a)) {
         return INFINITY;  // Infinity² = Infinity
     }
     
     // Check if can be converted to integer
     if (a == (int32_t)a) {
         int32_t int_a = (int32_t)a;
         
         // Only use Vedic methods if in range for int32
         if (int_a <= INT32_MAX && int_a >= INT32_MIN) {
             return (float)vedic_square_i32(int_a);
         }
     }
     
     // Otherwise use standard floating point multiplication
     return a * a;
 }
 
 double vedic_square_f64(double a) {
     if (isnan(a)) {
         return NAN;
     }
     
     if (isinf(a)) {
         return INFINITY;  // Infinity² = Infinity
     }
     
     // Check if can be converted to integer
     if (a == (int64_t)a) {
         int64_t int_a = (int64_t)a;
         
         // Only use Vedic methods if in range for int64
         if (int_a <= INT64_MAX && int_a >= INT64_MIN) {
             return (double)vedic_square_i64(int_a);
         }
     }
     
     // Otherwise use standard floating point multiplication
     return a * a;
 }
 
 /**
  * Perform dynamic multiplication using the appropriate Vedic technique
  */
 VedicValue vedic_dynamic_multiply(VedicValue a, VedicValue b) {
    // Determine the result type based on operand types
    VedicNumberType result_type = vedic_result_type(a.type, b.type);
    VedicValue result;
    result.type = result_type;
    
    // Fast path for int32 * int32 that might overflow to int64
    if (a.type == VEDIC_INT32 && b.type == VEDIC_INT32) {
        // Check for potential overflow to int64
        int64_t product = (int64_t)a.value.i32 * (int64_t)b.value.i32;
        
        if (product > INT32_MAX || product < INT32_MIN) {
            // Result doesn't fit in int32, promote to int64
            result.type = VEDIC_INT64;
            result.value.i64 = product;
            return result;
        }
    }
    
    // For very large int64 values, check for potential overflow
    if (result_type == VEDIC_INT64) {
        int64_t a_val = vedic_to_int64(a);
        int64_t b_val = vedic_to_int64(b);
        
        // Check if multiplication would overflow int64
        if (a_val != 0 && b_val != 0) {
            // Simple overflow check: if a * b > INT64_MAX, then overflow
            if (a_val > INT64_MAX / llabs(b_val) || b_val > INT64_MAX / llabs(a_val)) {
                // Switch to double precision
                result.type = VEDIC_DOUBLE;
                result.value.f64 = (double)a_val * (double)b_val;
                return result;
            }
        }
        
        result.value.i64 = a_val * b_val;
        return result;
    }
     
     // Perform multiplication based on the result type
     switch (result_type) {
         case VEDIC_INT32:
             result.value.i32 = vedic_multiply_i32(
                 vedic_to_int32(a), 
                 vedic_to_int32(b)
             );
             break;
             
         case VEDIC_INT64:
             result.value.i64 = vedic_multiply_i64(
                 vedic_to_int64(a), 
                 vedic_to_int64(b)
             );
             break;
             
         case VEDIC_FLOAT:
             result.value.f32 = vedic_multiply_f32(
                 vedic_to_float(a), 
                 vedic_to_float(b)
             );
             break;
             
         case VEDIC_DOUBLE:
             result.value.f64 = vedic_multiply_f64(
                 vedic_to_double(a), 
                 vedic_to_double(b)
             );
             break;
             
         default:
             // Invalid type - return 0
             result.type = VEDIC_INT32;
             result.value.i32 = 0;
             break;
     }
     
     return result;
 }
 
 /**
  * Perform dynamic squaring using the appropriate Vedic technique
  */
 VedicValue vedic_dynamic_square(VedicValue a) {
     VedicValue result;
     result.type = a.type;
     
     // Perform squaring based on the type
     switch (a.type) {
         case VEDIC_INT32:
             result.value.i32 = vedic_square_i32(a.value.i32);
             break;
             
         case VEDIC_INT64:
             result.value.i64 = vedic_square_i64(a.value.i64);
             break;
             
         case VEDIC_FLOAT:
             result.value.f32 = vedic_square_f32(a.value.f32);
             break;
             
         case VEDIC_DOUBLE:
             result.value.f64 = vedic_square_f64(a.value.f64);
             break;
             
         default:
             // Invalid type - return 0
             result.type = VEDIC_INT32;
             result.value.i32 = 0;
             break;
     }
     
     return result;
 }
 
 /**
  * Perform dynamic division
  */
 VedicValue vedic_dynamic_divide(VedicValue a, VedicValue b) {
     // Determine the result type based on operand types
     VedicNumberType result_type = vedic_result_type(a.type, b.type);
     VedicValue result;
     result.type = result_type;
     
     // Check for division by zero
     bool is_zero_b = false;
     switch (b.type) {
         case VEDIC_INT32: is_zero_b = (b.value.i32 == 0); break;
         case VEDIC_INT64: is_zero_b = (b.value.i64 == 0); break;
         case VEDIC_FLOAT: is_zero_b = (b.value.f32 == 0.0f); break;
         case VEDIC_DOUBLE: is_zero_b = (b.value.f64 == 0.0); break;
         default: is_zero_b = true; break;
     }
     
     if (is_zero_b) {
         // Division by zero
         switch (result_type) {
             case VEDIC_INT32:
             case VEDIC_INT64:
                 // For integers, return a large value with correct sign
                 if (result_type == VEDIC_INT32) {
                     int32_t sign = (vedic_to_int32(a) < 0) ? -1 : 1;
                     result.value.i32 = sign * INT32_MAX;
                 } else {
                     int64_t sign = (vedic_to_int64(a) < 0) ? -1 : 1;
                     result.value.i64 = sign * INT64_MAX;
                 }
                 break;
                 
             case VEDIC_FLOAT:
                 // For float, return infinity with correct sign
                 result.value.f32 = vedic_to_float(a) < 0 ? -INFINITY : INFINITY;
                 break;
                 
             case VEDIC_DOUBLE:
                 // For double, return infinity with correct sign
                 result.value.f64 = vedic_to_double(a) < 0 ? -INFINITY : INFINITY;
                 break;
                 
             default:
                 // Invalid type - return 0
                 result.type = VEDIC_INT32;
                 result.value.i32 = 0;
                 break;
         }
         
         return result;
     }
     
     // Perform division based on the result type
     switch (result_type) {
         case VEDIC_INT32: {
             int32_t dividend = vedic_to_int32(a);
             int32_t divisor = vedic_to_int32(b);
             long remainder;
             result.value.i32 = (int32_t)vedic_divide(dividend, divisor, &remainder);
             break;
         }
             
         case VEDIC_INT64: {
             int64_t dividend = vedic_to_int64(a);
             int64_t divisor = vedic_to_int64(b);
             
             // If both values fit in long, use vedic_divide
             if (dividend <= LONG_MAX && dividend >= LONG_MIN && 
                 divisor <= LONG_MAX && divisor >= LONG_MIN) {
                 long remainder;
                 result.value.i64 = (int64_t)vedic_divide((long)dividend, (long)divisor, &remainder);
             } else {
                 // Fall back to standard division
                 result.value.i64 = dividend / divisor;
             }
             break;
         }
             
         case VEDIC_FLOAT:
             result.value.f32 = vedic_to_float(a) / vedic_to_float(b);
             break;
             
         case VEDIC_DOUBLE:
             result.value.f64 = vedic_to_double(a) / vedic_to_double(b);
             break;
             
         default:
             // Invalid type - return 0
             result.type = VEDIC_INT32;
             result.value.i32 = 0;
             break;
     }
     
     return result;
 }
 
 /**
  * Perform dynamic addition
  */
 VedicValue vedic_dynamic_add(VedicValue a, VedicValue b) {
     // Determine the result type based on operand types
     VedicNumberType result_type = vedic_result_type(a.type, b.type);
     VedicValue result;
     result.type = result_type;

     // Check for int64 overflow
    if (result_type == VEDIC_INT64) {
        int64_t a_val = vedic_to_int64(a);
        int64_t b_val = vedic_to_int64(b);
        
        // Check for potential overflow
        if ((b_val > 0 && a_val > INT64_MAX - b_val) ||
            (b_val < 0 && a_val < INT64_MIN - b_val)) {
            // Overflow, switch to double
            result.type = VEDIC_DOUBLE;
            result.value.f64 = (double)a_val + (double)b_val;
            return result;
        }
        
        result.value.i64 = a_val + b_val;
        return result;
    }
     
     // Perform addition based on the result type
     switch (result_type) {
         case VEDIC_INT32:
             result.value.i32 = vedic_to_int32(a) + vedic_to_int32(b);
             break;
             
         case VEDIC_INT64:
             result.value.i64 = vedic_to_int64(a) + vedic_to_int64(b);
             break;
             
         case VEDIC_FLOAT:
             result.value.f32 = vedic_to_float(a) + vedic_to_float(b);
             break;
             
         case VEDIC_DOUBLE:
             result.value.f64 = vedic_to_double(a) + vedic_to_double(b);
             break;
             
         default:
             // Invalid type - return 0
             result.type = VEDIC_INT32;
             result.value.i32 = 0;
             break;
     }
     
     return result;
 }
 
 /**
  * Perform dynamic subtraction
  */
 VedicValue vedic_dynamic_subtract(VedicValue a, VedicValue b) {
     // Determine the result type based on operand types
     VedicNumberType result_type = vedic_result_type(a.type, b.type);
     VedicValue result;
     result.type = result_type;
     
     // Perform subtraction based on the result type
     switch (result_type) {
         case VEDIC_INT32:
             result.value.i32 = vedic_to_int32(a) - vedic_to_int32(b);
             break;
             
         case VEDIC_INT64:
             result.value.i64 = vedic_to_int64(a) - vedic_to_int64(b);
             break;
             
         case VEDIC_FLOAT:
             result.value.f32 = vedic_to_float(a) - vedic_to_float(b);
             break;
             
         case VEDIC_DOUBLE:
             result.value.f64 = vedic_to_double(a) - vedic_to_double(b);
             break;
             
         default:
             // Invalid type - return 0
             result.type = VEDIC_INT32;
             result.value.i32 = 0;
             break;
     }
     
     return result;
 }
 
 /**
  * Perform dynamic modulo
  */
 VedicValue vedic_dynamic_modulo(VedicValue a, VedicValue b) {
     // Modulo only makes sense for integer types
     VedicNumberType result_type = vedic_result_type(a.type, b.type);
     
     // If either operand is floating point, convert to integer
     if (result_type == VEDIC_FLOAT || result_type == VEDIC_DOUBLE) {
         if (a.type == VEDIC_FLOAT || a.type == VEDIC_DOUBLE) {
             a = vedic_from_int64((int64_t)vedic_to_double(a));
         }
         if (b.type == VEDIC_FLOAT || b.type == VEDIC_DOUBLE) {
             b = vedic_from_int64((int64_t)vedic_to_double(b));
         }
         result_type = vedic_result_type(a.type, b.type);
     }
     
     VedicValue result;
     result.type = result_type;
     
     // Check for modulo by zero
     bool is_zero_b = false;
     switch (b.type) {
         case VEDIC_INT32: is_zero_b = (b.value.i32 == 0); break;
         case VEDIC_INT64: is_zero_b = (b.value.i64 == 0); break;
         default: is_zero_b = true; break;
     }
     
     if (is_zero_b) {
         // Modulo by zero - return the dividend
         switch (result_type) {
             case VEDIC_INT32:
                 result.value.i32 = vedic_to_int32(a);
                 break;
                 
             case VEDIC_INT64:
                 result.value.i64 = vedic_to_int64(a);
                 break;
                 
             default:
                 // Invalid type - return 0
                 result.type = VEDIC_INT32;
                 result.value.i32 = 0;
                 break;
         }
         
         return result;
     }
     
     // Perform modulo based on the result type
     switch (result_type) {
         case VEDIC_INT32: {
             int32_t dividend = vedic_to_int32(a);
             int32_t divisor = vedic_to_int32(b);
             long remainder;
             vedic_divide(dividend, divisor, &remainder);
             result.value.i32 = (int32_t)remainder;
             break;
         }
             
         case VEDIC_INT64: {
             int64_t dividend = vedic_to_int64(a);
             int64_t divisor = vedic_to_int64(b);
             
             // If both values fit in long, use vedic_divide
             if (dividend <= LONG_MAX && dividend >= LONG_MIN && 
                 divisor <= LONG_MAX && divisor >= LONG_MIN) {
                 long remainder;
                 vedic_divide((long)dividend, (long)divisor, &remainder);
                 result.value.i64 = (int64_t)remainder;
             } else {
                 // Fall back to standard modulo
                 result.value.i64 = dividend % divisor;
             }
             break;
         }
             
         default:
             // Invalid type - return 0
             result.type = VEDIC_INT32;
             result.value.i32 = 0;
             break;
     }
     
     return result;
 }
 
 /**
  * Perform a dynamic operation based on operator type
  */
 VedicValue vedic_dynamic_operation(VedicValue a, VedicValue b, VedicOperation op) {
     switch (op) {
         case VEDIC_OP_ADD:
             return vedic_dynamic_add(a, b);
             
         case VEDIC_OP_SUBTRACT:
             return vedic_dynamic_subtract(a, b);
             
         case VEDIC_OP_MULTIPLY:
             return vedic_dynamic_multiply(a, b);
             
         case VEDIC_OP_DIVIDE:
             return vedic_dynamic_divide(a, b);
             
         case VEDIC_OP_MODULO:
             return vedic_dynamic_modulo(a, b);
             
         case VEDIC_OP_POWER: {
             // Power operation
             double base = vedic_to_double(a);
             double exponent = vedic_to_double(b);
             double result = pow(base, exponent);
             
             // Determine best type for the result
             if (exponent == (int)exponent && exponent >= 0) {
                 // Integer exponent - might be able to return integer
                 if (base == (int)base) {
                     // Integer base with positive integer exponent
                     // Check if result is an integer too
                     if (result == (int64_t)result) {
                         return vedic_from_int64((int64_t)result);
                     }
                 }
             }
             
             // Default to double for general power operation
             return vedic_from_double(result);
         }
             
         default:
             // Invalid operation - return input a
             return a;
     }
 }
 
 /**
  * Parse and evaluate an expression with dynamic types
  */
 VedicValue vedic_dynamic_evaluate(const char* expression) {
     // Simple parser for "a op b" expressions
     char* expr_copy = strdup(expression);
     if (!expr_copy) {
         // Memory allocation failed
         VedicValue result;
         result.type = VEDIC_INT32;
         result.value.i32 = 0;
         return result;
     }
     
     // Find the operator
     char* op_str = NULL;
     const char* operators = "+-*/^%";
     
     for (char* p = expr_copy; *p; p++) {
         if (strchr(operators, *p)) {
             op_str = p;
             break;
         }
     }
     
     if (!op_str) {
         // No operator found - try to parse as single number
         VedicValue result = vedic_parse_number(expr_copy);
         free(expr_copy);
         return result;
     }
     
     // Split the expression at the operator
     char op_char = *op_str;
     *op_str = '\0';
     char* left_str = expr_copy;
     char* right_str = op_str + 1;
     
     // Trim whitespace
     while (isspace(*left_str)) left_str++;
     while (isspace(*right_str)) right_str++;
     
     char* end = left_str + strlen(left_str) - 1;
     while (end > left_str && isspace(*end)) *end-- = '\0';
     
     end = right_str + strlen(right_str) - 1;
     while (end > right_str && isspace(*end)) *end-- = '\0';
     
     // Parse the operands
     VedicValue left = vedic_parse_number(left_str);
     VedicValue right = vedic_parse_number(right_str);
     
     // Determine the operation
     VedicOperation op;
     switch (op_char) {
         case '+': op = VEDIC_OP_ADD; break;
         case '-': op = VEDIC_OP_SUBTRACT; break;
         case '*': op = VEDIC_OP_MULTIPLY; break;
         case '/': op = VEDIC_OP_DIVIDE; break;
         case '%': op = VEDIC_OP_MODULO; break;
         case '^': op = VEDIC_OP_POWER; break;
         default: op = VEDIC_OP_INVALID; break;
     }
     
     // Perform the operation
     VedicValue result = vedic_dynamic_operation(left, right, op);
     
     // Clean up
     free(expr_copy);
     
     return result;
 }
 
 /**
  * Create a VedicValue from a number in a string
  */
 VedicValue vedic_dynamic_from_string(const char* number_str) {
     return vedic_parse_number(number_str);
 }