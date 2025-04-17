# Vedic Mathematics Library - Project Structure

This document describes the organization of the Vedic Mathematics Library project.

## Directory Structure

```
VedicMathEngine/
├── CMakeLists.txt           # Main build configuration
├── DOCUMENTATION.md         # Comprehensive library documentation
├── LICENSE                  # License file
├── PROJECT_STRUCTURE.md     # This file
├── README.md                # Quick start guide
├── README_DYNAMIC.md        # Dynamic type system documentation
├── include/                 # Public header files
│   ├── vedicmath.h          # Main library header
│   ├── vedicmath_types.h    # Dynamic type system definitions
│   ├── vedicmath_dynamic.h  # Dynamic API declarations
│   └── vedicmath_optimized.h # Optimized API declarations
├── src/                     # Source files
│   ├── core/                # Core Vedic techniques
│   │   ├── ekadhikena_purvena.c       # "By one more than the previous one"
│   │   ├── nikhilam_navatashcaramam.c # "All from 9 and the last from 10"
│   │   ├── urdhva_tiryagbhyam.c       # "Vertically and crosswise"
│   │   ├── paravartya_yojayet.c       # "Transpose and adjust"
│   │   ├── yaavadunam.c               # "Whatever the extent of its deficiency"
│   │   ├── antyayordasake.c           # "Last totaling 10"
│   │   ├── sankalana_vyavakalanabhyam.c # "By addition and by subtraction"
│   │   ├── shunyam_saamyasamuccaye.c  # "When the sum is the same that sum is zero"
│   │   ├── ekanyunena_purvena.c       # "By one less than the previous one"
│   │   └── puranapuranabhyam.c        # "By the completion or non-completion"
│   ├── subsutra/            # Sub-sutra implementations
│   │   ├── vestanam.c        # "By osculation"
│   │   └── anurupyena.c      # "Proportionately"
│   ├── dynamic/             # Dynamic type system
│   │   ├── vedicmath_types.c    # Dynamic type implementation
│   │   └── vedicmath_dynamic.c  # Dynamic API implementation
│   ├── optimized/           # Optimized implementation
│   │   └── vedicmath_optimized.c # Optimized API implementation
│   └── common/              # Common utilities
│       ├── vedicmath_utils.c     # Utility functions
│       ├── vedicmath_dispatcher.c # Central dispatcher
│       └── vedicmath_operators.c  # Standard operators
├── tests/                  # Test files
│   ├── vedicmath_test.c           # Basic test program
│   ├── vedicmath_test_suite.c     # Comprehensive test suite
│   ├── vedicmath_test_main.c      # Test runner
│   └── vedicmath_dynamic_test.c   # Dynamic API tests
└── benchmarks/             # Benchmark files
    ├── vedicmath_benchmark.h   # Benchmark framework header
    ├── vedicmath_benchmark.c   # Benchmark implementation
    └── benchmark_main.c        # Benchmark runner
```

## Core Components

The library is organized around several key components:

### 1. Core Vedic Techniques (src/core/)

Each Vedic sutra is implemented in its own source file:

- **ekadhikena_purvena.c**: Squaring numbers ending in 5
- **nikhilam_navatashcaramam.c**: Multiplication near a base
- **urdhva_tiryagbhyam.c**: General multiplication
- **paravartya_yojayet.c**: Division method
- **yaavadunam.c**: Squaring near a base
- **antyayordasake.c**: Multiplication when last digits sum to 10
- **sankalana_vyavakalanabhyam.c**: Addition/subtraction methods
- **shunyam_saamyasamuccaye.c**: Equation solving
- **ekanyunena_purvena.c**: Multiplication by all 9s
- **puranapuranabhyam.c**: Calculations using rounding

### 2. Sub-Sutras (src/subsutra/)

Implementations of supplementary techniques:

- **vestanam.c**: Divisibility tests
- **anurupyena.c**: Proportional scaling

### 3. Dynamic Type System (src/dynamic/)

Type-aware implementation with automatic method selection:

- **vedicmath_types.c**: Dynamic type implementation
- **vedicmath_dynamic.c**: Dynamic API implementation

### 4. Optimized Implementation (src/optimized/)

