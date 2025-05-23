#ifndef DISPATCH_MIXED_MODE_H
#define DISPATCH_MIXED_MODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// Set execution mode flags
void set_execution_mode(bool dynamic, bool optimized);
bool is_using_optimized_mode(void);

// Adaptive dispatcher functions
int64_t dispatch_multiply(int64_t a, int64_t b);
int64_t dispatch_square(int64_t n);
int64_t dispatch_divide(int64_t a, int64_t b, int64_t* rem_out);

#ifdef __cplusplus
}
#endif

#endif // DISPATCH_MIXED_MODE_H
