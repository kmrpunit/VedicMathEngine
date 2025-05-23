#include "dispatch_mixed_mode.h"
#include <stdio.h>

int main() {
    // Initialize dispatcher
    dispatch_mixed_mode_init(NULL);
    
    // Test system-aware execution
    VedicValue a = vedic_from_int32(98);
    VedicValue b = vedic_from_int32(97);
    VedicValue result = dispatch_multiply(a, b);
    
    char result_str[64];
    vedic_to_string(result, result_str, sizeof(result_str));
    printf("98 * 97 = %s\n", result_str);
    
    // Show system status
    SystemResourceMonitor status = dispatch_get_system_status();
    printf("CPU Usage: %.1f%%\n", status.cpu_usage_percent);
    printf("Memory Usage: %.1f%%\n", status.memory_usage_percent);
    
    return 0;
}