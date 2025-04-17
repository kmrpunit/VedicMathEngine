/**
 * vedicmath_benchmark.h - Performance benchmark framework
 * 
 * This file defines the benchmark framework for comparing the performance
 * of different Vedic math implementations.
 */

 #ifndef VEDICMATH_BENCHMARK_H
 #define VEDICMATH_BENCHMARK_H
 
 #include <stddef.h>
 #include <time.h>
 
 /**
  * Benchmark result structure
  */
 typedef struct {
     const char* name;            // Name of the benchmark
     const char* implementation;  // Implementation being benchmarked
     double elapsed_time;         // Time in seconds
     double operations_per_sec;   // Operations per second
     size_t iterations;           // Number of iterations performed
     int success;                 // 1 if benchmark succeeded, 0 if failed
 } BenchmarkResult;
 
 /**
  * Run a benchmark function and measure its performance
  * 
  * @param name Benchmark name
  * @param implementation Implementation name
  * @param func Function to benchmark
  * @param iterations Number of iterations to run
  * @param data User data to pass to the function
  * @return Benchmark result
  */
 BenchmarkResult run_benchmark(
     const char* name,
     const char* implementation,
     int (*func)(size_t iterations, void* data),
     size_t iterations,
     void* data
 );
 
 /**
  * Print a benchmark result
  * 
  * @param result The benchmark result to print
  */
 void print_benchmark_result(const BenchmarkResult* result);
 
 /**
  * Compare two benchmark results and print the speedup
  * 
  * @param baseline The baseline benchmark result
  * @param optimized The optimized benchmark result
  */
 void print_benchmark_comparison(const BenchmarkResult* baseline, const BenchmarkResult* optimized);
 
 /**
  * Run a standard set of benchmarks on all implementations
  * 
  * @param count Number of iterations for each benchmark
  */
 void run_all_benchmarks(size_t count);
 
 /**
  * Benchmark specific operations
  */
 
 // Benchmark standard multiplication
 int benchmark_standard_multiply(size_t iterations, void* data);
 
 // Benchmark original Vedic multiplication
 int benchmark_vedic_multiply(size_t iterations, void* data);
 
 // Benchmark dynamic multiplication
 int benchmark_dynamic_multiply(size_t iterations, void* data);
 
 // Benchmark optimized multiplication
 int benchmark_optimized_multiply(size_t iterations, void* data);
 
 // Benchmark standard expression evaluation
 int benchmark_standard_evaluate(size_t iterations, void* data);
 
 // Benchmark dynamic expression evaluation
 int benchmark_dynamic_evaluate(size_t iterations, void* data);
 
 // Benchmark optimized expression evaluation
 int benchmark_optimized_evaluate(size_t iterations, void* data);
 
 // Benchmark batch operations
 int benchmark_batch_multiply_standard(size_t iterations, void* data);
 int benchmark_batch_multiply_optimized(size_t iterations, void* data);
 
 // Benchmark special cases
 int benchmark_ekadhikena_purvena(size_t iterations, void* data);
 int benchmark_nikhilam_mul(size_t iterations, void* data);
 int benchmark_antyayordasake(size_t iterations, void* data);
 
 #endif /* VEDICMATH_BENCHMARK_H */