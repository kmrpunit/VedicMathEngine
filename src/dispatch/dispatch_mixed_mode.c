/**
 * dispatch_mixed_mode.c - System-aware dispatcher for runtime optimization
 * 
 * This module implements intelligent runtime decision-making based on:
 * - Current system resource usage (CPU, memory)
 * - Platform constraints (ESP32 vs desktop vs cloud)
 * - Operation complexity and input patterns
 * - Energy consumption considerations
 */

#include "dispatch_mixed_mode.h"
#include "vedic_core.h"
#include "vedicmath.h"
#include "vedicmath_optimized.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vedicmath_dynamic.h"

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#elif defined(__linux__)
    #include <sys/sysinfo.h>
    #include <unistd.h>
#elif defined(ESP32_PLATFORM)
    #include "freertos/FreeRTOS.h"
    #include "esp_system.h"
#endif

// Global system monitor
static SystemResourceMonitor system_monitor = {0};
static DispatcherConfig dispatcher_config = {
    .cpu_threshold_high = 80.0,
    .cpu_threshold_low = 30.0,
    .memory_threshold_high = 0.8,
    .memory_threshold_low = 0.3,
    .energy_aware = true,
    .adaptive_thresholds = true,
    .monitoring_interval_ms = 100
};

// Performance history for adaptive thresholds
static PerformanceHistory perf_history = {0};

/**
 * Initialize the mixed mode dispatcher
 */
DispatchResult dispatch_mixed_mode_init(const DispatcherConfig* config) {
    if (config) {
        dispatcher_config = *config;
    }
    
    // Initialize system monitoring
    update_system_resources();
    
    // Initialize performance history
    memset(&perf_history, 0, sizeof(perf_history));
    perf_history.window_size = PERF_HISTORY_SIZE;
    
    return DISPATCH_SUCCESS;
}

/**
 * Update system resource information
 */
static void update_system_resources(void) {
    clock_t current_time = clock();
    
#ifdef _WIN32
    // Windows system monitoring
    MEMORYSTATUSEX mem_status;
    mem_status.dwLength = sizeof(mem_status);
    if (GlobalMemoryStatusEx(&mem_status)) {
        system_monitor.memory_total_mb = mem_status.ullTotalPhys / (1024 * 1024);
        system_monitor.memory_available_mb = mem_status.ullAvailPhys / (1024 * 1024);
        system_monitor.memory_usage_percent = 
            (double)(mem_status.ullTotalPhys - mem_status.ullAvailPhys) / mem_status.ullTotalPhys * 100.0;
    }
    
    // CPU usage would require more complex implementation
    system_monitor.cpu_usage_percent = 50.0; // Placeholder
    
#elif defined(__linux__)
    // Linux system monitoring
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        system_monitor.memory_total_mb = info.totalram / (1024 * 1024);
        system_monitor.memory_available_mb = info.freeram / (1024 * 1024);
        system_monitor.memory_usage_percent = 
            (double)(info.totalram - info.freeram) / info.totalram * 100.0;
    }
    
    // Read CPU usage from /proc/loadavg
    FILE* loadavg = fopen("/proc/loadavg", "r");
    if (loadavg) {
        double load1, load5, load15;
        if (fscanf(loadavg, "%lf %lf %lf", &load1, &load5, &load15) == 3) {
            system_monitor.cpu_usage_percent = load1 * 100.0 / sysconf(_SC_NPROCESSORS_ONLN);
        }
        fclose(loadavg);
    }
    
#elif defined(ESP32_PLATFORM)
    // ESP32 system monitoring
    system_monitor.memory_total_mb = CONFIG_ESP32_SPIRAM_SIZE / (1024 * 1024);
    system_monitor.memory_available_mb = esp_get_free_heap_size() / (1024 * 1024);
    system_monitor.memory_usage_percent = 
        (1.0 - (double)esp_get_free_heap_size() / (CONFIG_ESP32_SPIRAM_SIZE)) * 100.0;
    
    // ESP32 doesn't have traditional CPU usage, use task count as proxy
    system_monitor.cpu_usage_percent = 30.0; // Placeholder
    system_monitor.temperature_celsius = 45.0; // Typical ESP32 temperature
    
#else
    // Generic fallback
    system_monitor.memory_total_mb = 4096; // Assume 4GB
    system_monitor.memory_available_mb = 2048;
    system_monitor.memory_usage_percent = 50.0;
    system_monitor.cpu_usage_percent = 50.0;
#endif
    
    system_monitor.last_update = current_time;
}

/**
 * Determine optimal execution mode based on system state
 */
