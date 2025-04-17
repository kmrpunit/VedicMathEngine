/**
 * vedicmath_types.c - Implementation of dynamic type system for Vedic Mathematics Library
 */
#include "../../include/vedicmath_types.h"
#include "../../include/vedicmath_platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>

/**
 * Convert a VedicValue to a string
 */
char *vedic_to_string(VedicValue value, char *buffer, size_t buffer_size)
{
    if (!buffer || buffer_size < 2)
    {
        return NULL;
    }

    switch (value.type)
    {
    case VEDIC_INT32:
#ifdef VEDICMATH_PLATFORM_WINDOWS
        sprintf_s(buffer, buffer_size, "%d", value.value.i32);
#else
        snprintf(buffer, buffer_size, "%d", value.value.i32);
#endif
        break;

    case VEDIC_INT64:
#ifdef VEDICMATH_PLATFORM_WINDOWS
        sprintf_s(buffer, buffer_size, "%lld", (long long)value.value.i64);
#else
        snprintf(buffer, buffer_size, "%lld", (long long)value.value.i64);
#endif
        break;

    case VEDIC_FLOAT:
#ifdef VEDICMATH_PLATFORM_WINDOWS
        sprintf_s(buffer, buffer_size, "%g", (double)value.value.f32);
#else
        snprintf(buffer, buffer_size, "%g", (double)value.value.f32);
#endif
        break;

    case VEDIC_DOUBLE:
#ifdef VEDICMATH_PLATFORM_WINDOWS
        sprintf_s(buffer, buffer_size, "%g", value.value.f64);
#else
        snprintf(buffer, buffer_size, "%g", value.value.f64);
#endif
        break;

    default:
#ifdef VEDICMATH_PLATFORM_WINDOWS
        strcpy_s(buffer, buffer_size, "INVALID");
#else
        strncpy(buffer, "INVALID", buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
#endif
        return NULL;
    }

    return buffer;
}

/**
 * Determine the appropriate type based on a string representation
 */
VedicNumberType vedic_detect_type(const char *number_str)
{
    if (!number_str || !*number_str)
    {
        return VEDIC_INVALID;
    }

    // Skip leading whitespace
    while (isspace(*number_str))
    {
        number_str++;
    }

    // Check for sign
    if (*number_str == '+' || *number_str == '-')
    {
        number_str++;
    }

    // Check for decimal point or exponent (indicating floating point)
    const char *ptr = number_str;
    bool has_decimal = false;
    bool has_exponent = false;
    int digit_count = 0;

    while (*ptr)
    {
        if (*ptr == '.')
        {
            has_decimal = true;
        }
        else if (*ptr == 'e' || *ptr == 'E')
        {
            has_exponent = true;
        }
        else if (isdigit(*ptr))
        {
            digit_count++;
        }
        else if (!isspace(*ptr) && *ptr != '+' && *ptr != '-')
        {
            // Allow +/- after exponent marker
            if (!(has_exponent && (ptr > number_str) &&
                  (*(ptr - 1) == 'e' || *(ptr - 1) == 'E') &&
                  (*ptr == '+' || *ptr == '-')))
            {
                // Invalid character
                return VEDIC_INVALID;
            }
        }
        ptr++;
    }

    // If it has a decimal point or exponent, it's a floating point
    if (has_decimal || has_exponent)
    {
        // Use double for values with exponents as they may need higher precision
        if (has_exponent || digit_count > 7)
        {
            return VEDIC_DOUBLE;
        }
        else
        {
            return VEDIC_FLOAT;
        }
    }

    // For integers, check the range
    char *endptr;
    int64_t value = strtoll(number_str, &endptr, 10);

    // Special case for INT32_MIN
    if (value == INT32_MIN)
    {
        return VEDIC_INT32;
    }

    // If it fits in an int32, use that
    if (value >= INT32_MIN && value <= INT32_MAX)
    {
        return VEDIC_INT32;
    }
    else
    {
        // Otherwise use int64
        return VEDIC_INT64;
    }
}

/**
 * Create a VedicValue from a string, automatically detecting type
 */
VedicValue vedic_parse_number(const char *number_str)
{
    VedicValue result;
    VedicNumberType type = vedic_detect_type(number_str);
    result.type = type;

    // Parse according to detected type
    switch (type)
    {
    case VEDIC_INT32:
        result.value.i32 = (int32_t)strtol(number_str, NULL, 10);
        break;

    case VEDIC_INT64:
        result.value.i64 = strtoll(number_str, NULL, 10);
        break;

    case VEDIC_FLOAT:
        result.value.f32 = strtof(number_str, NULL);
        break;

    case VEDIC_DOUBLE:
        result.value.f64 = strtod(number_str, NULL);
        break;

    default:
        // Invalid type
        result.type = VEDIC_INVALID;
        memset(&result.value, 0, sizeof(result.value));
        break;
    }

    return result;
}

/**
 * Determine the resulting type when operating on two VedicValues
 */
VedicNumberType vedic_result_type(VedicNumberType a, VedicNumberType b)
{
    // If either type is invalid, result is invalid
    if (a == VEDIC_INVALID || b == VEDIC_INVALID)
    {
        return VEDIC_INVALID;
    }

    // Type promotion rules:
    // 1. If either operand is DOUBLE, result is DOUBLE
    // 2. If either operand is FLOAT, result is FLOAT
    // 3. If either operand is INT64, result is INT64
    // 4. Otherwise result is INT32

    if (a == VEDIC_DOUBLE || b == VEDIC_DOUBLE)
    {
        return VEDIC_DOUBLE;
    }
    else if (a == VEDIC_FLOAT || b == VEDIC_FLOAT)
    {
        return VEDIC_FLOAT;
    }
    else if (a == VEDIC_INT64 || b == VEDIC_INT64)
    {
        return VEDIC_INT64;
    }
    else
    {
        return VEDIC_INT32;
    }
}

/**
 * Detect the operation from a string
 */
VedicOperation vedic_detect_operation(const char *op_str)
{
    if (!op_str || !*op_str)
    {
        return VEDIC_OP_INVALID;
    }

    // Skip leading whitespace
    while (isspace(*op_str))
    {
        op_str++;
    }

    // Check the operation
    switch (*op_str)
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
 * Convert an int32_t value to a VedicValue
 */
VedicValue vedic_from_int32(int32_t value)
{
    VedicValue result;
    result.type = VEDIC_INT32;
    result.value.i32 = value;
    return result;
}

/**
 * Convert an int64_t value to a VedicValue
 */
VedicValue vedic_from_int64(int64_t value)
{
    VedicValue result;

    // If it fits in int32, use that instead
    if (value >= INT32_MIN && value <= INT32_MAX)
    {
        result.type = VEDIC_INT32;
        result.value.i32 = (int32_t)value;
    }
    else
    {
        result.type = VEDIC_INT64;
        result.value.i64 = value;
    }

    return result;
}

/**
 * Convert a float value to a VedicValue
 */
VedicValue vedic_from_float(float value)
{
    VedicValue result;

    // If it's a whole number that fits in int32, convert to int32
    if (value == (int32_t)value &&
        value >= INT32_MIN && value <= INT32_MAX)
    {
        result.type = VEDIC_INT32;
        result.value.i32 = (int32_t)value;
    }
    // If it's a whole number that fits in int64, convert to int64
    else if (value == (int64_t)value &&
             value >= (float)INT64_MIN && value <= (float)INT64_MAX)
    {
        result.type = VEDIC_INT64;
        result.value.i64 = (int64_t)value;
    }
    else
    {
        result.type = VEDIC_FLOAT;
        result.value.f32 = value;
    }

    return result;
}

/**
 * Convert a double value to a VedicValue
 */
VedicValue vedic_from_double(double value)
{
    VedicValue result;

    // If it's a whole number that fits in int32, convert to int32
    if (value == (int32_t)value &&
        value >= INT32_MIN && value <= INT32_MAX)
    {
        result.type = VEDIC_INT32;
        result.value.i32 = (int32_t)value;
    }
    // If it's a whole number that fits in int64, convert to int64
    else if (value == (int64_t)value &&
             value >= INT64_MIN && value <= INT64_MAX)
    {
        result.type = VEDIC_INT64;
        result.value.i64 = (int64_t)value;
    }
    // If it fits in float without losing precision, use float
    else if (value >= -FLT_MAX && value <= FLT_MAX &&
             (float)value == value)
    {
        result.type = VEDIC_FLOAT;
        result.value.f32 = (float)value;
    }
    else
    {
        result.type = VEDIC_DOUBLE;
        result.value.f64 = value;
    }

    return result;
}

/**
 * Get the int32_t value from a VedicValue
 */
int32_t vedic_to_int32(VedicValue value)
{
    switch (value.type)
    {
    case VEDIC_INT32:
        return value.value.i32;
    case VEDIC_INT64:
        // Clamp to int32 range if needed
        if (value.value.i64 < INT32_MIN)
            return INT32_MIN;
        if (value.value.i64 > INT32_MAX)
            return INT32_MAX;
        return (int32_t)value.value.i64;
    case VEDIC_FLOAT:
        // Clamp to int32 range if needed
        if (value.value.f32 < INT32_MIN)
            return INT32_MIN;
        if (value.value.f32 > INT32_MAX)
            return INT32_MAX;
        return (int32_t)value.value.f32;
    case VEDIC_DOUBLE:
        // Clamp to int32 range if needed
        if (value.value.f64 < INT32_MIN)
            return INT32_MIN;
        if (value.value.f64 > INT32_MAX)
            return INT32_MAX;
        return (int32_t)value.value.f64;
    default:
        return 0;
    }
}

/**
 * Get the int64_t value from a VedicValue
 */
int64_t vedic_to_int64(VedicValue value)
{
    switch (value.type)
    {
    case VEDIC_INT32:
        return (int64_t)value.value.i32;
    case VEDIC_INT64:
        return value.value.i64;
    case VEDIC_FLOAT:
        // Clamp to int64 range if needed
        if (value.value.f32 < (float)INT64_MIN)
            return INT64_MIN;
        if (value.value.f32 > (float)INT64_MAX)
            return INT64_MAX;
        return (int64_t)value.value.f32;
    case VEDIC_DOUBLE:
        // Clamp to int64 range if needed
        if (value.value.f64 < (double)INT64_MIN)
            return INT64_MIN;
        if (value.value.f64 > (double)INT64_MAX)
            return INT64_MAX;
        return (int64_t)value.value.f64;
    default:
        return 0;
    }
}

/**
 * Get the float value from a VedicValue
 */
float vedic_to_float(VedicValue value)
{
    switch (value.type)
    {
    case VEDIC_INT32:
        return (float)value.value.i32;
    case VEDIC_INT64:
        return (float)value.value.i64;
    case VEDIC_FLOAT:
        return value.value.f32;
    case VEDIC_DOUBLE:
        return (float)value.value.f64;
    default:
        return 0.0f;
    }
}

/**
 * Get the double value from a VedicValue
 */
double vedic_to_double(VedicValue value)
{
    switch (value.type)
    {
    case VEDIC_INT32:
        return (double)value.value.i32;
    case VEDIC_INT64:
        return (double)value.value.i64;
    case VEDIC_FLOAT:
        return (double)value.value.f32;
    case VEDIC_DOUBLE:
        return value.value.f64;
    default:
        return 0.0;
    }
}