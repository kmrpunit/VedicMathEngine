#include "vedic_core.h"
#include <stdio.h>

int main() {
    // Initialize core engine
    VedicCoreConfig config = {
        .mode = VEDIC_MODE_ADAPTIVE,
        .logging_enabled = true,
        .platform = VEDIC_PLATFORM_DESKTOP
    };
    
    vedic_core_init(&config);
    
    // Test unified interface
    VedicValue a = vedic_from_int32(102);
    VedicValue b = vedic_from_int32(32);
    VedicValue result = multiply_vedic_unified(a, b);
    
    char result_str[64];
    vedic_to_string(result, result_str, sizeof(result_str));
    printf("102 * 32 = %s\n", result_str);
    
    // Export dataset
    vedic_core_export_dataset("demo_dataset.csv");
    
    // Cleanup
    vedic_core_cleanup();
    return 0;
}