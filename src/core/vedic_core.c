/**
 * vedic_core.c - Unified wrapper layer for all Vedic sutras
 * 
 * This module provides a clean, unified interface to all Vedic mathematics
 * operations, abstracting the complexity of individual sutra implementations.
 */

#include "vedic_core.h"
#include "vedicmath.h"
#include "vedicmath_types.h"
#include "vedicmath_dynamic.h"
#include "vedicmath_optimized.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global configuration for the core engine
static VedicCoreConfig core_config = {
    .mode = VEDIC_MODE_ADAPTIVE,
    .optimization_level = VEDIC_OPT_BALANCED,
    .logging_enabled = true,
    .platform = VEDIC_PLATFORM_DESKTOP,
    .resource_monitoring = true
};

// Operation logging for dataset generation
static VedicOperationLog* operation_log = NULL;
static size_t log_capacity = 0;
static size_t log_count = 0;

// Performance counters
static VedicPerformanceCounters perf_counters = {0};

/**
 * Initialize the Vedic core engine
 */
VedicResult vedic_core_init(const VedicCoreConfig* config) {
    if (config) {
        core_config = *config;
    }
    
    // Initialize logging system
    if (core_config.logging_enabled) {
        log_capacity = VEDIC_DEFAULT_LOG_SIZE;
        operation_log = malloc(sizeof(VedicOperationLog) * log_capacity);
        if (!operation_log) {
            return VEDIC_ERROR_MEMORY;
        }
        log_count = 0;
    }
    
    // Initialize optimized engine if needed
    if (core_config.mode == VEDIC_MODE_OPTIMIZED || 
        core_config.mode == VEDIC_MODE_ADAPTIVE) {
        vedic_optimized_init();
    }
    
    // Reset performance counters
    memset(&perf_counters, 0, sizeof(perf_counters));
    
    return VEDIC_SUCCESS;
}

/**
 * Cleanup the Vedic core engine
 */
void vedic_core_cleanup(void) {
    if (operation_log) {
        free(operation_log);
        operation_log = NULL;
        log_capacity = 0;
        log_count = 0;
    }
    
    if (core_config.mode == VEDIC_MODE_OPTIMIZED || 
        core_config.mode == VEDIC_MODE_ADAPTIVE) {
        vedic_optimized_cleanup();
    }
}

/**
 * Log an operation for dataset generation
 */
static void log_operation(VedicOperationType op_type, VedicValue a, VedicValue b, 
                         VedicValue result, const char* sutra_used, 
                         double execution_time_ms, VedicMode mode_used) {
    if (!core_config.logging_enabled || !operation_log) return;
    
    // Expand log if needed
    if (log_count >= log_capacity) {
        log_capacity *= 2;
        operation_log = realloc(operation_log, sizeof(VedicOperationLog) * log_capacity);
        if (!operation_log) return;
    }
    
    // Record the operation
    VedicOperationLog* entry = &operation_log[log_count++];
    entry->timestamp = time(NULL);
    entry->operation_type = op_type;
    entry->operand_a = a;
    entry->operand_b = b;
    entry->result = result;
    entry->execution_time_ms = execution_time_ms;
    entry->mode_used = mode_used;
    entry->platform = core_config.platform;
    
    // Copy sutra name (safely)
    strncpy(entry->sutra_used, sutra_used, sizeof(entry->sutra_used) - 1);
    entry->sutra_used[sizeof(entry->sutra_used) - 1] = '\0';
    
    // Update performance counters
    perf_counters.total_operations++;
    perf_counters.total_execution_time_ms += execution_time_ms;
    
    if (strcmp(sutra_used, "Standard") != 0) {
        perf_counters.vedic_operations++;
        perf_counters.vedic_time_ms += execution_time_ms;
    }
}

/**
 * Unified multiplication interface
 */
