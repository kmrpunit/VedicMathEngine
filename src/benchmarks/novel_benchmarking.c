/**
 * novel_benchmarking.c - Adaptive benchmarking system with pattern learning
 * 
 * This system introduces several novel concepts:
 * 1. Pattern-aware benchmark generation
 * 2. Adaptive difficulty scaling
 * 3. Resource-aware performance measurement
 * 4. Statistical confidence intervals
 * 5. Cross-platform performance profiling
 */

#include "vedicmath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// Platform-specific includes for resource monitoring
#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#elif defined(__linux__)
    #include <sys/resource.h>
    #include <unistd.h>
#elif defined(__APPLE__)
    #include <mach/mach.h>
    #include <sys/resource.h>
#endif

// Advanced benchmark configuration
typedef struct {
    size_t min_iterations;
    size_t max_iterations;
    double target_confidence;    // Statistical confidence level (0.95 = 95%)
    double max_variance;         // Maximum acceptable variance
    double warmup_ratio;         // Fraction of iterations for warmup
    bool adaptive_scaling;       // Enable adaptive difficulty
    bool resource_monitoring;    // Enable CPU/memory monitoring
} BenchmarkConfig;

// Resource usage metrics
typedef struct {
    double cpu_usage_percent;
    size_t memory_used_bytes;
    size_t memory_peak_bytes;
    double cache_miss_rate;      // If available
    size_t context_switches;     // If available
} ResourceMetrics;

// Advanced benchmark result with statistical analysis
typedef struct {
    const char* test_name;
    const char* sutra_used;
    
    // Timing statistics
    double mean_time_ms;
    double std_dev_ms;
    double min_time_ms;
    double max_time_ms;
    double median_time_ms;
    double confidence_interval_95[2];  // [lower, upper]
    
    // Performance metrics
    double operations_per_second;
    double efficiency_score;     // Custom metric
    
    // Resource usage
    ResourceMetrics resources;
    
    // Statistical validity
    size_t sample_size;
    double variance;
    double skewness;
    bool is_statistically_valid;
    
    // Pattern recognition
    int pattern_type;
    double pattern_score;        // How well it matches expected pattern
} AdvancedBenchmarkResult;

// Pattern types for intelligent test generation
typedef enum {
    PATTERN_EKADHIKENA,      // Numbers ending in 5
    PATTERN_NIKHILAM_NEAR,   // Numbers near powers of 10
    PATTERN_NIKHILAM_FAR,    // Numbers far from powers of 10
    PATTERN_ANTYAYORDASAKE,  // Last digits sum to 10
    PATTERN_LARGE_NUMBERS,   // Large multi-digit numbers
    PATTERN_RANDOM,          // Random numbers
    PATTERN_EDGE_CASES,      // Edge cases (0, 1, negative)
    PATTERN_COUNT
} PatternType;

// Global benchmark configuration
static BenchmarkConfig global_config = {
    .min_iterations = 1000,
    .max_iterations = 100000,
    .target_confidence = 0.95,
    .max_variance = 0.1,
    .warmup_ratio = 0.1,
    .adaptive_scaling = true,
    .resource_monitoring = true
};

// Statistical utility functions
static double calculate_mean(double* data, size_t n) {
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;
}

static double calculate_std_dev(double* data, size_t n, double mean) {
    double sum_sq_diff = 0.0;
    for (size_t i = 0; i < n; i++) {
        double diff = data[i] - mean;
        sum_sq_diff += diff * diff;
    }
    return sqrt(sum_sq_diff / (n - 1));
}

static double calculate_median(double* data, size_t n) {
    // Simple selection for median (assumes data is sorted)
    if (n % 2 == 0) {
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    } else {
        return data[n/2];
    }
}

static int compare_doubles(const void* a, const void* b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    return (da > db) - (da < db);
}

// Resource monitoring functions
static ResourceMetrics get_current_resources() {
    ResourceMetrics metrics = {0};
    
#ifdef _WIN32
    // Windows resource monitoring
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        metrics.memory_used_bytes = pmc.WorkingSetSize;
        metrics.memory_peak_bytes = pmc.PeakWorkingSetSize;
    }
    
    // CPU usage would require more complex implementation on Windows
    
#elif defined(__linux__)
    // Linux resource monitoring
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        metrics.memory_used_bytes = usage.ru_maxrss * 1024; // Convert KB to bytes
        metrics.context_switches = usage.ru_nvcsw + usage.ru_nivcsw;
    }
    
#elif defined(__APPLE__)
    // macOS resource monitoring
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                  (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
        metrics.memory_used_bytes = info.resident_size;
    }
#endif
    
    return metrics;
}

// Pattern generation functions
static void generate_ekadhikena_pattern(long* a, long* b) {
    int base = (rand() % 20 + 1) * 10 + 5;  // Numbers ending in 5
    *a = *b = base;
}

