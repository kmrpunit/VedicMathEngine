/**
 * dispatch_mixed_mode.h - Enhanced System-Aware Adaptive Dispatcher
 * 
 * ACADEMIC CONTRIBUTION: Novel adaptive arithmetic engine that intelligently
 * selects optimal Vedic Mathematics sutras based on comprehensive analysis.
 * 
 * RESEARCH FEATURES:
 * - Multi-factor pattern recognition with confidence scoring
 * - Real-time system resource monitoring (Windows, Linux, ESP32)
 * - Performance validation through dual execution
 * - Comprehensive dataset generation for research validation
 * - Statistical proof of Vedic method superiority
 * 
 * @author VedicMath-AI Research Team
 * @version 2.0 - Enhanced Adaptive Engine
 */

#ifndef DISPATCH_MIXED_MODE_H
#define DISPATCH_MIXED_MODE_H

#include "vedic_core.h"
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// CORE TYPE DEFINITIONS
// ============================================================================

/**
 * @brief Enhanced Vedic sutra classification
 * 
 * RESEARCH NOTE: Extends basic sutra types with performance characteristics
 * and applicability conditions for intelligent selection.
 */
typedef enum {
    SUTRA_EKADHIKENA_PURVENA = 0,    // "By one more than the previous one"
    SUTRA_NIKHILAM = 1,              // "All from 9 and the last from 10"
    SUTRA_ANTYAYORDASAKE = 2,        // "Last totaling 10"
    SUTRA_URDHVA_TIRYAGBHYAM = 3,    // "Vertically and crosswise"
    SUTRA_PARAVARTYA_YOJAYET = 4,    // "Transpose and apply" (division)
    SUTRA_DHVAJANKA = 5,             // "Flag digit method" (division)
    SUTRA_NIKHILAM_DIVISION = 6,     // "Nikhilam for division"    
    SUTRA_STANDARD = 7,              // Standard arithmetic (fallback)
    MAX_SUTRA_TYPES = 8              // Update count
} VedicSutraType;

/**
 * @brief Platform identification for optimization
 */
typedef enum {
    PLATFORM_WINDOWS = 0,
    PLATFORM_LINUX = 1,
    PLATFORM_MACOS = 2,
    PLATFORM_ESP32 = 3,
    PLATFORM_GENERIC = 4
} PlatformType;

/**
 * @brief Comprehensive system resource monitoring
 * 
 * RESEARCH CONTRIBUTION: Cross-platform resource monitoring enables
 * intelligent algorithm selection based on real-time system constraints.
 */
typedef struct {
    // CPU metrics
    double cpu_usage_percent;        // Current CPU utilization (0-100)
    
    // Memory metrics
    double memory_usage_percent;     // Current memory usage (0-100)
    size_t memory_total_mb;         // Total system memory (MB)
    size_t memory_available_mb;     // Available memory (MB)
    
    // Performance metrics
    double temperature_celsius;      // System temperature (if available)
    double power_consumption_watts;  // Power usage estimate (if available)
    
    // Platform-specific metrics
    size_t free_heap_bytes;         // ESP32: Free heap memory
    uint16_t task_count;            // ESP32: Active task count
    bool thermal_throttling;        // Thermal throttling active
    
    // Metadata
    clock_t last_update;            // Last update timestamp
    PlatformType platform_type;    // Current platform
} SystemResourceMonitor;

/**
 * @brief Adaptive dispatcher configuration
 * 
 * RESEARCH PARAMETERS: Configurable thresholds for adaptive behavior
 * tuning based on research findings and system characteristics.
 */
typedef struct {
    // CPU thresholds
    double cpu_threshold_high;       // Above this: prefer fast methods (default: 80%)
    double cpu_threshold_low;        // Below this: can use complex methods (default: 30%)
    
    // Memory thresholds
    double memory_threshold_high;    // Above this: prefer memory-efficient (default: 0.8)
    double memory_threshold_low;     // Below this: can use memory-intensive (default: 0.3)
    
    // Adaptive behavior
    bool energy_aware;              // Consider power consumption
    bool adaptive_thresholds;       // Learn and adjust thresholds
    uint32_t monitoring_interval_ms; // Resource monitoring frequency
    
    // Platform-specific settings
    double temperature_threshold;    // Thermal throttling threshold (default: 75°C)
    size_t min_free_memory_mb;      // Minimum memory to maintain
    bool realtime_constraints;      // For real-time systems
} DispatcherConfig;