static VedicMode determine_optimal_mode(VedicValue a, VedicValue b, VedicOperationType op_type) {
    // Update system resources if needed
    clock_t current_time = clock();
    if ((current_time - system_monitor.last_update) > 
        (dispatcher_config.monitoring_interval_ms * CLOCKS_PER_SEC / 1000)) {
        update_system_resources();
    }
    
    // Calculate operation complexity score
    double complexity_score = calculate_operation_complexity(a, b, op_type);
    
    // Decision matrix based on system state
    if (system_monitor.cpu_usage_percent > dispatcher_config.cpu_threshold_high ||
        system_monitor.memory_usage_percent > dispatcher_config.memory_threshold_high * 100) {
        
        // High resource usage - prefer simpler, faster methods
        if (complexity_score < 0.3) {
            return VEDIC_MODE_STANDARD; // Simplest for trivial operations
        } else {
            return VEDIC_MODE_OPTIMIZED; // Fast execution for complex operations
        }
        
    } else if (system_monitor.cpu_usage_percent < dispatcher_config.cpu_threshold_low &&
               system_monitor.memory_usage_percent < dispatcher_config.memory_threshold_low * 100) {
        
        // Low resource usage - can afford more sophisticated methods
        return VEDIC_MODE_ADAPTIVE; // Best algorithm selection
        
    } else {
        // Medium resource usage - balanced approach
        if (complexity_score > 0.7) {
            return VEDIC_MODE_DYNAMIC; // Type-aware with good performance
        } else {
            return VEDIC_MODE_OPTIMIZED; // Fast execution
        }
    }
}

/**
 * Calculate operation complexity score (0.0 - 1.0)
 */
static double calculate_operation_complexity(VedicValue a, VedicValue b, VedicOperationType op_type) {
    double complexity = 0.0;
    
    // Base complexity by operation type
    switch (op_type) {
        case VEDIC_OP_ADD:
        case VEDIC_OP_SUBTRACT:
            complexity = 0.1;
            break;
        case VEDIC_OP_MULTIPLY:
            complexity = 0.5;
            break;
        case VEDIC_OP_DIVIDE:
            complexity = 0.7;
            break;
        case VEDIC_OP_POWER:
            complexity = 0.9;
            break;
        default:
            complexity = 0.5;
            break;
    }
    
    // Adjust based on operand size
    if (a.type == VEDIC_INT64 || b.type == VEDIC_INT64) {
        complexity += 0.2;
    }
    if (a.type == VEDIC_DOUBLE || b.type == VEDIC_DOUBLE) {
        complexity += 0.1;
    }
    
    // Adjust based on value magnitude
    double max_val = fmax(fabs(vedic_to_double(a)), fabs(vedic_to_double(b)));
    if (max_val > 1e6) complexity += 0.2;
    else if (max_val > 1e3) complexity += 0.1;
    
    return fmin(complexity, 1.0);
}

/**
 * Record performance data for adaptive learning
 */
static void record_performance(VedicMode mode, double execution_time_ms, bool success) {
    PerformanceRecord* record = &perf_history.records[perf_history.current_index];
    
    record->mode = mode;
    record->execution_time_ms = execution_time_ms;
    record->success = success;
    record->cpu_usage = system_monitor.cpu_usage_percent;
    record->memory_usage = system_monitor.memory_usage_percent;
    record->timestamp = clock();
    
    perf_history.current_index = (perf_history.current_index + 1) % perf_history.window_size;
    if (perf_history.count < perf_history.window_size) {
        perf_history.count++;
    }
}

/**
 * Update adaptive thresholds based on performance history
 */
static void update_adaptive_thresholds(void) {
    if (!dispatcher_config.adaptive_thresholds || perf_history.count < 10) {
        return;
    }
    
    // Analyze performance patterns
    double total_time[4] = {0}; // For each VedicMode
    int counts[4] = {0};
    
    for (size_t i = 0; i < perf_history.count; i++) {
        PerformanceRecord* record = &perf_history.records[i];
        if (record->success && record->mode < 4) {
            total_time[record->mode] += record->execution_time_ms;
            counts[record->mode]++;
        }
    }
    
    // Find best performing mode
    double best_avg_time = INFINITY;
    VedicMode best_mode = VEDIC_MODE_STANDARD;
    
    for (int i = 0; i < 4; i++) {
        if (counts[i] > 0) {
            double avg_time = total_time[i] / counts[i];
            if (avg_time < best_avg_time) {
                best_avg_time = avg_time;
                best_mode = (VedicMode)i;
            }
        }
    }
    
    // Adjust thresholds to favor best performing mode
    if (best_mode == VEDIC_MODE_OPTIMIZED) {
        dispatcher_config.cpu_threshold_high *= 0.95; // Lower threshold
    } else if (best_mode == VEDIC_MODE_ADAPTIVE) {
        dispatcher_config.cpu_threshold_low *= 1.05; // Raise threshold
    }
}

