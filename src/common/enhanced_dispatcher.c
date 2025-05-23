/**
 * enhanced_dispatcher.c - Advanced rule-based dispatcher with dataset generation
 */
#include "vedicmath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Decision tracking structure for dataset generation
typedef struct {
    long operand_a;
    long operand_b;
    const char* sutra_used;
    double execution_time_ms;
    double cpu_usage_percent;
    size_t memory_used_bytes;
    int confidence_score;  // 0-100
    const char* reason;
} DecisionRecord;

// Rule confidence thresholds
#define HIGH_CONFIDENCE 90
#define MEDIUM_CONFIDENCE 70
#define LOW_CONFIDENCE 50

// Enhanced rule-based decision engine
typedef struct {
    const char* rule_name;
    int (*condition_check)(long a, long b);
    long (*sutra_function)(long a, long b);
    int confidence_score;
    const char* description;
} SutraRule;

// Rule condition functions
static int is_ekadhikena_case(long a, long b) {
    return (a == b) && (a % 10 == 5) && (a > 0);
}

static int is_nikhilam_case(long a, long b) {
    long base_a = nearest_power_of_10(a);
    long base_b = nearest_power_of_10(b);
    
    if (base_a != base_b) return 0;
    
    double ratio_a = (double)a / base_a;
    double ratio_b = (double)b / base_b;
    
    return (ratio_a >= 0.85 && ratio_a <= 1.15) && 
           (ratio_b >= 0.85 && ratio_b <= 1.15);
}

static int is_antyayordasake_case(long a, long b) {
    return last_digits_sum_to_10(a, b) && same_prefix(a, b);
}

static int is_urdhva_case(long a, long b) {
    // Use for large numbers or when no special pattern
    int digits_a = count_digits(a);
    int digits_b = count_digits(b);
    return (digits_a > 2 || digits_b > 2);
}

// Enhanced sutra rule table
static SutraRule sutra_rules[] = {
    {
        "Ekadhikena_Purvena",
        is_ekadhikena_case,
        (long (*)(long, long))ekadhikena_purvena,
        HIGH_CONFIDENCE,
        "Perfect square of number ending in 5"
    },
    {
        "Nikhilam",
        is_nikhilam_case,
        nikhilam_mul,
        HIGH_CONFIDENCE,
        "Numbers close to same power of 10"
    },
    {
        "Antyayordasake",
        is_antyayordasake_case,
        (long (*)(long, long))antya_dasake_mul,
        MEDIUM_CONFIDENCE,
        "Same prefix, last digits sum to 10"
    },
    {
        "Urdhva_Tiryagbhyam",
        is_urdhva_case,
        urdhva_mult,
        LOW_CONFIDENCE,
        "General multiplication for larger numbers"
    }
};

#define NUM_RULES (sizeof(sutra_rules) / sizeof(SutraRule))

// Dataset generation structure
static DecisionRecord* decision_dataset = NULL;
static size_t dataset_size = 0;
static size_t dataset_capacity = 0;

// Initialize dataset collection
void init_dataset_collection(size_t initial_capacity) {
    dataset_capacity = initial_capacity;
    decision_dataset = malloc(sizeof(DecisionRecord) * dataset_capacity);
    dataset_size = 0;
}

// Add decision to dataset
static void record_decision(long a, long b, const char* sutra, 
                           double exec_time, int confidence, const char* reason) {
    if (dataset_size >= dataset_capacity) {
        dataset_capacity *= 2;
        decision_dataset = realloc(decision_dataset, 
                                 sizeof(DecisionRecord) * dataset_capacity);
    }
    
    DecisionRecord* record = &decision_dataset[dataset_size++];
    record->operand_a = a;
    record->operand_b = b;
    record->sutra_used = sutra;
    record->execution_time_ms = exec_time;
    record->confidence_score = confidence;
    record->reason = reason;
    
    // TODO: Add actual CPU and memory measurement
    record->cpu_usage_percent = 0.0;
    record->memory_used_bytes = 0;
}

// Enhanced dispatcher with rule-based intelligence
long vedic_multiply_enhanced(long a, long b) {
    clock_t start_time = clock();
    
    // Handle edge cases
    if (a == 0 || b == 0) {
        record_decision(a, b, "Direct", 0.0, HIGH_CONFIDENCE, "Zero operand");
        return 0;
    }
    
    if (a == 1) {
        record_decision(a, b, "Direct", 0.0, HIGH_CONFIDENCE, "Unit operand");
        return b;
    }
    
    if (b == 1) {
        record_decision(a, b, "Direct", 0.0, HIGH_CONFIDENCE, "Unit operand");
        return a;
    }
    
    // Handle negative numbers
    int sign = 1;
    if (a < 0) { a = -a; sign = -sign; }
    if (b < 0) { b = -b; sign = -sign; }
    
    // Apply rule-based selection
    long result = 0;
    const char* selected_sutra = "Standard";
    int best_confidence = 0;
    
    for (size_t i = 0; i < NUM_RULES; i++) {
        if (sutra_rules[i].condition_check(a, b)) {
            if (sutra_rules[i].confidence_score > best_confidence) {
                best_confidence = sutra_rules[i].confidence_score;
                
                // Execute the sutra
                if (strcmp(sutra_rules[i].rule_name, "Ekadhikena_Purvena") == 0) {
                    result = ekadhikena_purvena(a);  // Special case for squaring
                } else {
                    result = sutra_rules[i].sutra_function(a, b);
                }
                
                selected_sutra = sutra_rules[i].rule_name;
            }
        }
    }
    
    // Fallback to standard multiplication
    if (best_confidence == 0) {
        result = a * b;
        selected_sutra = "Standard";
        best_confidence = HIGH_CONFIDENCE;
    }
    
    // Calculate execution time
    clock_t end_time = clock();
    double exec_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    
    // Record decision for dataset
    record_decision(a, b, selected_sutra, exec_time, best_confidence, 
                   "Rule-based selection");
    
    return sign * result;
}