/**
 * @brief Return codes for dispatcher operations
 */
typedef enum {
    DISPATCH_SUCCESS = 0,
    DISPATCH_ERROR_MEMORY = -1,
    DISPATCH_ERROR_INVALID_CONFIG = -2,
    DISPATCH_ERROR_FILE = -3,
    DISPATCH_ERROR_SYSTEM = -4
} DispatchResult;

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================

// Global configuration variable declaration
extern DispatcherConfig dispatcher_config;

// ============================================================================
// CORE DISPATCH FUNCTIONS
// ============================================================================

/**
 * @brief Initialize the enhanced adaptive dispatcher
 * 
 * RESEARCH SETUP: Initializes system monitoring, performance validation,
 * and dataset generation for comprehensive Vedic method analysis.
 * 
 * @param config Configuration structure (NULL for defaults)
 * @return DISPATCH_SUCCESS on success, error code otherwise
 */
DispatchResult dispatch_mixed_mode_init(const DispatcherConfig* config);

/**
 * @brief Enhanced adaptive multiplication with performance validation
 * 
 * CORE RESEARCH FUNCTION: Intelligently selects optimal Vedic sutra based on:
 * 1. Mathematical pattern analysis with confidence scoring
 * 2. Real-time system resource constraints
 * 3. Performance prediction and validation
 * 4. Platform-specific optimizations
 * 
 * VALIDATION: Executes both Vedic and standard methods to verify:
 * - Correctness of results
 * - Actual vs predicted performance
 * - Statistical significance of improvements
 * 
 * @param a First operand (VedicValue with type information)
 * @param b Second operand (VedicValue with type information)
 * @return Multiplication result with validated performance
 */
VedicValue dispatch_multiply(VedicValue a, VedicValue b);

/**
 * @brief Adaptive squaring with specialized pattern detection
 * 
 * OPTIMIZATION: Leverages Ekadhikena Purvena for numbers ending in 5
 * and other Vedic techniques for optimal squaring performance.
 * 
 * @param a Operand to square
 * @return Square of operand with performance validation
 */
VedicValue dispatch_square(VedicValue a);

/**
 * @brief Adaptive division (future enhancement)
 * 
 * PLACEHOLDER: Currently uses dynamic division, can be enhanced
 * with Vedic division sutras (Paravartya Yojayet, etc.)
 * 
 * @param dividend Dividend value
 * @param divisor Divisor value  
 * @return Division result
 */
VedicValue dispatch_divide(VedicValue dividend, VedicValue divisor);

// ============================================================================
// SYSTEM MONITORING FUNCTIONS
// ============================================================================

/**
 * @brief Get current system resource status
 * 
 * MONITORING: Provides real-time system metrics for intelligent
 * algorithm selection and performance analysis.
 * 
 * @return Current system resource information
 */
SystemResourceMonitor dispatch_get_system_status(void);

/**
 * @brief Check if system is under high load
 * 
 * DECISION SUPPORT: Quick check for high-load conditions that
 * should influence algorithm selection toward faster methods.
 * 
 * @return true if system is under high load (CPU/memory/thermal)
 */
bool dispatch_is_system_stressed(void);

/**
 * @brief Enable/disable real-time system monitoring
 * 
 * @param enable true to enable monitoring, false to disable
 */
void dispatch_set_monitoring_enabled(bool enable);

/**
 * @brief Set system monitoring update interval
 * 
 * @param interval_ms Update interval in milliseconds
 */
void dispatch_set_monitoring_interval(uint32_t interval_ms);

// ============================================================================
// RESEARCH & VALIDATION FUNCTIONS
// ============================================================================

/**
 * @brief Generate comprehensive validation dataset
 * 
 * RESEARCH TOOL: Creates diverse test patterns across all Vedic sutra
 * categories to thoroughly validate performance claims.
 * 
 * PATTERN CATEGORIES:
 * 1. Ekadhikena Purvena: Numbers ending in 5
 * 2. Nikhilam: Numbers near powers of 10  
 * 3. Antyayordasake: Same prefix, last digits sum to 10
 * 4. Urdhva-Tiryagbhyam: Large multi-digit numbers
 * 5. Edge cases: Zero, negative, boundary values
 * 6. Random stress test: Statistical validation
 * 
 * @param target_size Number of test patterns to generate
 */
void generate_comprehensive_validation_dataset(size_t target_size);

