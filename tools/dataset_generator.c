#include "vedic_core.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int count = 10000;
    const char* output = "vedic_dataset.csv";
    
    if (argc > 1) count = atoi(argv[1]);
    if (argc > 2) output = argv[2];
    
    // Initialize with logging
    VedicCoreConfig config = {
        .mode = VEDIC_MODE_ADAPTIVE,
        .logging_enabled = true,
        .platform = VEDIC_PLATFORM_DESKTOP
    };
    vedic_core_init(&config);
    
    printf("Generating %d samples...\n", count);
    
    // Generate diverse patterns
    for (int i = 0; i < count; i++) {
        VedicValue a, b;
        
        switch (i % 6) {
            case 0: // Ekadhikena pattern
                a = b = vedic_from_int32((rand() % 20 + 1) * 10 + 5);
                break;
            case 1: // Nikhilam pattern  
                a = vedic_from_int32(85 + rand() % 30);
                b = vedic_from_int32(85 + rand() % 30);
                break;
            case 2: // Antyayordasake pattern
                {
                    int prefix = rand() % 9 + 1;
                    int last_a = rand() % 9 + 1;
                    int last_b = 10 - last_a;
                    a = vedic_from_int32(prefix * 10 + last_a);
                    b = vedic_from_int32(prefix * 10 + last_b);
                }
                break;
            default: // Random
                a = vedic_from_int32(rand() % 1000 + 1);
                b = vedic_from_int32(rand() % 1000 + 1);
                break;
        }
        
        multiply_vedic_unified(a, b);
        
        if (i % 1000 == 0) {
            printf("Generated %d samples\n", i);
        }
    }
    
    // Export dataset
    vedic_core_export_dataset(output);
    printf("Dataset exported to %s\n", output);

    vedic_core_cleanup();
    return 0;
}