// Export dataset to CSV for analysis
void export_dataset_csv(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    // CSV header
    fprintf(file, "operand_a,operand_b,sutra_used,execution_time_ms,confidence_score,reason\n");
    
    // Data rows
    for (size_t i = 0; i < dataset_size; i++) {
        DecisionRecord* record = &decision_dataset[i];
        fprintf(file, "%ld,%ld,%s,%.6f,%d,%s\n",
                record->operand_a, record->operand_b, record->sutra_used,
                record->execution_time_ms, record->confidence_score, record->reason);
    }
    
    fclose(file);
}

// Generate diverse test dataset
void generate_test_patterns(size_t count) {
    srand(time(NULL));
    
    printf("Generating %zu test patterns...\n", count);
    
    for (size_t i = 0; i < count; i++) {
        long a, b;
        
        // Generate different types of number patterns
        switch (i % 6) {
            case 0: // Numbers ending in 5 (Ekadhikena)
                a = b = (rand() % 20 + 1) * 10 + 5;
                break;
                
            case 1: // Numbers near 100 (Nikhilam)
                a = 85 + rand() % 30;  // 85-115
                b = 85 + rand() % 30;
                break;
                
            case 2: // Numbers near 1000 (Nikhilam)
                a = 950 + rand() % 100; // 950-1050
                b = 950 + rand() % 100;
                break;
                
            case 3: // Antyayordasake pattern
                {
                    int prefix = rand() % 9 + 1;
                    int last_a = rand() % 9 + 1;
                    int last_b = 10 - last_a;
                    a = prefix * 10 + last_a;
                    b = prefix * 10 + last_b;
                }
                break;
                
            case 4: // Large numbers (Urdhva)
                a = 1000 + rand() % 9000;
                b = 1000 + rand() % 9000;
                break;
                
            case 5: // Random numbers
                a = rand() % 1000 + 1;
                b = rand() % 1000 + 1;
                break;
        }
        
        // Test the enhanced dispatcher
        vedic_multiply_enhanced(a, b);
    }
    
    printf("Generated %zu decision records\n", dataset_size);
}

// Analyze dataset patterns
void analyze_dataset() {
    if (dataset_size == 0) return;
    
    // Count sutra usage
    int ekadhikena_count = 0, nikhilam_count = 0, antyayordasake_count = 0;
    int urdhva_count = 0, standard_count = 0;
    double total_time = 0.0;
    
    for (size_t i = 0; i < dataset_size; i++) {
        DecisionRecord* record = &decision_dataset[i];
        total_time += record->execution_time_ms;
        
        if (strcmp(record->sutra_used, "Ekadhikena_Purvena") == 0) ekadhikena_count++;
        else if (strcmp(record->sutra_used, "Nikhilam") == 0) nikhilam_count++;
        else if (strcmp(record->sutra_used, "Antyayordasake") == 0) antyayordasake_count++;
        else if (strcmp(record->sutra_used, "Urdhva_Tiryagbhyam") == 0) urdhva_count++;
        else standard_count++;
    }
    
    printf("\n=== Dataset Analysis ===\n");
    printf("Total records: %zu\n", dataset_size);
    printf("Average execution time: %.6f ms\n", total_time / dataset_size);
    printf("\nSutra Usage Distribution:\n");
    printf("- Ekadhikena Purvena: %d (%.1f%%)\n", ekadhikena_count, 
           100.0 * ekadhikena_count / dataset_size);
    printf("- Nikhilam: %d (%.1f%%)\n", nikhilam_count, 
           100.0 * nikhilam_count / dataset_size);
    printf("- Antyayordasake: %d (%.1f%%)\n", antyayordasake_count, 
           100.0 * antyayordasake_count / dataset_size);
    printf("- Urdhva-Tiryagbhyam: %d (%.1f%%)\n", urdhva_count, 
           100.0 * urdhva_count / dataset_size);
    printf("- Standard: %d (%.1f%%)\n", standard_count, 
           100.0 * standard_count / dataset_size);
}

// Cleanup
void cleanup_dataset() {
    if (decision_dataset) {
        free(decision_dataset);
        decision_dataset = NULL;
        dataset_size = 0;
        dataset_capacity = 0;
    }
}

// Example usage function
void demo_enhanced_dispatcher() {
    printf("=== Enhanced Rule-Based Dispatcher Demo ===\n");
    
    // Initialize dataset collection
    init_dataset_collection(10000);
    
    // Generate test patterns
    generate_test_patterns(1000);
    
    // Analyze results
    analyze_dataset();
    
    // Export for further analysis
    export_dataset_csv("vedic_decisions.csv");
    printf("\nDataset exported to 'vedic_decisions.csv'\n");
    
    // Cleanup
    cleanup_dataset();
}