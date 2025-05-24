/**
 * vedic_core.h - Unified wrapper layer for all Vedic sutras
 * 
 * This header provides a clean, unified interface to all Vedic mathematics
 * operations with comprehensive logging and performance monitoring.
 */

#ifndef VEDIC_CORE_H
#define VEDIC_CORE_H

#include "vedicmath_types.h"
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// Configuration constants
#define VEDIC_DEFAULT_LOG_SIZE 10000
#define VEDIC_MAX_SUTRA_NAME 64

// Return codes
typedef enum {
    VEDIC_SUCCESS = 0,
    VEDIC_ERROR_MEMORY = -1,
    VEDIC_ERROR_INVALID_INPUT = -2,
    VEDIC_ERROR_NO_DATA = -3,
    VEDIC_ERROR_FILE = -4
} VedicResult;

// Operation modes
typedef enum {
    VEDIC_MODE_STANDARD = 0,    // Use only standard arithmetic
    VEDIC_MODE_DYNAMIC = 1,     // Use dynamic type-aware operations
    VEDIC_MODE_OPTIMIZED = 2,   // Use performance-optimized operations
    VEDIC_MODE_ADAPTIVE = 3,    // Intelligently select best method
    VEDIC_MODE_SPECIFIC = 4     // Use specific sutra (for direct calls)
} VedicMode;

// Optimization levels
typedef enum {
    VEDIC_OPT_SIZE = 0,        // Optimize for code size
    VEDIC_OPT_SPEED = 1,       // Optimize for execution speed
    VEDIC_OPT_BALANCED = 2,    // Balance size and speed
    VEDIC_OPT_POWER = 3        // Optimize for power consumption
} VedicOptLevel;

// Platform types
typedef enum {
    VEDIC_PLATFORM_DESKTOP = 0,
    VEDIC_PLATFORM_ESP32 = 1,
    VEDIC_PLATFORM_CLOUD = 2,
    VEDIC_PLATFORM_MOBILE = 3
} VedicPlatform;

// Operation types for logging
typedef enum {
    VEDIC_OP_TYPE_ADD = VEDIC_OP_ADD,
    VEDIC_OP_TYPE_SUBTRACT = VEDIC_OP_SUBTRACT,
    VEDIC_OP_TYPE_MULTIPLY = VEDIC_OP_MULTIPLY,
    VEDIC_OP_TYPE_DIVIDE = VEDIC_OP_DIVIDE,
    VEDIC_OP_TYPE_SQUARE = VEDIC_OP_SQUARE,
    VEDIC_OP_TYPE_MODULO = VEDIC_OP_MODULO,
    VEDIC_OP_TYPE_POWER = VEDIC_OP_POWER
} VedicOperationType;

// Core configuration structure
typedef struct {
    VedicMode mode;
    VedicOptLevel optimization_level;
    bool logging_enabled;
    VedicPlatform platform;
    bool resource_monitoring;
    size_t max_log_entries;
} VedicCoreConfig;

// Operation log entry for dataset generation
typedef struct {
    time_t timestamp;
    VedicOperationType operation_type;
    VedicValue operand_a;
    VedicValue operand_b;
    VedicValue result;
    char sutra_used[VEDIC_MAX_SUTRA_NAME];
    double execution_time_ms;
    VedicMode mode_used;
    VedicPlatform platform;
    
    // Extended metadata (for research)
    uint32_t cpu_cycles;        // If available
    size_t memory_used;         // If available
    float confidence_score;     // Algorithm confidence (0-1)
} VedicOperationLog;

// Performance counters
typedef struct {
    uint64_t total_operations;
    uint64_t vedic_operations;
    double total_execution_time_ms;
    double vedic_time_ms;
    double avg_execution_time_ms;
    double avg_vedic_time_ms;
    double vedic_efficiency;    // Ratio of vedic ops to total ops
    
    // Resource usage (if monitoring enabled)
    size_t peak_memory_bytes;
    double avg_cpu_usage;
} VedicPerformanceCounters;

// Core engine functions
/**
 * Initialize the Vedic core engine
 * @param config Configuration structure (NULL for defaults)
 * @return VEDIC_SUCCESS on success, error code otherwise
 */
