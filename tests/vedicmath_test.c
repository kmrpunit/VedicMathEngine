/**
 * vedicmath_test.c - Test program for Vedic Mathematics Library
 * 
 * This program tests various Vedic math functions and compares the results
 * with standard arithmetic operations to verify correctness.
 */

 #include "vedicmath.h"
 #include <stdio.h>
 #include <stdlib.h>
 
 /**
  * Run a multiplication test case and verify the result
  */
 void test_multiplication(long a, long b) {
     long expected = a * b;
     long result = vedic_multiply(a, b);
     
     printf("Testing %ld × %ld:\n", a, b);
     printf("  Vedic result: %ld\n", result);
     printf("  Expected: %ld\n", expected);
     printf("  %s\n\n", (result == expected) ? "PASS" : "FAIL");
 }
 
 /**
  * Run a squaring test case and verify the result
  */
 void test_square(long n) {
     long expected = n * n;
     long result = vedic_square(n);
     
     printf("Testing %ld²:\n", n);
     printf("  Vedic result: %ld\n", result);
     printf("  Expected: %ld\n", expected);
     printf("  %s\n\n", (result == expected) ? "PASS" : "FAIL");
 }
 
 /**
  * Run a division test case and verify the result
  */
 void test_division(long dividend, long divisor) {
     long expected_quotient = dividend / divisor;
     long expected_remainder = dividend % divisor;
     
     long remainder;
     long quotient = vedic_divide(dividend, divisor, &remainder);
     
     printf("Testing %ld ÷ %ld:\n", dividend, divisor);
     printf("  Vedic result: quotient = %ld, remainder = %ld\n", quotient, remainder);
     printf("  Expected: quotient = %ld, remainder = %ld\n", expected_quotient, expected_remainder);
     printf("  %s\n\n", (quotient == expected_quotient && remainder == expected_remainder) ? "PASS" : "FAIL");
 }
 
 int main() {
     printf("Vedic Mathematics Library Test Program\n");
     printf("=====================================\n\n");
     printf("%s\n\n", vedicmath_version());
     
     // Test Ekadhikena Purvena (squaring numbers ending in 5)
     printf("Testing Ekadhikena Purvena (Squaring numbers ending in 5):\n");
     printf("------------------------------------------------------\n");
     test_square(25);
     test_square(15);
     test_square(85);
     test_square(995);
     printf("\n");
     
     // Test Nikhilam multiplication (numbers near a base)
     printf("Testing Nikhilam multiplication (numbers near a base):\n");
     printf("--------------------------------------------------\n");
     test_multiplication(98, 97);    // Both below 100
     test_multiplication(104, 103);  // Both above 100
     test_multiplication(98, 103);   // One below, one above 100
     test_multiplication(999, 997);  // Both near 1000
     printf("\n");
     
     // Test Antyayordasake (Last totaling 10)
     printf("Testing Antyayordasake (Last totaling 10):\n");
     printf("---------------------------------------\n");
     test_multiplication(46, 44);    // 4(4+1)(6×4) = 4×5×24 = 20×24 = 2024
     test_multiplication(73, 77);    // 7(7+1)(3×7) = 7×8×21 = 56×21 = 5621
     printf("\n");
     
     // Test Yaavadunam squaring (numbers near a base)
     printf("Testing Yaavadunam squaring (numbers near a base):\n");
     printf("----------------------------------------------\n");
     test_square(98);     // Near 100
     test_square(997);    // Near 1000
     test_square(102);    // Just above 100
     printf("\n");
     
     // Test Urdhva-Tiryagbhyam (general multiplication)
     printf("Testing Urdhva-Tiryagbhyam (general multiplication):\n");
     printf("------------------------------------------------\n");
     test_multiplication(123, 456);
     test_multiplication(9876, 1234);
     printf("\n");
     
     // Test Division with Paravartya Yojayet
     printf("Testing Division with Paravartya Yojayet:\n");
     printf("--------------------------------------\n");
     test_division(1234, 56);
     test_division(9876, 12);
     test_division(1001, 13);
     printf("\n");
     
     // Test the central dispatcher with miscellaneous examples
     printf("Testing central dispatcher with miscellaneous examples:\n");
     printf("--------------------------------------------------\n");
     test_multiplication(102, 32);   // First example from the requirements
     test_multiplication(0, 123);    // Zero case
     test_multiplication(-25, 4);    // Negative number
     test_multiplication(999, -999); // Negative and special case
     printf("\n");
     
     return 0;
 }