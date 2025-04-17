/**
 * benchmark_main.c - Main program to run the benchmarks
 */

 #include "vedicmath_benchmark.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 int main(int argc, char* argv[]) {
     // Default number of iterations
     size_t iterations = 1000000;
     
     // Check for command line arguments
     if (argc > 1) {
         // Try to parse the first argument as iteration count
         char* endptr;
         long count = strtol(argv[1], &endptr, 10);
         
         if (*endptr == '\0' && count > 0) {
             iterations = (size_t)count;
         } else {
             printf("Invalid iteration count. Using default: %zu\n", iterations);
         }
     }
     
     printf("Vedic Mathematics Library Benchmark\n");
     printf("==================================\n\n");
     printf("Running benchmarks with %zu iterations...\n", iterations);
     
     // Run all benchmarks
     run_all_benchmarks(iterations);
     
     return 0;
 }