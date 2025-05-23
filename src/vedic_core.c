#include "vedic_core.h"
#include "vedicmath.h"

// Multiplication wrappers
int64_t multiply_urdhva(int64_t a, int64_t b) {
    return urdhva_mult(a, b);
}

int64_t multiply_nikhilam(int64_t a, int64_t b) {
    return nikhilam_mul(a, b);
}

int64_t multiply_antyayor(int64_t a, int64_t b) {
    return antya_dasake_mul((int)a, (int)b);
}

int64_t multiply_ekanyunena(int64_t a, int64_t all9s) {
    return ekanyunena_purvena_mul(a, all9s);
}

int64_t multiply_puranapuranabhyam(int64_t a, int64_t b) {
    return puranapuranabhyam_mul(a, b);
}

int64_t multiply_anurupyena(int64_t a, int64_t b, int scale) {
    return anurupyena_mul(a, b, scale);
}

// Squaring wrappers
int64_t square_ekadhikena(int64_t n) {
    return ekadhikena_purvena(n);
}

int64_t square_yaavadunam(int64_t n) {
    long base = nearest_power_of_10(n);
    return yaavadunam_square(n, base);
}

// Division wrapper
int64_t divide_paravartya(int64_t dividend, int64_t divisor, int64_t *remainder) {
    return paravartya_divide(dividend, divisor, remainder);
}

// Equation solving
int solve_linear_sankalana(int a1, int b1, int c1, int a2, int b2, int c2, double *x, double *y) {
    return sankalana_vyavakalanabhyam_solve(a1, b1, c1, a2, b2, c2, x, y);
}

int solve_polynomial_shunyam(double *coefficients, int degree, double *root) {
    return shunyam_samuccaye(coefficients, degree, root);
}

// Divisibility
int is_divisible_vestanam(int64_t number, int prime) {
    return vestanam_divisibility(number, prime);
}

// Basic utilities
int64_t vedic_add(int64_t a, int64_t b) {
    return vedic_add(a, b);
}

int64_t vedic_subtract(int64_t a, int64_t b) {
    return vedic_subtract(a, b);
}
