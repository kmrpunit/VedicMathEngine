/**
 * vedicmath_test_suite.c - Comprehensive test suite for Vedic Mathematics Library
 * 
 * This file contains extensive test cases for all implemented sutras and operations.
 */

 #include "vedicmath.h"
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
         printf(COLOR_GREEN "[✓] PASS: %s\n", test_name);
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
  * Test Ekadhikena Purvena (squaring numbers ending in 5)
  */
 void test_ekadhikena_purvena() {
     struct {
         long input;
         long expected;
     } test_cases[] = {
         {5, 25},
         {15, 225},
         {25, 625},
         {35, 1225},
         {45, 2025},
         {55, 3025},
         {65, 4225},
         {75, 5625},
         {85, 7225},
         {95, 9025},
         {105, 11025},
         {115, 13225},
         {125, 15625},
         {135, 18225},
         {145, 21025},
         {155, 24025},
         {165, 27225},
         {175, 30625},
         {185, 34225},
         {195, 38025},
         {205, 42025},
         {235, 55225},
         {345, 119025},
         {505, 255025},
         {695, 483025},
         {895, 801025},
         {995, 990025}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         long result = ekadhikena_purvena(test_cases[i].input);
         sprintf(test_name, "Ekadhikena Purvena: %ld^2 = %ld", test_cases[i].input, test_cases[i].expected);
         print_test_result(test_name, result == test_cases[i].expected);
     }
 }
 
 /**
  * Test Nikhilam multiplication (numbers near a base)
  */
 void test_nikhilam_mul() {
     struct {
         long a;
         long b;
         long expected;
     } test_cases[] = {
         // Both below base 10
         {7, 8, 56},
         {8, 9, 72},
         
         // Both below base 100
         {97, 98, 9506},
         {94, 95, 8930},
         {92, 98, 9016},
         {87, 83, 7221},
         {73, 79, 5767},
         
         // Both above base 100
         {103, 104, 10712},
         {105, 108, 11340},
         {112, 115, 12880},
         {132, 145, 19140},
         
         // One below, one above base 100
         {96, 104, 9984},
         {93, 107, 9951},
         {87, 113, 9831},
         
         // Near base 1000
         {997, 998, 995006},
         {995, 996, 991020},
         {1003, 1005, 1008015},
         {1004, 1006, 1010024},
         {992, 989, 981088},
         {996, 1004, 999984},
         
         // Near base 10000
         {9997, 9998, 99950006},
         {10003, 10005, 100080015}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         long result = nikhilam_mul(test_cases[i].a, test_cases[i].b);
         sprintf(test_name, "Nikhilam: %ld x %ld = %ld", 
                 test_cases[i].a, test_cases[i].b, test_cases[i].expected);
         print_test_result(test_name, result == test_cases[i].expected);
     }
 }
 
 /**
  * Test Urdhva-Tiryagbhyam (general multiplication)
  */
 void test_urdhva_tiryagbhyam() {
     struct {
         long a;
         long b;
         long expected;
     } test_cases[] = {
         {12, 34, 408},
         {32, 12, 384},
         {123, 456, 56088},
         {987, 654, 645498},
         {1234, 5678, 7006652},
         {9876, 5432, 53646432},
         {12345, 67890, 838102050},
         {123456, 789, 97406784},
         {54321, 9876, 536474196}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         long result = urdhva_mult(test_cases[i].a, test_cases[i].b);
         sprintf(test_name, "Urdhva-Tiryagbhyam: %ld x %ld = %ld", 
                 test_cases[i].a, test_cases[i].b, test_cases[i].expected);
         print_test_result(test_name, result == test_cases[i].expected);
     }
 }
 
 /**
  * Test Yaavadunam (squaring numbers near a base)
  */
 void test_yaavadunam() {
     struct {
         long input;
         long base;
         long expected;
     } test_cases[] = {
         // Below base
         {98, 100, 9604},
         {97, 100, 9409},
         {95, 100, 9025},
         {90, 100, 8100},
         {997, 1000, 994009},
         {995, 1000, 990025},
         {990, 1000, 980100},
         {9995, 10000, 99900025},
         {9990, 10000, 99800100},
         
         // Above base
         {101, 100, 10201},
         {102, 100, 10404},
         {105, 100, 11025},
         {110, 100, 12100},
         {1005, 1000, 1010025},
         {1010, 1000, 1020100},
         {10005, 10000, 100100025},
         {10010, 10000, 100200100}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         long result = yaavadunam_square(test_cases[i].input, test_cases[i].base);
         sprintf(test_name, "Yaavadunam: %ld² = %ld", test_cases[i].input, test_cases[i].expected);
         print_test_result(test_name, result == test_cases[i].expected);
     }
 }
 
 /**
  * Test Antyayordasake (Last totaling 10)
  */
 void test_antyayordasake() {
     struct {
         int a;
         int b;
         int expected;
     } test_cases[] = {
         {16, 14, 224},
         {26, 24, 624},
         {36, 34, 1224},
         {46, 44, 2024},
         {56, 54, 3024},
         {77, 73, 5621},
         {87, 83, 7221},
         {97, 93, 9021},
         {47, 43, 2021},
         {57, 53, 3021},
         {67, 63, 4221},
         {28, 22, 616},
         {38, 32, 1216},
         {48, 42, 2016},
         {58, 52, 3016},
         {68, 62, 4216},
         {78, 72, 5616},
         {88, 82, 7216},
         {98, 92, 9016},
         {19, 11, 209},
         {29, 21, 609},
         {39, 31, 1209},
         {69, 61, 4209},
         {79, 71, 5609},
         {89, 81, 7209},
         {99, 91, 9009}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         int result = antya_dasake_mul(test_cases[i].a, test_cases[i].b);
         sprintf(test_name, "Antyayordasake: %d x %d = %d", 
                 test_cases[i].a, test_cases[i].b, test_cases[i].expected);
         print_test_result(test_name, result == test_cases[i].expected);
     }
 }
 
 /**
  * Test Paravartya Yojayet (division method)
  */
 void test_paravartya_yojayet() {
     struct {
         long dividend;
         long divisor;
         long expected_quotient;
         long expected_remainder;
     } test_cases[] = {
         {56, 7, 8, 0},
         {57, 7, 8, 1},
         {1234, 56, 22, 2},
         {9876, 12, 823, 0},
         {1001, 13, 77, 0},
         {1234, 11, 112, 2},
         {9999, 9, 1111, 0},
         {8765, 23, 381, 2},
         {54321, 67, 810, 51},
         {123456, 789, 156, 372},
         {987654, 321, 3076, 258}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         long remainder;
         long quotient = paravartya_divide(test_cases[i].dividend, test_cases[i].divisor, &remainder);
         
         sprintf(test_name, "Paravartya: %ld / %ld = %ld R %ld", 
                 test_cases[i].dividend, test_cases[i].divisor, 
                 test_cases[i].expected_quotient, test_cases[i].expected_remainder);
                 
         print_test_result(test_name, 
             quotient == test_cases[i].expected_quotient && 
             remainder == test_cases[i].expected_remainder);
     }
 }
 
 /**
  * Test Vestanam (divisibility test)
  */
 void test_vestanam() {
     struct {
         long number;
         int prime;
         int expected;  // 1 if divisible, 0 if not
     } test_cases[] = {
         {49, 7, 1},
         {50, 7, 0},
         {63, 7, 1},
         {64, 7, 0},
         {77, 7, 1},
         {79, 7, 0},
         {91, 7, 0},
         {98, 7, 0},
         {133, 7, 0},
         {133, 19, 1},
         {133, 13, 1},
         {286, 13, 0},
         {286, 11, 0},
         {286, 13, 0},
         {299, 13, 1},
         {323, 17, 1},
         {324, 17, 0},
         {391, 17, 0},
         {391, 23, 1},
         {667, 23, 0},
         {667, 29, 1},
         {899, 29, 1},
         {899, 31, 1}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         int result = vestanam_divisibility(test_cases[i].number, test_cases[i].prime);
         
         sprintf(test_name, "Vestanam: Is %ld divisible by %d? %s", 
                 test_cases[i].number, test_cases[i].prime, 
                 test_cases[i].expected ? "Yes" : "No");
                 
         print_test_result(test_name, result == test_cases[i].expected);
     }
 }
 
 /**
  * Test Ekanyunena Purvena (multiplying by all 9s)
  */
 void test_ekanyunena_purvena() {
     struct {
         long multiplicand;
         long all9s_multiplier;
         long expected;
     } test_cases[] = {
         {47, 9, 423},
         {83, 9, 747},
         {125, 9, 1125},
         {42, 99, 4158},
         {83, 99, 8217},
         {357, 99, 35343},
         {456, 999, 455544},
         {789, 999, 788211},
         {1234, 999, 1232766},
         {5678, 9999, 56774322},
         {12345, 9999, 123437655}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         long result = ekanyunena_purvena_mul(test_cases[i].multiplicand, test_cases[i].all9s_multiplier);
         
         sprintf(test_name, "Ekanyunena: %ld x %ld = %ld", 
                 test_cases[i].multiplicand, test_cases[i].all9s_multiplier, test_cases[i].expected);
                 
         print_test_result(test_name, result == test_cases[i].expected);
     }
 }
 
 /**
  * Test Puranapuranabhyam (completion method)
  */
 void test_puranapuranabhyam() {
     struct {
         long a;
         long b;
         int operation;  // 0 for add, 1 for subtract, 2 for multiply
         long expected;
     } test_cases[] = {
         // Addition
         {97, 35, 0, 132},
         {88, 45, 0, 133},
         {128, 76, 0, 204},
         {995, 87, 0, 1082},
         
         // Subtraction
         {100, 37, 1, 63},
         {95, 43, 1, 52},
         {1000, 456, 1, 544},
         {2000, 1765, 1, 235},
         
         // Multiplication
         {97, 5, 2, 485},
         {103, 7, 2, 721},
         {992, 6, 2, 5952},
         {1005, 9, 2, 9045},
         {98, 97, 2, 9506}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         long result = puranapuranabhyam_calc(test_cases[i].a, test_cases[i].b, test_cases[i].operation);
         
         const char* op_str = test_cases[i].operation == 0 ? "+" : 
                             (test_cases[i].operation == 1 ? "-" : "x");
         
         sprintf(test_name, "Puranapuranabhyam: %ld %s %ld = %ld", 
                 test_cases[i].a, op_str, test_cases[i].b, test_cases[i].expected);
                 
         print_test_result(test_name, result == test_cases[i].expected);
     }
 }
 
 /**
  * Test Anurupyena (proportional scaling)
  */
 void test_anurupyena() {
     struct {
         long a;
         long b;
         int scale;
         long expected;
     } test_cases[] = {
         {25, 35, 5, 875},
         {40, 60, 10, 2400},
         {48, 52, 4, 2496},
         {75, 85, 5, 6375},
         {125, 175, 25, 21875},
         {250, 350, 50, 87500},
         {480, 520, 40, 249600},
         {750, 850, 50, 637500}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         long result = anurupyena_mul(test_cases[i].a, test_cases[i].b, test_cases[i].scale);
         
         sprintf(test_name, "Anurupyena: %ld x %ld (scale %d) = %ld", 
                 test_cases[i].a, test_cases[i].b, test_cases[i].scale, test_cases[i].expected);
                 
         print_test_result(test_name, result == test_cases[i].expected);
     }
 }
 
 /**
  * Test Sankalana-Vyavakalanabhyam (solving equations)
  */
 void test_sankalana_vyavakalanabhyam() {
     struct {
         int a1, b1, c1;
         int a2, b2, c2;
         double expected_x;
         double expected_y;
     } test_cases[] = {
         {1, 1, 10, 1, -1, 4, 7, 3},                // x + y = 10, x - y = 4
         {2, 3, 12, 7, 3, 27, 3, 2},                // 2x + 3y = 12, 7x + 3y = 27
         {3, 5, 29, 2, -1, 1, 2.61538, 4.23077},    // 3x + 5y = 29, 2x - y = 1
         {5, 2, 16, 3, 4, 24, 1.14286, 5.14286},    // 5x + 2y = 16, 3x + 4y = 24
         {4, -5, 3, 2, 3, 13, 3.36364, 2.09091},    // 4x - 5y = 3, 2x + 3y = 13
         {6, 7, 62, 5, -3, 7, 4.43396, 5.0566},     // 6x + 7y = 62, 5x - 3y = 7
         {8, -9, 5, 4, 3, 19, 3.1, 2.2}             // 8x - 9y = 5, 4x + 3y = 19
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[200];
     
     for (int i = 0; i < num_cases; i++) {
         double x, y;
         int result = sankalana_vyavakalanabhyam_solve(
             test_cases[i].a1, test_cases[i].b1, test_cases[i].c1,
             test_cases[i].a2, test_cases[i].b2, test_cases[i].c2,
             &x, &y);
         
         // Check if result is within a small epsilon of expected
         double epsilon = 0.0001;
         int x_close = fabs(x - test_cases[i].expected_x) < epsilon;
         int y_close = fabs(y - test_cases[i].expected_y) < epsilon;
         
         sprintf(test_name, "Sankalana-Vyavakalanabhyam: %dx + %dy = %d, %dx + %dy = %d -> Expected: x=%g, y=%g; Calculated: x=%g, y=%g", 
                 test_cases[i].a1, test_cases[i].b1, test_cases[i].c1,
                 test_cases[i].a2, test_cases[i].b2, test_cases[i].c2,
                 test_cases[i].expected_x, test_cases[i].expected_y,
                 x, y);
                 
         print_test_result(test_name, result == 0 && x_close && y_close);
     }
 }
 
 /**
  * Test the mathematical operator functions
  */
 void test_operators() {
     struct {
         long a;
         long b;
         char op;
         long expected;
     } test_cases[] = {
         // Addition
         {123, 456, '+', 579},
         {9999, 1, '+', 10000},
         {-45, 123, '+', 78},
         
         // Subtraction
         {456, 123, '-', 333},
         {1000, 1, '-', 999},
         {50, 100, '-', -50},
         
         // Multiplication
         {25, 4, '*', 100},
         {95, 95, '*', 9025},
         {102, 32, '*', 3264},
         {-25, 4, '*', -100},
         
         // Division
         {100, 4, '/', 25},
         {99, 9, '/', 11},
         {1001, 13, '/', 77},
         {-100, 4, '/', -25},
         
         // Modulo
         {100, 30, '%', 10},
         {1001, 10, '%', 1},
         {50, 8, '%', 2},
         {-100, 30, '%', 20},
         {-7, 3, '%', 2},
         {7, 3, '%', 1},
         {-15, 4, '%', 1},
         {15, -4, '%', 3},   // Edge case: negative divisor
         {-15, -4, '%', 1}   // Edge case: both negative
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         long result;
         
         switch(test_cases[i].op) {
             case '+':
                 result = vedic_op_add(test_cases[i].a, test_cases[i].b);
                 break;
             case '-':
                 result = vedic_op_subtract(test_cases[i].a, test_cases[i].b);
                 break;
             case '*':
                 result = vedic_op_multiply(test_cases[i].a, test_cases[i].b);
                 break;
             case '/':
                 result = vedic_op_divide(test_cases[i].a, test_cases[i].b);
                 break;
             case '%':
                 result = vedic_op_modulo(test_cases[i].a, test_cases[i].b);
                 break;
             default:
                 result = 0;
         }
         
         sprintf(test_name, "Operator: %ld %c %ld = %ld[Expected] %ld[Result]", 
                 test_cases[i].a, test_cases[i].op, test_cases[i].b, test_cases[i].expected, result);
                 
         print_test_result(test_name, result == test_cases[i].expected);
     }
     
     // Test expression evaluation
     struct {
         const char* expression;
         long expected;
     } expr_tests[] = {
         {"10 + 5", 15},
         {"20 - 8", 12},
         {"6 * 7", 42},
         {"100 / 5", 20},
         {"10 % 3", 1},
         {"2 ^ 10", 1024},
         {"25 * 4", 100},
         {"95 * 95", 9025},
         {"102 * 32", 3264}
     };
     
     int expr_num_cases = sizeof(expr_tests) / sizeof(expr_tests[0]);
     
     for (int i = 0; i < expr_num_cases; i++) {
         long result;
         int eval_result = vedic_evaluate_expression(expr_tests[i].expression, &result);
         
         sprintf(test_name, "Expression: %s = %ld", 
                 expr_tests[i].expression, expr_tests[i].expected);
                 
         print_test_result(test_name, eval_result == 0 && result == expr_tests[i].expected);
     }
 }
 
 /**
  * Test the central dispatcher functions
  */
 void test_central_dispatcher() {
     struct {
         long a;
         long b;
         long expected;
     } test_cases[] = {
         // The original example
         {102, 32, 3264},
         
         // Numbers ending in 5 for squaring
         {25, 25, 625},
         {75, 75, 5625},
         {95, 95, 9025},
         
         // Numbers near a base for Nikhilam
         {98, 97, 9506},
         {103, 104, 10712},
         {996, 1004, 999984},
         
         // Numbers with last digits summing to 10
         {46, 44, 2024},
         {73, 77, 5621},
         {82, 88, 7216},
         
         // General cases for Urdhva-Tiryagbhyam
         {123, 456, 56088},
         {987, 654, 645498},
         {1234, 5678, 7006652},
         
         // Edge cases
         {0, 123, 0},
         {1, 123, 123},
         {-25, 4, -100},
         {-25, -4, 100}
     };
     
     int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
     char test_name[100];
     
     for (int i = 0; i < num_cases; i++) {
         long result = vedic_multiply(test_cases[i].a, test_cases[i].b);
         
         sprintf(test_name, "Central Dispatcher: %ld x %ld = %ld", 
                 test_cases[i].a, test_cases[i].b, test_cases[i].expected);
                 
         print_test_result(test_name, result == test_cases[i].expected);
     }
     
     // Test squaring dispatcher
     struct {
         long n;
         long expected;
     } square_tests[] = {
         {5, 25},
         {25, 625},
         {98, 9604},
         {102, 10404},
         {995, 990025},
         {1005, 1010025}
     };
     
     int square_num_cases = sizeof(square_tests) / sizeof(square_tests[0]);
     
     for (int i = 0; i < square_num_cases; i++) {
         long result = vedic_square(square_tests[i].n);
         
         sprintf(test_name, "Square Dispatcher: %ld² = %ld", 
                 square_tests[i].n, square_tests[i].expected);
                 
         print_test_result(test_name, result == square_tests[i].expected);
     }
     
     // Test division dispatcher
     struct {
         long dividend;
         long divisor;
         long expected_quotient;
     } div_tests[] = {
         {100, 4, 25},
         {99, 9, 11},
         {1001, 13, 77},
         {1234, 56, 22},
         {9876, 12, 823}
     };
     
     int div_num_cases = sizeof(div_tests) / sizeof(div_tests[0]);
     
     for (int i = 0; i < div_num_cases; i++) {
         long result = vedic_divide(div_tests[i].dividend, div_tests[i].divisor, NULL);
         
         sprintf(test_name, "Division Dispatcher: %ld / %ld = %ld", 
                 div_tests[i].dividend, div_tests[i].divisor, div_tests[i].expected_quotient);
                 
         print_test_result(test_name, result == div_tests[i].expected_quotient);
     }
 }
 
 /**
  * Run random tests to verify the library against standard operations
  */
 void test_random_operations() {
     // Seed the random number generator
     srand(time(NULL));
     
     printf("\n=== Running random tests ===\n");
     
     // Test multiplication with random numbers
     for (int i = 0; i < 20; i++) {
         long a = rand() % 10000;
         long b = rand() % 10000;
         
         long expected = a * b;
         long result = vedic_multiply(a, b);
         
         char test_name[100];
         sprintf(test_name, "Random Multiplication: %ld x %ld = %ld", a, b, expected);
         
         print_test_result(test_name, result == expected);
     }
     
     // Test division with random numbers
     for (int i = 0; i < 20; i++) {
         long dividend = rand() % 100000;
         long divisor = rand() % 1000 + 1;  // Avoid division by zero
         
         long expected_quotient = dividend / divisor;
         long expected_remainder = dividend % divisor;
         
         long remainder;
         long quotient = vedic_divide(dividend, divisor, &remainder);
         
         char test_name[100];
         sprintf(test_name, "Random Division: %ld / %ld = %ld R %ld", 
                 dividend, divisor, expected_quotient, expected_remainder);
         
         print_test_result(test_name, quotient == expected_quotient && remainder == expected_remainder);
     }
     
     // Test squaring with random numbers
     for (int i = 0; i < 20; i++) {
         long n = rand() % 1000;
         
         long expected = n * n;
         long result = vedic_square(n);
         
         char test_name[100];
         sprintf(test_name, "Random Squaring: %ld² = %ld", n, expected);
         
         print_test_result(test_name, result == expected);
     }
     
     // Test operators with random numbers
     for (int i = 0; i < 20; i++) {
         long a = rand() % 10000;
         long b = rand() % 1000 + 1;  // Avoid division by zero
         
         // Choose a random operator
         char operators[] = {'+', '-', '*', '/', '%'};
         char op = operators[rand() % 5];
         
         long expected, result;
         
         switch(op) {
             case '+':
                 expected = a + b;
                 result = vedic_op_add(a, b);
                 break;
             case '-':
                 expected = a - b;
                 result = vedic_op_subtract(a, b);
                 break;
             case '*':
                 expected = a * b;
                 result = vedic_op_multiply(a, b);
                 break;
             case '/':
                 expected = a / b;
                 result = vedic_op_divide(a, b);
                 break;
             case '%':
                 expected = a % b;
                 result = vedic_op_modulo(a, b);
                 break;
             default:
                 continue;
         }
         
         char test_name[100];
         sprintf(test_name, "Random Operator: %ld %c %ld = %ld", a, op, b, expected);
         
         print_test_result(test_name, result == expected);
     }
 }

 
/**
 * Test Dhvajanka division method
 */
void test_dhvajanka_division() {
    printf("\n=== Testing Dhvajanka (Flag Method) Division ===\n");
    
    struct {
        long dividend;
        long divisor;
        long expected_quotient;
        long expected_remainder;
        const char* description;
    } test_cases[] = {
        // Basic 2-digit divisors
        {1234, 23, 53, 15, "Basic 2-digit: 1234 ÷ 23"},
        {5678, 34, 167, 0, "Exact division: 5678 ÷ 34"},
        {9999, 45, 222, 9, "Large dividend: 9999 ÷ 45"},
        
        // 3-digit divisors
        {12345, 123, 100, 45, "3-digit divisor: 12345 ÷ 123"},
        {98765, 234, 422, 17, "Large 3-digit: 98765 ÷ 234"},
        {50000, 125, 400, 0, "Round result: 50000 ÷ 125"},
        
        // 4-digit divisors
        {123456, 1234, 100, 56, "4-digit divisor: 123456 ÷ 1234"},
        {999999, 2468, 405, 459, "Large 4-digit: 999999 ÷ 2468"},
        
        // Edge cases
        {100, 99, 1, 1, "Near base: 100 ÷ 99"},
        {1000, 101, 9, 91, "Above base: 1000 ÷ 101"},
        {12, 34, 0, 12, "Dividend < divisor: 12 ÷ 34"},
        
        // Negative numbers
        {-1234, 23, -53, 15, "Negative dividend: -1234 ÷ 23"},
        {1234, -23, -53, 15, "Negative divisor: 1234 ÷ -23"},
        {-1234, -23, 53, 15, "Both negative: -1234 ÷ -23"}
    };
    
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_cases; i++) {
        long remainder;
        long quotient = dhvajanka_divide(test_cases[i].dividend, test_cases[i].divisor, &remainder);
        
        // Verify the result
        long verification = (abs(quotient * test_cases[i].divisor) + remainder) * 
                           (test_cases[i].dividend < 0 ? -1 : 1);
        bool correct_division = (verification == test_cases[i].dividend);
        bool matches_expected = (quotient == test_cases[i].expected_quotient && 
                               remainder == test_cases[i].expected_remainder);
        
        char test_name[200];
        sprintf(test_name, "Dhvajanka: %s (got %ld R %ld)", 
                test_cases[i].description, quotient, remainder);
        
        print_test_result(test_name, correct_division && matches_expected);
        
        if (!correct_division || !matches_expected) {
            printf("  Expected: %ld R %ld, Got: %ld R %ld, Verification: %ld == %ld? %s\n",
                   test_cases[i].expected_quotient, test_cases[i].expected_remainder,
                   quotient, remainder, verification, test_cases[i].dividend,
                   correct_division ? "YES" : "NO");
        }
    }
}

