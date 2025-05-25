// Add this debug code to investigate the failing Nikhilam cases
#include <stdio.h>
#include "utf8_console.h"
#include "vedicmath.h"

void debug_nikhilam_failures() {
    printf("\n=== DEBUGGING NIKHILAM FAILURES ===\n");
    
    // Test case 1: 997 × 998
    printf("Case 1: 997 × 998\n");
    printf("Expected: 994906\n");
    
    long result1 = nikhilam_mul(997, 998);
    printf("Nikhilam result: %ld\n", result1);
    printf("Standard result: %ld\n", 997 * 998);
    printf("Difference: %ld\n", (997 * 998) - result1);
    
    // Manual Nikhilam calculation for 997 × 998 (base 1000):
    // 997 = 1000 - 3
    // 998 = 1000 - 2  
    // Result should be: (1000-3-2) || (3×2) = 995 || 06 = 995006
    // But expected is 994906, which suggests base might be wrong
    
    printf("\nManual Nikhilam (base 1000):\n");
    printf("997 = 1000 - 3\n");
    printf("998 = 1000 - 2\n");
    printf("Result = (1000-3-2) || (3×2) = 995 || 06 = 995006\n");
    printf("But this doesn't match expected 994906\n");
    
    printf("\n");
    for (int i = 0; i < 50; ++i) printf("=");
    printf("\n");
    
    // Test case 2: 995 × 996  
    printf("Case 2: 995 × 996\n");
    printf("Expected: 990420\n");
    
    long result2 = nikhilam_mul(995, 996);
    printf("Nikhilam result: %ld\n", result2);
    printf("Standard result: %ld\n", 995 * 996);
    printf("Difference: %ld\n", (995 * 996) - result2);
    
    printf("\nManual Nikhilam (base 1000):\n");
    printf("995 = 1000 - 5\n");
    printf("996 = 1000 - 4\n");
    printf("Result = (1000-5-4) || (5×4) = 991 || 20 = 991020\n");
    printf("But expected is 990420\n");
    
    printf("\n=== BASE DETECTION ANALYSIS ===\n");
    
    // Check what base your nearest_power_of_10 function returns
    printf("Base detection for 997: %ld\n", nearest_power_of_10(997));
    printf("Base detection for 998: %ld\n", nearest_power_of_10(998));
    printf("Base detection for 995: %ld\n", nearest_power_of_10(995));
    printf("Base detection for 996: %ld\n", nearest_power_of_10(996));
    
    printf("\n=== POSSIBLE ISSUES ===\n");
    printf("1. Base detection might be choosing 100 instead of 1000\n");
    printf("2. Nikhilam implementation might have overflow issues\n");
    printf("3. Digit concatenation (||) operation might be incorrect\n");
    printf("4. The expected results in test might be wrong\n");
    
    // Verify the expected results are actually correct
    printf("\n=== VERIFICATION ===\n");
    printf("997 × 998 = %d (calculator verification)\n", 997 * 998);
    printf("995 × 996 = %d (calculator verification)\n", 995 * 996);
}

// Alternative: Add debug prints inside your nikhilam_mul function
void debug_nikhilam_step_by_step(int a, int b) {
    printf("\n--- Nikhilam Debug for %d × %d ---\n", a, b);
    
    long base_a = nearest_power_of_10(a);
    long base_b = nearest_power_of_10(b);
    printf("Base for %d: %ld\n", a, base_a);
    printf("Base for %d: %ld\n", b, base_b);
    
    if (base_a != base_b) {
        printf("ERROR: Different bases detected!\n");
        return;
    }
    
    long base = base_a;
    long diff_a = base - a;  // How far a is from base
    long diff_b = base - b;  // How far b is from base
    
    printf("Differences from base %ld:\n", base);
    printf("  %d is %ld away (base - a = %ld - %d = %ld)\n", a, diff_a, base, a, diff_a);
    printf("  %d is %ld away (base - b = %ld - %d = %ld)\n", b, diff_b, base, b, diff_b);
    
    // Nikhilam formula: (base - diff_a - diff_b) || (diff_a × diff_b)
    long left_part = base - diff_a - diff_b;
    long right_part = diff_a * diff_b;
    
    printf("Nikhilam calculation:\n");
    printf("  Left part: %ld - %ld - %ld = %ld\n", base, diff_a, diff_b, left_part);
    printf("  Right part: %ld × %ld = %ld\n", diff_a, diff_b, right_part);
    
    // The concatenation step is critical
    printf("  Final result: %ld || %ld = ?\n", left_part, right_part);
    
    long result = nikhilam_mul(a, b);
    printf("  Actual nikhilam_mul result: %ld\n", result);
    printf("  Expected (standard): %d\n", a * b);
    printf("--- End Debug ---\n\n");
}

/**
 * MODULO OPERATION WITH NEGATIVE NUMBERS
 * 
 * The issue: Different programming languages handle negative modulo differently
 */

