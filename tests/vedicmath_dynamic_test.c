/**
 * vedicmath_dynamic_test.c - Test program for dynamic type system
 * 
 * This program tests the dynamic type system's ability to automatically
 * select appropriate implementations based on input types.
 */

 #include "vedicmath.h"
 #include "vedicmath_types.h"
 #include "vedicmath_dynamic.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 
 // Define test result macros
 #define TEST_PASS 1
 #define TEST_FAIL 0

 #define COLOR_GREEN "\033[0;32m"
 #define COLOR_RED   "\033[0;31m"
 #define COLOR_RESET "\033[0m"
 
 // Track test statistics
 static int total_tests = 0;
 static int passed_tests = 0;
 
 /**
  * Print test result with colored output
  */
 void print_test_result(const char* test_name, int result) {
     total_tests++;
     if (result) {
         passed_tests++;
         printf(COLOR_GREEN "[v] PASS: %s\n", test_name);
     } else {
         printf(COLOR_RED "[x] FAIL: %s\n", test_name);
     }
     printf(COLOR_RESET);
 }
 
 /**
  * Print test summary
  */
 void print_test_summary() {
     printf("\n==== TEST SUMMARY ====\n");
     printf("Total tests: %d\n", total_tests);
     printf("Passed: %d (%.1f%%)\n", passed_tests, (float)passed_tests / total_tests * 100);
     printf("Failed: %d (%.1f%%)\n", total_tests - passed_tests, 
            (float)(total_tests - passed_tests) / total_tests * 100);
     printf("=====================\n");
 }
 
 /**
  * Test type detection
  */
 void test_type_detection() {
     struct {
         const char* number_str;
         VedicNumberType expected_type;
     } test_cases[] = {
         {"0", VEDIC_INT32},
         {"42", VEDIC_INT32},
         {"-123", VEDIC_INT32},
         {"2147483647", VEDIC_INT32},      // INT32_MAX
         {"-2147483648", VEDIC_INT32},     // INT32_MIN
         {"2147483648", VEDIC_INT64},      // INT32_MAX + 1
         {"-2147483649", VEDIC_INT64},     // INT32_MIN - 1
         {"9223372036854775807", VEDIC_INT64},  // INT64_MAX
         {"-9223372036854775808", VEDIC_INT64}, // INT64_MIN
         {"3.14", VEDIC_FLOAT},
         {"-0.5", VEDIC_FLOAT},
         {"1e-6", VEDIC_FLOAT},
         {"1.23456789", VEDIC_DOUBLE},
         {"1e-10", VEDIC_DOUBLE},
         {"1.7976931348623157e+308", VEDIC_DOUBLE}  // Near DBL_MAX
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         VedicNumberType detected = vedic_detect_type(test_cases[i].number_str);
         
         sprintf(test_name, "Type detection: \"%s\" should be %d", 
                 test_cases[i].number_str, test_cases[i].expected_type);
                 
         print_test_result(test_name, detected == test_cases[i].expected_type);
     }
 }
 
 /**
  * Test parsing and string conversion
  */
 void test_parsing() {
     struct {
         const char* number_str;
         VedicNumberType expected_type;
     } test_cases[] = {
         {"42", VEDIC_INT32},
         {"2147483648", VEDIC_INT64},
         {"-9223372036854775808", VEDIC_INT64},
         {"3.14159", VEDIC_FLOAT},
         {"2.71828182845904523536", VEDIC_DOUBLE}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     char result_str[64];
     
     for (int i = 0; i < num_cases; i++) {
         VedicValue value = vedic_parse_number(test_cases[i].number_str);
         
         sprintf(test_name, "Parse: \"%s\" should be type %d", 
                 test_cases[i].number_str, test_cases[i].expected_type);
                 
         print_test_result(test_name, value.type == test_cases[i].expected_type);
         
         vedic_to_string(value, result_str, sizeof(result_str));
         
         sprintf(test_name, "Roundtrip: \"%s\" converts back to similar value", 
                 test_cases[i].number_str);
                 
         // For roundtrip test, we don't compare exact strings because formatting might differ
         // Instead, parse the result string and compare the values
         VedicValue reparsed = vedic_parse_number(result_str);
         bool values_similar = false;
         
         switch (value.type) {
             case VEDIC_INT32:
                 values_similar = (reparsed.type == VEDIC_INT32 && 
                                  reparsed.value.i32 == value.value.i32);
                 break;
                 
             case VEDIC_INT64:
                 values_similar = ((reparsed.type == VEDIC_INT64 && 
                                   reparsed.value.i64 == value.value.i64) ||
                                  (reparsed.type == VEDIC_INT32 && 
                                   (int64_t)reparsed.value.i32 == value.value.i64));
                 break;
                 
             case VEDIC_FLOAT: {
                 float rel_diff = (reparsed.type == VEDIC_FLOAT) ? 
                     fabsf((reparsed.value.f32 - value.value.f32) / value.value.f32) :
                     fabsf((float)vedic_to_double(reparsed) - value.value.f32) / value.value.f32;
                 values_similar = (rel_diff < 0.0001f);
                 break;
             }
                 
             case VEDIC_DOUBLE: {
                 double rel_diff = fabsf(vedic_to_double(reparsed) - value.value.f64) / value.value.f64;
                 values_similar = (rel_diff < 0.0001);
                 break;
             }
                 
             default:
                 values_similar = false;
                 break;
         }
         
         print_test_result(test_name, values_similar);
     }
 }
 
 /**
  * Test dynamic multiplication
  */
 void test_dynamic_multiplication() {
     struct {
         const char* a_str;
         const char* b_str;
         const char* expected_result;
         VedicNumberType expected_type;
     } test_cases[] = {
         // Integer * Integer = Integer
         {"12", "34", "408", VEDIC_INT32},
         {"1234", "5678", "7006652", VEDIC_INT32},
         
         // Testing special Vedic cases
         {"25", "25", "625", VEDIC_INT32},       // Ekadhikena Purvena
         {"98", "97", "9506", VEDIC_INT32},      // Nikhilam
         {"46", "44", "2024", VEDIC_INT32},      // Antyayordasake
         
         // Large integers
         {"2147483647", "2", "4294967294", VEDIC_INT64},  // INT32_MAX * 2
         
         // Float cases
         {"3.5", "2", "7", VEDIC_FLOAT},        // Float * Integer = Float
         {"2.5", "3.5", "8.75", VEDIC_FLOAT},   // Float * Float = Float
         
         // Double precision cases
         {"123456789.123", "987654321.987", "1.21931851e+17", VEDIC_DOUBLE}, // High precision
         
         // Mixed integer and float cases
         {"102", "32", "3264", VEDIC_INT32},    // Original example
         {"102.0", "32", "3264", VEDIC_FLOAT}   // Same but with float
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     char result_str[64];
     
     for (int i = 0; i < num_cases; i++) {
         VedicValue a = vedic_parse_number(test_cases[i].a_str);
         VedicValue b = vedic_parse_number(test_cases[i].b_str);
         VedicValue result = vedic_dynamic_multiply(a, b);
         
         vedic_to_string(result, result_str, sizeof(result_str));
         
         sprintf(test_name, "Multiply: %s * %s = %s (type %d)", 
                 test_cases[i].a_str, test_cases[i].b_str, 
                 test_cases[i].expected_result, test_cases[i].expected_type);
         
         // For floating point, compare numerically rather than as strings
         bool success = (result.type == test_cases[i].expected_type);
         
         if (success) {
             if (result.type == VEDIC_FLOAT || result.type == VEDIC_DOUBLE) {
                 double expected = atof(test_cases[i].expected_result);
                 double actual = vedic_to_double(result);
                 double rel_diff = fabs((actual - expected) / expected);
                 success = (rel_diff < 0.001);  // Allow small relative error
             } else {
                 // For integers, compare the string values directly
                 VedicValue expected = vedic_parse_number(test_cases[i].expected_result);
                 if (result.type == VEDIC_INT32) {
                     success = (result.value.i32 == expected.value.i32);
                 } else if (result.type == VEDIC_INT64) {
                     success = (result.value.i64 == expected.value.i64);
                 }
             }
         }
         
         print_test_result(test_name, success);
     }
 }
 
 /**
  * Test dynamic operations (all operators)
  */
 void test_dynamic_operations() {
     struct {
         const char* a_str;
         const char* b_str;
         VedicOperation op;
         const char* expected_result;
     } test_cases[] = {
         // Addition
         {"123", "456", VEDIC_OP_ADD, "579"},
         {"1234.5", "5678.5", VEDIC_OP_ADD, "6913"},
         {"9223372036854775000", "1000", VEDIC_OP_ADD, "9223372036854776000"},
         
         // Subtraction
         {"456", "123", VEDIC_OP_SUBTRACT, "333"},
         {"1000.5", "500.5", VEDIC_OP_SUBTRACT, "500"},
         
         // Multiplication (just a few cases since we test separately)
         {"9", "9", VEDIC_OP_MULTIPLY, "81"},
         {"12.5", "4", VEDIC_OP_MULTIPLY, "50"},
         
         // Division
         {"100", "4", VEDIC_OP_DIVIDE, "25"},
         {"10", "3", VEDIC_OP_DIVIDE, "3.33333"},
         {"1000", "10", VEDIC_OP_DIVIDE, "100"},
         
         // Modulo
         {"10", "3", VEDIC_OP_MODULO, "1"},
         {"100", "30", VEDIC_OP_MODULO, "10"},
         
         // Power
         {"2", "10", VEDIC_OP_POWER, "1024"},
         {"3", "4", VEDIC_OP_POWER, "81"},
         {"10", "0", VEDIC_OP_POWER, "1"},
         {"2.5", "2", VEDIC_OP_POWER, "6.25"}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     char result_str[64];
     const char* op_symbols[] = {"+", "-", "*", "/", "%", "^"};
     
     for (int i = 0; i < num_cases; i++) {
         VedicValue a = vedic_parse_number(test_cases[i].a_str);
         VedicValue b = vedic_parse_number(test_cases[i].b_str);
         VedicValue result = vedic_dynamic_operation(a, b, test_cases[i].op);
         
         vedic_to_string(result, result_str, sizeof(result_str));
         
         sprintf(test_name, "Operation: %s %s %s = %s", 
                 test_cases[i].a_str, op_symbols[test_cases[i].op], 
                 test_cases[i].b_str, test_cases[i].expected_result);
         
         // Compare numerically for all operations
         VedicValue expected = vedic_parse_number(test_cases[i].expected_result);
         bool success = false;
         
         if (result.type == VEDIC_INT32 || result.type == VEDIC_INT64) {
             int64_t result_val = vedic_to_int64(result);
             int64_t expected_val = vedic_to_int64(expected);
             success = (result_val == expected_val);
         } else {
             double result_val = vedic_to_double(result);
             double expected_val = vedic_to_double(expected);
             double rel_diff = fabs((result_val - expected_val) / expected_val);
             success = (rel_diff < 0.01);  // Allow 1% relative error for floating point
         }
         
         print_test_result(test_name, success);
     }
 }
 
 /**
  * Test expression evaluation
  */
 void test_expression_evaluation() {
     struct {
         const char* expression;
         const char* expected_result;
     } test_cases[] = {
         {"12 + 34", "46"},
         {"56 - 23", "33"},
         {"12 * 34", "408"},
         {"100 / 4", "25"},
         {"10 % 3", "1"},
         {"2 ^ 10", "1024"},
         {"2.5 * 4", "10"},
         {"10 / 3", "3.33333"},
         {"102 * 32", "3264"},   // Our special case
         {"25 * 25", "625"},     // Ekadhikena Purvena case
         {"97 * 98", "9506"}     // Nikhilam case
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     char result_str[64];
     
     for (int i = 0; i < num_cases; i++) {
         VedicValue result = vedic_dynamic_evaluate(test_cases[i].expression);
         vedic_to_string(result, result_str, sizeof(result_str));
         
         sprintf(test_name, "Expression: %s = %s", 
                 test_cases[i].expression, test_cases[i].expected_result);
         
         // Compare numerically
         VedicValue expected = vedic_parse_number(test_cases[i].expected_result);
         bool success = false;
         
         if (result.type == VEDIC_INT32 || result.type == VEDIC_INT64) {
             int64_t result_val = vedic_to_int64(result);
             int64_t expected_val = vedic_to_int64(expected);
             success = (result_val == expected_val);
         } else {
             double result_val = vedic_to_double(result);
             double expected_val = vedic_to_double(expected);
             double rel_diff = fabs((result_val - expected_val) / expected_val);
             success = (rel_diff < 0.01);  // Allow 1% relative error for floating point
         }
         
         print_test_result(test_name, success);
     }
 }
 
 /**
  * Interactive calculator mode
  */
 void interactive_calculator() {
     char input[256];
     char result_str[64];
     
     printf("\n=== Vedic Math Dynamic Calculator ===\n");
     printf("Enter expressions (e.g., '102 * 32') or 'q' to quit\n");
     
     while (1) {
         printf("\nExpression > ");
         if (fgets(input, sizeof(input), stdin) == NULL) {
             break;
         }
         
         // Remove newline
         size_t len = strlen(input);
         if (len > 0 && input[len-1] == '\n') {
             input[len-1] = '\0';
         }
         
         // Check for quit command
         if (input[0] == 'q' || input[0] == 'Q') {
             break;
         }
         
         // Skip empty input
         if (input[0] == '\0') {
             continue;
         }
         
         // Evaluate the expression
         VedicValue result = vedic_dynamic_evaluate(input);
         
         // Display result with type information
         vedic_to_string(result, result_str, sizeof(result_str));
         
         const char* type_str = "unknown";
         switch (result.type) {
             case VEDIC_INT32: type_str = "int32"; break;
             case VEDIC_INT64: type_str = "int64"; break;
             case VEDIC_FLOAT: type_str = "float"; break;
             case VEDIC_DOUBLE: type_str = "double"; break;
             default: type_str = "invalid"; break;
         }
         
         printf("Result: %s (type: %s)\n", result_str, type_str);
     }
     
     printf("Goodbye!\n");
 }
 
 /**
  * Main function
  */
 int main(int argc, char* argv[]) {
     printf("Vedic Mathematics Library - Dynamic Type System Test\n");
     printf("==================================================\n\n");
     
     if (argc > 1) {
         // If expression provided as command-line argument, evaluate it
         char result_str[64];
         
         // Concatenate all arguments to form the expression
         char expression[256] = "";
         for (int i = 1; i < argc; i++) {
             strcat(expression, argv[i]);
             if (i < argc - 1) {
                 strcat(expression, " ");
             }
         }
         
         printf("Evaluating: %s\n", expression);
         
         VedicValue result = vedic_dynamic_evaluate(expression);
         vedic_to_string(result, result_str, sizeof(result_str));
         
         const char* type_str = "unknown";
         switch (result.type) {
             case VEDIC_INT32: type_str = "int32"; break;
             case VEDIC_INT64: type_str = "int64"; break;
             case VEDIC_FLOAT: type_str = "float"; break;
             case VEDIC_DOUBLE: type_str = "double"; break;
             default: type_str = "invalid"; break;
         }
         
         printf("Result: %s (type: %s)\n", result_str, type_str);
         return 0;
     }
     
     // No arguments - run the test suite or interactive mode
     printf("Running test suite...\n\n");
     
     test_type_detection();
     test_parsing();
     test_dynamic_multiplication();
     test_dynamic_operations();
     test_expression_evaluation();
     
     print_test_summary();
     
     // Launch interactive calculator
     interactive_calculator();
     
     return 0;
 }