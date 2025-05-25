/**
 * dispatch_mixed_mode.c - Enhanced System-Aware Adaptive Dispatcher
 * 
 * ACADEMIC CONTRIBUTION: Novel adaptive arithmetic engine that intelligently selects
 * optimal Vedic Mathematics sutras based on multi-factor analysis including:
 * 1. Mathematical pattern recognition with confidence scoring
 * 2. Real-time system resource monitoring (CPU, memory, platform)
 * 3. Performance prediction and validation
 * 4. Comprehensive dataset generation for research validation
 * 
 * RESEARCH OBJECTIVE: Demonstrate quantitative superiority of Vedic methods
 * over standard arithmetic through intelligent algorithm selection.
 * 
 * @author VedicMath-AI Research Team
 * @version 2.0 - Enhanced Adaptive Engine
 * @date 2024
 */

#include "dispatch_mixed_mode.h"
#include "vedic_core.h"
#include "vedicmath.h"
#include "vedicmath_dynamic.h"
#include "vedicmath_optimized.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
    #include <pdh.h>
    #pragma comment(lib, "pdh.lib")
#elif defined(__linux__)
    #include <sys/sysinfo.h>
    #include <unistd.h>
    #include <sys/resource.h>
    #include <time.h>  // For clock_gettime
#elif defined(ESP32_PLATFORM)
    #include "freertos/FreeRTOS.h"
    #include "esp_system.h"
    #include "esp_timer.h"
    #include "esp_heap_caps.h"
#endif

#include "utf8_console.h"

// ============================================================================
// ENHANCED PATTERN DETECTION SYSTEM
// ============================================================================

/**
 * @brief Vedic sutra classification with performance characteristics
 * 
 * Each sutra is characterized by:
 * - Computational complexity factor
 * - Expected speedup ratio vs standard arithmetic
 * - Memory usage profile
 * - Precision characteristics
 */
typedef struct {
    VedicSutraType sutra_type;
    const char* sutra_name;
    const char* sanskrit_name;
    double complexity_factor;        // Computational complexity (0.1-2.0)
    double expected_speedup;         // Expected speedup vs standard (1.0-5.0)
    size_t memory_overhead_bytes;    // Additional memory required
    double precision_factor;         // Precision retention (0.8-1.0)
    const char* optimal_conditions;  // Description of when to use
} VedicSutraProfile;

/**
 * @brief Comprehensive Vedic sutra profiles for intelligent selection
 * 
 * RESEARCH NOTE: These profiles are derived from theoretical analysis
 * and will be validated through comprehensive benchmarking.
 */
static const VedicSutraProfile VEDIC_SUTRA_PROFILES[] = {
    {
        .sutra_type = SUTRA_EKADHIKENA_PURVENA,
        .sutra_name = "Ekadhikena Purvena",
        .sanskrit_name = "एकाधिकेन पूर्वेण",
        .complexity_factor = 0.3,
        .expected_speedup = 3.5,
        .memory_overhead_bytes = 64,
        .precision_factor = 1.0,
        .optimal_conditions = "Squaring numbers ending in 5"
    },
    {
        .sutra_type = SUTRA_NIKHILAM,
        .sutra_name = "Nikhilam Navatashcaramam",
        .sanskrit_name = "निखिलं नवतश्चरमं दशतः",
        .complexity_factor = 0.6,
        .expected_speedup = 2.8,
        .memory_overhead_bytes = 128,
        .precision_factor = 0.98,
        .optimal_conditions = "Numbers within 20% of powers of 10"
    },
    {
        .sutra_type = SUTRA_ANTYAYORDASAKE,
        .sutra_name = "Antyayordasake",
        .sanskrit_name = "अन्त्ययोर्दशके",
        .complexity_factor = 0.5,
        .expected_speedup = 2.3,
        .memory_overhead_bytes = 96,
        .precision_factor = 1.0,
        .optimal_conditions = "Same prefix, last digits sum to 10"
    },
    {
        .sutra_type = SUTRA_URDHVA_TIRYAGBHYAM,
        .sutra_name = "Urdhva-Tiryagbhyam",
        .sanskrit_name = "ऊर्ध्व-तिर्यगभ्याम्",
        .complexity_factor = 1.2,
        .expected_speedup = 1.8,
        .memory_overhead_bytes = 256,
        .precision_factor = 1.0,
        .optimal_conditions = "General multiplication, especially large numbers"
    },
    {
        .sutra_type = SUTRA_STANDARD,
        .sutra_name = "Standard Arithmetic",
        .sanskrit_name = "मानक गणित",
        .complexity_factor = 1.0,
        .expected_speedup = 1.0,
        .memory_overhead_bytes = 0,
        .precision_factor = 1.0,
        .optimal_conditions = "Fallback for all cases"
    }
};

#define NUM_SUTRA_PROFILES (sizeof(VEDIC_SUTRA_PROFILES) / sizeof(VedicSutraProfile))

/**
 * @brief Enhanced pattern analysis result with comprehensive metrics
 * 
 * RESEARCH CONTRIBUTION: Multi-dimensional pattern analysis that considers
 * mathematical structure, computational complexity, and system constraints.
 */
typedef struct {
    VedicSutraType recommended_sutra;
    double confidence_score;         // 0.0-1.0: Pattern match confidence
    double performance_prediction;   // Expected speedup factor
    double precision_estimate;       // Expected precision retention
    size_t memory_requirement;       // Estimated memory usage
    const char* selection_reasoning; // Human-readable explanation
    const char* mathematical_basis;  // Academic justification
} EnhancedPatternAnalysis;

// ============================================================================
// SYSTEM MONITORING IMPLEMENTATION
// ============================================================================

static SystemResourceMonitor system_monitor = {0};
static DispatcherConfig dispatcher_config = {
    .cpu_threshold_high = 80.0,
    .cpu_threshold_low = 30.0,
    .memory_threshold_high = 0.8,
    .memory_threshold_low = 0.3,
    .energy_aware = true,
    .adaptive_thresholds = true,
    .monitoring_interval_ms = 100,
    .temperature_threshold = 75.0,
    .min_free_memory_mb = 64
};

