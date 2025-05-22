/**
 * vedicmath_optimized.c - Implementation of performance-optimized operations
 */
#include "../../include/vedicmath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Rest of the file...
#include "vedicmath_optimized.h"
// #include "vedicmath.h"
#include "vedicmath_types.h"
#include "vedicmath_dynamic.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>
// #include <ctype.h>

// Define a maximum size for expression cache
#define EXPRESSION_CACHE_SIZE 128

// Define function pointer types for operation handlers
typedef VedicValue (*binary_op_func)(VedicValue, VedicValue);

// Operation lookup tables - accessed by [op][type1][type2]
static binary_op_func operation_table[6][4][4];

// Expression cache
typedef struct
{
    char *expression;
    VedicValue result;
    int last_used; // For LRU replacement
} CachedExpression;

static CachedExpression expression_cache[EXPRESSION_CACHE_SIZE];
static int cache_use_counter = 0;
static int cache_initialized = 0;

/**
 * Initialize the operation lookup tables
 */
void vedic_optimized_init(void)
{
    // Clear the lookup tables
    memset(operation_table, 0, sizeof(operation_table));

    // Initialize the expression cache
    if (!cache_initialized)
    {
        memset(expression_cache, 0, sizeof(expression_cache));
        cache_initialized = 1;
    }

    // We'll use the dynamic functions as fallbacks for now
    // In a full implementation, we'd create specialized handlers
    // for each combination of types and operations

    // Initialize all table entries with dynamic operations
    for (int op = 0; op < 6; op++)
    {
        for (int type1 = 0; type1 < 4; type1++)
        {
            for (int type2 = 0; type2 < 4; type2++)
            {
                // Default to dynamic operations
                switch (op)
                {
                case VEDIC_OP_ADD:
                    operation_table[op][type1][type2] = vedic_dynamic_add;
                    break;
                case VEDIC_OP_SUBTRACT:
                    operation_table[op][type1][type2] = vedic_dynamic_subtract;
                    break;
                case VEDIC_OP_MULTIPLY:
                    operation_table[op][type1][type2] = vedic_dynamic_multiply;
                    break;
                case VEDIC_OP_DIVIDE:
                    operation_table[op][type1][type2] = vedic_dynamic_divide;
                    break;
                case VEDIC_OP_MODULO:
                    operation_table[op][type1][type2] = vedic_dynamic_modulo;
                    break;
                case VEDIC_OP_POWER:
                    // We'll implement this directly with specialized handlers
                    operation_table[op][type1][type2] = NULL;
                    break;
                }
            }
        }
    }

    // In the future, we'd override specific combinations with specialized
    // implementations for common cases
}

/**
 * Cleanup optimization resources
 */
void vedic_optimized_cleanup(void)
{
    // Free any resources in the expression cache
    for (int i = 0; i < EXPRESSION_CACHE_SIZE; i++)
    {
        if (expression_cache[i].expression)
        {
            free(expression_cache[i].expression);
            expression_cache[i].expression = NULL;
        }
    }

    cache_initialized = 0;
}

/**
 * Fast path int32 * int32 multiplication
 */
static inline VedicValue fast_int32_multiply(int32_t a, int32_t b)
{
    VedicValue result;

    // Check for potential overflow
    int64_t product = (int64_t)a * (int64_t)b;

    // If result fits in int32, use int32
    if (product >= INT32_MIN && product <= INT32_MAX)
    {
        result.type = VEDIC_INT32;
        result.value.i32 = (int32_t)product;
    }
    else
    {
        // Otherwise use int64
        result.type = VEDIC_INT64;
        result.value.i64 = product;
    }

    return result;
}

/**
 * Optimized dynamic multiplication
 */