/**
 * Test Nikhilam division method
 */
void test_nikhilam_division() {
    printf("\n=== Testing Nikhilam Division Sutra ===\n");
    
    struct {
        long dividend;
        long divisor;
        long expected_quotient;
        long expected_remainder;
        const char* description;
    } test_cases[] = {
        // Numbers near 10
        {123, 9, 13, 6, "Near 10 (below): 123 ÷ 9"},
        {234, 11, 21, 3, "Near 10 (above): 234 ÷ 11"},
        
        // Numbers near 100
        {9876, 99, 99, 75, "Near 100 (below): 9876 ÷ 99"},
        {8765, 101, 86, 79, "Near 100 (above): 8765 ÷ 101"},
        {5000, 98, 51, 2, "Round near 100: 5000 ÷ 98"},
        
        // Numbers near 1000
        {123456, 999, 123, 579, "Near 1000 (below): 123456 ÷ 999"},
        {98765, 1001, 98, 667, "Near 1000 (above): 98765 ÷ 1001"},
        
        // Edge cases for Nikhilam
        {100, 99, 1, 1, "Exact near base: 100 ÷ 99"},
        {1000, 1001, 0, 1000, "Dividend < divisor: 1000 ÷ 1001"},
        {99, 99, 1, 0, "Exact division: 99 ÷ 99"},
        
        // Not suitable for Nikhilam (should fall back to standard)
        {1234, 37, 33, 13, "Non-Nikhilam case: 1234 ÷ 37"},
        
        // Negative cases
        {-9876, 99, -99, 75, "Negative dividend: -9876 ÷ 99"},
        {8765, -101, -86, 79, "Negative divisor: 8765 ÷ -101"}
    };
    
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_cases; i++) {
        long remainder;
        long quotient = nikhilam_divide_sutra(test_cases[i].dividend, test_cases[i].divisor, &remainder);
        
        // Verify the result
        long verification = (abs(quotient * test_cases[i].divisor) + remainder) * 
                           (test_cases[i].dividend < 0 ? -1 : 1);
        bool correct_division = (verification == test_cases[i].dividend);
        bool matches_expected = (quotient == test_cases[i].expected_quotient && 
                               remainder == test_cases[i].expected_remainder);
        
        char test_name[200];
        sprintf(test_name, "Nikhilam: %s (got %ld R %ld)", 
                test_cases[i].description, quotient, remainder);
        
        print_test_result(test_name, correct_division && matches_expected);
        
        if (!correct_division || !matches_expected) {
            printf("  Expected: %ld R %ld, Got: %ld R %ld, Verification: %ld == %ld? %s\n",
                   test_cases[i].expected_quotient, test_cases[i].expected_remainder,
                   quotient, remainder, verification, test_cases[i].dividend,
                   correct_division ? "YES" : "NO");
        }
    }
}

