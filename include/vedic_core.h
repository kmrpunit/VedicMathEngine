#ifndef VEDIC_CORE_H
#define VEDIC_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Multiplication Methods
int64_t multiply_urdhva(int64_t a, int64_t b);
int64_t multiply_nikhilam(int64_t a, int64_t b);
int64_t multiply_antyayor(int64_t a, int64_t b);
int64_t multiply_ekanyunena(int64_t a, int64_t all9s);
int64_t multiply_puranapuranabhyam(int64_t a, int64_t b);
int64_t multiply_anurupyena(int64_t a, int64_t b, int scale);

// Squaring Methods
int64_t square_ekadhikena(int64_t n);
int64_t square_yaavadunam(int64_t n);

// Division
int64_t divide_paravartya(int64_t dividend, int64_t divisor, int64_t *remainder);

// Equation Solving
int solve_linear_sankalana(int a1, int b1, int c1, int a2, int b2, int c2, double *x, double *y);
int solve_polynomial_shunyam(double *coefficients, int degree, double *root);

// Divisibility Test
int is_divisible_vestanam(int64_t number, int prime);

// Utilities
int64_t vedic_add(int64_t a, int64_t b);
int64_t vedic_subtract(int64_t a, int64_t b);

#ifdef __cplusplus
}
#endif

#endif // VEDIC_CORE_H