inline VedicValue vedic_optimized_multiply(VedicValue a, VedicValue b)
{
    // Fast path for int32 * int32 (most common case)
    if (a.type == VEDIC_INT32 && b.type == VEDIC_INT32)
    {
        // Check for special Vedic math cases
        int32_t a_val = a.value.i32;
        int32_t b_val = b.value.i32;

        // Case 1: Squaring a number ending in 5
        if (a_val == b_val && a_val % 10 == 5)
        {
            int32_t result = (int32_t)vedic_square((long)a_val);
            VedicValue v;
            v.type = VEDIC_INT32;
            v.value.i32 = result;
            return v;
        }

        // Case 2: Both numbers near a base (Nikhilam)
        long base_a = nearest_power_of_10(a_val);
        if (is_close_to_base(a_val, base_a) &&
            is_close_to_base(b_val, base_a))
        {
            int32_t result = (int32_t)nikhilam_mul((long)a_val, (long)b_val);
            VedicValue v;
            v.type = VEDIC_INT32;
            v.value.i32 = result;
            return v;
        }

        // Case 3: Last digits sum to 10 (Antyayordasake)
        if (last_digits_sum_to_10(a_val, b_val) && same_prefix(a_val, b_val))
        {
            int32_t result = antya_dasake_mul(a_val, b_val);
            VedicValue v;
            v.type = VEDIC_INT32;
            v.value.i32 = result;
            return v;
        }

        // General case
        return fast_int32_multiply(a_val, b_val);
    }

    // For other type combinations, use the lookup table
    if (operation_table[VEDIC_OP_MULTIPLY][a.type][b.type])
    {
        return operation_table[VEDIC_OP_MULTIPLY][a.type][b.type](a, b);
    }

    // Fallback to dynamic multiplication
    return vedic_dynamic_multiply(a, b);
}

/**
 * Optimized dynamic addition
 */
inline VedicValue vedic_optimized_add(VedicValue a, VedicValue b)
{
    // Fast path for int32 + int32
    if (a.type == VEDIC_INT32 && b.type == VEDIC_INT32)
    {
        int64_t sum = (int64_t)a.value.i32 + (int64_t)b.value.i32;
        if (sum >= INT32_MIN && sum <= INT32_MAX)
        {
            VedicValue result;
            result.type = VEDIC_INT32;
            result.value.i32 = (int32_t)sum;
            return result;
        }
        else
        {
            VedicValue result;
            result.type = VEDIC_INT64;
            result.value.i64 = sum;
            return result;
        }
    }

    // Use the lookup table for other type combinations
    if (operation_table[VEDIC_OP_ADD][a.type][b.type])
    {
        return operation_table[VEDIC_OP_ADD][a.type][b.type](a, b);
    }

    // Fallback to dynamic addition
    return vedic_dynamic_add(a, b);
}

/**
 * Optimized dynamic subtraction
 */
inline VedicValue vedic_optimized_subtract(VedicValue a, VedicValue b)
{
    // Fast path for int32 - int32
    if (a.type == VEDIC_INT32 && b.type == VEDIC_INT32)
    {
        int64_t diff = (int64_t)a.value.i32 - (int64_t)b.value.i32;
        if (diff >= INT32_MIN && diff <= INT32_MAX)
        {
            VedicValue result;
            result.type = VEDIC_INT32;
            result.value.i32 = (int32_t)diff;
            return result;
        }
        else
        {
            VedicValue result;
            result.type = VEDIC_INT64;
            result.value.i64 = diff;
            return result;
        }
    }

    // Use the lookup table for other type combinations
    if (operation_table[VEDIC_OP_SUBTRACT][a.type][b.type])
    {
        return operation_table[VEDIC_OP_SUBTRACT][a.type][b.type](a, b);
    }

    // Fallback to dynamic subtraction
    return vedic_dynamic_subtract(a, b);
}

/**
 * Optimized dynamic division
 */
