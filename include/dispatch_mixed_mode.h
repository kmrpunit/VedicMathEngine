/**
 * dispatch_mixed_mode.h - System-aware dispatcher for runtime optimization
 * 
 * This header defines the interface for intelligent runtime decision-making
 * based on system resources, platform constraints, and operation complexity.
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

// Constants
#define PERF_HISTORY_SIZE 1000
#define MAX_TEMP_CELSIUS 85.0
#define MIN_MEMORY_MB 64

// Return codes
typedef enum {
    DISPATCH_SUCCESS = 0,
    DISPATCH_ERROR_MEMORY = -1,
    DISPATCH_ERROR_INVALID_CONFIG = -2,
    DISPATCH_ERROR_FILE = -3,
    DISPATCH_ERROR_SYSTEM = -4
} DispatchResult;

// Dispatcher configuration
typedef struct {
    double cpu_threshold_high;      // Above this, prefer faster methods
    double cpu_threshold_low;       // Below this, can use complex methods
    double memory_threshold_high;   // Above this, prefer memory-efficient methods
    double memory_threshold_low;    // Below this, can use memory-intensive methods
    bool energy_aware;             // Consider power consumption
    bool adaptive_thresholds;      // Learn and adjust thresholds
    uint32_t monitoring_interval_ms; // How often to check system resources
    
    // Platform-specific settings
    double temperature_threshold;   // For thermal throttling
    size_t min_free_memory_mb;     // Minimum memory to maintain
    bool realtime_constraints;     // For real-time systems
} DispatcherConfig;

// System resource monitoring
typedef struct {
    double cpu_usage_percent;      // Current CPU usage (0-100)
    double memory_usage_percent;   // Current memory usage (0-100)
    size_t memory_total_mb;        // Total system memory
    size_t memory_available_mb;    // Available memory
    double temperature_celsius;    // System temperature (if available)
    double power_consumption_watts; // Power usage (if available)
    clock_t last_update;          // Last update timestamp
    
    // Platform-specific metrics
    uint32_t free_heap_bytes;     // For embedded systems
    uint16_t task_count;          // Number of active tasks
    bool thermal_throttling;      // Thermal throttling active
} SystemResourceMonitor;

// Performance record for adaptive learning
typedef struct {
    VedicMode mode;               // Mode that was used
    double execution_time_ms;     // Execution time
    bool success;                 // Whether operation succeeded
    double cpu_usage;             // CPU usage at time of execution
    double memory_usage;          // Memory usage at time of execution
    clock_t timestamp;            // When the operation occurred
} PerformanceRecord;

// Performance history for adaptive learning
typedef struct {
    PerformanceRecord records[PERF_HISTORY_SIZE];
    size_t current_index;         // Current write position
    size_t count;                 // Number of valid records
    size_t window_size;           // Size of sliding window
} PerformanceHistory;

// Performance statistics
typedef struct {
    uint64_t total_operations;
    double avg_execution_time_ms;
    double mode_usage_percent[4]; // Usage percentage for each mode
    double mode_avg_time_ms[4];   // Average time for each mode
    VedicMode best_mode;          // Currently best performing mode
    double efficiency_score;      // Overall efficiency (ops/ms/MB)
    
    // System impact
    double avg_cpu_impact;        // Average CPU usage during operations
    double avg_memory_impact;     // Average memory usage during operations
} DispatchPerformanceStats;

// Initialization and configuration functions
/**
 * Initialize the mixed mode dispatcher
 * @param config Configuration structure (NULL for defaults)
 * @return DISPATCH_SUCCESS on success, error code otherwise
 */
DispatchResult dispatch_mixed_mode_init(const DispatcherConfig* config);

/**
 * Set dispatcher configuration
 * @param config New configuration
 */
void dispatch_set_config(const DispatcherConfig* config);

/**
 * Get current dispatcher configuration
 * @return Current configuration
 */
DispatcherConfig dispatch_get_config(void);

// Main dispatch functions
/**
 * Execute operation with intelligent mode selection
 * @param a First operand
 * @param b Second operand
 * @param op_type Type of operation
 * @return Result of operation
 */
VedicValue dispatch_mixed_mode_execute(VedicValue a, VedicValue b, VedicOperationType op_type);

/**
 * Execute multiplication with system-aware optimization
 * @param a First operand
 * @param b Second operand
 * @return Result of multiplication
 */