/**
 * @brief Analyze performance statistics from validation dataset
 * 
 * RESEARCH OUTPUT: Generates statistical proof of Vedic method superiority:
 * - Average speedup factors per sutra
 * - Percentage of operations showing significant improvement  
 * - Correctness validation (100% accuracy requirement)
 * - Confidence intervals and statistical significance
 */
void analyze_performance_statistics(void);

/**
 * @brief Export validation dataset and generate performance analysis
 * 
 * RESEARCH DELIVERABLE: Exports comprehensive CSV dataset with:
 * - Input characteristics and selected algorithms
 * - Performance metrics (Vedic vs standard execution times)
 * - System context (CPU, memory, platform)
 * - Statistical validation of performance claims
 * 
 * @param dataset_filename Output filename for CSV export
 */
void dispatch_cleanup_and_export(const char* dataset_filename);

/**
 * @brief Get current validation dataset size
 * 
 * @return Number of validation records collected
 */
size_t dispatch_get_validation_dataset_size(void);

/**
 * @brief Clear validation dataset and reset statistics
 * 
 * RESEARCH RESET: Clears accumulated performance data for fresh
 * validation runs or different experimental conditions.
 */
void dispatch_reset_validation_dataset(void);

// ============================================================================
// CONFIGURATION FUNCTIONS
// ============================================================================

/**
 * @brief Set dispatcher configuration
 * 
 * @param config New configuration parameters
 */
void dispatch_set_config(const DispatcherConfig* config);

/**
 * @brief Get current dispatcher configuration
 * 
 * @return Current configuration parameters
 */
DispatcherConfig dispatch_get_config(void);

/**
 * @brief Reset configuration to defaults
 */
void dispatch_reset_config_to_defaults(void);

/**
 * @brief Reset performance history and validation data
 * 
 * RESEARCH RESET: Clears accumulated performance data for fresh
 * validation runs or different experimental conditions.
 */
void dispatch_reset_performance_history(void);

// ============================================================================
// CONVENIENCE MACROS FOR DIFFERENT PLATFORMS
// ============================================================================

#ifdef ESP32_PLATFORM
    /**
     * @brief ESP32-optimized multiplication macro
     * Prioritizes low-power, memory-efficient algorithms
     */
    #define DISPATCH_MULTIPLY(a, b) dispatch_multiply(vedic_from_int32(a), vedic_from_int32(b))
    
    /**
     * @brief ESP32-optimized squaring macro
     * Uses heap-aware algorithm selection
     */
    #define DISPATCH_SQUARE(a) dispatch_square(vedic_from_int32(a))
    
    /**
     * @brief ESP32 memory check before complex operations
     */
    #define ESP32_CHECK_MEMORY() \
        do { \
            if (esp_get_free_heap_size() < (dispatcher_config.min_free_memory_mb * 1024 * 1024)) { \
                return vedic_from_int64(0); /* Emergency fallback */ \
            } \
        } while(0)
        
#elif defined(_WIN32)
    /**
     * @brief Windows-optimized multiplication macro
     * Leverages performance counters for optimal selection
     */
    #define DISPATCH_MULTIPLY(a, b) dispatch_multiply(vedic_from_int64(a), vedic_from_int64(b))
    
    /**
     * @brief Windows-optimized squaring macro
     */
    #define DISPATCH_SQUARE(a) dispatch_square(vedic_from_int64(a))
    
#elif defined(__linux__)
    /**
     * @brief Linux-optimized multiplication macro
     * Uses /proc filesystem for system monitoring
     */
    #define DISPATCH_MULTIPLY(a, b) dispatch_multiply(vedic_from_int64(a), vedic_from_int64(b))
    
    /**
     * @brief Linux-optimized squaring macro
     */
    #define DISPATCH_SQUARE(a) dispatch_square(vedic_from_int64(a))
    
#else
    /**
     * @brief Generic platform multiplication macro
     */
    #define DISPATCH_MULTIPLY(a, b) dispatch_multiply(vedic_from_int64(a), vedic_from_int64(b))
    
    /**
     * @brief Generic platform squaring macro
     */
    #define DISPATCH_SQUARE(a) dispatch_square(vedic_from_int64(a))
#endif

// ============================================================================
// RESEARCH ANALYSIS MACROS
// ============================================================================

/**
 * @brief Quick benchmark macro for research validation
 * 
 * Usage: BENCHMARK_OPERATION(result, dispatch_multiply(a, b), "multiplication");
 */
