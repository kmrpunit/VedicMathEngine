#include "dispatch_mixed_mode.h"
#include "vedic_core.h"
#include "vedicmath_dynamic.h"
#include "vedicmath_optimized.h"
#include "vedicmath_types.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


typedef struct {
    int64_t result;
    const char* mode;
    double time_taken_ns;
} DispatchResult;

// Simulated system flags (can be set by the adaptive engine)
bool use_dynamic_mode = false;
bool use_optimized_mode = true;

// Set mode externally
void set_execution_mode(bool dynamic, bool optimized) {
    use_dynamic_mode = dynamic;
    use_optimized_mode = optimized;
}

bool is_using_optimized_mode(void) {
    return use_optimized_mode;
}


// int64_t dispatch_multiply(int64_t a, int64_t b) {
//     if (use_optimized_mode) {
//         VedicValue va = vedic_from_int64(a);
//         VedicValue vb = vedic_from_int64(b);
//         VedicValue result = vedic_optimized_multiply(va, vb);
//         return vedic_to_int64(result);
//     } else if (use_dynamic_mode) {
//         VedicValue va = vedic_from_int64(a);
//         VedicValue vb = vedic_from_int64(b);
//         VedicValue result = vedic_dynamic_multiply(va, vb);
//         return vedic_to_int64(result);
//     } else {
//         return multiply_urdhva(a, b); // fallback
//     }
// }

// Smart dispatcher for multiplication
int64_t dispatch_multiply(int64_t a, int64_t b) {
    DispatchResult r;
    clock_t start = clock();
    int64_t result;

    if (use_optimized_mode) {
        r.result = vedic_to_int64(vedic_optimized_multiply(vedic_from_int64(a), vedic_from_int64(b)));
        printf("[optimized] %ld x %ld = %ld\n", a, b, r.result);
        r.mode = "optimized";
    } else if (use_dynamic_mode) {
        r.result = vedic_to_int64(vedic_dynamic_multiply(vedic_from_int64(a), vedic_from_int64(b)));
        printf("[dynamic] %ld x %ld = %ld\n", a, b, r.result);
        r.mode = "dynamic";
    } else {
        r.result = multiply_urdhva(a, b);
        printf("[standard] %ld x %ld = %ld\n", a, b, r.result);
        r.mode = "standard";
    }

    r.time_taken_ns = (double)(clock() - start) / CLOCKS_PER_SEC * 1e9;
    printf("Time taken: %.2f ns\n", r.time_taken_ns);
    FILE* log = fopen("benchmark_log.csv", "a");
    fprintf(log, "%ld,%ld,%s,%ld,%.2f\n", a, b, r.mode, r.result, r.time_taken_ns);
    fclose(log);

    return result;
}


// Smart dispatcher for squaring
int64_t dispatch_square(int64_t n) {
    if (use_optimized_mode) {
        VedicValue va = vedic_from_int64(n);
        VedicValue result = vedic_optimized_multiply(va, va);
        return vedic_to_int64(result);
    } else if (use_dynamic_mode) {
        VedicValue va = vedic_from_int64(n);
        VedicValue result = vedic_dynamic_multiply(va, va);
        return vedic_to_int64(result);
    } else {
        return n * n;
    }
}

// Smart dispatcher for division
int64_t dispatch_divide(int64_t a, int64_t b, int64_t* rem_out) {
    if (use_optimized_mode) {
        VedicValue va = vedic_from_int64(a);
        VedicValue vb = vedic_from_int64(b);
        VedicValue result = vedic_optimized_divide(va, vb);
        if (rem_out) *rem_out = a % b;
        return vedic_to_int64(result);
    } else if (use_dynamic_mode) {
        VedicValue va = vedic_from_int64(a);
        VedicValue vb = vedic_from_int64(b);
        VedicValue result = vedic_dynamic_divide(va, vb);
        if (rem_out) *rem_out = a % b;
        return vedic_to_int64(result);
    } else {
        if (rem_out) *rem_out = a % b;
        return a / b;
    }
}