Performance-optimized version with reduced overhead:

- **vedicmath_optimized.c**: Fast path implementations

### 5. Common Utilities (src/common/)

Shared functionality used across the library:

- **vedicmath_utils.c**: Utility functions
- **vedicmath_dispatcher.c**: Central dispatcher for method selection
- **vedicmath_operators.c**: Standard operator implementations

### 6. Tests (tests/)

Comprehensive testing framework:

- **vedicmath_test.c**: Basic test program
- **vedicmath_test_suite.c**: Comprehensive test suite
- **vedicmath_test_main.c**: Test runner
- **vedicmath_dynamic_test.c**: Dynamic API tests

### 7. Benchmarks (benchmarks/)

Performance benchmarking framework:

- **vedicmath_benchmark.h**: Benchmark framework header
- **vedicmath_benchmark.c**: Benchmark implementation
- **benchmark_main.c**: Benchmark runner

## Building and Development Workflow

1. **Setup**: Clone the repository and create a build directory
2. **Configure**: Run CMake to configure the build
3. **Build**: Compile the library and tests
4. **Test**: Run the test suites to verify functionality
5. **Benchmark**: Run benchmarks to measure performance
6. **Install**: Install the library for system-wide use

## Code Organization Principles

The library follows several key design principles:

### 1. Modularity

Each sutra is implemented in its own source file, allowing:
- Independent development and testing
- Selective compilation
- Clear separation of concerns

### 2. Multiple Interfaces

Three distinct APIs provide different trade-offs:
- **Standard API**: Simple, direct implementation for basic use
- **Dynamic API**: Flexible, type-aware interface for complex scenarios
- **Optimized API**: Performance-focused implementation for high-demand applications

### 3. Performance Optimization Techniques

Several techniques are used to optimize performance:
- **Function inlining**: Critical functions are marked `inline`
- **Fast paths**: Optimized code paths for common cases
- **Lookup tables**: Replace conditional branches with table lookups
- **Expression caching**: Store results of previously evaluated expressions
- **Batch processing**: Efficiently process multiple operations at once
- **OpenMP parallelization**: Utilize multiple CPU cores when available

### 4. Cross-Platform Compatibility

The library is designed to work across different platforms:
- Standard C99 compliance for portability
- CMake build system for cross-platform builds
- Platform-specific optimizations wrapped in preprocessor guards
- Fallback implementations when specialized features aren't available

### 5. Comprehensive Testing

Multiple testing approaches ensure reliability:
- Unit tests for individual functions
- Integration tests for complete operations
- Random tests with verification against standard operations
- Special case tests for Vedic patterns
- Benchmarks to ensure performance

## Dependencies

The library has minimal dependencies to maximize portability:
- Standard C library
- Math library (`-lm`)
- Optional: OpenMP for parallelization

## Extension Points

The library is designed to be extensible in several ways:

1. **Adding New Sutras**: Create a new source file and update the dispatcher

2. **Supporting New Types**: Extend the dynamic type system in `vedicmath_types.h/c`

3. **Additional Optimizations**: Add new fast paths in `vedicmath_optimized.c`

4. **Custom Operations**: Implement new operations in `vedicmath_operators.c`

5. **Language Bindings**: The C API can be wrapped for use in other languages

## Version Control Strategy

For development, the recommended branching strategy is:

- `main`: Stable release branch
- `develop`: Integration branch for next release
- `feature/xxx`: Feature development branches
- `bugfix/xxx`: Bug fix branches
- `release/x.y.z`: Release preparation branches
- `hotfix/x.y.z`: Hotfix branches for production issues

## Documentation

The project includes extensive documentation:

- **DOCUMENTATION.md**: Comprehensive user documentation
- **README.md**: Quick start guide
- **README_DYNAMIC.md**: Guide to the dynamic type system
- **PROJECT_STRUCTURE.md**: This document, explaining code organization
- **Function comments**: Detailed explanations in the source code
- **Examples**: Usage examples for different scenarios

## Continuous Integration

Recommended CI workflow:

1. Build on multiple platforms (Linux, Windows, macOS)
2. Run all tests
3. Run benchmarks and compare against baseline
4. Generate documentation
5. Create binary packages