// Demonstration of the problem
void demonstrate_modulo_issue() {
    printf("=== MODULO WITH NEGATIVE NUMBERS ===\n");
    
    // C/C++ standard behavior (implementation-defined for negative numbers)
    printf("C/C++ Standard Behavior:\n");
    printf("-100 %% 30 = %d\n", -100 % 30);  // Usually -10
    printf("-7 %% 3 = %d\n", -7 % 3);        // Usually -1
    printf("7 %% 3 = %d\n", 7 % 3);          // Always 1
    
    printf("\nMathematical Definition (Euclidean):\n");
    printf("-100 mod 30 should be 20\n");
    printf("-7 mod 3 should be 2\n");
    printf("7 mod 3 should be 1\n");
    
    printf("\nWhy the difference?\n");
    printf("C/C++: remainder has same sign as dividend\n");
    printf("Math: remainder is always non-negative (0 ≤ r < divisor)\n");
}

/**
 * SOLUTION 1: Euclidean Modulo (Always Positive Remainder)
 * 
 * This matches mathematical definition: 0 ≤ (a mod n) < n
 */
int euclidean_mod(int a, int n) {
    if (n <= 0) {
        printf("Error: Modulo by non-positive number\n");
        return 0;
    }
    
    int r = a % n;
    return (r < 0) ? r + n : r;
}

/**
 * SOLUTION 2: Floored Division Modulo (Python-style)
 * 
 * This matches Python's behavior where result has same sign as divisor
 */
int floored_mod(int a, int n) {
    if (n == 0) {
        printf("Error: Division by zero\n");
        return 0;
    }
    
    int r = a % n;
    if (r != 0 && ((n > 0) != (r > 0))) {
        r += n;
    }
    return r;
}

/**
 * SOLUTION 3: Truncated Division Modulo (C standard)
 * 
 * This is what C/C++ does by default - remainder has same sign as dividend
 */
int truncated_mod(int a, int n) {
    return a % n;  // Standard C behavior
}

/**
 * Test all three modulo implementations
 */
void test_modulo_implementations() {
    printf("\n=== TESTING MODULO IMPLEMENTATIONS ===\n");
    
    int test_cases[][2] = {
        {-100, 30},
        {-7, 3},
        {7, 3},
        {100, 30},
        {-15, 4},
        {15, -4},   // Edge case: negative divisor
        {-15, -4}   // Edge case: both negative
    };
    
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    
    printf("%-8s %-8s %-12s %-12s %-12s\n", 
           "a", "n", "Euclidean", "Floored", "Truncated");
    printf("%-8s %-8s %-12s %-12s %-12s\n", 
           "---", "---", "---------", "-------", "---------");
    
    for (int i = 0; i < num_tests; i++) {
        int a = test_cases[i][0];
        int n = test_cases[i][1];
        
        if (n != 0) {  // Avoid division by zero
            printf("%-8d %-8d %-12d %-12d %-12d\n",
                   a, n,
                   euclidean_mod(a, abs(n)),  // Euclidean needs positive divisor
                   floored_mod(a, n),
                   truncated_mod(a, n));
        }
    }
}

/**
 * FOR YOUR VEDIC MATH ENGINE:
 * Choose the appropriate modulo based on your mathematical requirements
 */

// If your test expects mathematical modulo (always positive):
int vedic_mod(int a, int n) {
    return euclidean_mod(a, n);
}

// If your test expects C standard behavior:
int vedic_mod_standard(int a, int n) {
    return a % n;
}

/**
 * QUICK FIX FOR YOUR SPECIFIC CASE:
 * -100 % 30 should equal 20 (not -10)
 */
void fix_specific_case() {
    printf("\n=== FIXING YOUR SPECIFIC CASE ===\n");
    
    int a = -100, n = 30;
    
    printf("Problem: %d %% %d = %d (C standard)\n", a, n, a % n);
    printf("Expected: %d %% %d = %d (mathematical)\n", a, n, euclidean_mod(a, n));
    
    // Verification:
    // -100 = -4 × 30 + 20
    // So -100 mod 30 = 20
    int quotient = -4;
    int remainder = 20;
    printf("Verification: %d = %d × %d + %d\n", a, quotient, n, remainder);
    printf("Check: %d × %d + %d = %d ✓\n", quotient, n, remainder, quotient * n + remainder);
}

/**
 * RECOMMENDATION FOR YOUR TEST SUITE:
 */
void recommendation() {
    printf("\n=== RECOMMENDATION ===\n");
    printf("1. Decide which modulo definition your Vedic engine should use:\n");
    printf("   - Mathematical (Euclidean): Always positive remainder\n");
    printf("   - Programming (C standard): Remainder matches dividend sign\n");
    printf("\n");
    printf("2. Update either:\n");
    printf("   - Your modulo implementation to use euclidean_mod()\n");
    printf("   - Your test expectation from 20 to -10\n");
    printf("\n");
    printf("3. For Vedic Mathematics, mathematical modulo is usually preferred\n");
    printf("   because it aligns with traditional mathematical definitions.\n");
}

int main() {
    // Run the modulo implementations test    
    set_utf8_console();
    printf("=== Vedic Mathematics Debugging Test ===\n");
    printf("This test suite is designed to investigate specific issues in Vedic Mathematics implementations.\n");
    printf("It includes debugging for Nikhilam failures, modulo issues, and performance validation.\n\n");
    printf("Running tests...\n\n");
    // Run the tests

    demonstrate_modulo_issue();
    test_modulo_implementations();
    fix_specific_case();
    recommendation();
    // Run the debug function to investigate Nikhilam failures
    debug_nikhilam_failures();
    
    // Alternatively, you can test specific cases step by step
    debug_nikhilam_step_by_step(997, 998);
    debug_nikhilam_step_by_step(995, 996);
    
    return 0;
}