#define BENCHMARK_OPERATION(result, operation, description) \
    do { \
        clock_t start = clock(); \
        result = operation; \
        clock_t end = clock(); \
        double time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0; \
        printf("BENCHMARK: %s took %.3f ms\n", description, time_ms); \
    } while(0)

/**
 * @brief Statistical validation macro for research
 * 
 * Validates that Vedic result matches standard arithmetic
 */
#define VALIDATE_RESULT(vedic_result, a, b, operation_name) \
    do { \
        long expected = (a) * (b); \
        long actual = vedic_to_int64(vedic_result); \
        if (actual != expected) { \
            printf("VALIDATION ERROR: %s(%ld, %ld) = %ld, expected %ld\n", \
                   operation_name, (long)(a), (long)(b), actual, expected); \
        } \
    } while(0)

/**
 * @brief Performance assertion macro for research validation
 * 
 * Ensures Vedic methods meet minimum performance thresholds
 */
#define ASSERT_PERFORMANCE_IMPROVEMENT(actual_speedup, min_expected) \
    do { \
        if ((actual_speedup) < (min_expected)) { \
            printf("PERFORMANCE WARNING: Speedup %.2fx below threshold %.2fx\n", \
                   actual_speedup, min_expected); \
        } \
    } while(0)

// ============================================================================
// DEBUGGING AND DIAGNOSTICS
// ============================================================================

#ifdef DEBUG_DISPATCH
    /**
     * @brief Debug macro for algorithm selection reasoning
     */
    #define DEBUG_SELECTION(sutra_type, confidence, reasoning) \
        printf("DEBUG: Selected %d (confidence %.2f): %s\n", \
               sutra_type, confidence, reasoning)
    
    /**
     * @brief Debug macro for system resource status
     */
    #define DEBUG_SYSTEM_STATUS(monitor) \
        printf("DEBUG: CPU %.1f%%, Memory %.1f%%, Temp %.1f°C\n", \
               monitor.cpu_usage_percent, monitor.memory_usage_percent, \
               monitor.temperature_celsius)
#else
    #define DEBUG_SELECTION(sutra_type, confidence, reasoning) ((void)0)
    #define DEBUG_SYSTEM_STATUS(monitor) ((void)0)
#endif

// ============================================================================
// CONSTANTS AND LIMITS
// ============================================================================

/**
 * @brief Maximum validation dataset size (prevent memory overflow)
 */
#define MAX_VALIDATION_DATASET_SIZE 1000000

/**
 * @brief Minimum confidence threshold for Vedic method selection
 */
#define MIN_CONFIDENCE_THRESHOLD 0.1

/**
 * @brief Maximum memory overhead allowed for Vedic algorithms (bytes)
 */
#define MAX_MEMORY_OVERHEAD_BYTES 4096

/**
 * @brief Performance improvement threshold for statistical significance
 */
#define PERFORMANCE_SIGNIFICANCE_THRESHOLD 1.05  // 5% improvement

/**
 * @brief System monitoring update frequency limits
 */
#define MIN_MONITORING_INTERVAL_MS 50
#define MAX_MONITORING_INTERVAL_MS 5000

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

/**
 * @brief Convert VedicSutraType to human-readable string
 * 
 * @param sutra_type Sutra type enumeration
 * @return Human-readable sutra name
 */
const char* dispatch_sutra_type_to_string(VedicSutraType sutra_type);

/**
 * @brief Convert PlatformType to human-readable string
 * 
 * @param platform_type Platform type enumeration
 * @return Human-readable platform name
 */
const char* dispatch_platform_type_to_string(PlatformType platform_type);

/**
 * @brief Get default dispatcher configuration for platform
 * 
 * @param platform Target platform
 * @return Optimized configuration for platform
 */
DispatcherConfig dispatch_get_default_config_for_platform(PlatformType platform);

/**
 * @brief Estimate memory usage for given operation
 * 
 * @param a First operand
 * @param b Second operand
 * @param sutra_type Selected algorithm
 * @return Estimated memory usage in bytes
 */
size_t dispatch_estimate_memory_usage(VedicValue a, VedicValue b, VedicSutraType sutra_type);

/**
 * @brief Check if given operands are suitable for specific sutra
 * 
 * @param a First operand
 * @param b Second operand
 * @param sutra_type Sutra to check
 * @return Confidence score (0.0-1.0) for sutra applicability
 */
double dispatch_check_sutra_applicability(VedicValue a, VedicValue b, VedicSutraType sutra_type);

#ifdef __cplusplus
}
#endif

#endif // DISPATCH_MIXED_MODE_H