VedicValue multiply_vedic_unified(VedicValue a, VedicValue b) {
    clock_t start_time = clock();
    VedicValue result;
    const char* sutra_used = "Unknown";
    VedicMode mode_used = core_config.mode;
    
    switch (core_config.mode) {
        case VEDIC_MODE_STANDARD:
            // Use only standard multiplication
            result.type = vedic_result_type(a.type, b.type);
            if (result.type == VEDIC_INT32) {
                result.value.i32 = vedic_to_int32(a) * vedic_to_int32(b);
            } else if (result.type == VEDIC_INT64) {
                result.value.i64 = vedic_to_int64(a) * vedic_to_int64(b);
            } else if (result.type == VEDIC_FLOAT) {
                result.value.f32 = vedic_to_float(a) * vedic_to_float(b);
            } else {
                result.value.f64 = vedic_to_double(a) * vedic_to_double(b);
            }
            sutra_used = "Standard";
            break;
            
        case VEDIC_MODE_DYNAMIC:
            result = vedic_dynamic_multiply(a, b);
            sutra_used = "Dynamic";
            break;
            
        case VEDIC_MODE_OPTIMIZED:
            result = vedic_optimized_multiply(a, b);
            sutra_used = "Optimized";
            break;
            
        case VEDIC_MODE_ADAPTIVE:
        default:
            // Use intelligent selection based on input patterns
            result = select_best_multiplication_method(a, b, &sutra_used);
            break;
    }
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
    
    // Log the operation
    log_operation(VEDIC_OP_MULTIPLY, a, b, result, sutra_used, execution_time, mode_used);
    
    return result;
}

/**
 * Intelligent method selection for adaptive mode
 */
static VedicValue select_best_multiplication_method(VedicValue a, VedicValue b, const char** sutra_used) {
    // Convert to appropriate types for pattern analysis
    if (a.type <= VEDIC_INT64 && b.type <= VEDIC_INT64) {
        long a_long = vedic_to_int64(a);
        long b_long = vedic_to_int64(b);
        
        // Check for Ekadhikena Purvena (squaring numbers ending in 5)
        if (a_long == b_long && a_long % 10 == 5 && a_long > 0) {
            *sutra_used = "Ekadhikena_Purvena";
            long result_long = ekadhikena_purvena(a_long);
            return vedic_from_int64(result_long);
        }
        
        // Check for Nikhilam (numbers near powers of 10)
        long base_a = nearest_power_of_10(a_long);
        long base_b = nearest_power_of_10(b_long);
        if (base_a == base_b && is_close_to_base(a_long, base_a) && is_close_to_base(b_long, base_b)) {
            *sutra_used = "Nikhilam";
            long result_long = nikhilam_mul(a_long, b_long);
            return vedic_from_int64(result_long);
        }
        
        // Check for Antyayordasake (last digits sum to 10)
        if (last_digits_sum_to_10(a_long, b_long) && same_prefix(a_long, b_long)) {
            *sutra_used = "Antyayordasake";
            int result_int = antya_dasake_mul((int)a_long, (int)b_long);
            return vedic_from_int32(result_int);
        }
        
        // Check for large numbers (use Urdhva-Tiryagbhyam)
        if (count_digits(a_long) > 2 || count_digits(b_long) > 2) {
            *sutra_used = "Urdhva_Tiryagbhyam";
            long result_long = urdhva_mult(a_long, b_long);
            return vedic_from_int64(result_long);
        }
        
        // Default to standard multiplication
        *sutra_used = "Standard";
        return vedic_from_int64(a_long * b_long);
    } else {
        // For floating point, use optimized version
        *sutra_used = "Optimized_Float";
        return vedic_optimized_multiply(a, b);
    }
}

/**
 * Unified squaring interface
 */
VedicValue square_vedic_unified(VedicValue a) {
    return multiply_vedic_unified(a, a);
}

/**
 * Unified division interface
 */