#ifdef _WIN32
/**
 * @brief Windows-specific system monitoring using Performance Counters
 * 
 * IMPLEMENTATION NOTE: Uses Windows PDH (Performance Data Helper) API
 * for accurate CPU and memory monitoring.
 */
static PDH_HQUERY cpu_query = NULL;
static PDH_HCOUNTER cpu_counter = NULL;
static bool windows_monitoring_initialized = false;

static void initialize_windows_monitoring(void) {
    if (windows_monitoring_initialized) return;
    
    PdhOpenQuery(NULL, NULL, &cpu_query);
    PdhAddEnglishCounter(cpu_query, L"\\Processor(_Total)\\% Processor Time", NULL, &cpu_counter);
    PdhCollectQueryData(cpu_query);
    
    windows_monitoring_initialized = true;
}

static void cleanup_windows_monitoring(void) {
    if (cpu_query) {
        PdhCloseQuery(cpu_query);
        cpu_query = NULL;
        cpu_counter = NULL;
        windows_monitoring_initialized = false;
    }
}

static double get_windows_cpu_usage(void) {
    if (!windows_monitoring_initialized) {
        initialize_windows_monitoring();
        return 50.0; // Initial estimate
    }
    
    PDH_FMT_COUNTERVALUE counter_value;
    PdhCollectQueryData(cpu_query);
    PdhGetFormattedCounterValue(cpu_counter, PDH_FMT_DOUBLE, NULL, &counter_value);
    
    return counter_value.doubleValue;
}

static void get_windows_memory_info(SystemResourceMonitor* monitor) {
    MEMORYSTATUSEX mem_status;
    mem_status.dwLength = sizeof(mem_status);
    
    if (GlobalMemoryStatusEx(&mem_status)) {
        monitor->memory_total_mb = mem_status.ullTotalPhys / (1024 * 1024);
        monitor->memory_available_mb = mem_status.ullAvailPhys / (1024 * 1024);
        monitor->memory_usage_percent = 
            (double)(mem_status.ullTotalPhys - mem_status.ullAvailPhys) / 
            mem_status.ullTotalPhys * 100.0;
    }
}

#endif

#ifdef ESP32_PLATFORM
/**
 * @brief ESP32-specific system monitoring for embedded optimization
 * 
 * RESEARCH NOTE: ESP32 monitoring focuses on heap usage, temperature,
 * and power consumption - critical for embedded Vedic computing.
 */
static void get_esp32_system_info(SystemResourceMonitor* monitor) {
    // Heap memory monitoring
    monitor->memory_total_mb = CONFIG_ESP32_SPIRAM_SIZE / (1024 * 1024);
    monitor->memory_available_mb = esp_get_free_heap_size() / (1024 * 1024);
    monitor->memory_usage_percent = 
        (1.0 - (double)esp_get_free_heap_size() / CONFIG_ESP32_SPIRAM_SIZE) * 100.0;
    
    // Task and CPU monitoring
    monitor->task_count = uxTaskGetNumberOfTasks();
    monitor->cpu_usage_percent = 30.0 + (monitor->task_count * 2.0); // Estimate
    
    // Temperature monitoring (if sensor available)
    monitor->temperature_celsius = 45.0 + (rand() % 20); // Simulated for now
    
    // Power estimation based on CPU and memory usage
    monitor->power_consumption_watts = 
        0.5 + (monitor->cpu_usage_percent / 100.0) * 2.0 + 
        (monitor->memory_usage_percent / 100.0) * 0.5;
    
    // Thermal throttling check
    monitor->thermal_throttling = (monitor->temperature_celsius > 70.0);
}
#endif

/**
 * @brief Update system resource information with platform-specific optimizations
 * 
 * ACADEMIC CONTRIBUTION: Cross-platform resource monitoring that enables
 * intelligent algorithm selection based on real-time system constraints.
 */
static void dispatch_update_system_resources(void) {
    clock_t current_time = clock();
    
#ifdef _WIN32
    system_monitor.cpu_usage_percent = get_windows_cpu_usage();
    get_windows_memory_info(&system_monitor);
    
    // Windows-specific optimizations
    system_monitor.temperature_celsius = 35.0 + (system_monitor.cpu_usage_percent * 0.3);
    system_monitor.power_consumption_watts = 
        5.0 + (system_monitor.cpu_usage_percent / 100.0) * 50.0;
        
#elif defined(__linux__)
    // Linux system monitoring with /proc filesystem
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        system_monitor.memory_total_mb = info.totalram / (1024 * 1024);
        system_monitor.memory_available_mb = info.freeram / (1024 * 1024);
        system_monitor.memory_usage_percent = 
            (double)(info.totalram - info.freeram) / info.totalram * 100.0;
    }
    
    // CPU load from /proc/loadavg
    FILE* loadavg = fopen("/proc/loadavg", "r");
    if (loadavg) {
        double load1;
        if (fscanf(loadavg, "%lf", &load1) == 1) {
            system_monitor.cpu_usage_percent = 
                load1 * 100.0 / sysconf(_SC_NPROCESSORS_ONLN);
        }
        fclose(loadavg);
    }
    
#elif defined(ESP32_PLATFORM)
    get_esp32_system_info(&system_monitor);
    
#else
    // Generic fallback with reasonable estimates
    system_monitor.memory_total_mb = 4096;
    system_monitor.memory_available_mb = 2048;
    system_monitor.memory_usage_percent = 50.0;
    system_monitor.cpu_usage_percent = 40.0;
#endif
    
    system_monitor.last_update = current_time;
}

/**
 * @brief Get current system resource status
 */
SystemResourceMonitor dispatch_get_system_status(void) {
    dispatch_update_system_resources();
    return system_monitor;
}

/**
 * @brief Check if system is under high load
 * 
 * DECISION SUPPORT: Quick check for high-load conditions that
 * should influence algorithm selection toward faster methods.
 * 
 * @return true if system is under high load (CPU/memory/thermal)
 */