inline VedicValue vedic_optimized_divide(VedicValue a, VedicValue b)
{
    // Check for division by zero
    bool is_zero_b = false;

    switch (b.type)
    {
    case VEDIC_INT32:
        is_zero_b = (b.value.i32 == 0);
        break;
    case VEDIC_INT64:
        is_zero_b = (b.value.i64 == 0);
        break;
    case VEDIC_FLOAT:
        is_zero_b = (b.value.f32 == 0.0f);
        break;
    case VEDIC_DOUBLE:
        is_zero_b = (b.value.f64 == 0.0);
        break;
    default:
        is_zero_b = true;
        break;
    }

    if (is_zero_b)
    {
        // Division by zero - return MAX_VALUE with appropriate sign
        VedicValue result;
        switch (a.type)
        {
        case VEDIC_INT32:
            result.type = VEDIC_INT32;
            result.value.i32 = (a.value.i32 < 0) ? INT32_MIN : INT32_MAX;
            break;
        case VEDIC_INT64:
            result.type = VEDIC_INT64;
            result.value.i64 = (a.value.i64 < 0) ? INT64_MIN : INT64_MAX;
            break;
        case VEDIC_FLOAT:
            result.type = VEDIC_FLOAT;
            result.value.f32 = (a.value.f32 < 0) ? -INFINITY : INFINITY;
            break;
        case VEDIC_DOUBLE:
            result.type = VEDIC_DOUBLE;
            result.value.f64 = (a.value.f64 < 0) ? -INFINITY : INFINITY;
            break;
        default:
            result.type = VEDIC_INT32;
            result.value.i32 = 0;
            break;
        }
        return result;
    }

    // Fast path for int32 / int32
    if (a.type == VEDIC_INT32 && b.type == VEDIC_INT32)
    {
        // Use Vedic division if possible
        int32_t dividend = a.value.i32;
        int32_t divisor = b.value.i32;

        // Check for exact division
        if (dividend % divisor == 0)
        {
            VedicValue result;
            result.type = VEDIC_INT32;
            result.value.i32 = dividend / divisor;
            return result;
        }

        // If division would be non-integer, we have a choice:
        // 1. Return float/double to preserve fractional part
        // 2. Return integer part only

        // For now, we'll return float to preserve fractional part
        VedicValue result;
        result.type = VEDIC_FLOAT;
        result.value.f32 = (float)dividend / (float)divisor;
        return result;
    }

    // Use the lookup table for other type combinations
    if (operation_table[VEDIC_OP_DIVIDE][a.type][b.type])
    {
        return operation_table[VEDIC_OP_DIVIDE][a.type][b.type](a, b);
    }

    // Fallback to dynamic division
    return vedic_dynamic_divide(a, b);
}

/**
 * Optimized dynamic modulo
 */
inline VedicValue vedic_optimized_modulo(VedicValue a, VedicValue b)
{
    // Fast path for int32 % int32
    if (a.type == VEDIC_INT32 && b.type == VEDIC_INT32)
    {
        // Check for modulo by zero
        if (b.value.i32 == 0)
        {
            VedicValue result;
            result.type = VEDIC_INT32;
            result.value.i32 = a.value.i32;
            return result;
        }

        // Compute modulo
        VedicValue result;
        result.type = VEDIC_INT32;
        result.value.i32 = a.value.i32 % b.value.i32;
        return result;
    }

    // Use the lookup table for other type combinations
    if (operation_table[VEDIC_OP_MODULO][a.type][b.type])
    {
        return operation_table[VEDIC_OP_MODULO][a.type][b.type](a, b);
    }

    // Fallback to dynamic modulo
    return vedic_dynamic_modulo(a, b);
}

/**
 * Optimized dynamic power operation
 */
