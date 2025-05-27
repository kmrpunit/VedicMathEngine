/**
 * unified_adaptive_dispatcher.h - Complete Header for DAY 1
 * 
 * SAVE AS: include/unified_adaptive_dispatcher.h
 * 
 * Single Point of Intelligence for VedicMath-AI
 * Consolidates all dispatch mechanisms into one intelligent system
 */

#ifndef UNIFIED_ADAPTIVE_DISPATCHER_H
#define UNIFIED_ADAPTIVE_DISPATCHER_H

#include "vedicmath_types.h"
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// ENUMERATIONS AND CONSTANTS
// ============================================================================

/**
 * @brief Vedic sutra types for intelligent selection
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
    MAX_SUTRA_TYPES = 8
} VedicSutraType;

/**
 * @brief Adaptive dispatch modes - configurable intelligence levels
 */
typedef enum {
    DISPATCH_MODE_STANDARD = 0,      // Traditional arithmetic only
    DISPATCH_MODE_PATTERN_AWARE = 1, // Pattern detection, no system awareness
    DISPATCH_MODE_SYSTEM_AWARE = 2,  // Pattern + system resource awareness
    DISPATCH_MODE_FULL_ADAPTIVE = 3, // Pattern + system + learning (DEFAULT)
    DISPATCH_MODE_RESEARCH = 4       // Full logging for academic analysis
} UnifiedDispatchMode;

/**
 * @brief Operation categories for unified handling
 */
typedef enum {
    OPERATION_ARITHMETIC = 0,        // Basic arithmetic operations
    OPERATION_MATRIX = 1,           // Matrix operations (Day 2 target)
    OPERATION_DIVISION = 2,         // Enhanced division operations
    OPERATION_EXPRESSION = 3,       // Complex expression evaluation
    OPERATION_BATCH = 4            // Batch/parallel operations
} OperationCategory;

// ============================================================================
// CONFIGURATION STRUCTURES
// ============================================================================

/**
 * @brief Comprehensive configuration for adaptive behavior
 */
typedef struct {
    // Core adaptivity settings
    UnifiedDispatchMode mode;
    bool enable_learning;           // Learn from performance history
    bool enable_system_monitoring;  // Monitor CPU/memory/platform
    bool enable_dataset_logging;    // Generate research dataset
    
    // Performance thresholds
    double min_speedup_threshold;   // Minimum improvement to use Vedic (default: 1.1)
    double confidence_threshold;    // Minimum pattern confidence (default: 0.3)
    
    // System resource thresholds
    double cpu_threshold_high;      // CPU usage above which prefer fast methods
    double memory_threshold;        // Memory constraint for algorithm selection
    double energy_efficiency_weight; // Weight for energy-aware selection
    
    // Learning parameters
    size_t learning_window_size;    // Number of operations to consider
    double learning_rate;           // How quickly to adapt (0.0-1.0)
    bool enable_predictive_caching; // Cache optimal choices for patterns
    
    // Research/logging configuration
    bool export_decision_reasoning; // Include human-readable explanations
    bool log_system_context;       // Include CPU/memory/platform in logs
    bool validate_all_operations;  // Run both Vedic and standard for comparison
    const char* dataset_export_path; // Where to save research data
    
    // Platform optimizations
    bool optimize_for_platform;    // Enable platform-specific optimizations
    bool enable_parallel_batch;    // Use OpenMP for batch operations
    size_t max_memory_usage_mb;    // Memory budget for operations
} UnifiedDispatchConfig;

/**
 * @brief Comprehensive result with full academic metadata
 */
typedef struct {
    // Core result
    VedicValue result;
    
    // Decision metadata (for academic analysis)
    const char* selected_algorithm;
    const char* sutra_name_sanskrit;  // Original Sanskrit name
    double pattern_confidence;
    double predicted_speedup;
    double actual_speedup;
    const char* decision_reasoning;   // Human-readable explanation
    
    // Performance metrics
    double execution_time_ms;
    double standard_execution_time_ms;
    size_t memory_used_bytes;
    double cpu_usage_during_operation;
    
    // System context
    uint64_t operation_id;           // Unique identifier
    time_t timestamp;
    const char* platform_info;
    OperationCategory operation_type;
    
    // Quality assurance
    bool correctness_verified;
    double precision_error;
    bool performance_expectation_met;
    
    // Research metadata
    bool contributed_to_learning;
    bool added_to_dataset;
    size_t total_operations_count;   // Running total for research
} UnifiedDispatchResult;