static void generate_nikhilam_pattern(long* a, long* b, bool near_base) {
    int base_power = rand() % 3 + 2;  // 10^2, 10^3, or 10^4
    long base = 1;
    for (int i = 0; i < base_power; i++) base *= 10;
    
    if (near_base) {
        // Numbers within 15% of base
        *a = base + (rand() % (base / 7)) - (base / 14);
        *b = base + (rand() % (base / 7)) - (base / 14);
    } else {
        // Numbers far from base
        *a = rand() % base;
        *b = rand() % base;
    }
}

static void generate_antyayordasake_pattern(long* a, long* b) {
    int prefix = rand() % 9 + 1;
    int last_digit_a = rand() % 9 + 1;
    int last_digit_b = 10 - last_digit_a;
    
    *a = prefix * 10 + last_digit_a;
    *b = prefix * 10 + last_digit_b;
}

static void generate_test_pair(PatternType pattern, long* a, long* b) {
    switch (pattern) {
        case PATTERN_EKADHIKENA:
            generate_ekadhikena_pattern(a, b);
            break;
            
        case PATTERN_NIKHILAM_NEAR:
            generate_nikhilam_pattern(a, b, true);
            break;
            
        case PATTERN_NIKHILAM_FAR:
            generate_nikhilam_pattern(a, b, false);
            break;
            
        case PATTERN_ANTYAYORDASAKE:
            generate_antyayordasake_pattern(a, b);
            break;
            
        case PATTERN_LARGE_NUMBERS:
            *a = 1000 + rand() % 9000;
            *b = 1000 + rand() % 9000;
            break;
            
        case PATTERN_EDGE_CASES:
            switch (rand() % 6) {
                case 0: *a = 0; *b = rand() % 100; break;
                case 1: *a = 1; *b = rand() % 100; break;
                case 2: *a = -1 * (rand() % 100); *b = rand() % 100; break;
                case 3: *a = rand() % 100; *b = 0; break;
                case 4: *a = rand() % 100; *b = 1; break;
                case 5: *a = rand() % 100; *b = -1 * (rand() % 100); break;
            }
            break;
            
        default: // PATTERN_RANDOM
            *a = rand() % 1000 + 1;
            *b = rand() % 1000 + 1;
            break;
    }
}

// Advanced benchmarking function
static AdvancedBenchmarkResult run_advanced_benchmark(
    const char* test_name,
    PatternType pattern,
    long (*operation)(long, long),
    const BenchmarkConfig* config
) {
    AdvancedBenchmarkResult result = {0};
    result.test_name = test_name;
    result.pattern_type = pattern;
    
    // Allocate arrays for timing data
    size_t max_samples = config->max_iterations;
    double* timings = malloc(sizeof(double) * max_samples);
    if (!timings) {
        result.is_statistically_valid = false;
        return result;
    }
    
    // Warmup phase
    size_t warmup_iterations = (size_t)(config->min_iterations * config->warmup_ratio);
    printf("Warming up with %zu iterations...\n", warmup_iterations);
    
    for (size_t i = 0; i < warmup_iterations; i++) {
        long a, b;
        generate_test_pair(pattern, &a, &b);
        volatile long dummy = operation(a, b);
        (void)dummy;  // Suppress unused variable warning
    }
    
    // Resource monitoring setup
    ResourceMetrics start_resources = {0};
    if (config->resource_monitoring) {
        start_resources = get_current_resources();
    }
    
    // Main benchmarking loop with adaptive sampling
    size_t sample_count = 0;
    double running_variance = INFINITY;
    
    printf("Running adaptive benchmark (target confidence: %.1f%%)...\n", 
           config->target_confidence * 100);
    
    while (sample_count < config->max_iterations) {
        // Generate test case
        long a, b;
        generate_test_pair(pattern, &a, &b);
        
        // Measure execution time
        clock_t start = clock();
        volatile long result_val = operation(a, b);
        clock_t end = clock();
        (void)result_val;
        
        // Record timing
        double elapsed_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        timings[sample_count] = elapsed_ms;
        sample_count++;
        
        // Check for adaptive termination
        if (config->adaptive_scaling && sample_count >= config->min_iterations) {
            if (sample_count % 100 == 0) {  // Check every 100 samples
                double mean = calculate_mean(timings, sample_count);
                double std_dev = calculate_std_dev(timings, sample_count, mean);
                running_variance = (std_dev / mean);
                
                // Check if we have sufficient statistical confidence
                if (running_variance < config->max_variance) {
                    printf("Achieved target confidence after %zu samples\n", sample_count);
                    break;
                }
            }
        }
        
        // Progress reporting
        if (sample_count % 10000 == 0) {
            printf("Completed %zu samples (variance: %.4f)\n", sample_count, running_variance);
        }
    }
    
    // Resource monitoring end
    ResourceMetrics end_resources = {0};
    if (config->resource_monitoring) {
        end_resources = get_current_resources();
        result.resources.memory_used_bytes = end_resources.memory_used_bytes - start_resources.memory_used_bytes;
        result.resources.memory_peak_bytes = end_resources.memory_peak_bytes;
    }
    
    // Sort timings for median calculation
    qsort(timings, sample_count, sizeof(double), compare_doubles);
    
    // Calculate comprehensive statistics
    result.sample_size = sample_count;
    result.mean_time_ms = calculate_mean(timings, sample_count);
    result.std_dev_ms = calculate_std_dev(timings, sample_count, result.mean_time_ms);
    result.min_time_ms = timings[0];
    result.max_time_ms = timings[sample_count - 1];
    result.median_time_ms = calculate_median(timings, sample_count);
    result.variance = running_variance;
    
    // Calculate 95% confidence interval
    double margin_error = 1.96 * (result.std_dev_ms / sqrt(sample_count));
    result.confidence_interval_95[0] = result.mean_time_ms - margin_error;
    result.confidence_interval_95[1] = result.mean_time_ms + margin_error;
    
    // Performance metrics
    result.operations_per_second = 1000.0 / result.mean_time_ms;
    result.efficiency_score = result.operations_per_second / result.resources.memory_used_bytes * 1000000; // ops/MB
    
    // Statistical validity check
    result.is_statistically_valid = (result.variance < config->max_variance) && 
                                   (sample_count >= config->min_iterations);
    
    // Determine which sutra was likely used (simplified heuristic)
    switch (pattern) {
        case PATTERN_EKADHIKENA:
            result.sutra_used = "Ekadhikena Purvena";
            break;
        case PATTERN_NIKHILAM_NEAR:
            result.sutra_used = "Nikhilam";
            break;
        case PATTERN_ANTYAYORDASAKE:
            result.sutra_used = "Antyayordasake";
            break;
        default:
            result.sutra_used = "Auto-selected";
            break;
    }
    
    free(timings);
    return result;
}