inline VedicValue vedic_optimized_power(VedicValue a, VedicValue b)
{
    // Fast path for int32 ^ int32 (with small exponents)
    if (a.type == VEDIC_INT32 && b.type == VEDIC_INT32)
    {
        // Only handle non-negative integer exponents
        if (b.value.i32 >= 0)
        {
            // Special case for x^0 = 1
            if (b.value.i32 == 0)
            {
                VedicValue result;
                result.type = VEDIC_INT32;
                result.value.i32 = 1;
                return result;
            }

            // Special case for x^1 = x
            if (b.value.i32 == 1)
            {
                return a;
            }

            // Special case for x^2 = x²
            if (b.value.i32 == 2)
            {
                return vedic_optimized_multiply(a, a);
            }

            // For small exponents, compute iteratively with Vedic multiplication
            if (b.value.i32 <= 10)
            {
                VedicValue result = a;
                for (int i = 1; i < b.value.i32; i++)
                {
                    result = vedic_optimized_multiply(result, a);
                }
                return result;
            }

            // For larger exponents, use binary exponentiation
            int exponent = b.value.i32;
            VedicValue result;
            result.type = VEDIC_INT32;
            result.value.i32 = 1;

            VedicValue base = a;

            while (exponent > 0)
            {
                if (exponent % 2 == 1)
                {
                    result = vedic_optimized_multiply(result, base);
                }
                base = vedic_optimized_multiply(base, base);
                exponent /= 2;
            }

            return result;
        }
    }

    // For all other cases, convert to double and use pow()
    double base = vedic_to_double(a);
    double exponent = vedic_to_double(b);
    double result_val = pow(base, exponent);

    // Convert back to the most appropriate type
    if (result_val == (int32_t)result_val &&
        result_val >= INT32_MIN && result_val <= INT32_MAX)
    {
        VedicValue result;
        result.type = VEDIC_INT32;
        result.value.i32 = (int32_t)result_val;
        return result;
    }
    else if (result_val == (int64_t)result_val &&
             result_val >= INT64_MIN && result_val <= INT64_MAX)
    {
        VedicValue result;
        result.type = VEDIC_INT64;
        result.value.i64 = (int64_t)result_val;
        return result;
    }
    else
    {
        VedicValue result;
        result.type = VEDIC_DOUBLE;
        result.value.f64 = result_val;
        return result;
    }
}

/**
 * Find operation from string with lookup table
 */
static inline VedicOperation fast_detect_operation(const char op_char)
{
    switch (op_char)
    {
    case '+':
        return VEDIC_OP_ADD;
    case '-':
        return VEDIC_OP_SUBTRACT;
    case '*':
        return VEDIC_OP_MULTIPLY;
    case '/':
        return VEDIC_OP_DIVIDE;
    case '%':
        return VEDIC_OP_MODULO;
    case '^':
        return VEDIC_OP_POWER;
    default:
        return VEDIC_OP_INVALID;
    }
}

/**
 * Check if expression is cached and return result if found
 */
static bool get_cached_expression(const char *expression, VedicValue *result)
{
    if (!cache_initialized)
    {
        return false;
    }

    for (int i = 0; i < EXPRESSION_CACHE_SIZE; i++)
    {
        if (expression_cache[i].expression &&
            strcmp(expression_cache[i].expression, expression) == 0)
        {
            // Cache hit - return the cached result
            *result = expression_cache[i].result;

            // Update the usage counter
            expression_cache[i].last_used = cache_use_counter++;

            return true;
        }
    }

    return false;
}

/**
 * Store expression result in cache
 */
static void cache_expression(const char *expression, VedicValue result)
{
    if (!cache_initialized)
    {
        vedic_optimized_init();
    }

    // Find an empty slot or the least recently used slot
    int target_slot = -1;
    int oldest_use = cache_use_counter;

    for (int i = 0; i < EXPRESSION_CACHE_SIZE; i++)
    {
        if (!expression_cache[i].expression)
        {
            // Found an empty slot
            target_slot = i;
            break;
        }

        if (expression_cache[i].last_used < oldest_use)
        {
            oldest_use = expression_cache[i].last_used;
            target_slot = i;
        }
    }

    // If we found a slot, store the result
    if (target_slot >= 0)
    {
        // Free the old expression if any
        if (expression_cache[target_slot].expression)
        {
            free(expression_cache[target_slot].expression);
        }

        // Store the new expression
    #ifdef VEDICMATH_PLATFORM_WINDOWS
        expression_cache[target_slot].expression = _strdup(expression);
    #else
        expression_cache[target_slot].expression = strdup(expression);
    #endif
        expression_cache[target_slot].result = result;
        expression_cache[target_slot].last_used = cache_use_counter++;
    }
}

