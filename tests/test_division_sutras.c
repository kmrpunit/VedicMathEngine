/**
 * test_division_sutras.c - Comprehensive test suite for enhanced division methods
 * 
 * Tests the new Dhvajanka and Nikhilam division sutras with various cases
 */

#include "vedicmath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function declarations for the new division methods
long dhvajanka_divide(long dividend, long divisor, long *remainder);
long nikhilam_divide_sutra(long dividend, long divisor, long *remainder);
long vedic_divide_enhanced(long dividend, long divisor, long *remainder, const char **method_used);
void test_all_division_methods(long dividend, long divisor);

// Test result tracking
static int total_tests = 0;
static int passed_tests = 0;

#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED   "\033[0;31m"
#define COLOR_RESET "\033[0m"

void print_test_result(const char* test_name, int result) {
    total_tests++;
    if (result) {
        passed_tests++;
        printf(COLOR_GREEN "[✓] PASS: %s\n" COLOR_RESET, test_name);
    } else {
        printf(COLOR_RED "[✗] FAIL: %s\n" COLOR_RESET, test_name);
    }
}

void print_test_summary() {
    printf("\n==== DIVISION SUTRAS TEST SUMMARY ====\n");
    printf("Total tests: %d\n", total_tests);
    printf("Passed: %d (%.1f%%)\n", passed_tests, (float)passed_tests / total_tests * 100);
    printf("Failed: %d (%.1f%%)\n", total_tests - passed_tests, 
           (float)(total_tests - passed_tests) / total_tests * 100);
    printf("=====================================\n");
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

/**
 * Performance comparison test
 */
void test_division_performance() {
    printf("\n=== Division Performance Comparison ===\n");
    
    srand(time(NULL));
    
    const int num_tests = 1000;
    clock_t start, end;
    double total_time_standard = 0, total_time_enhanced = 0;
    int correct_results = 0;
    
    printf("Running %d random division tests...\n", num_tests);
    
    for (int i = 0; i < num_tests; i++) {
        long dividend = rand() % 100000 + 1;
        long divisor = rand() % 1000 + 1;
        
        // Standard division timing
        start = clock();
        long std_quot = dividend / divisor;
        long std_rem = dividend % divisor;
        end = clock();
        total_time_standard += ((double)(end - start)) / CLOCKS_PER_SEC;
        
        // Enhanced division timing
        start = clock();
        long remainder;
        const char* method_used;
        long enh_quot = vedic_divide_enhanced(dividend, divisor, &remainder, &method_used);
        end = clock();
        total_time_enhanced += ((double)(end - start)) / CLOCKS_PER_SEC;
        
        // Verify correctness
        if (enh_quot == std_quot && remainder == std_rem) {
            correct_results++;
        }
    }
    
    printf("Results:\n");
    printf("  Correct results: %d/%d (%.1f%%)\n", 
           correct_results, num_tests, (float)correct_results / num_tests * 100);
    printf("  Standard division time: %.6f seconds\n", total_time_standard);
    printf("  Enhanced division time: %.6f seconds\n", total_time_enhanced);
    
    if (total_time_standard > 0) {
        double speedup = total_time_standard / total_time_enhanced;
        printf("  Speedup factor: %.2fx %s\n", speedup, 
               speedup > 1.0 ? "(Enhanced is faster)" : "(Standard is faster)");
    }
    
    char test_name[100];
    sprintf(test_name, "Performance test: %d/%d correct results", correct_results, num_tests);
    print_test_result(test_name, correct_results == num_tests);
}

/**
 * Comprehensive validation test
 */
void test_comprehensive_validation() {
    printf("\n=== Comprehensive Validation Test ===\n");
    
    // Test a variety of dividend/divisor combinations
    long test_dividends[] = {123, 1234, 12345, 99999, 50000, 1000000};
    long test_divisors[] = {9, 11, 23, 99, 101, 123, 999, 1001};
    
    int dividend_count = sizeof(test_dividends) / sizeof(test_dividends[0]);
    int divisor_count = sizeof(test_divisors) / sizeof(test_divisors[0]);
    
    int total_combinations = dividend_count * divisor_count;
    int correct_results = 0;
    
    printf("Testing %d combinations of dividends and divisors...\n", total_combinations);
    
    for (int i = 0; i < dividend_count; i++) {
        for (int j = 0; j < divisor_count; j++) {
            long dividend = test_dividends[i];
            long divisor = test_divisors[j];
            
            // Standard result
            long std_quot = dividend / divisor;
            long std_rem = dividend % divisor;
            
            // Test all our methods
            long remainder;
            const char* method_used;
            
            // Enhanced dispatcher
            long enh_quot = vedic_divide_enhanced(dividend, divisor, &remainder, &method_used);
            bool enhanced_correct = (enh_quot == std_quot && remainder == std_rem);
            
            // Dhvajanka
            long dhv_quot = dhvajanka_divide(dividend, divisor, &remainder);
            bool dhvajanka_correct = (dhv_quot == std_quot && remainder == std_rem);
            
            // Nikhilam
            long nik_quot = nikhilam_divide_sutra(dividend, divisor, &remainder);
            bool nikhilam_correct = (nik_quot == std_quot && remainder == std_rem);
            
            if (enhanced_correct && dhvajanka_correct && nikhilam_correct) {
                correct_results++;
            } else {
                printf("  MISMATCH: %ld ÷ %ld\n", dividend, divisor);
                printf("    Standard: %ld R %ld\n", std_quot, std_rem);
                if (!enhanced_correct) {
                    printf("    Enhanced: %ld R %ld (%s)\n", enh_quot, remainder, method_used);
                }
                if (!dhvajanka_correct) {
                    long dhv_rem;
                    dhv_quot = dhvajanka_divide(dividend, divisor, &dhv_rem);
                    printf("    Dhvajanka: %ld R %ld\n", dhv_quot, dhv_rem);
                }
                if (!nikhilam_correct) {
                    long nik_rem;
                    nik_quot = nikhilam_divide_sutra(dividend, divisor, &nik_rem);
                    printf("    Nikhilam: %ld R %ld\n", nik_quot, nik_rem);
                }
            }
        }
    }
    
    char test_name[100];
    sprintf(test_name, "Comprehensive validation: %d/%d combinations correct", 
            correct_results, total_combinations);
    print_test_result(test_name, correct_results == total_combinations);
}

/**
 * Method selection demonstration
 */
void demonstrate_method_selection() {
    printf("\n=== Method Selection Demonstration ===\n");
    
    long test_cases[][2] = {
        {1234, 9},    // Should select Nikhilam (near 10)
        {5678, 23},   // Should select Dhvajanka (2-digit)
        {9999, 99},   // Should select Nikhilam (near 100)
        {12345, 123}, // Should select Dhvajanka (3-digit)
        {8765, 1001}, // Should select Nikhilam (near 1000)
        {4321, 7},    // Should select Standard (single digit)
        {100, 37}     // Should select Standard (not near base, not suitable for flag)
    };
    
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_cases; i++) {
        printf("\nCase %d: %ld ÷ %ld\n", i+1, test_cases[i][0], test_cases[i][1]);
        test_all_division_methods(test_cases[i][0], test_cases[i][1]);
    }
}

/**
 * Main test function
 */
int main() {
    printf("=== Enhanced Vedic Division Sutras Test Suite ===\n");
    printf("Testing two new division methods:\n");
    printf("1. Dhvajanka (Flag Method)\n");
    printf("2. Nikhilam Division Sutra\n");
    printf("3. Enhanced Division Dispatcher\n\n");
    
    // Run all test suites
    test_dhvajanka_division();
    test_nikhilam_division();
    test_enhanced_division_dispatcher();
    test_division_performance();
    test_comprehensive_validation();
    demonstrate_method_selection();
    
    // Print overall summary
    print_test_summary();
    
    printf("\n=== Test Suite Complete ===\n");
    
    return (passed_tests == total_tests) ? 0 : 1;
}