/**
 * @brief Learning system statistics
 */
typedef struct {
    size_t total_operations;
    double average_speedup_achieved;
    double pattern_recognition_accuracy;
    size_t vedic_methods_used;
    size_t standard_fallbacks;
    double learning_effectiveness_score; // 0.0-1.0
    const char* most_effective_sutra;
    double energy_efficiency_improvement;
} LearningStatistics;

/**
 * @brief Matrix operation parameters (Day 2)
 */
typedef struct {
    size_t rows_a, cols_a, rows_b, cols_b;
    const VedicValue* matrix_a;
    const VedicValue* matrix_b;
    VedicValue* result_matrix;
} MatrixOperationParams;

/**
 * @brief Adaptation recommendations
 */
typedef struct {
    const char* recommendation_text;
    UnifiedDispatchConfig suggested_config;
    double expected_improvement_percent;
    const char* reasoning;
} AdaptationRecommendation;

/**
 * @brief Decision tree analysis for academic research
 */
typedef struct {
    const char* decision_tree_json;    // JSON format decision tree
    double pattern_scores[10];         // Scores for each pattern type
    double system_influence_factors[5]; // CPU, memory, etc. influence
    const char* winning_reasoning;     // Why this algorithm won
    const char* rejected_alternatives; // What was considered but rejected
} DecisionTreeAnalysis;

// ============================================================================
// CORE INTERFACE FUNCTIONS
// ============================================================================

/**
 * @brief Initialize the unified adaptive dispatcher
 * 
 * This replaces ALL individual dispatcher initializations.
 * Sets up pattern recognition, system monitoring, learning algorithms,
 * and dataset generation systems.
 * 
 * @param config Configuration (NULL for intelligent defaults)
 * @return 0 on success, negative on error
 */
int unified_dispatch_init(const UnifiedDispatchConfig* config);

/**
 * @brief THE UNIFIED OPERATION INTERFACE
 * 
 * Single function that handles ALL mathematical operations
 * with full adaptivity and intelligence.
 * 
 * @param operation_type Type of operation to perform
 * @param operands Array of operands (flexible for different operations)
 * @param operand_count Number of operands
 * @param operation_params Additional parameters (e.g., matrix dimensions)
 * @return Comprehensive result with full metadata
 */
UnifiedDispatchResult unified_dispatch_execute(
    OperationCategory operation_type,
    const VedicValue* operands,
    size_t operand_count,
    const void* operation_params
);

// ============================================================================
// CONVENIENT WRAPPER FUNCTIONS
// ============================================================================

/**
 * @brief Enhanced multiplication with unified intelligence
 */
UnifiedDispatchResult unified_multiply(VedicValue a, VedicValue b);

/**
 * @brief Enhanced division with unified intelligence
 */
UnifiedDispatchResult unified_divide(VedicValue dividend, VedicValue divisor);

/**
 * @brief Matrix operations (Day 2 implementation)
 */
UnifiedDispatchResult unified_matrix_multiply(const MatrixOperationParams* params);

/**
 * @brief Expression evaluation with unified intelligence
 */
UnifiedDispatchResult unified_evaluate_expression(const char* expression);

// ============================================================================
// LEARNING AND ADAPTATION INTERFACE
// ============================================================================

/**
 * @brief Get current learning statistics
 */
LearningStatistics unified_dispatch_get_learning_stats(void);

/**
 * @brief Export comprehensive research dataset
 * 
 * ACADEMIC OUTPUT: Complete dataset for Phase 2 and paper writing
 */
int unified_dispatch_export_research_dataset(const char* filename);

/**
 * @brief Get real-time adaptation recommendations
 */
AdaptationRecommendation unified_dispatch_get_recommendations(void);

/**
 * @brief Get detailed decision tree for last operation
 */
DecisionTreeAnalysis unified_dispatch_get_last_decision_tree(void);

// ============================================================================
// CONFIGURATION AND RUNTIME CONTROL
// ============================================================================

/**
 * @brief Get optimized configuration for different scenarios
 */
UnifiedDispatchConfig unified_dispatch_get_preset_config(const char* use_case);
// Use cases: "research", "performance", "energy_efficient", "embedded", "desktop"

/**
 * @brief Runtime configuration updates
 */
void unified_dispatch_update_config(const UnifiedDispatchConfig* new_config);
void unified_dispatch_set_mode(UnifiedDispatchMode mode);
void unified_dispatch_enable_learning(bool enable);

/**
 * @brief Enable/disable specific research modes
 */
