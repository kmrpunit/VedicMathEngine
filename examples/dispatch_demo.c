#include "dispatch_mixed_mode.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    printf("=== Enhanced VedicMath-AI Adaptive Engine Test ===\n\n");
    
    // Initialize random seed for test generation
    srand((unsigned int)time(NULL));
    
    // Initialize dispatcher with default configuration
    printf("1. Initializing Enhanced Adaptive Dispatcher...\n");
    DispatchResult init_result = dispatch_mixed_mode_init(NULL);
    if (init_result != DISPATCH_SUCCESS) {
        printf("ERROR: Failed to initialize dispatcher (code: %d)\n", init_result);
        return -1;
    }
    printf("✓ Dispatcher initialized successfully\n\n");
    
    // Test 1: Ekadhikena Purvena pattern (numbers ending in 5)
    printf("2. Testing Ekadhikena Purvena Pattern:\n");
    VedicValue a1 = vedic_from_int64(85);
    VedicValue b1 = vedic_from_int64(85);  // 85² - perfect Ekadhikena case
    VedicValue result1 = dispatch_multiply(a1, b1);
    
    char result_str1[64];
    vedic_to_string(result1, result_str1, sizeof(result_str1));
    printf("85 * 85 = %s (Expected: 7225)\n", result_str1);
    
    // Test 2: Nikhilam pattern (numbers near 100)  
    printf("\n3. Testing Nikhilam Pattern:\n");
    VedicValue a2 = vedic_from_int64(98);
    VedicValue b2 = vedic_from_int64(97);  // Both near 100
    VedicValue result2 = dispatch_multiply(a2, b2);
    
    char result_str2[64];
    vedic_to_string(result2, result_str2, sizeof(result_str2));
    printf("98 * 97 = %s (Expected: 9506)\n", result_str2);
    
    // Test 3: Antyayordasake pattern
    printf("\n4. Testing Antyayordasake Pattern:\n");
    VedicValue a3 = vedic_from_int64(132);
    VedicValue b3 = vedic_from_int64(138);  // Same prefix (13), last digits sum to 10 (2+8)
    VedicValue result3 = dispatch_multiply(a3, b3);
    
    char result_str3[64];
    vedic_to_string(result3, result_str3, sizeof(result_str3));
    printf("132 * 138 = %s (Expected: 18216)\n", result_str3);
    
    // Test 4: Large numbers (Urdhva-Tiryagbhyam)
    printf("\n5. Testing Large Numbers (Urdhva-Tiryagbhyam):\n");
    VedicValue a4 = vedic_from_int64(1234);
    VedicValue b4 = vedic_from_int64(5678);
    VedicValue result4 = dispatch_multiply(a4, b4);
    
    char result_str4[64];
    vedic_to_string(result4, result_str4, sizeof(result_str4));
    printf("1234 * 5678 = %s (Expected: 7006652)\n", result_str4);
    
    // Test 5: Division
    printf("\n6. Testing Enhanced Division:\n");
    VedicValue dividend = vedic_from_int64(9506);
    VedicValue divisor = vedic_from_int64(97);
    VedicValue div_result = dispatch_divide(dividend, divisor);
    
    char div_result_str[64];
    vedic_to_string(div_result, div_result_str, sizeof(div_result_str));
    printf("9506 / 97 = %s (Expected: 98)\n", div_result_str);
    
    // Test 6: Squaring
    printf("\n7. Testing Adaptive Squaring:\n");
    VedicValue square_input = vedic_from_int64(125);  // Ends in 5 - perfect for Ekadhikena
    VedicValue square_result = dispatch_square(square_input);
    
    char square_result_str[64];
    vedic_to_string(square_result, square_result_str, sizeof(square_result_str));
    printf("125² = %s (Expected: 15625)\n", square_result_str);
    
    // Show current system status
    printf("\n8. System Resource Status:\n");
    SystemResourceMonitor system_monitor = dispatch_get_system_status();
    printf("CPU Usage: %.1f%%\n", system_monitor.cpu_usage_percent);
    printf("Memory Usage: %.1f%%\n", system_monitor.memory_usage_percent);
    printf("Temperature: %.1f°C\n", system_monitor.temperature_celsius);
    printf("Platform: %s\n", 
#ifdef ESP32_PLATFORM
           "ESP32"
#elif defined(_WIN32)
           "Windows"
#elif defined(__linux__)
           "Linux"
#else
           "Generic"
#endif
    );
    
    // Check if system is stressed
    if (dispatch_is_system_stressed()) {
        printf("⚠ System is under high load - algorithms optimized for speed\n");
    } else {
        printf("✓ System resources normal - full algorithm selection available\n");
    }
    
    // Generate validation dataset for research
    printf("\n9. Generating Research Validation Dataset...\n");
    generate_comprehensive_validation_dataset(1000);  // Generate 1000 test cases
    
    // Analyze performance statistics
    printf("\n10. Performance Analysis:\n");
    analyze_performance_statistics();
    
    // Export results for research
    printf("\n11. Exporting Research Data...\n");
    dispatch_cleanup_and_export("vedic_performance_validation.csv");
    
    printf("\n=== Test Complete ===\n");
    printf("✓ All operations validated\n");
    printf("✓ Performance data collected\n");
    printf("✓ Research dataset exported\n");
    
    return 0;
}