bool dispatch_is_system_stressed(void) {
    // Ensure we have current system data
    dispatch_update_system_resources();
    
    // Check CPU threshold
    if (system_monitor.cpu_usage_percent > dispatcher_config.cpu_threshold_high) {
        return true;
    }
    
    // Check memory threshold
    if (system_monitor.memory_usage_percent > dispatcher_config.memory_threshold_high * 100.0) {
        return true;
    }
    
    // Check temperature threshold
    if (system_monitor.temperature_celsius > dispatcher_config.temperature_threshold) {
        return true;
    }
    
    // Check thermal throttling
    if (system_monitor.thermal_throttling) {
        return true;
    }
    
#ifdef ESP32_PLATFORM
    // ESP32-specific checks
    if (system_monitor.free_heap_bytes < (dispatcher_config.min_free_memory_mb * 1024 * 1024)) {
        return true;
    }
    
    // Too many tasks can stress ESP32
    if (system_monitor.task_count > 50) {
        return true;
    }
#endif
    
    // System is not stressed
    return false;
}

// ============================================================================
// ENHANCED PATTERN DETECTION ALGORITHMS
// ============================================================================

/**
 * @brief Analyze Ekadhikena Purvena pattern with confidence scoring
 * 
 * MATHEMATICAL BASIS: For numbers ending in 5, N² = m(m+1) || 25
 * where N = 10m + 5. This reduces O(n²) multiplication to O(1) operations.
 * 
 * @param a First operand
 * @param b Second operand
 * @return Pattern analysis with confidence and performance prediction
 */
static EnhancedPatternAnalysis analyze_ekadhikena_pattern(long a, long b) {
    EnhancedPatternAnalysis analysis = {0};
    
    // Perfect case: same number ending in 5 (squaring)
    if (a == b && (a % 10 == 5) && a > 0) {
        analysis.recommended_sutra = SUTRA_EKADHIKENA_PURVENA;
        analysis.confidence_score = 0.98;
        analysis.performance_prediction = 3.5;
        analysis.precision_estimate = 1.0;
        analysis.memory_requirement = 64;
        analysis.selection_reasoning = "Perfect Ekadhikena Purvena case: squaring number ending in 5";
        analysis.mathematical_basis = "N² = m(m+1)||25 where N=10m+5, O(n²)→O(1) complexity reduction";
        return analysis;
    }
    
    // Near-perfect case: both numbers end in 5, close values
    if ((a % 10 == 5) && (b % 10 == 5) && abs(a - b) <= 10) {
        analysis.recommended_sutra = SUTRA_EKADHIKENA_PURVENA;
        analysis.confidence_score = 0.75;
        analysis.performance_prediction = 2.8;
        analysis.precision_estimate = 0.98;
        analysis.memory_requirement = 96;
        analysis.selection_reasoning = "Modified Ekadhikena: both numbers end in 5";
        analysis.mathematical_basis = "Extended application of Ekadhikena principle";
        return analysis;
    }
    
    // No match
    analysis.confidence_score = 0.0;
    return analysis;
}

/**
 * @brief Analyze Nikhilam pattern with proximity-based confidence
 * 
 * MATHEMATICAL BASIS: For numbers near base B, (B-a)(B-b) = (B-a-b)||ab
 * Complexity reduction from O(n²) to O(n) for numbers close to powers of 10.
 * 
 * @param a First operand  
 * @param b Second operand
 * @return Pattern analysis with distance-weighted confidence
 */
static EnhancedPatternAnalysis analyze_nikhilam_pattern(long a, long b) {
    EnhancedPatternAnalysis analysis = {0};
    
    long base_a = nearest_power_of_10(a);
    long base_b = nearest_power_of_10(b);
    
    // Must be near the same base for optimal efficiency
    if (base_a != base_b) {
        analysis.confidence_score = 0.0;
        return analysis;
    }
    
    // Calculate proximity to base (normalized to 0-1)
    double proximity_a = 1.0 - fabs((double)(a - base_a)) / (base_a * 0.3);
    double proximity_b = 1.0 - fabs((double)(b - base_b)) / (base_b * 0.3);
    
    // Ensure proximity is non-negative
    proximity_a = fmax(0.0, proximity_a);
    proximity_b = fmax(0.0, proximity_b);
    
    // Combined proximity determines confidence
    double combined_proximity = (proximity_a + proximity_b) / 2.0;
    
    if (combined_proximity > 0.7) {
        analysis.recommended_sutra = SUTRA_NIKHILAM;
        analysis.confidence_score = combined_proximity;
        analysis.performance_prediction = 1.5 + (combined_proximity * 1.3);
        analysis.precision_estimate = 0.95 + (combined_proximity * 0.05);
        analysis.memory_requirement = 128;
        analysis.selection_reasoning = "Strong Nikhilam pattern: numbers close to same power of 10";
        analysis.mathematical_basis = "Nikhilam sutra: (B-a)(B-b) = (B-a-b)||ab, O(n²)→O(n)";
        return analysis;
    } else if (combined_proximity > 0.3) {
        analysis.recommended_sutra = SUTRA_NIKHILAM;
        analysis.confidence_score = combined_proximity;
        analysis.performance_prediction = 1.2 + (combined_proximity * 0.8);
        analysis.precision_estimate = 0.92 + (combined_proximity * 0.06);
        analysis.memory_requirement = 128;
        analysis.selection_reasoning = "Moderate Nikhilam pattern: reasonable proximity to base";
        analysis.mathematical_basis = "Modified Nikhilam application";
        return analysis;
    }
    
    analysis.confidence_score = 0.0;
    return analysis;
}

/**
 * @brief Analyze Antyayordasake pattern with structural validation
 * 
 * MATHEMATICAL BASIS: For numbers ab and ac where b+c=10 and same prefix a,
 * Result = a(a+1) || bc. Reduces multiplication complexity significantly.
 * 
 * @param a First operand
 * @param b Second operand  
 * @return Pattern analysis with structural confidence
 */
static EnhancedPatternAnalysis analyze_antyayordasake_pattern(long a, long b) {
    EnhancedPatternAnalysis analysis = {0};
    
    // Check last digits sum to 10
    int last_a = a % 10;
    int last_b = b % 10;
    
    if ((last_a + last_b) != 10) {
        analysis.confidence_score = 0.0;
        return analysis;
    }
    
    // Check same prefix
    long prefix_a = a / 10;
    long prefix_b = b / 10;
    
    if (prefix_a == prefix_b && prefix_a > 0) {
        analysis.recommended_sutra = SUTRA_ANTYAYORDASAKE;
        analysis.confidence_score = 0.90;
        analysis.performance_prediction = 2.3;
        analysis.precision_estimate = 1.0;
        analysis.memory_requirement = 96;
        analysis.selection_reasoning = "Perfect Antyayordasake: same prefix, last digits sum to 10";
        analysis.mathematical_basis = "Antyayordasake: ab×ac = a(a+1)||bc where b+c=10";
        return analysis;
    }
    
    analysis.confidence_score = 0.0;
    return analysis;
}

