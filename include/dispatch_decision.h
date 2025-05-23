#ifndef DISPATCH_DECISION_H
#define DISPATCH_DECISION_H

#include <stdint.h>

typedef struct {
    int64_t result;
    const char* method_used;
    const char* reason;
    const char* mode; // "standard", "dynamic", "optimized"
    double time_taken_ns;
} DispatchDecision;

#endif
