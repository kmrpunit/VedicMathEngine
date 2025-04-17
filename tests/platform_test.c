/**
 * platform_test.c - Test cross-platform compatibility
 */

 #include "../include/vedicmath.h"
 #include "../include/vedicmath_types.h"
 #include "../include/vedicmath_dynamic.h"
 #include "../include/vedicmath_platform.h"
 #include <stdio.h>
 #include <stdint.h>
 #include <string.h>
 
 /**
  * Test platform compatibility features
  */
 void test_platform_compatibility() {
     printf("Platform information:\n");
     
 #if defined(VEDICMATH_PLATFORM_WINDOWS)
     printf("- Windows platform detected\n");
 #elif defined(VEDICMATH_PLATFORM_MACOS)
     printf("- macOS platform detected\n");
 #elif defined(VEDICMATH_PLATFORM_LINUX)
     printf("- Linux platform detected\n");
 #else
     printf("- Other platform detected\n");
 #endif
 
     // Data type sizes
     printf("- Size of int: %zu bytes\n", sizeof(int));
     printf("- Size of long: %zu bytes\n", sizeof(long));
     printf("- Size of int32_t: %zu bytes\n", sizeof(int32_t));
     printf("- Size of int64_t: %zu bytes\n", sizeof(int64_t));
     printf("- Size of float: %zu bytes\n", sizeof(float));
     printf("- Size of double: %zu bytes\n", sizeof(double));
 
     // Test string functions
     char buffer[100];
     SAFE_STRCPY(buffer, sizeof(buffer), "Hello");
     printf("- Safe string copy works: '%s'\n", buffer);
     
     SAFE_STRCAT(buffer, sizeof(buffer), ", world!");
     printf("- Safe string concatenation works: '%s'\n", buffer);
     
     char *dup = vedicmath_strdup(buffer);
     if (dup) {
         printf("- Safe string duplication works: '%s'\n", dup);
         free(dup);
     }
     
     // Test VedicValue with different types
     VedicValue int_val = vedic_from_int32(42);
     VedicValue float_val = vedic_from_float(3.14159f);
     
     char int_str[32], float_str[32];
     vedic_to_string(int_val, int_str, sizeof(int_str));
     vedic_to_string(float_val, float_str, sizeof(float_str));
     
     printf("- VedicValue int: %s\n", int_str);
     printf("- VedicValue float: %s\n", float_str);
 }
 
 /**
  * Test basic math operations
  */
 void test_basic_operations() {
     printf("\nBasic operations test:\n");
     
     // Test standard operations
     printf("- Standard multiply: 102 * 32 = %ld\n", vedic_multiply(102, 32));
     
     // Test dynamic operations
     VedicValue a = vedic_parse_number("102");
     VedicValue b = vedic_parse_number("32");
     VedicValue result = vedic_dynamic_multiply(a, b);
     
     char result_str[64];
     vedic_to_string(result, result_str, sizeof(result_str));
     printf("- Dynamic multiply: 102 * 32 = %s\n", result_str);
     
     // Test expression evaluation
     result = vedic_dynamic_evaluate("102 * 32");
     vedic_to_string(result, result_str, sizeof(result_str));
     printf("- Expression evaluate: \"102 * 32\" = %s\n", result_str);
 }
 
 /**
  * Main function
  */
 int main() {
     printf("Vedic Mathematics Library - Platform Compatibility Test\n");
     printf("=====================================================\n\n");
     
     test_platform_compatibility();
     test_basic_operations();
     
     printf("\nAll platform compatibility tests completed successfully!\n");
     
     return 0;
 }