void unified_dispatch_set_research_mode(bool enable_full_logging, 
                                       bool enable_dual_execution,
                                       bool enable_decision_tree_export);

/**
 * @brief Runtime monitoring controls
 */
void unified_dispatch_set_monitoring_enabled(bool enable);
void unified_dispatch_set_monitoring_interval(uint32_t interval_ms);

// ============================================================================
// CLEANUP AND FINALIZATION
// ============================================================================

/**
 * @brief Comprehensive cleanup and final dataset export
 */
void unified_dispatch_finalize(const char* final_dataset_filename);

/**
 * @brief Reset performance history and validation data
 */
void unified_dispatch_reset_performance_history(void);

/**
 * @brief Get current validation dataset size
 */
size_t unified_dispatch_get_validation_dataset_size(void);

/**
 * @brief Clear validation dataset and reset statistics
 */
void unified_dispatch_reset_validation_dataset(void);

// ============================================================================
// CONVENIENCE MACROS FOR DIFFERENT PLATFORMS
// ============================================================================

#ifdef ESP32_PLATFORM
    /**
     * @brief ESP32-optimized multiplication macro
     */
    #define DISPATCH_MULTIPLY(a, b) unified_multiply(vedic_from_int32(a), vedic_from_int32(b))
    #define DISPATCH_SQUARE(a) unified_multiply(vedic_from_int32(a), vedic_from_int32(a))
    
    /**
     * @brief ESP32 memory check before complex operations
     */
    #define ESP32_CHECK_MEMORY() \
        do { \
            extern UnifiedDispatchConfig global_config; \
            if (esp_get_free_heap_size() < (global_config.max_memory_usage_mb * 1024 * 1024)) { \
                return vedic_from_int64(0); \
            } \
        } while(0)
        
#elif defined(_WIN32)
    /**
     * @brief Windows-optimized macros with performance counters
     */
    #define DISPATCH_MULTIPLY(a, b) unified_multiply(vedic_from_int64(a), vedic_from_int64(b))
    #define DISPATCH_SQUARE(a) unified_multiply(vedic_from_int64(a), vedic_from_int64(a))
    
#elif defined(__linux__)
    /**
     * @brief Linux-optimized macros with /proc monitoring
     */
    #define DISPATCH_MULTIPLY(a, b) unified_multiply(vedic_from_int64(a), vedic_from_int64(b))
    #define DISPATCH_SQUARE(a) unified_multiply(vedic_from_int64(a), vedic_from_int64(a))
    
#else
    /**
     * @brief Generic platform macros
     */
    #define DISPATCH_MULTIPLY(a, b) unified_multiply(vedic_from_int64(a), vedic_from_int64(b))
    #define DISPATCH_SQUARE(a) unified_multiply(vedic_from_int64(a), vedic_from_int64(a))
#endif

// ============================================================================
// RESEARCH ANALYSIS MACROS
// ============================================================================

/**
 * @brief Quick benchmark macro for research validation
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
 */
#define VALIDATE_RESULT(vedic_result, a, b, operation_name) \
    do { \
        long expected = (a) * (b); \
        long actual = vedic_to_int64(vedic_result.result); \
        if (actual != expected) { \
            printf("VALIDATION ERROR: %s(%ld, %ld) = %ld, expected %ld\n", \
                   operation_name, (long)(a), (long)(b), actual, expected); \
        } \
    } while(0)

/**
 * @brief Performance assertion macro for research validation
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
    #define DEBUG_SYSTEM_STATUS(cpu, memory, temp) \
        printf("DEBUG: CPU %.1f%%, Memory %.1f%%, Temp %.1f°C\n", \
               cpu, memory, temp)
#else
    #define DEBUG_SELECTION(sutra_type, confidence, reasoning) ((void)0)
    #define DEBUG_SYSTEM_STATUS(cpu, memory, temp) ((void)0)
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
 */
const char* unified_dispatch_sutra_type_to_string(VedicSutraType sutra_type);

/**
 * @brief Get default dispatcher configuration for platform
 */
UnifiedDispatchConfig unified_dispatch_get_default_config_for_platform(const char* platform);

/**
 * @brief Estimate memory usage for given operation
 */
size_t unified_dispatch_estimate_memory_usage(VedicValue a, VedicValue b, VedicSutraType sutra_type);

/**
 * @brief Check if given operands are suitable for specific sutra
 */
double unified_dispatch_check_sutra_applicability(VedicValue a, VedicValue b, VedicSutraType sutra_type);