VedicValue divide_vedic_unified(VedicValue dividend, VedicValue divisor) {
    clock_t start_time = clock();
    VedicValue result;
    const char* sutra_used = "Unknown";
    VedicMode mode_used = core_config.mode;
    
    // Check for division by zero
    bool is_zero_divisor = false;
    switch (divisor.type) {
        case VEDIC_INT32: is_zero_divisor = (divisor.value.i32 == 0); break;
        case VEDIC_INT64: is_zero_divisor = (divisor.value.i64 == 0); break;
        case VEDIC_FLOAT: is_zero_divisor = (divisor.value.f32 == 0.0f); break;
        case VEDIC_DOUBLE: is_zero_divisor = (divisor.value.f64 == 0.0); break;
        default: is_zero_divisor = true; break;
    }
    
    if (is_zero_divisor) {
        // Return infinity with appropriate sign for division by zero
        result.type = VEDIC_DOUBLE;
        double dividend_val = vedic_to_double(dividend);
        result.value.f64 = (dividend_val < 0) ? -INFINITY : INFINITY;
        sutra_used = "Error_Handling";
        
        clock_t end_time = clock();
        double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
        log_operation(VEDIC_OP_DIVIDE, dividend, divisor, result, sutra_used, execution_time, mode_used);
        return result;
    }
    
    switch (core_config.mode) {
        case VEDIC_MODE_STANDARD:
            // Use standard division
            result.type = vedic_result_type(dividend.type, divisor.type);
            if (result.type == VEDIC_INT32) {
                int32_t div = vedic_to_int32(dividend);
                int32_t dis = vedic_to_int32(divisor);
                result.value.i32 = div / dis;
            } else if (result.type == VEDIC_INT64) {
                int64_t div = vedic_to_int64(dividend);
                int64_t dis = vedic_to_int64(divisor);
                result.value.i64 = div / dis;
            } else if (result.type == VEDIC_FLOAT) {
                result.value.f32 = vedic_to_float(dividend) / vedic_to_float(divisor);
            } else {
                result.value.f64 = vedic_to_double(dividend) / vedic_to_double(divisor);
            }
            sutra_used = "Standard";
            break;
            
        case VEDIC_MODE_DYNAMIC:
            result = vedic_dynamic_divide(dividend, divisor);
            sutra_used = "Dynamic";
            break;
            
        case VEDIC_MODE_OPTIMIZED:
            result = vedic_optimized_divide(dividend, divisor);
            sutra_used = "Optimized";
            break;
            
        case VEDIC_MODE_ADAPTIVE:
        default:
            // Use intelligent selection for division
            result = select_best_division_method(dividend, divisor, &sutra_used);
            break;
    }
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
    
    // Log the operation
    log_operation(VEDIC_OP_DIVIDE, dividend, divisor, result, sutra_used, execution_time, mode_used);
    
    return result;
}

/**
 * Intelligent method selection for division (add to vedic_core.c)
 */
static VedicValue select_best_division_method(VedicValue dividend, VedicValue divisor, const char** sutra_used) {
    // Convert to appropriate types for pattern analysis
    if (dividend.type <= VEDIC_INT64 && divisor.type <= VEDIC_INT64) {
        long dividend_long = vedic_to_int64(dividend);
        long divisor_long = vedic_to_int64(divisor);
        
        // Check for simple divisors (powers of 2, 5, 10)
        if (divisor_long > 0 && (divisor_long & (divisor_long - 1)) == 0) {
            // Power of 2 - use bit shifting equivalent
            *sutra_used = "Binary_Division";
            long result_long = dividend_long / divisor_long;
            return vedic_from_int64(result_long);
        }
        
        // Check for divisor near a power of 10 (good for Paravartya Yojayet)
        long base = nearest_power_of_10(divisor_long);
        if (is_close_to_base(divisor_long, base) && count_digits(divisor_long) <= 2) {
            *sutra_used = "Paravartya_Yojayet";
            long remainder;
            long quotient = paravartya_divide(dividend_long, divisor_long, &remainder);
            return vedic_from_int64(quotient);
        }
        
        // For single digit divisors, use direct division
        if (divisor_long < 10 && divisor_long > 0) {
            *sutra_used = "Direct_Division";
            long result_long = dividend_long / divisor_long;
            return vedic_from_int64(result_long);
        }
        
        // Default to standard division
        *sutra_used = "Standard";
        long result_long = dividend_long / divisor_long;
        return vedic_from_int64(result_long);
    } else {
        // For floating point, use optimized version
        *sutra_used = "Optimized_Float";
        return vedic_optimized_divide(dividend, divisor);
    }
}

/**
 * Specific sutra interfaces
 */
VedicValue multiply_urdhva(VedicValue a, VedicValue b) {
    clock_t start_time = clock();
    
    long a_long = vedic_to_int64(a);
    long b_long = vedic_to_int64(b);
    long result_long = urdhva_mult(a_long, b_long);
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
    
    VedicValue result = vedic_from_int64(result_long);
    log_operation(VEDIC_OP_MULTIPLY, a, b, result, "Urdhva_Tiryagbhyam", execution_time, VEDIC_MODE_SPECIFIC);
    
    return result;
}

VedicValue square_ekadhikena(VedicValue a) {
    clock_t start_time = clock();
    
    long a_long = vedic_to_int64(a);
    long result_long = ekadhikena_purvena(a_long);
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
    
    VedicValue result = vedic_from_int64(result_long);
    log_operation(VEDIC_OP_SQUARE, a, vedic_from_int32(0), result, "Ekadhikena_Purvena", execution_time, VEDIC_MODE_SPECIFIC);
    
    return result;
}