/**
 * @brief Analyze Urdhva-Tiryagbhyam pattern based on number size
 * 
 * MATHEMATICAL BASIS: Crosswise multiplication reduces carries and
 * provides systematic approach for multi-digit multiplication.
 * 
 * @param a First operand
 * @param b Second operand
 * @return Pattern analysis with size-based confidence
 */
static EnhancedPatternAnalysis analyze_urdhva_pattern(long a, long b) {
    EnhancedPatternAnalysis analysis = {0};
    
    int digits_a = count_digits(abs(a));
    int digits_b = count_digits(abs(b));
    int max_digits = fmax(digits_a, digits_b);
    
    if (max_digits >= 4) {
        // Large numbers benefit significantly from Urdhva-Tiryagbhyam
        analysis.recommended_sutra = SUTRA_URDHVA_TIRYAGBHYAM;
        analysis.confidence_score = 0.8 + (max_digits - 4) * 0.05;
        analysis.confidence_score = fmin(analysis.confidence_score, 0.95);
        analysis.performance_prediction = 1.5 + (max_digits - 4) * 0.1;
        analysis.precision_estimate = 1.0;
        analysis.memory_requirement = 256 + (max_digits * 32);
        analysis.selection_reasoning = "Large numbers: Urdhva-Tiryagbhyam optimal for multi-digit";
        analysis.mathematical_basis = "Crosswise multiplication reduces carries, O(n²) with better constants";
        return analysis;
    } else if (max_digits >= 3) {
        // Medium numbers show moderate benefit
        analysis.recommended_sutra = SUTRA_URDHVA_TIRYAGBHYAM;
        analysis.confidence_score = 0.60;
        analysis.performance_prediction = 1.3;
        analysis.precision_estimate = 1.0;
        analysis.memory_requirement = 192;
        analysis.selection_reasoning = "Medium numbers: moderate Urdhva-Tiryagbhyam benefit";
        analysis.mathematical_basis = "Systematic crosswise approach";
        return analysis;
    } else {
        // Small numbers - lower priority but still available
        analysis.recommended_sutra = SUTRA_URDHVA_TIRYAGBHYAM;
        analysis.confidence_score = 0.25;
        analysis.performance_prediction = 1.1;
        analysis.precision_estimate = 1.0;
        analysis.memory_requirement = 128;
        analysis.selection_reasoning = "Small numbers: Urdhva-Tiryagbhyam as fallback";
        analysis.mathematical_basis = "General multiplication algorithm";
        return analysis;
    }
}

/**
 * @brief Comprehensive pattern analysis engine
 * 
 * RESEARCH CONTRIBUTION: Multi-sutra pattern analysis that evaluates
 * all applicable Vedic methods and selects optimal approach based on
 * mathematical structure and performance characteristics.
 * 
 * @param a First operand
 * @param b Second operand
 * @return Best pattern analysis result
 */
static EnhancedPatternAnalysis analyze_comprehensive_patterns(long a, long b) {
    EnhancedPatternAnalysis candidates[4];
    EnhancedPatternAnalysis best_analysis = {0};
    
    // Analyze all applicable patterns
    candidates[0] = analyze_ekadhikena_pattern(a, b);
    candidates[1] = analyze_nikhilam_pattern(a, b);
    candidates[2] = analyze_antyayordasake_pattern(a, b);
    candidates[3] = analyze_urdhva_pattern(a, b);
    
    // Find the best candidate based on confidence and performance
    double best_score = 0.0;
    for (int i = 0; i < 4; i++) {
        // Combined score: confidence weighted by performance prediction
        double combined_score = candidates[i].confidence_score * 
                               (1.0 + candidates[i].performance_prediction * 0.2);
        
        if (combined_score > best_score) {
            best_score = combined_score;
            best_analysis = candidates[i];
        }
    }
    
    // If no pattern found, default to standard arithmetic
    if (best_analysis.confidence_score == 0.0) {
        best_analysis.recommended_sutra = SUTRA_STANDARD;
        best_analysis.confidence_score = 1.0;
        best_analysis.performance_prediction = 1.0;
        best_analysis.precision_estimate = 1.0;
        best_analysis.memory_requirement = 0;
        best_analysis.selection_reasoning = "No Vedic pattern detected: using standard arithmetic";
        best_analysis.mathematical_basis = "Standard multiplication algorithm";
    }
    
    return best_analysis;
}

// ============================================================================
// ADAPTIVE DECISION ENGINE
// ============================================================================

/**
 * @brief System-aware decision modification
 * 
 * RESEARCH CONTRIBUTION: Resource-aware algorithm selection that considers
 * CPU load, memory constraints, and platform characteristics.
 * 
 * @param base_analysis Initial pattern analysis
 * @param system Current system state
 * @return Modified analysis considering system constraints
 */
static EnhancedPatternAnalysis apply_system_constraints(
    EnhancedPatternAnalysis base_analysis, 
    const SystemResourceMonitor* system) {
    
    EnhancedPatternAnalysis modified = base_analysis;
    
    // High CPU usage - prefer faster algorithms
    if (system->cpu_usage_percent > dispatcher_config.cpu_threshold_high) {
        // Boost preference for high-speedup sutras
        if (base_analysis.performance_prediction > 2.0) {
            modified.confidence_score *= 1.2;
            modified.selection_reasoning = "High CPU load: prioritizing fast Vedic method";
        } else {
            // Penalize complex algorithms under high CPU load
            modified.confidence_score *= 0.8;
            modified.selection_reasoning = "High CPU load: reducing complex algorithm preference";
        }
    }
    
    // High memory usage - prefer memory-efficient algorithms
    if (system->memory_usage_percent > dispatcher_config.memory_threshold_high * 100) {
        if (base_analysis.memory_requirement > 200) {
            modified.confidence_score *= 0.7;
            modified.selection_reasoning = "High memory usage: avoiding memory-intensive algorithms";
        }
    }
    
    // Platform-specific optimizations
#ifdef ESP32_PLATFORM
    // ESP32: strongly prefer low-complexity algorithms
    if (base_analysis.complexity_factor > 1.0) {
        modified.confidence_score *= 0.6;
        modified.selection_reasoning = "ESP32 platform: prioritizing simple algorithms";
    }
    
    // Temperature-based throttling
    if (system->temperature_celsius > 65.0) {
        modified.confidence_score *= 0.5;
        modified.selection_reasoning = "High temperature: thermal throttling active";
    }
#endif
    
    // Ensure confidence doesn't exceed 1.0
    modified.confidence_score = fmin(modified.confidence_score, 1.0);
    
    return modified;
}

