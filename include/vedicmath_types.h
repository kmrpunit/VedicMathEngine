/**
 * vedicmath_types.h - Data type definitions for Vedic Mathematics Library
 * 
 * This file defines the dynamic type system used by the library to 
 * automatically select the appropriate data type based on input values.
 */

 #ifndef VEDICMATH_TYPES_H
 #define VEDICMATH_TYPES_H
 
 #include <stdint.h>
 #include <stdbool.h>
 
 /**
  * Enumeration of supported numeric types
  */
 typedef enum {
     VEDIC_INT32,    // 32-bit signed integer
     VEDIC_INT64,    // 64-bit signed integer
     VEDIC_FLOAT,    // Single-precision floating point
     VEDIC_DOUBLE,   // Double-precision floating point
     VEDIC_INVALID   // Invalid type (for error handling)
 } VedicNumberType;
 
 /**
  * Union to hold any supported numeric type
  */
 typedef union {
     int32_t i32;
     int64_t i64;
     float f32;
     double f64;
 } VedicNumber;
 
 /**
  * Complete value with data and type information
  */
 typedef struct {
     VedicNumber value;
     VedicNumberType type;
 } VedicValue;
 
 /**
  * Operation codes for arithmetic operations
  */
 typedef enum {
     VEDIC_OP_ADD,      // Addition
     VEDIC_OP_SUBTRACT, // Subtraction
     VEDIC_OP_MULTIPLY, // Multiplication
     VEDIC_OP_DIVIDE,   // Division
     VEDIC_OP_MODULO,   // Modulo
     VEDIC_OP_POWER,    // Power
     VEDIC_OP_INVALID   // Invalid operation
 } VedicOperation;
 
 /**
  * Determine the appropriate type based on a string representation
  * 
  * @param number_str String representation of a number
  * @return The most appropriate VedicNumberType
  */
 VedicNumberType vedic_detect_type(const char* number_str);
 
 /**
  * Create a VedicValue from a string, automatically detecting type
  * 
  * @param number_str String representation of a number
  * @return VedicValue with the parsed number and detected type
  */
 VedicValue vedic_parse_number(const char* number_str);
 
 /**
  * Convert a VedicValue to a string
  * 
  * @param value The VedicValue to convert
  * @param buffer Buffer to store the resulting string
  * @param buffer_size Size of the buffer
  * @return The buffer, or NULL if conversion failed
  */
 char* vedic_to_string(VedicValue value, char* buffer, size_t buffer_size);
 
 /**
  * Determine the resulting type when operating on two VedicValues
  * 
  * @param a First operand type
  * @param b Second operand type
  * @return The appropriate result type
  */
 VedicNumberType vedic_result_type(VedicNumberType a, VedicNumberType b);
 
 /**
  * Detect the operation from a string
  * 
  * @param op_str String representation of an operation (e.g., "+", "*")
  * @return The corresponding VedicOperation
  */
 VedicOperation vedic_detect_operation(const char* op_str);
 
 /**
  * Convert an int32_t value to a VedicValue
  */
 VedicValue vedic_from_int32(int32_t value);
 
 /**
  * Convert an int64_t value to a VedicValue
  */
 VedicValue vedic_from_int64(int64_t value);
 
 /**
  * Convert a float value to a VedicValue
  */
 VedicValue vedic_from_float(float value);
 
 /**
  * Convert a double value to a VedicValue
  */
 VedicValue vedic_from_double(double value);
 
 /**
  * Get the int32_t value from a VedicValue
  * Will perform type conversion if necessary
  */
 int32_t vedic_to_int32(VedicValue value);
 
 /**
  * Get the int64_t value from a VedicValue
  * Will perform type conversion if necessary
  */
 int64_t vedic_to_int64(VedicValue value);
 
 /**
  * Get the float value from a VedicValue
  * Will perform type conversion if necessary
  */
 float vedic_to_float(VedicValue value);
 
 /**
  * Get the double value from a VedicValue
  * Will perform type conversion if necessary
  */
 double vedic_to_double(VedicValue value);
 
 #endif /* VEDICMATH_TYPES_H */