// ============================================================================
// INTEGRATION WITH EXISTING APIs
// ============================================================================

/**
 * @brief Drop-in replacements for existing dispatch functions
 * 
 * These maintain compatibility with your existing code while adding
 * unified intelligence. Simply replace your existing calls:
 * 
 * OLD: dispatch_multiply(a, b)
 * NEW: unified_dispatch_multiply_compatible(a, b)
 */
VedicValue unified_dispatch_multiply_compatible(VedicValue a, VedicValue b);
VedicValue unified_dispatch_divide_compatible(VedicValue dividend, VedicValue divisor);
VedicValue unified_dispatch_square_compatible(VedicValue a);

/**
 * @brief Get last operation metadata for debugging
 */
UnifiedDispatchResult unified_dispatch_get_last_operation_result(void);

/**
 * @brief Performance comparison with existing dispatchers
 */
typedef struct {
    const char* dispatcher_name;
    double average_execution_time_ms;
    double average_speedup;
    size_t operations_count;
    double reliability_score;
} DispatcherComparison;

DispatcherComparison* unified_dispatch_compare_with_existing(size_t* comparison_count);

#ifdef __cplusplus
}
#endif

#endif // UNIFIED_ADAPTIVE_DISPATCHER_H

// ============================================================================
// USAGE EXAMPLES AND INTEGRATION GUIDE
// ============================================================================

/*
// EXAMPLE 1: Drop-in replacement for existing dispatch calls
// OLD CODE:
//   VedicValue result = dispatch_multiply(a, b);
// NEW CODE:
   VedicValue result_value = unified_multiply(a, b).result;

// EXAMPLE 2: Full academic analysis mode
UnifiedDispatchConfig research_config = unified_dispatch_get_preset_config("research");
unified_dispatch_init(&research_config);

UnifiedDispatchResult result = unified_multiply(vedic_from_int32(102), vedic_from_int32(32));
printf("Used: %s (Sanskrit: %s)\n", result.selected_algorithm, result.sutra_name_sanskrit);
printf("Confidence: %.2f, Speedup: %.2fx\n", result.pattern_confidence, result.actual_speedup);
printf("Reasoning: %s\n", result.decision_reasoning);

// EXAMPLE 3: Performance monitoring mode
unified_dispatch_set_mode(DISPATCH_MODE_FULL_ADAPTIVE);
for (int i = 0; i < 1000; i++) {
    UnifiedDispatchResult r = unified_multiply(vedic_from_int32(rand() % 1000), 
                                              vedic_from_int32(rand() % 1000));
    // System learns and adapts automatically
}

LearningStatistics stats = unified_dispatch_get_learning_stats();
printf("System achieved %.1f%% average improvement over %zu operations\n",
       (stats.average_speedup_achieved - 1.0) * 100, stats.total_operations);

// EXAMPLE 4: Export comprehensive dataset for academic paper
unified_dispatch_export_research_dataset("vedic_performance_analysis.csv");

// EXAMPLE 5: Matrix multiplication (Day 2)
MatrixOperationParams matrix_params = {
    .rows_a = 100, .cols_a = 100, .rows_b = 100, .cols_b = 100,
    .matrix_a = matrix_a_data, .matrix_b = matrix_b_data, .result_matrix = result_data
};
UnifiedDispatchResult matrix_result = unified_matrix_multiply(&matrix_params);

// EXAMPLE 6: Embedded/resource-constrained mode
UnifiedDispatchConfig embedded_config = unified_dispatch_get_preset_config("embedded");
embedded_config.max_memory_usage_mb = 32;  // Very limited memory
embedded_config.confidence_threshold = 0.7; // Higher safety threshold
unified_dispatch_init(&embedded_config);

// EXAMPLE 7: Integration with existing VedicMath-AI codebase
// Replace your existing dispatch calls gradually:
#define dispatch_multiply(a, b) unified_dispatch_multiply_compatible(a, b)
#define dispatch_divide(a, b) unified_dispatch_divide_compatible(a, b)
// This gives you unified intelligence with zero code changes!

// EXAMPLE 8: Real-time adaptation recommendations
AdaptationRecommendation rec = unified_dispatch_get_recommendations();
printf("Recommendation: %s\n", rec.recommendation_text);
printf("Expected improvement: %.1f%%\n", rec.expected_improvement_percent);
if (rec.expected_improvement_percent > 10.0) {
    unified_dispatch_update_config(&rec.suggested_config);
}
*/