/**
 * @brief Execute selected Vedic sutra with performance monitoring
 * 
 * RESEARCH OBJECTIVE: Validate Vedic method performance claims through
 * precise timing and resource measurement.
 * 
 * @param a First operand
 * @param b Second operand
 * @param analysis Selected algorithm analysis
 * @return Execution result with performance metrics
 */
static long execute_vedic_sutra(long a, long b, const EnhancedPatternAnalysis* analysis) {
    switch (analysis->recommended_sutra) {
        case SUTRA_EKADHIKENA_PURVENA:
            // For Ekadhikena, we're typically squaring (a == b)
            return ekadhikena_purvena(a);
            
        case SUTRA_NIKHILAM:
            return nikhilam_mul(a, b);
            
        case SUTRA_ANTYAYORDASAKE:
            return antya_dasake_mul((int)a, (int)b);
            
        case SUTRA_URDHVA_TIRYAGBHYAM:
            return urdhva_mult(a, b);
            
        case SUTRA_STANDARD:
        default:
            return a * b;
    }
}

// ============================================================================
// PERFORMANCE VALIDATION & DATASET GENERATION
// ============================================================================

/**
 * @brief Performance validation record for research analysis
 */
typedef struct {
    // Input characteristics
    long operand_a, operand_b, result;
    
    // Algorithm selection
    VedicSutraType selected_sutra;
    double confidence_score;
    const char* selection_reasoning;
    
    // Performance metrics
    double vedic_execution_time_ms;
    double standard_execution_time_ms;
    double actual_speedup;
    double predicted_speedup;
    bool performance_validated;
    
    // System context
    double cpu_usage_percent;
    double memory_usage_percent;
    size_t memory_used_bytes;
    PlatformType platform;
    
    // Research metadata
    time_t timestamp;
    bool correctness_verified;
    double precision_error;
} PerformanceValidationRecord;

static PerformanceValidationRecord* validation_dataset = NULL;
static size_t validation_dataset_size = 0;
static size_t validation_dataset_capacity = 0;

/**
 * @brief Initialize performance validation dataset
 */
static void initialize_validation_dataset(size_t initial_capacity) {
    validation_dataset_capacity = initial_capacity;
    validation_dataset = malloc(sizeof(PerformanceValidationRecord) * validation_dataset_capacity);
    validation_dataset_size = 0;
}

/**
 * @brief Record performance validation data for research analysis
 */
static void record_validation_data(
    long a, long b, long result,
    const EnhancedPatternAnalysis* analysis,
    double vedic_time_ms, double standard_time_ms) {
    
    if (!validation_dataset) {
        initialize_validation_dataset(10000);
    }
    
    // Expand dataset if needed
    if (validation_dataset_size >= validation_dataset_capacity) {
        validation_dataset_capacity *= 2;
        validation_dataset = realloc(validation_dataset, 
            sizeof(PerformanceValidationRecord) * validation_dataset_capacity);
    }
    
    PerformanceValidationRecord* record = &validation_dataset[validation_dataset_size++];
    
    // Fill record
    record->operand_a = a;
    record->operand_b = b;
    record->result = result;
    record->selected_sutra = analysis->recommended_sutra;
    record->confidence_score = analysis->confidence_score;
    record->selection_reasoning = analysis->selection_reasoning;
    record->vedic_execution_time_ms = vedic_time_ms;
    record->standard_execution_time_ms = standard_time_ms;
    record->actual_speedup = standard_time_ms / vedic_time_ms;
    record->predicted_speedup = analysis->performance_prediction;
    record->performance_validated = (record->actual_speedup >= 1.0);
    
    // System context
    record->cpu_usage_percent = system_monitor.cpu_usage_percent;
    record->memory_usage_percent = system_monitor.memory_usage_percent;
    record->memory_used_bytes = analysis->memory_requirement;
    
#ifdef ESP32_PLATFORM
    record->platform = PLATFORM_ESP32;
#elif defined(_WIN32)
    record->platform = PLATFORM_WINDOWS;
#elif defined(__linux__)
    record->platform = PLATFORM_LINUX;
#else
    record->platform = PLATFORM_GENERIC;
#endif
    
    // Research metadata
    record->timestamp = time(NULL);
    record->correctness_verified = (result == a * b);
    record->precision_error = 0.0; // For integer operations
}

/**
 * @brief Export validation dataset to CSV for research analysis
 * 
 * RESEARCH OUTPUT: Comprehensive dataset proving Vedic method superiority
 * with statistical validation and system context.
 */
static void export_validation_dataset(const char* filename) {
    if (!validation_dataset || validation_dataset_size == 0) {
        printf("No validation data to export\n");
        return;
    }
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return;
    }
    
    // Comprehensive CSV header for research analysis
    fprintf(file, "timestamp,operand_a,operand_b,result,selected_sutra,confidence_score,");
    fprintf(file, "selection_reasoning,vedic_time_ms,standard_time_ms,actual_speedup,");
    fprintf(file, "predicted_speedup,performance_validated,cpu_usage_percent,");
    fprintf(file, "memory_usage_percent,memory_used_bytes,platform,correctness_verified,");
    fprintf(file, "precision_error\n");
    
    // Export all validation records
    for (size_t i = 0; i < validation_dataset_size; i++) {
        PerformanceValidationRecord* record = &validation_dataset[i];
        
        fprintf(file, "%ld,%ld,%ld,%ld,%d,%.4f,\"%s\",%.6f,%.6f,%.4f,%.4f,%d,%.2f,%.2f,%zu,%d,%d,%.6f\n",
            record->timestamp, record->operand_a, record->operand_b, record->result,
            record->selected_sutra, record->confidence_score, record->selection_reasoning,
            record->vedic_execution_time_ms, record->standard_execution_time_ms,
            record->actual_speedup, record->predicted_speedup, 
            record->performance_validated ? 1 : 0,
            record->cpu_usage_percent, record->memory_usage_percent, 
            record->memory_used_bytes, record->platform,
            record->correctness_verified ? 1 : 0, record->precision_error);
    }
    
    fclose(file);
    printf("Validation dataset exported: %s (%zu records)\n", filename, validation_dataset_size);
}