/**
 * Main mixed mode dispatch function
 */
VedicValue dispatch_mixed_mode_execute(VedicValue a, VedicValue b, VedicOperationType op_type) {
    clock_t start_time = clock();
    
    // Determine optimal mode
    VedicMode selected_mode = determine_optimal_mode(a, b, op_type);
    
    // Execute operation based on selected mode
    VedicValue result;
    bool success = true;
    
    switch (op_type) {
        case VEDIC_OP_MULTIPLY:
            // Set mode in core and execute
            {
                VedicCoreConfig config = vedic_core_get_config();
                config.mode = selected_mode;
                vedic_core_set_config(&config);
                
                result = multiply_vedic_unified(a, b);
            }
            break;
            
        case VEDIC_OP_DIVIDE:
            {
                VedicCoreConfig config = vedic_core_get_config();
                config.mode = selected_mode;
                vedic_core_set_config(&config);
                
                result = divide_vedic_unified(a, b);
            }
            break;
            
        case VEDIC_OP_SQUARE:
            {
                VedicCoreConfig config = vedic_core_get_config();
                config.mode = selected_mode;
                vedic_core_set_config(&config);
                
                result = square_vedic_unified(a);
            }
            break;
            
        default:
            // Fallback to standard operation
            result = vedic_dynamic_operation(a, b, (VedicOperation)op_type);
            break;
    }
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
    
    // Record performance for learning
    record_performance(selected_mode, execution_time, success);
    
    // Update adaptive thresholds periodically
    static int update_counter = 0;
    if (++update_counter % 100 == 0) {
        update_adaptive_thresholds();
    }
    
    return result;
}

/**
 * Get current system resource status
 */
SystemResourceMonitor dispatch_get_system_status(void) {
    update_system_resources();
    return system_monitor;
}

/**
 * Get performance statistics
 */
DispatchPerformanceStats dispatch_get_performance_stats(void) {
    DispatchPerformanceStats stats = {0};
    
    if (perf_history.count == 0) {
        return stats;
    }
    
    // Calculate statistics
    double total_time = 0;
    int mode_counts[4] = {0};
    double mode_times[4] = {0};
    
    for (size_t i = 0; i < perf_history.count; i++) {
        PerformanceRecord* record = &perf_history.records[i];
        if (record->success) {
            total_time += record->execution_time_ms;
            if (record->mode < 4) {
                mode_counts[record->mode]++;
                mode_times[record->mode] += record->execution_time_ms;
            }
        }
    }
    
    stats.total_operations = perf_history.count;
    stats.avg_execution_time_ms = total_time / perf_history.count;
    
    // Calculate mode statistics
    for (int i = 0; i < 4; i++) {
        stats.mode_usage_percent[i] = (double)mode_counts[i] / perf_history.count * 100.0;
        if (mode_counts[i] > 0) {
            stats.mode_avg_time_ms[i] = mode_times[i] / mode_counts[i];
        }
    }
    
    // Find best mode
    double best_time = INFINITY;
    for (int i = 0; i < 4; i++) {
        if (mode_counts[i] > 0 && stats.mode_avg_time_ms[i] < best_time) {
            best_time = stats.mode_avg_time_ms[i];
            stats.best_mode = (VedicMode)i;
        }
    }
    
    return stats;
}

/**
 * Export performance data to CSV
 */
DispatchResult dispatch_export_performance_data(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        return DISPATCH_ERROR_FILE;
    }
    
    // Write header
    fprintf(file, "timestamp,mode,execution_time_ms,cpu_usage,memory_usage,success\n");
    
    // Write data
    for (size_t i = 0; i < perf_history.count; i++) {
        PerformanceRecord* record = &perf_history.records[i];
        fprintf(file, "%ld,%d,%.6f,%.2f,%.2f,%d\n",
                record->timestamp, record->mode, record->execution_time_ms,
                record->cpu_usage, record->memory_usage, record->success ? 1 : 0);
    }
    
    fclose(file);
    return DISPATCH_SUCCESS;
}

/**
 * Set dispatcher configuration
 */
void dispatch_set_config(const DispatcherConfig* config) {
    if (config) {
        dispatcher_config = *config;
    }
}

/**
 * Get current dispatcher configuration
 */
DispatcherConfig dispatch_get_config(void) {
    return dispatcher_config;
}

/**
 * Reset performance history
 */
void dispatch_reset_performance_history(void) {
    memset(&perf_history, 0, sizeof(perf_history));
    perf_history.window_size = PERF_HISTORY_SIZE;
}
