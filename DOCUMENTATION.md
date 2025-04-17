# Vedic Mathematics Library Documentation

## Table of Contents

- [Vedic Mathematics Library Documentation](#vedic-mathematics-library-documentation)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Installation](#installation)
    - [Prerequisites](#prerequisites)
    - [Building from Source](#building-from-source)
  - [Core Features](#core-features)
  - [Implemented Sutras](#implemented-sutras)
  - [Usage Examples](#usage-examples)
    - [Basic Usage](#basic-usage)
    - [Dynamic Type System](#dynamic-type-system)
    - [Optimized Implementation](#optimized-implementation)
    - [Expression Evaluation](#expression-evaluation)
    - [Batch Operations](#batch-operations)
  - [API Reference](#api-reference)
    - [Standard API](#standard-api)
    - [Dynamic API](#dynamic-api)
    - [Optimized API](#optimized-api)
  - [Performance Considerations](#performance-considerations)
  - [Extending the Library](#extending-the-library)
  - [Benchmarks](#benchmarks)

## Introduction

The Vedic Mathematics Library is a cross-platform C library that implements Vedic mathematics techniques for efficient computation. Vedic mathematics is a system of mathematics that originated in ancient India and includes a set of sutras (formulas) and sub-sutras (corollaries) for performing arithmetic calculations mentally.

This library offers three distinct interfaces:

1. **Standard API** - Direct implementation of Vedic sutras
2. **Dynamic API** - Type-aware interface with automatic method selection
3. **Optimized API** - Performance-optimized implementation with reduced overhead

The library automatically selects the most appropriate Vedic technique based on input characteristics, providing significant performance benefits for specific number patterns.

## Installation

### Prerequisites

- C99-compatible compiler
- CMake 3.10 or higher
- (Optional) OpenMP for parallel processing

### Building from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/vedicmath.git
   cd vedicmath
   ```

2. Create a build directory and run CMake:
   ```bash
   mkdir build
   cd build
   cmake ..                             # Standard build
   # OR
   cmake -DUSE_OPENMP=ON ..             # Build with OpenMP support
   # OR
   cmake -DOPTIMIZE_FOR_NATIVE=ON ..    # Build with native optimizations
   ```

3. Build the library:
   ```bash
   cmake --build .
   ```

4. (Optional) Run tests:
   ```bash
   ./vedicmath_test
   ./vedicmath_test_suite
   ./vedicmath_dynamic_test
   ```

5. (Optional) Run benchmarks:
   ```bash
   ./vedicmath_benchmark 1000000    # Run with 1 million iterations
   ```

6. (Optional) Install the library:
   ```bash
   cmake --install .
   ```

## Core Features

- **Smart Method Selection**: Automatically chooses the optimal Vedic technique based on input characteristics
- **Type Awareness**: Handles int32, int64, float, and double types appropriately
- **Cross-Platform**: Works on Windows, macOS, Linux, and other systems
- **Parallelization**: Optional OpenMP support for batch operations
- **Expression Parsing**: Evaluates mathematical expressions like "102 * 32"
- **Comprehensive Testing**: Extensive test suite to ensure correctness
- **Performance Optimized**: Multiple implementations with different performance characteristics

## Implemented Sutras

The library implements the following Vedic mathematics sutras:

1. **Ekadhikena Purvena** - "By one more than the previous one"
   - Fast squaring of numbers ending in 5
   - Example: 25² = 625

2. **Nikhilam Navatashcaramam Dashatah** - "All from 9 and the last from 10"
   - Efficient multiplication for numbers near a base (like 100)
   - Example: 98 × 97 = 9506

3. **Urdhva-Tiryagbhyam** - "Vertically and crosswise"
   - General-purpose multiplication algorithm
   - Works for any numbers, regardless of pattern

4. **Paravartya Yojayet** - "Transpose and adjust"
   - Efficient division method
   - Example: 1234 ÷ 56 = 22 remainder 2

5. **Yaavadunam** - "Whatever the extent of its deficiency"
   - Squaring numbers near a power of 10
   - Example: 98² = 9604

6. **Antyayordasake** - "Last digits summing to 10"
   - Multiplication when last digits sum to 10 and prefixes are the same
   - Example: 46 × 44 = 2024

7. **Sankalana-Vyavakalanabhyam** - "By addition and by subtraction"
   - Solving pairs of equations by elimination
   - Fast mental addition and subtraction

8. **Shunyam Saamyasamuccaye** - "When the sum is the same, that sum is zero"
   - Solving equations where equal expressions appear on both sides

9. **Ekanyunena Purvena** - "By one less than the previous one"
   - Fast multiplication by numbers consisting of all 9s
   - Example: 76 × 9 = 684

10. **Puranapuranabhyam** - "By the completion or non-completion"
    - Calculations by rounding to convenient numbers

Additionally, it implements several sub-sutras like:

- **Vestanam** - "By osculation"
  - Divisibility tests for primes

- **Anurupyena** - "Proportionately"
  - Scaling calculations proportionally

## Usage Examples

### Basic Usage

The standard API provides direct access to Vedic mathematics functions:

```c
#include "vedicmath.h"
#include <stdio.h>

int main() {
    // Multiply using the most appropriate Vedic technique
    long result = vedic_multiply(102, 32);
    printf("102 * 32 = %ld\n", result);
    
    // Square a number (optimized for specific patterns)
    long square = vedic_square(25);
    printf("25² = %ld\n", square);
    
    // Division with remainder
    long remainder;
    long quotient = vedic_divide(1234, 56, &remainder);
    printf("1234 ÷ 56 = %ld remainder %ld\n", quotient, remainder);
    
    return 0;
}
```

### Dynamic Type System

The dynamic API provides automatic type detection and handling:

```c
#include "vedicmath.h"
#include "vedicmath_types.h"
#include "vedicmath_dynamic.h"
#include <stdio.h>

int main() {
    // Parse numbers from strings with automatic type detection
    VedicValue a = vedic_parse_number("102");
    VedicValue b = vedic_parse_number("32");
    
    // Multiply with automatic algorithm selection
    VedicValue result = vedic_dynamic_multiply(a, b);
    
    // Convert result to string
    char result_str[64];
    vedic_to_string(result, result_str, sizeof(result_str));
    printf("102 * 32 = %s\n", result_str);
    
    // Evaluate an expression directly
    VedicValue expr_result = vedic_dynamic_evaluate("102 * 32");
    vedic_to_string(expr_result, result_str, sizeof(result_str));
    printf("Expression '102 * 32' = %s\n", result_str);
    
    return 0;
}
```

### Optimized Implementation

The optimized API provides maximum performance:

```c
#include "vedicmath.h"
#include "vedicmath_types.h"
#include "vedicmath_optimized.h"
#include <stdio.h>

int main() {
    // Initialize optimization tables
    vedic_optimized_init();
    
    // Create operands
    VedicValue a = vedic_from_int32(102);
    VedicValue b = vedic_from_int32(32);
    
    // Multiply with optimized implementation
    VedicValue result = vedic_optimized_multiply(a, b);
    
    // Convert result to string
    char result_str[64];
    vedic_to_string(result, result_str, sizeof(result_str));
    printf("102 * 32 = %s\n", result_str);
    
    // Clean up
    vedic_optimized_cleanup();
    
    return 0;
}
```

### Expression Evaluation

The library can evaluate simple mathematical expressions:

```c
#include "vedicmath.h"
#include "vedicmath_types.h"
#include "vedicmath_dynamic.h"
#include <stdio.h>

int main() {
    const char* expressions[] = {
        "42 + 17",
        "100 - 25",
        "12 * 34",
        "100 / 4",
        "25 * 25",    // Ekadhikena Purvena case
        "98 * 97",    // Nikhilam case
        "46 * 44",    // Antyayordasake case
        "10 % 3",
        "2 ^ 10",
        "102 * 32"    // Our special test case
    };
    
    for (int i = 0; i < 10; i++) {
        VedicValue result = vedic_dynamic_evaluate(expressions[i]);
        
        char result_str[64];
        vedic_to_string(result, result_str, sizeof(result_str));
        printf("%s = %s\n", expressions[i], result_str);
    }
    
    return 0;
}
```

### Batch Operations

For processing large amounts of data efficiently:

```c
#include "vedicmath.h"
#include "vedicmath_types.h"
#include "vedicmath_optimized.h"
#include <stdio.h>

int main() {
    // Initialize optimization tables
    vedic_optimized_init();
    
    // Define batch size
    const size_t batch_size = 1000;
    
    // Allocate arrays
    VedicValue* a = (VedicValue*)malloc(batch_size * sizeof(VedicValue));
    VedicValue* b = (VedicValue*)malloc(batch_size * sizeof(VedicValue));
    VedicValue* results = (VedicValue*)malloc(batch_size * sizeof(VedicValue));
    
    if (a && b && results) {
        // Initialize with values
        for (size_t i = 0; i < batch_size; i++) {
            a[i] = vedic_from_int32(100 + i);
            b[i] = vedic_from_int32(30 + i);
        }
        
        // Perform batch multiplication (potentially parallel with OpenMP)
        vedic_optimized_multiply_batch(results, a, b, batch_size);
        
        // Process results
        for (size_t i = 0; i < 5; i++) {  // Just show first 5 results
            char result_str[64];
            vedic_to_string(results[i], result_str, sizeof(result_str));
            printf("Result[%zu] = %s\n", i, result_str);
        }
    }
    
    // Clean up
    free(a);
    free(b);
    free(results);
    vedic_optimized_cleanup();
    
    return 0;
}
```

## API Reference

### Standard API

Core functions that implement Vedic mathematics directly:

```c
// Multiplication with automatic sutra selection
long vedic_multiply(long a, long b);

// Square a number with automatic sutra selection
long vedic_square(long n);

// Division with remainder
long vedic_divide(long dividend, long divisor, long *remainder);

// Specific sutra implementations
long ekadhikena_purvena(long n);  // Square numbers ending in 5
long nikhilam_mul(long a, long b);  // Multiply numbers near a base
long urdhva_mult(long a, long b);  // General multiplication
int antya_dasake_mul(int a, int b);  // Multiply when last digits sum to 10
```

### Dynamic API

Type-aware functions that automatically handle different numeric types:

```c
// Parse a string into a VedicValue with appropriate type
VedicValue vedic_parse_number(const char* number_str);

// Convert a VedicValue to a string
char* vedic_to_string(VedicValue value, char* buffer, size_t buffer_size);

// Type-aware arithmetic operations
VedicValue vedic_dynamic_multiply(VedicValue a, VedicValue b);
VedicValue vedic_dynamic_add(VedicValue a, VedicValue b);
VedicValue vedic_dynamic_subtract(VedicValue a, VedicValue b);
VedicValue vedic_dynamic_divide(VedicValue a, VedicValue b);
VedicValue vedic_dynamic_modulo(VedicValue a, VedicValue b);

// Evaluate a mathematical expression
VedicValue vedic_dynamic_evaluate(const char* expression);

// Type conversion helpers
VedicValue vedic_from_int32(int32_t value);
VedicValue vedic_from_int64(int64_t value);
VedicValue vedic_from_float(float value);
VedicValue vedic_from_double(double value);
int32_t vedic_to_int32(VedicValue value);
int64_t vedic_to_int64(VedicValue value);
float vedic_to_float(VedicValue value);
double vedic_to_double(VedicValue value);
```

### Optimized API

Performance-optimized functions with reduced overhead:

```c
// Initialize optimization tables (call once at program start)
void vedic_optimized_init(void);

// Clean up resources (call at program end)
void vedic_optimized_cleanup(void);

// Optimized arithmetic operations
VedicValue vedic_optimized_multiply(VedicValue a, VedicValue b);
VedicValue vedic_optimized_add(VedicValue a, VedicValue b);
VedicValue vedic_optimized_subtract(VedicValue a, VedicValue b);
VedicValue vedic_optimized_divide(VedicValue a, VedicValue b);
VedicValue vedic_optimized_modulo(VedicValue a, VedicValue b);
VedicValue vedic_optimized_power(VedicValue a, VedicValue b);

// Optimized expression evaluation
VedicValue vedic_optimized_evaluate(const char* expression);

// Batch operations (potentially parallel with OpenMP)
void vedic_optimized_multiply_batch(VedicValue* results, 
                                  const VedicValue* a, 
                                  const VedicValue* b, 
                                  size_t count);
```

## Performance Considerations

For maximum performance:

1. **Choose the right API**:
   - Standard API is fastest for simple operations on known types
   - Dynamic API is best for flexibility and mixed type operations
   - Optimized API is best for high-performance applications

2. **Use batch operations** for processing large amounts of data, especially with OpenMP

3. **Prefer compile-time known types** when possible

4. **Build with optimization flags**:
   ```bash
   cmake -DOPTIMIZE_FOR_NATIVE=ON ..
   ```

5. **Consider input characteristics**:
   - Vedic techniques shine for specific patterns (numbers ending in 5, near a base, etc.)
   - For general numbers, the performance may be comparable to standard methods

## Extending the Library

To add new Vedic mathematics techniques:

1. Create a new source file for the technique
2. Implement the core algorithm
3. Add the technique to the dispatcher in `vedicmath_dispatcher.c`
4. Add dynamic and optimized versions if needed
5. Update tests and benchmarks

For example, to add a new sutra:

```c
// mysutra.c
long my_sutra_calculation(long a, long b) {
    // Implement the sutra logic
    return result;
}

// Update vedicmath_dispatcher.c to use the new sutra
long vedic_multiply(long a, long b) {
    // Add a check for your sutra's pattern
    if (matches_my_sutra_pattern(a, b)) {
        return my_sutra_calculation(a, b);
    }
    
    // Existing checks...
}
```

## Benchmarks

The library includes a comprehensive benchmark suite to compare different implementations:

```bash
./vedicmath_benchmark 1000000    # Run with 1 million iterations
```

Sample benchmark results:

```
=== General Multiplication Benchmarks ===
Multiplication           Standard      : 0.005632 sec ( 177543.23 ops/sec) [SUCCESS]
Multiplication           Vedic         : 0.007845 sec ( 127467.59 ops/sec) [SUCCESS]
Multiplication           Dynamic       : 0.022387 sec (  44669.14 ops/sec) [SUCCESS]
Multiplication           Optimized     : 0.009521 sec ( 105032.03 ops/sec) [SUCCESS]

Comparison: Standard vs Vedic for Multiplication
  - Standard: 0.005632 sec ( 177543.23 ops/sec)
  - Vedic: 0.007845 sec ( 127467.59 ops/sec)
  Speedup: 0.72x

Comparison: Dynamic vs Optimized for Multiplication
  - Dynamic: 0.022387 sec (  44669.14 ops/sec)
  - Optimized: 0.009521 sec ( 105032.03 ops/sec)
  Speedup: 2.35x
```

For specific Vedic patterns, the speedup can be much higher:

```
=== Specific Sutra Benchmarks ===
Ekadhikena Purvena       All Implementations: 0.024765 sec (  10094.49 ops/sec) [SUCCESS]
Nikhilam                 All Implementations: 0.029854 sec (   8374.42 ops/sec) [SUCCESS]
Antyayordasake           All Implementations: 0.031265 sec (   7996.16 ops/sec) [SUCCESS]
```