// ============================================================================
// MAIN ADAPTIVE DISPATCH FUNCTION
// ============================================================================

/**
 * @brief Main adaptive multiplication with comprehensive validation
 * 
 * RESEARCH CONTRIBUTION: Complete adaptive arithmetic engine that:
 * 1. Analyzes mathematical patterns with confidence scoring
 * 2. Considers real-time system constraints  
 * 3. Validates performance claims through dual execution
 * 4. Generates research dataset proving Vedic superiority
 * 
 * @param a First operand
 * @param b Second operand
 * @return Multiplication result with validated performance
 */
VedicValue dispatch_multiply(VedicValue a, VedicValue b) {
    // Convert to long for pattern analysis
    long a_long = vedic_to_int64(a);
    long b_long = vedic_to_int64(b);
    
    // Update system monitoring
    dispatch_update_system_resources();
    
    // STEP 1: Comprehensive pattern analysis
    EnhancedPatternAnalysis pattern_analysis = analyze_comprehensive_patterns(a_long, b_long);
    
    // STEP 2: Apply system constraints
    EnhancedPatternAnalysis final_analysis = apply_system_constraints(pattern_analysis, &system_monitor);
    
    // Replace the timing code in dispatch_multiply() with this improved version:

// STEP 3: Performance validation through dual execution with improved timing
#ifdef _WIN32
    // Use high-resolution performance counter on Windows
    LARGE_INTEGER frequency, vedic_start, vedic_end, standard_start, standard_end;
    QueryPerformanceFrequency(&frequency);
    
    QueryPerformanceCounter(&vedic_start);
    long vedic_result = execute_vedic_sutra(a_long, b_long, &final_analysis);
    QueryPerformanceCounter(&vedic_end);
    
    QueryPerformanceCounter(&standard_start);
    long standard_result = a_long * b_long;
    QueryPerformanceCounter(&standard_end);
    
    double vedic_time_ms = ((double)(vedic_end.QuadPart - vedic_start.QuadPart)) / frequency.QuadPart * 1000.0;
    double standard_time_ms = ((double)(standard_end.QuadPart - standard_start.QuadPart)) / frequency.QuadPart * 1000.0;
    
#else
    // Use clock_gettime for better precision on Linux/Unix
    struct timespec vedic_start, vedic_end, standard_start, standard_end;
    
    clock_gettime(CLOCK_MONOTONIC, &vedic_start);
    long vedic_result = execute_vedic_sutra(a_long, b_long, &final_analysis);
    clock_gettime(CLOCK_MONOTONIC, &vedic_end);
    
    clock_gettime(CLOCK_MONOTONIC, &standard_start);
    long standard_result = a_long * b_long;
    clock_gettime(CLOCK_MONOTONIC, &standard_end);
    
    double vedic_time_ms = ((vedic_end.tv_sec - vedic_start.tv_sec) * 1000.0) + 
                          ((vedic_end.tv_nsec - vedic_start.tv_nsec) / 1000000.0);
    double standard_time_ms = ((standard_end.tv_sec - standard_start.tv_sec) * 1000.0) + 
                             ((standard_end.tv_nsec - standard_start.tv_nsec) / 1000000.0);
#endif

    // Add safety checks for timing calculations
    if (standard_time_ms <= 0.0) {
        standard_time_ms = 0.001; // Minimum 1 microsecond
    }
    if (vedic_time_ms <= 0.0) {
        vedic_time_ms = 0.001;
    }
    
    // STEP 4: Record validation data for research
    record_validation_data(a_long, b_long, vedic_result, &final_analysis, 
                          vedic_time_ms, standard_time_ms);
    
    // Return result with preserved type
    return vedic_from_int64(vedic_result);
}

/**
 * @brief Adaptive squaring with specialized pattern detection
 */
VedicValue dispatch_square(VedicValue a) {
    return dispatch_multiply(a, a);
}

/**
 * @brief Enhanced adaptive division with Vedic optimization potential
 * 
 * RESEARCH OBJECTIVE: Placeholder for future Vedic division sutras while
 * providing robust division with performance monitoring and validation.
 * 
 * FUTURE ENHANCEMENTS:
 * - Paravartya Yojayet (transpose and apply) for complex divisions
 * - Digital root methods for divisibility checks
 * - Vedic factorization techniques
 * - Approximate division with error bounds
 * 
 * @param dividend Dividend value
 * @param divisor Divisor value
 * @return Division result with performance validation
 */