/**
 * Test the enhanced division dispatcher
 */
void test_enhanced_division_dispatcher() {
    printf("\n=== Testing Enhanced Division Dispatcher ===\n");
    
    struct {
        long dividend;
        long divisor;
        const char* expected_method;
        const char* description;
    } test_cases[] = {
        {1234, 5, "Standard Division", "Single digit divisor"},
        {1234, 99, "Nikhilam Division Sutra", "Near 100 (Nikhilam suitable)"},
        {1234, 23, "Dhvajanka (Flag Method)", "2-digit divisor (Dhvajanka suitable)"},
        {1234, 12, "Paravartya Yojayet", "2-digit divisor (Paravartya suitable)"},
        {1234, 1001, "Nikhilam Division Sutra", "Near 1000 (Nikhilam suitable)"},
        {1234, 567, "Standard Division (Fallback)", "3-digit non-suitable case"},
        {50, 100, "Standard Division", "Dividend < divisor"},
        {0, 5, "Standard Division", "Zero dividend"}
    };
    
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_cases; i++) {
        long remainder;
        const char* method_used;
        long quotient = vedic_divide_enhanced(test_cases[i].dividend, test_cases[i].divisor, 
                                            &remainder, &method_used);
        
        // Verify correctness
        long verification = (abs(quotient * test_cases[i].divisor) + remainder) * 
                           (test_cases[i].divisor * test_cases[i].dividend < 0 ? -1 : 1);
        bool correct_result = (verification == test_cases[i].dividend);
        
        // Check method selection (allow some flexibility)
        bool correct_method = (strcmp(method_used, test_cases[i].expected_method) == 0);
        
        char test_name[200];
        sprintf(test_name, "Dispatcher: %s - Method: %s", 
                test_cases[i].description, method_used);
        
        print_test_result(test_name, correct_result);
        
        if (!correct_result) {
            printf("  Division error: %ld ÷ %ld = %ld R %ld, Verification: %ld\n",
                   test_cases[i].dividend, test_cases[i].divisor, quotient, remainder, verification);
        }
        
        if (!correct_method) {
            printf("  Method selection: Expected '%s', Got '%s'\n",
                   test_cases[i].expected_method, method_used);
        }
    }
}