static inline VedicValue dispatch_multiply(VedicValue a, VedicValue b) {
    return dispatch_mixed_mode_execute(a, b, VEDIC_OP_MULTIPLY);
}

/**
 * Execute division with system-aware optimization
 * @param dividend Dividend
 * @param divisor Divisor
 * @return Result of division
 */
static inline VedicValue dispatch_divide(VedicValue dividend, VedicValue divisor) {
    return dispatch_mixed_mode_execute(dividend, divisor, VEDIC_OP_DIVIDE);
}

/**
 * Execute squaring with system-aware optimization
 * @param a Operand to square
 * @return Result of squaring
 */
static inline VedicValue dispatch_square(VedicValue a) {
    return dispatch_mixed_mode_execute(a, vedic_from_int32(0), VEDIC_OP_SQUARE);
}

// System monitoring functions
/**
 * Get current system resource status
 * @return System resource information
 */
SystemResourceMonitor dispatch_get_system_status(void);

/**
 * Check if system is under high load
 * @return true if system is under high load
 */
static inline bool dispatch_is_system_stressed(void) {
    SystemResourceMonitor status = dispatch_get_system_status();
    DispatcherConfig config = dispatch_get_config();
    
    return (status.cpu_usage_percent > config.cpu_threshold_high ||
            status.memory_usage_percent > config.memory_threshold_high * 100 ||
            status.temperature_celsius > config.temperature_threshold);
}

// Performance and analytics functions
/**
 * Get performance statistics
 * @return Performance statistics
 */
DispatchPerformanceStats dispatch_get_performance_stats(void);

/**
 * Export performance data to CSV
 * @param filename Output filename
 * @return DISPATCH_SUCCESS on success, error code otherwise
 */
DispatchResult dispatch_export_performance_data(const char* filename);

/**
 * Reset performance history
 */
void dispatch_reset_performance_history(void);

// Internal helper functions
/**
 * Update system resource information
 */
static void update_system_resources(void);

/**
 * Determine optimal execution mode based on system state
 * @param a First operand
 * @param b Second operand
 * @param op_type Operation type
 * @return Recommended execution mode
 */
static VedicMode determine_optimal_mode(VedicValue a, VedicValue b, VedicOperationType op_type);

/**
 * Calculate operation complexity score (0.0 - 1.0)
 * @param a First operand
 * @param b Second operand
 * @param op_type Operation type
 * @return Complexity score
 */
static double calculate_operation_complexity(VedicValue a, VedicValue b, VedicOperationType op_type);

/**
 * Record performance data for adaptive learning
 * @param mode Mode that was used
 * @param execution_time_ms Execution time
 * @param success Whether operation succeeded
 */
static void record_performance(VedicMode mode, double execution_time_ms, bool success);

/**
 * Update adaptive thresholds based on performance history
 */
static void update_adaptive_thresholds(void);

// Convenience macros for different platforms
#ifdef ESP32_PLATFORM
    #define DISPATCH_MULTIPLY(a, b) dispatch_multiply(vedic_from_int32(a), vedic_from_int32(b))
    #define DISPATCH_SQUARE(a) dispatch_square(vedic_from_int32(a))
    #define DISPATCH_DIVIDE(a, b) dispatch_divide(vedic_from_int32(a), vedic_from_int32(b))
#else
    #define DISPATCH_MULTIPLY(a, b) dispatch_multiply(vedic_from_int64(a), vedic_from_int64(b))
    #define DISPATCH_SQUARE(a) dispatch_square(vedic_from_int64(a))
    #define DISPATCH_DIVIDE(a, b) dispatch_divide(vedic_from_int64(a), vedic_from_int64(b))
#endif

// Default configuration values
#define DEFAULT_CPU_THRESHOLD_HIGH 80.0
#define DEFAULT_CPU_THRESHOLD_LOW 30.0
#define DEFAULT_MEMORY_THRESHOLD_HIGH 0.8
#define DEFAULT_MEMORY_THRESHOLD_LOW 0.3
#define DEFAULT_MONITORING_INTERVAL_MS 100
#define DEFAULT_TEMPERATURE_THRESHOLD 75.0

#ifdef __cplusplus
}
#endif

#endif /* DISPATCH_MIXED_MODE_H */