VedicResult vedic_core_init(const VedicCoreConfig* config);

/**
 * Cleanup the Vedic core engine
 */
void vedic_core_cleanup(void);

/**
 * Set runtime configuration
 * @param config New configuration
 */
void vedic_core_set_config(const VedicCoreConfig* config);

/**
 * Get current configuration
 * @return Current configuration
 */
VedicCoreConfig vedic_core_get_config(void);

// Unified operation interfaces
/**
 * Unified multiplication with automatic method selection
 * @param a First operand
 * @param b Second operand
 * @return Result of multiplication
 */
VedicValue multiply_vedic_unified(VedicValue a, VedicValue b);

/**
 * Unified squaring with automatic method selection  
 * @param a Operand to square
 * @return Result of squaring
 */
VedicValue square_vedic_unified(VedicValue a);

/**
 * Unified division with automatic method selection
 * @param dividend Dividend
 * @param divisor Divisor
 * @return Result of division (quotient)
 */
VedicValue divide_vedic_unified(VedicValue dividend, VedicValue divisor);

// Specific sutra interfaces
/**
 * Multiplication using Urdhva-Tiryagbhyam sutra
 * @param a First operand
 * @param b Second operand
 * @return Result of multiplication
 */
VedicValue multiply_urdhva(VedicValue a, VedicValue b);

/**
 * Squaring using Ekadhikena Purvena sutra (numbers ending in 5)
 * @param a Operand to square
 * @return Result of squaring
 */
VedicValue square_ekadhikena(VedicValue a);

/**
 * Multiplication using Nikhilam sutra (numbers near powers of 10)
 * @param a First operand
 * @param b Second operand
 * @return Result of multiplication
 */
VedicValue multiply_nikhilam(VedicValue a, VedicValue b);

/**
 * Division using Paravartya Yojayet sutra
 * @param dividend Dividend
 * @param divisor Divisor
 * @return Result of division (quotient)
 */
VedicValue divide_paravartya(VedicValue dividend, VedicValue divisor);

/**
 * Multiplication using Antyayordasake sutra (last digits sum to 10)
 * @param a First operand
 * @param b Second operand
 * @return Result of multiplication
 */
VedicValue multiply_antyayordasake(VedicValue a, VedicValue b);

// Dataset and performance functions
/**
 * Export operation dataset to CSV file
 * @param filename Output filename
 * @return VEDIC_SUCCESS on success, error code otherwise
 */
VedicResult vedic_core_export_dataset(const char* filename);

/**
 * Get performance statistics
 * @return Performance counters structure
 */
VedicPerformanceCounters vedic_core_get_performance(void);

/**
 * Clear performance counters and operation log
 */
void vedic_core_reset_stats(void);

/**
 * Get current operation log
 * @param count Pointer to store number of log entries
 * @return Pointer to log array (read-only)
 */
const VedicOperationLog* vedic_core_get_log(size_t* count);

// Internal helper functions (for adaptive mode)
/**
 * Intelligent method selection for multiplication
 * @param a First operand
 * @param b Second operand
 * @param sutra_used Output parameter for selected sutra name
 * @return Result of multiplication
 */
static VedicValue select_best_multiplication_method(VedicValue a, VedicValue b, const char** sutra_used);
static VedicValue select_best_division_method(VedicValue dividend, VedicValue divisor, const char** sutra_used);

// Convenience macros for common operations
#define VEDIC_MULTIPLY(a, b) multiply_vedic_unified(vedic_from_int64(a), vedic_from_int64(b))
#define VEDIC_SQUARE(a) square_vedic_unified(vedic_from_int64(a))
#define VEDIC_DIVIDE(a, b) divide_vedic_unified(vedic_from_int64(a), vedic_from_int64(b))

// Platform-specific optimizations
#ifdef ESP32_PLATFORM
    #define VEDIC_INLINE __attribute__((always_inline)) inline
    #define VEDIC_FAST_MATH __attribute__((optimize("fast-math")))
#else
    #define VEDIC_INLINE inline
    #define VEDIC_FAST_MATH
#endif

#ifdef __cplusplus
}
#endif

#endif /* VEDIC_CORE_H */