/**
 * Optimized expression evaluation
 */
VedicValue vedic_optimized_evaluate(const char *expression)
{
    VedicValue result;

    // Check if the expression is cached
    if (get_cached_expression(expression, &result))
    {
        return result;
    }

    // Make a copy to avoid modifying the original string
    char *expr_copy = vedicmath_strdup(expression);
    if (!expr_copy)
    {
        // Memory allocation failed - return 0
        result.type = VEDIC_INT32;
        result.value.i32 = 0;
        return result;
    }

    // Find the operator using a single pass through the string
    char *op_ptr = NULL;
    for (char *p = expr_copy; *p; p++)
    {
        if (*p == '+' || *p == '-' || *p == '*' ||
            *p == '/' || *p == '%' || *p == '^')
        {
            op_ptr = p;
            break;
        }
    }

    if (!op_ptr)
    {
        // No operator found - try to parse as a single number
        result = vedic_parse_number(expr_copy);
        free(expr_copy);
        return result;
    }

    // Split the expression at the operator
    char op_char = *op_ptr;
    *op_ptr = '\0';
    char *left_str = expr_copy;
    char *right_str = op_ptr + 1;

    // Trim whitespace
    while (isspace(*left_str))
        left_str++;
    while (isspace(*right_str))
        right_str++;

    char *end = left_str + strlen(left_str) - 1;
    while (end > left_str && isspace(*end))
        *end-- = '\0';

    end = right_str + strlen(right_str) - 1;
    while (end > right_str && isspace(*end))
        *end-- = '\0';

    // Parse the operands
    VedicValue left = vedic_parse_number(left_str);
    VedicValue right = vedic_parse_number(right_str);

    // Get operation from lookup table
    VedicOperation op = fast_detect_operation(op_char);

    // Perform the operation
    switch (op)
    {
    case VEDIC_OP_ADD:
        result = vedic_optimized_add(left, right);
        break;
    case VEDIC_OP_SUBTRACT:
        result = vedic_optimized_subtract(left, right);
        break;
    case VEDIC_OP_MULTIPLY:
        result = vedic_optimized_multiply(left, right);
        break;
    case VEDIC_OP_DIVIDE:
        result = vedic_optimized_divide(left, right);
        break;
    case VEDIC_OP_MODULO:
        result = vedic_optimized_modulo(left, right);
        break;
    case VEDIC_OP_POWER:
        result = vedic_optimized_power(left, right);
        break;
    default:
        // Invalid operation - return 0
        result.type = VEDIC_INT32;
        result.value.i32 = 0;
        break;
    }

    // Cache the result
    cache_expression(expression, result);

    // Clean up
    free(expr_copy);

    return result;
}

/**
 * Optimized batch multiplication
 */
void vedic_optimized_multiply_batch(VedicValue *results,
                                    const VedicValue *a,
                                    const VedicValue *b,
                                    size_t count)
{
    if (count > INT_MAX)
    {
        // Fallback to serial loop if count is too large for OpenMP
        for (size_t i = 0; i < count; i++)
            results[i] = vedic_optimized_multiply(a[i], b[i]);
        return;
    }

    int i;
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (i = 0; i < (int)count; i++)
    {
        results[i] = vedic_optimized_multiply(a[i], b[i]);
    }
}

/**
 * Optimized batch expression evaluation
 */
void vedic_optimized_evaluate_batch(VedicValue *results,
                                    const char **expressions,
                                    size_t count)
{
    if (count > INT_MAX)
    {
        // Fallback to serial loop if count is too large for OpenMP
        for (size_t i = 0; i < count; i++)
            results[i] = vedic_optimized_evaluate(expressions[i]);
        return;
    }
    int i = 0;
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (i = 0; i < count; i++)
    {
        results[i] = vedic_optimized_evaluate(expressions[i]);
    }
}