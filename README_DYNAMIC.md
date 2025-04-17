# Dynamic Type System for Vedic Mathematics Library

The dynamic type system is an additional interface for the Vedic Mathematics Library that automatically detects and handles different numeric types. This makes the library much more flexible and easier to use, especially when working with mixed types or when the exact type is not known at compile time.

## Features

- **Automatic Type Detection**: Automatically selects int32, int64, float, or double based on input values
- **Type Promotion**: Intelligently promotes types when performing operations (e.g., int32 + float = float)
- **String Parsing**: Parses numbers from strings with automatic type detection
- **Expression Evaluation**: Evaluates simple mathematical expressions like "102 * 32"
- **Interactive Calculator**: Provides a simple command-line calculator

## Types Supported

The dynamic system supports four main numeric types:

1. **VEDIC_INT32**: 32-bit signed integers (-2,147,483,648 to 2,147,483,647)
2. **VEDIC_INT64**: 64-bit signed integers (for larger whole numbers)
3. **VEDIC_FLOAT**: Single-precision floating point (for simple decimal values)
4. **VEDIC_DOUBLE**: Double-precision floating point (for high precision decimals)

## Usage

### Basic Usage

```c
#include "vedicmath.h"
#include "vedicmath_types.h"
#include "vedicmath_dynamic.h"
#include <stdio.h>

int main() {
    // Parse numbers from strings with automatic type detection
    VedicValue a = vedic_parse_number("102");
    VedicValue b = vedic_parse_number("32");
    
    // Perform multiplication with automatic algorithm selection
    VedicValue result = vedic_dynamic_multiply(a, b);
    
    // Convert result to string and print
    char result_str[64];
    vedic_to_string(result, result_str, sizeof(result_str));
    printf("102 * 32 = %s\n", result_str);
    
    return 0;
}
```

### Expression Evaluation

```c
#include "vedicmath.h"
#include "vedicmath_types.h"
#include "vedicmath_dynamic.h"
#include <stdio.h>

int main() {
    // Evaluate a mathematical expression
    VedicValue result = vedic_dynamic_evaluate("102 * 32");
    
    // Convert result to string and print
    char result_str[64];
    vedic_to_string(result, result_str, sizeof(result_str));
    printf("Result: %s\n", result_str);
    
    return 0;
}
```

### Command-Line Calculator

The dynamic system includes a simple command-line calculator:

```
$ ./vedicmath_dynamic_test
Vedic Mathematics Library - Dynamic Type System Test
==================================================

[Test results will display here]

=== Vedic Math Dynamic Calculator ===
Enter expressions (e.g., '102 * 32') or 'q' to quit

Expression > 102 * 32
Result: 3264 (type: int32)

Expression > 25 * 25
Result: 625 (type: int32)

Expression > 3.14159 * 2
Result: 6.28318 (type: float)

Expression > q
Goodbye!
```

You can also evaluate expressions directly from the command line:

```
$ ./vedicmath_dynamic_test "102 * 32"
Evaluating: 102 * 32
Result: 3264 (type: int32)
```

## Supported Operations

The dynamic system supports all standard mathematical operations:

- **Addition**: `vedic_dynamic_add(a, b)` or `a + b` in expressions
- **Subtraction**: `vedic_dynamic_subtract(a, b)` or `a - b` in expressions
- **Multiplication**: `vedic_dynamic_multiply(a, b)` or `a * b` in expressions
- **Division**: `vedic_dynamic_divide(a, b)` or `a / b` in expressions
- **Modulo**: `vedic_dynamic_modulo(a, b)` or `a % b` in expressions
- **Power**: `a ^ b` in expressions

## Benefits

- **Simplified Interface**: No need to choose the correct function based on data types
- **Smart Algorithm Selection**: Automatically selects the most efficient Vedic method
- **String Parsing**: Easily work with numeric input from users or files
- **Type Safety**: Proper handling of type conversions and promotions

This dynamic system makes the Vedic Mathematics Library even more powerful and easier to use, especially for applications where the input types may vary or are not known at compile time.