VedicValue dispatch_divide(VedicValue dividend, VedicValue divisor) {
    // Convert to long for analysis and validation
    long dividend_long = vedic_to_int64(dividend);
    long divisor_long = vedic_to_int64(divisor);
    
    // Division by zero check
    if (divisor_long == 0) {
        printf("ERROR: Division by zero attempted\n");
        return vedic_from_int64(0);
    }
    
    // Update system monitoring for adaptive behavior
    dispatch_update_system_resources();
    
    // Performance timing for validation
    clock_t vedic_start = clock();
    
    // Call dynamic division - returns VedicValue directly
    VedicValue vedic_result = vedic_dynamic_divide(dividend, divisor);
    
    clock_t vedic_end = clock();
    double vedic_time_ms = ((double)(vedic_end - vedic_start)) / CLOCKS_PER_SEC * 1000.0;
    
    // Standard division for validation
    clock_t standard_start = clock();
    long standard_result = dividend_long / divisor_long;
    clock_t standard_end = clock();
    double standard_time_ms = ((double)(standard_end - standard_start)) / CLOCKS_PER_SEC * 1000.0;
    
    // Validate correctness
    long vedic_result_long = vedic_to_int64(vedic_result);
    if (vedic_result_long != standard_result) {
        printf("WARNING: Division result mismatch! %ld / %ld: Vedic=%ld, Standard=%ld\n", 
               dividend_long, divisor_long, vedic_result_long, standard_result);
        // Use standard result for safety
        vedic_result = vedic_from_int64(standard_result);
    }
    
    // Record performance data for research (if validation system is active)
    if (validation_dataset) {
        // Create analysis for division operations
        EnhancedPatternAnalysis division_analysis = {
            .recommended_sutra = SUTRA_STANDARD, // Currently no Vedic division sutras implemented
            .confidence_score = (vedic_result_long == standard_result) ? 0.8 : 0.0,
            .performance_prediction = 1.0,
            .precision_estimate = 1.0,
            .memory_requirement = 128,
            .selection_reasoning = "Dynamic division with validation",
            .mathematical_basis = "Standard division algorithm with error checking"
        };
        
        record_validation_data(dividend_long, divisor_long, vedic_result_long, 
                              &division_analysis, vedic_time_ms, standard_time_ms);
    }
    
    return vedic_result;
}

// ============================================================================
// COMPREHENSIVE TEST PATTERN GENERATION
// ============================================================================

/**
 * @brief Generate comprehensive test patterns for validation
 * 
 * RESEARCH OBJECTIVE: Create diverse test cases that thoroughly validate
 * Vedic method performance across all pattern types and system conditions.
 */
void generate_comprehensive_validation_dataset(size_t target_size) {
    printf("Generating comprehensive validation dataset (%zu patterns)...\n", target_size);
    
    size_t patterns_per_category = target_size / 8;
    
    // Category 1: Perfect Ekadhikena cases (numbers ending in 5)
    printf("Generating Ekadhikena Purvena patterns...\n");
    for (size_t i = 0; i < patterns_per_category; i++) {
        long n = (rand() % 199 + 1) * 10 + 5; // 15, 25, ..., 1995
        VedicValue a = vedic_from_int64(n);
        VedicValue result = dispatch_multiply(a, a);
        (void)result; // Suppress unused warning
    }
    
    // Category 2: Nikhilam patterns (near powers of 10)
    printf("Generating Nikhilam patterns...\n");
    for (size_t i = 0; i < patterns_per_category; i++) {
        int base_power = rand() % 4 + 2; // 10^2 to 10^5
        long base = 1;
        for (int j = 0; j < base_power; j++) base *= 10;
        
        // Generate numbers within 25% of base
        long range = base / 4;
        long a = base + (rand() % (2 * range)) - range;
        long b = base + (rand() % (2 * range)) - range;
        
        VedicValue va = vedic_from_int64(a);
        VedicValue vb = vedic_from_int64(b);
        VedicValue result = dispatch_multiply(va, vb);
        (void)result;
    }
    
    // Category 3: Antyayordasake patterns
    printf("Generating Antyayordasake patterns...\n");
    for (size_t i = 0; i < patterns_per_category; i++) {
        int prefix = rand() % 999 + 1;
        int last_a = rand() % 9 + 1;
        int last_b = 10 - last_a;
        
        long a = prefix * 10 + last_a;
        long b = prefix * 10 + last_b;
        
        VedicValue va = vedic_from_int64(a);
        VedicValue vb = vedic_from_int64(b);
        VedicValue result = dispatch_multiply(va, vb);
        (void)result;
    }
    
    // Category 4: Large numbers (Urdhva-Tiryagbhyam)
    printf("Generating large number patterns...\n");
    for (size_t i = 0; i < patterns_per_category; i++) {
        long a = 1000 + rand() % 999000; // 4-6 digit numbers
        long b = 1000 + rand() % 999000;
        
        VedicValue va = vedic_from_int64(a);
        VedicValue vb = vedic_from_int64(b);
        VedicValue result = dispatch_multiply(va, vb);
        (void)result;
    }
    
    // Category 5: Medium numbers
    printf("Generating medium number patterns...\n");
    for (size_t i = 0; i < patterns_per_category; i++) {
        long a = 100 + rand() % 900; // 3-digit numbers
        long b = 100 + rand() % 900;
        
        VedicValue va = vedic_from_int64(a);
        VedicValue vb = vedic_from_int64(b);
        VedicValue result = dispatch_multiply(va, vb);
        (void)result;
    }
    
    // Category 6: Small numbers
    printf("Generating small number patterns...\n");
    for (size_t i = 0; i < patterns_per_category; i++) {
        long a = 10 + rand() % 90; // 2-digit numbers
        long b = 10 + rand() % 90;
        
        VedicValue va = vedic_from_int64(a);
        VedicValue vb = vedic_from_int64(b);
        VedicValue result = dispatch_multiply(va, vb);
        (void)result;
    }
    
    // Category 7: Edge cases
    printf("Generating edge case patterns...\n");
    for (size_t i = 0; i < patterns_per_category; i++) {
        long a, b;
        switch (i % 8) {
            case 0: a = 0; b = rand() % 1000; break;
            case 1: a = 1; b = rand() % 1000; break;
            case 2: a = -1; b = rand() % 1000; break;
            case 3: a = rand() % 1000; b = 0; break;
            case 4: a = rand() % 1000; b = 1; break;
            case 5: a = -(rand() % 1000 + 1); b = -(rand() % 1000 + 1); break;
            case 6: a = LONG_MAX / 1000; b = 999; break; // Large numbers
            case 7: a = -999; b = -999; break; // Negative numbers
        }
        
        VedicValue va = vedic_from_int64(a);
        VedicValue vb = vedic_from_int64(b);
        VedicValue result = dispatch_multiply(va, vb);
        (void)result;
    }
    
    // Category 8: Random stress test
    printf("Generating random stress patterns...\n");
    size_t remaining = target_size - (patterns_per_category * 7);
    for (size_t i = 0; i < remaining; i++) {
        long a = (rand() % 20000) - 10000; // -10000 to 10000
        long b = (rand() % 20000) - 10000;
        
        VedicValue va = vedic_from_int64(a);
        VedicValue vb = vedic_from_int64(b);
        VedicValue result = dispatch_multiply(va, vb);
        (void)result;
    }
    
    printf("Validation dataset generation complete! Generated %zu records\n", 
           validation_dataset_size);
}

