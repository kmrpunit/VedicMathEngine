/**
 * esp32_vedic_main.c - VedicMath-AI for ESP32-S3 (Xiao)
 * 
 * Optimized for resource-constrained environment:
 * - 512KB SRAM
 * - 8MB Flash
 * - Dual-core 240MHz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/uart.h"

// Include VedicMath library (adapted for ESP32)
#include "vedicmath_esp32.h"

static const char* TAG = "VedicMath_ESP32";

// ESP32-specific configuration
#define UART_NUM UART_NUM_0
#define BUF_SIZE 1024
#define STACK_SIZE 4096

// Reduced dataset size for ESP32
#define ESP32_MAX_DATASET 1000

// ESP32-optimized benchmark structure
typedef struct {
    uint32_t operand_a;
    uint32_t operand_b;
    uint32_t result;
    uint32_t execution_time_us;  // Microseconds
    uint8_t sutra_id;
    uint8_t confidence;
} esp32_benchmark_record_t;

// Global dataset for ESP32
static esp32_benchmark_record_t* esp32_dataset = NULL;
static size_t esp32_dataset_count = 0;

// ESP32-specific timing functions
static uint64_t get_time_us() {
    return esp_timer_get_time();
}

// Memory monitoring for ESP32
static void print_memory_info() {
    size_t free_heap = esp_get_free_heap_size();
    size_t min_free_heap = esp_get_minimum_free_heap_size();
    
    ESP_LOGI(TAG, "Free heap: %d bytes, Min free: %d bytes", 
             free_heap, min_free_heap);
}

// ESP32-optimized Vedic multiplication with resource monitoring
static uint32_t vedic_multiply_esp32(uint32_t a, uint32_t b, uint8_t* sutra_used) {
    *sutra_used = 0;  // Default to standard
    
    // Simplified pattern detection for ESP32
    if (a == b && (a % 10 == 5)) {
        *sutra_used = 1;  // Ekadhikena Purvena
        return ekadhikena_purvena_esp32(a);
    }
    
    if (is_near_base_esp32(a) && is_near_base_esp32(b)) {
        *sutra_used = 2;  // Nikhilam
        return nikhilam_mul_esp32(a, b);
    }
    
    if ((a % 10 + b % 10) == 10 && (a / 10 == b / 10)) {
        *sutra_used = 3;  // Antyayordasake
        return antyayordasake_esp32(a, b);
    }
    
    // Fallback to standard multiplication
    return a * b;
}

// ESP32-specific pattern generation
static void generate_test_pattern_esp32(int pattern_type, uint32_t* a, uint32_t* b) {
    switch (pattern_type) {
        case 0: // Ekadhikena pattern
            *a = *b = (esp_random() % 20 + 1) * 10 + 5;
            break;
            
        case 1: // Nikhilam pattern
            *a = 90 + (esp_random() % 20);
            *b = 90 + (esp_random() % 20);
            break;
            
        case 2: // Antyayordasake pattern
            {
                uint8_t prefix = (esp_random() % 9) + 1;
                uint8_t last_a = (esp_random() % 9) + 1;
                uint8_t last_b = 10 - last_a;
                *a = prefix * 10 + last_a;
                *b = prefix * 10 + last_b;
            }
            break;
            
        default: // Random
            *a = (esp_random() % 1000) + 1;
            *b = (esp_random() % 1000) + 1;
            break;
    }
}

// Run ESP32 benchmark task
static void esp32_benchmark_task(void* pvParameters) {
    ESP_LOGI(TAG, "Starting ESP32 VedicMath benchmark...");
    
    // Allocate dataset memory
    esp32_dataset = malloc(sizeof(esp32_benchmark_record_t) * ESP32_MAX_DATASET);
    if (!esp32_dataset) {
        ESP_LOGE(TAG, "Failed to allocate dataset memory");
        vTaskDelete(NULL);
        return;
    }
    
    print_memory_info();
    
    // Run benchmarks
    for (int pattern = 0; pattern < 4; pattern++) {
        ESP_LOGI(TAG, "Testing pattern type %d", pattern);
        
        for (int i = 0; i < 250 && esp32_dataset_count < ESP32_MAX_DATASET; i++) {
            uint32_t a, b;
            generate_test_pattern_esp32(pattern, &a, &b);
            
            uint8_t sutra_used;
            uint64_t start_time = get_time_us();
            uint32_t result = vedic_multiply_esp32(a, b, &sutra_used);
            uint64_t end_time = get_time_us();
            
            // Record result
            esp32_benchmark_record_t* record = &esp32_dataset[esp32_dataset_count++];
            record->operand_a = a;
            record->operand_b = b;
            record->result = result;
            record->execution_time_us = (uint32_t)(end_time - start_time);
            record->sutra_id = sutra_used;
            record->confidence = (sutra_used > 0) ? 90 : 100;
            
            // Verify correctness
            if (result != a * b) {
                ESP_LOGW(TAG, "Incorrect result: %u * %u = %u (expected %u)", 
                         a, b, result, a * b);
            }
            
            // Yield to other tasks periodically
            if (i % 50 == 0) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
        
        print_memory_info();
    }
    
    // Analyze results
    analyze_esp32_results();
    
    // Send results via UART
    send_results_uart();
    
    // Cleanup
    free(esp32_dataset);
    esp32_dataset = NULL;
    
    ESP_LOGI(TAG, "Benchmark task completed");
    vTaskDelete(NULL);
}

// Analyze ESP32 benchmark results
static void analyze_esp32_results() {
    if (esp32_dataset_count == 0) return;
    
    uint32_t sutra_counts[4] = {0}; // Standard, Ekadhikena, Nikhilam, Antyayordasake
    uint64_t total_time = 0;
    uint32_t min_time = UINT32_MAX;
    uint32_t max_time = 0;
    
    for (size_t i = 0; i < esp32_dataset_count; i++) {
        esp32_benchmark_record_t* record = &esp32_dataset[i];
        
        sutra_counts[record->sutra_id]++;
        total_time += record->execution_time_us;
        
        if (record->execution_time_us < min_time) {
            min_time = record->execution_time_us;
        }
        if (record->execution_time_us > max_time) {
            max_time = record->execution_time_us;
        }
    }
    
    ESP_LOGI(TAG, "=== ESP32 Benchmark Analysis ===");
    ESP_LOGI(TAG, "Total operations: %d", esp32_dataset_count);
    ESP_LOGI(TAG, "Average time: %llu us", total_time / esp32_dataset_count);
    ESP_LOGI(TAG, "Time range: [%u, %u] us", min_time, max_time);
    ESP_LOGI(TAG, "Sutra distribution:");
    ESP_LOGI(TAG, "  Standard: %u (%.1f%%)", sutra_counts[0], 
             100.0f * sutra_counts[0] / esp32_dataset_count);
    ESP_LOGI(TAG, "  Ekadhikena: %u (%.1f%%)", sutra_counts[1], 
             100.0f * sutra_counts[1] / esp32_dataset_count);
    ESP_LOGI(TAG, "  Nikhilam: %u (%.1f%%)", sutra_counts[2], 
             100.0f * sutra_counts[2] / esp32_dataset_count);
    ESP_LOGI(TAG, "  Antyayordasake: %u (%.1f%%)", sutra_counts[3], 
             100.0f * sutra_counts[3] / esp32_dataset_count);
}

// Send results via UART for data collection
static void send_results_uart() {
    const char* header = "operand_a,operand_b,result,time_us,sutra_id,confidence\n";
    uart_write_bytes(UART_NUM, header, strlen(header));
    
    char buffer[128];
    for (size_t i = 0; i < esp32_dataset_count; i++) {
        esp32_benchmark_record_t* record = &esp32_dataset[i];
        
        int len = snprintf(buffer, sizeof(buffer), 
                          "%u,%u,%u,%u,%u,%u\n",
                          record->operand_a, record->operand_b, record->result,
                          record->execution_time_us, record->sutra_id, record->confidence);
        
        uart_write_bytes(UART_NUM, buffer, len);
        
        // Small delay to avoid overwhelming UART buffer
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    
    ESP_LOGI(TAG, "Results sent via UART");
}

// Interactive command processor
static void process_uart_command(char* command) {
    if (strncmp(command, "MULTIPLY", 8) == 0) {
        uint32_t a, b;
        if (sscanf(command + 9, "%u %u", &a, &b) == 2) {
            uint8_t sutra_used;
            uint64_t start_time = get_time_us();
            uint32_t result = vedic_multiply_esp32(a, b, &sutra_used);
            uint64_t end_time = get_time_us();
            
            char response[100];
            snprintf(response, sizeof(response), 
                    "RESULT: %u * %u = %u (sutra: %u, time: %llu us)\n",
                    a, b, result, sutra_used, end_time - start_time);
            uart_write_bytes(UART_NUM, response, strlen(response));
        }
    }
    else if (strncmp(command, "BENCHMARK", 9) == 0) {
        ESP_LOGI(TAG, "Starting benchmark via command");
        xTaskCreate(esp32_benchmark_task, "vedic_benchmark", STACK_SIZE, NULL, 5, NULL);
    }
    else if (strncmp(command, "MEMORY", 6) == 0) {
        print_memory_info();
    }
    else if (strncmp(command, "STATUS", 6) == 0) {
        char status[200];
        snprintf(status, sizeof(status), 
                "STATUS: Dataset count: %d, Free heap: %d bytes\n",
                esp32_dataset_count, esp_get_free_heap_size());
        uart_write_bytes(UART_NUM, status, strlen(status));
    }
}

// UART command listening task
static void uart_command_task(void* pvParameters) {
    uint8_t* data = (uint8_t*) malloc(BUF_SIZE);
    char command_buffer[256];
    int cmd_pos = 0;
    
    ESP_LOGI(TAG, "UART command processor started");
    
    while (1) {
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE, 100 / portTICK_PERIOD_MS);
        
        if (len > 0) {
            for (int i = 0; i < len; i++) {
                if (data[i] == '\n' || data[i] == '\r') {
                    if (cmd_pos > 0) {
                        command_buffer[cmd_pos] = '\0';
                        process_uart_command(command_buffer);
                        cmd_pos = 0;
                    }
                } else if (cmd_pos < sizeof(command_buffer) - 1) {
                    command_buffer[cmd_pos++] = data[i];
                }
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    free(data);
}

// WiFi data transmission task (optional)
#ifdef CONFIG_ENABLE_WIFI
static void wifi_data_task(void* pvParameters) {
    // Initialize WiFi and send data to cloud
    // This would connect to your GCP/AWS endpoint
    // Implementation depends on your cloud setup
    
    ESP_LOGI(TAG, "WiFi data transmission not implemented yet");
    vTaskDelete(NULL);
}
#endif

// Main application entry point
void app_main(void) {
    ESP_LOGI(TAG, "VedicMath-AI ESP32-S3 starting...");
    
    // Initialize UART
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    
    print_memory_info();
    
    // Send startup message
    const char* startup_msg = "VedicMath-AI ESP32-S3 Ready\n"
                             "Commands: MULTIPLY a b, BENCHMARK, MEMORY, STATUS\n";
    uart_write_bytes(UART_NUM, startup_msg, strlen(startup_msg));
    
    // Create tasks
    xTaskCreate(uart_command_task, "uart_cmd", STACK_SIZE, NULL, 10, NULL);
    
    // Auto-start benchmark after delay
    vTaskDelay(pdMS_TO_TICKS(2000));
    ESP_LOGI(TAG, "Auto-starting initial benchmark...");
    xTaskCreate(esp32_benchmark_task, "vedic_benchmark", STACK_SIZE, NULL, 5, NULL);
    
#ifdef CONFIG_ENABLE_WIFI
    xTaskCreate(wifi_data_task, "wifi_data", STACK_SIZE, NULL, 3, NULL);
#endif
}