VedicValue divide_paravartya(VedicValue dividend, VedicValue divisor) {
    clock_t start_time = clock();
    
    long dividend_long = vedic_to_int64(dividend);
    long divisor_long = vedic_to_int64(divisor);
    long remainder;
    long quotient_long = paravartya_divide(dividend_long, divisor_long, &remainder);
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
    
    VedicValue result = vedic_from_int64(quotient_long);
    log_operation(VEDIC_OP_DIVIDE, dividend, divisor, result, "Paravartya_Yojayet", execution_time, VEDIC_MODE_SPECIFIC);
    
    return result;
}

/**
 * Export dataset to CSV
 */
VedicResult vedic_core_export_dataset(const char* filename) {
    if (!operation_log || log_count == 0) {
        return VEDIC_ERROR_NO_DATA;
    }
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        return VEDIC_ERROR_FILE;
    }
    
    // Write CSV header
    fprintf(file, "timestamp,operation_type,operand_a_type,operand_a_value,operand_b_type,operand_b_value,");
    fprintf(file, "result_type,result_value,sutra_used,execution_time_ms,mode_used,platform\n");
    
    // Write data rows
    for (size_t i = 0; i < log_count; i++) {
        VedicOperationLog* entry = &operation_log[i];
        
        fprintf(file, "%ld,%d,%d,", entry->timestamp, entry->operation_type, entry->operand_a.type);
        
        // Write operand A value based on type
        switch (entry->operand_a.type) {
            case VEDIC_INT32: fprintf(file, "%d,", entry->operand_a.value.i32); break;
            case VEDIC_INT64: fprintf(file, "%lld,", (long long)entry->operand_a.value.i64); break;
            case VEDIC_FLOAT: fprintf(file, "%.6f,", entry->operand_a.value.f32); break;
            case VEDIC_DOUBLE: fprintf(file, "%.12f,", entry->operand_a.value.f64); break;
            default: fprintf(file, "0,"); break;
        }
        
        fprintf(file, "%d,", entry->operand_b.type);
        
        // Write operand B value based on type
        switch (entry->operand_b.type) {
            case VEDIC_INT32: fprintf(file, "%d,", entry->operand_b.value.i32); break;
            case VEDIC_INT64: fprintf(file, "%lld,", (long long)entry->operand_b.value.i64); break;
            case VEDIC_FLOAT: fprintf(file, "%.6f,", entry->operand_b.value.f32); break;
            case VEDIC_DOUBLE: fprintf(file, "%.12f,", entry->operand_b.value.f64); break;
            default: fprintf(file, "0,"); break;
        }
        
        fprintf(file, "%d,", entry->result.type);
        
        // Write result value based on type
        switch (entry->result.type) {
            case VEDIC_INT32: fprintf(file, "%d,", entry->result.value.i32); break;
            case VEDIC_INT64: fprintf(file, "%lld,", (long long)entry->result.value.i64); break;
            case VEDIC_FLOAT: fprintf(file, "%.6f,", entry->result.value.f32); break;
            case VEDIC_DOUBLE: fprintf(file, "%.12f,", entry->result.value.f64); break;
            default: fprintf(file, "0,"); break;
        }
        
        fprintf(file, "%s,%.6f,%d,%d\n", 
                entry->sutra_used, entry->execution_time_ms, 
                entry->mode_used, entry->platform);
    }
    
    fclose(file);
    return VEDIC_SUCCESS;
}

/**
 * Get performance statistics
 */
VedicPerformanceCounters vedic_core_get_performance(void) {
    // Calculate derived metrics
    if (perf_counters.total_operations > 0) {
        perf_counters.avg_execution_time_ms = perf_counters.total_execution_time_ms / perf_counters.total_operations;
    }
    
    if (perf_counters.vedic_operations > 0) {
        perf_counters.avg_vedic_time_ms = perf_counters.vedic_time_ms / perf_counters.vedic_operations;
        perf_counters.vedic_efficiency = (double)perf_counters.vedic_operations / perf_counters.total_operations;
    }
    
    return perf_counters;
}

/**
 * Set configuration
 */
void vedic_core_set_config(const VedicCoreConfig* config) {
    if (config) {
        core_config = *config;
    }
}

/**
 * Get current configuration
 */
VedicCoreConfig vedic_core_get_config(void) {
    return core_config;
}