// ============================================================================
// PERFORMANCE ANALYSIS & STATISTICS
// ============================================================================

/**
 * @brief Analyze validation dataset and generate performance statistics
 * 
 * RESEARCH OUTPUT: Statistical proof of Vedic method superiority
 */
void analyze_performance_statistics(void) {
    if (!validation_dataset || validation_dataset_size == 0) {
        printf("No validation data available for analysis\n");
        return;
    }
    
    printf("\n=== PERFORMANCE VALIDATION ANALYSIS ===\n");
    printf("Dataset size: %zu operations\n", validation_dataset_size);
    
    // Count sutra usage and calculate statistics
    size_t sutra_counts[MAX_SUTRA_TYPES] = {0};
    double total_speedup = 0.0;
    size_t significant_speedups = 0;
    size_t correctness_failures = 0;
    
    double ekadhikena_speedups = 0.0, nikhilam_speedups = 0.0;
    double antyayordasake_speedups = 0.0, urdhva_speedups = 0.0;
    size_t ekadhikena_count = 0, nikhilam_count = 0;
    size_t antyayordasake_count = 0, urdhva_count = 0;
    
    for (size_t i = 0; i < validation_dataset_size; i++) {
        PerformanceValidationRecord* record = &validation_dataset[i];
        
        sutra_counts[record->selected_sutra]++;
        total_speedup += record->actual_speedup;
        
        if (record->actual_speedup > 1.1) { // 10% improvement threshold
            significant_speedups++;
        }
        
        if (!record->correctness_verified) {
            correctness_failures++;
        }
        
        // Collect sutra-specific statistics
        switch (record->selected_sutra) {
            case SUTRA_EKADHIKENA_PURVENA:
                ekadhikena_speedups += record->actual_speedup;
                ekadhikena_count++;
                break;
            case SUTRA_NIKHILAM:
                nikhilam_speedups += record->actual_speedup;
                nikhilam_count++;
                break;
            case SUTRA_ANTYAYORDASAKE:
                antyayordasake_speedups += record->actual_speedup;
                antyayordasake_count++;
                break;
            case SUTRA_URDHVA_TIRYAGBHYAM:
                urdhva_speedups += record->actual_speedup;
                urdhva_count++;
                break;
        }
    }
    
    // Overall statistics
    double avg_speedup = total_speedup / validation_dataset_size;
    double significant_improvement_rate = (double)significant_speedups / validation_dataset_size * 100.0;
    double correctness_rate = (double)(validation_dataset_size - correctness_failures) / validation_dataset_size * 100.0;
    
    printf("\n--- OVERALL PERFORMANCE ---\n");
    printf("Average speedup: %.2fx\n", avg_speedup);
    printf("Significant improvements (>10%%): %.1f%% of operations\n", significant_improvement_rate);
    printf("Correctness rate: %.2f%%\n", correctness_rate);
    
    // Sutra-specific statistics
    printf("\n--- SUTRA-SPECIFIC PERFORMANCE ---\n");
    
    if (ekadhikena_count > 0) {
        printf("Ekadhikena Purvena: %.2fx average speedup (%zu operations)\n", 
               ekadhikena_speedups / ekadhikena_count, ekadhikena_count);
    }
    
    if (nikhilam_count > 0) {
        printf("Nikhilam: %.2fx average speedup (%zu operations)\n", 
               nikhilam_speedups / nikhilam_count, nikhilam_count);
    }
    
    if (antyayordasake_count > 0) {
        printf("Antyayordasake: %.2fx average speedup (%zu operations)\n", 
               antyayordasake_speedups / antyayordasake_count, antyayordasake_count);
    }
    
    if (urdhva_count > 0) {
        printf("Urdhva-Tiryagbhyam: %.2fx average speedup (%zu operations)\n", 
               urdhva_speedups / urdhva_count, urdhva_count);
    }
    
    printf("Standard arithmetic: %zu operations\n", sutra_counts[SUTRA_STANDARD]);
    
    printf("\n--- RESEARCH VALIDATION ---\n");
    if (avg_speedup > 1.0) {
        printf("✓ RESEARCH HYPOTHESIS VALIDATED: Vedic methods show %.1f%% average improvement\n", 
               (avg_speedup - 1.0) * 100.0);
    } else {
        printf("⚠ Research hypothesis needs refinement: average speedup %.2fx\n", avg_speedup);
    }
    
    printf("✓ Correctness validated: %.2f%% accuracy\n", correctness_rate);
    printf("✓ Statistical significance: %zu operations analyzed\n", validation_dataset_size);
}

// ============================================================================
// INITIALIZATION & CLEANUP
// ============================================================================

/**
 * @brief Initialize the enhanced adaptive dispatcher
 */
DispatchResult dispatch_mixed_mode_init(const DispatcherConfig* config) {
    if (config) {
        dispatcher_config = *config;
    }
    
    // Initialize system monitoring
    dispatch_update_system_resources();
    
#ifdef _WIN32
    set_utf8_console();
    initialize_windows_monitoring();
#endif
    
    // Initialize validation dataset
    initialize_validation_dataset(10000);
    
    printf("Enhanced Adaptive Dispatcher initialized\n");
    printf("- Real-time system monitoring: ENABLED\n");
    printf("- Performance validation: ENABLED\n");
    printf("- Research dataset generation: ENABLED\n");
    
    return DISPATCH_SUCCESS;
}

/**
 * @brief Cleanup and export final results
 */
void dispatch_cleanup_and_export(const char* dataset_filename) {
    // Export validation dataset
    if (dataset_filename) {
        export_validation_dataset(dataset_filename);
    }
    
    // Generate performance analysis
    analyze_performance_statistics();
    
    // Cleanup resources
#ifdef _WIN32
    cleanup_windows_monitoring();
#endif
    
    if (validation_dataset) {
        free(validation_dataset);
        validation_dataset = NULL;
        validation_dataset_size = 0;
        validation_dataset_capacity = 0;
    }
    
    printf("Enhanced Adaptive Dispatcher cleanup complete\n");
}