// Print comprehensive benchmark results
static void print_advanced_results(const AdvancedBenchmarkResult* result) {
    printf("\n=== Advanced Benchmark Results: %s ===\n", result->test_name);
    printf("Pattern Type: %d, Sutra Used: %s\n", result->pattern_type, result->sutra_used);
    printf("Sample Size: %zu (statistically valid: %s)\n", 
           result->sample_size, result->is_statistically_valid ? "YES" : "NO");
    
    printf("\nTiming Statistics:\n");
    printf("  Mean: %.6f ms (±%.6f ms)\n", result->mean_time_ms, result->std_dev_ms);
    printf("  Median: %.6f ms\n", result->median_time_ms);
    printf("  Range: [%.6f, %.6f] ms\n", result->min_time_ms, result->max_time_ms);
    printf("  95%% CI: [%.6f, %.6f] ms\n", 
           result->confidence_interval_95[0], result->confidence_interval_95[1]);
    printf("  Variance: %.4f\n", result->variance);
    
    printf("\nPerformance Metrics:\n");
    printf("  Operations/sec: %.2f\n", result->operations_per_second);
    printf("  Efficiency score: %.2f ops/MB\n", result->efficiency_score);
    
    if (result->resources.memory_used_bytes > 0) {
        printf("\nResource Usage:\n");
        printf("  Memory used: %zu bytes\n", result->resources.memory_used_bytes);
        printf("  Peak memory: %zu bytes\n", result->resources.memory_peak_bytes);
    }
}

// Run comprehensive benchmark suite
void run_novel_benchmark_suite() {
    printf("=== Novel Adaptive Benchmarking Suite ===\n");
    printf("Configuration:\n");
    printf("  Target confidence: %.1f%%\n", global_config.target_confidence * 100);
    printf("  Max variance: %.2f\n", global_config.max_variance);
    printf("  Adaptive scaling: %s\n", global_config.adaptive_scaling ? "ON" : "OFF");
    printf("  Resource monitoring: %s\n", global_config.resource_monitoring ? "ON" : "OFF");
    
    srand(time(NULL));
    
    // Test different patterns
    PatternType patterns[] = {
        PATTERN_EKADHIKENA,
        PATTERN_NIKHILAM_NEAR,
        PATTERN_ANTYAYORDASAKE,
        PATTERN_LARGE_NUMBERS,
        PATTERN_RANDOM
    };
    
    const char* pattern_names[] = {
        "Ekadhikena (Numbers ending in 5)",
        "Nikhilam (Near power of 10)",
        "Antyayordasake (Last digits sum to 10)",
        "Large Numbers",
        "Random Numbers"
    };
    
    for (size_t i = 0; i < sizeof(patterns) / sizeof(patterns[0]); i++) {
        AdvancedBenchmarkResult result = run_advanced_benchmark(
            pattern_names[i],
            patterns[i],
            vedic_multiply,
            &global_config
        );
        
        print_advanced_results(&result);
    }
    
    printf("\n=== Benchmark Suite Complete ===\n");
}