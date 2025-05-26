/**
 * vedicmath_test_main.c - Main driver for the Vedic Mathematics Library test suite
 */

#include "vedicmath.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function declarations from test suite
void print_test_summary();
void test_ekadhikena_purvena();
void test_nikhilam_mul();
void test_urdhva_tiryagbhyam();
void test_yaavadunam();
void test_antyayordasake();
void test_paravartya_yojayet();
void test_vestanam();
void test_ekanyunena_purvena();
void test_puranapuranabhyam();
void test_anurupyena();
void test_sankalana_vyavakalanabhyam();
void test_operators();
void test_central_dispatcher();
void test_random_operations();
void test_dhvajanka_division();
void test_nikhilam_division();
void test_enhanced_division();

int main(int argc, char *argv[])
{
    printf("Vedic Mathematics Library - Comprehensive Test Suite\n");
    printf("==================================================\n\n");
    printf("Library version: %s\n\n", vedicmath_version());

    // Display menu if no arguments provided
    if (argc == 1)
    {
        printf("Available test suites:\n");
        printf("  1. Ekadhikena Purvena (squaring numbers ending in 5)\n");
        printf("  2. Nikhilam (multiplication near a base)\n");
        printf("  3. Urdhva-Tiryagbhyam (general multiplication)\n");
        printf("  4. Yaavadunam (squaring near a base)\n");
        printf("  5. Antyayordasake (Last digits summing to 10)\n");
        printf("  6. Paravartya Yojayet (division)\n");
        printf("  7. Vestanam (divisibility tests)\n");
        printf("  8. Ekanyunena Purvena (multiplication by 9s)\n");
        printf("  9. Puranapuranabhyam (completion method)\n");
        printf(" 10. Anurupyena (proportional scaling)\n");
        printf(" 11. Sankalana-Vyavakalanabhyam (equation solving)\n");
        printf(" 12. Mathematical operators\n");
        printf(" 13. Central dispatcher\n");
        printf(" 14. Random operations\n");
        printf(" 15. All tests\n");
        printf("\nUsage: %s [test_number]\n", argv[0]);
        return 0;
    }

    // Parse the test number from arguments
    int test_number = atoi(argv[1]);

    // Run specific test or all tests
    switch (test_number)
    {
    case 1:
        printf("Running Ekadhikena Purvena tests...\n\n");
        test_ekadhikena_purvena();
        break;

    case 2:
        printf("Running Nikhilam tests...\n\n");
        test_nikhilam_mul();
        break;

    case 3:
        printf("Running Urdhva-Tiryagbhyam tests...\n\n");
        test_urdhva_tiryagbhyam();
        break;

    case 4:
        printf("Running Yaavadunam tests...\n\n");
        test_yaavadunam();
        break;

    case 5:
        printf("Running Antyayordasake tests...\n\n");
        test_antyayordasake();
        break;

    case 6:
        printf("Running Paravartya Yojayet tests...\n\n");
        test_paravartya_yojayet();
        break;

    case 7:
        printf("Running Vestanam tests...\n\n");
        test_vestanam();
        break;

    case 8:
        printf("Running Ekanyunena Purvena tests...\n\n");
        test_ekanyunena_purvena();
        break;

    case 9:
        printf("Running Puranapuranabhyam tests...\n\n");
        test_puranapuranabhyam();
        break;

    case 10:
        printf("Running Anurupyena tests...\n\n");
        test_anurupyena();
        break;

    case 11:
        printf("Running Sankalana-Vyavakalanabhyam tests...\n\n");
        test_sankalana_vyavakalanabhyam();
        break;

    case 12:
        printf("Running mathematical operator tests...\n\n");
        test_operators();
        break;

    case 13:
        printf("Running central dispatcher tests...\n\n");
        test_central_dispatcher();
        break;

    case 14:
        printf("Running random operation tests...\n\n");
        test_random_operations();
        break;

    case 15:
        printf("Running ALL tests...\n\n");

        printf("=== Ekadhikena Purvena Tests ===\n");
        test_ekadhikena_purvena();
        printf("\n");

        printf("=== Nikhilam Tests ===\n");
        test_nikhilam_mul();
        printf("\n");

        printf("=== Urdhva-Tiryagbhyam Tests ===\n");
        test_urdhva_tiryagbhyam();
        printf("\n");

        printf("=== Yaavadunam Tests ===\n");
        test_yaavadunam();
        printf("\n");

        printf("=== Antyayordasake Tests ===\n");
        test_antyayordasake();
        printf("\n");

        printf("=== Paravartya Yojayet Tests ===\n");
        test_paravartya_yojayet();
        printf("\n");

        printf("=== Ekanyunena Purvena Tests ===\n");
        test_ekanyunena_purvena();
        printf("\n");

        printf("=== Puranapuranabhyam Tests ===\n");
        test_puranapuranabhyam();
        printf("\n");

        printf("=== Anurupyena Tests ===\n");
        test_anurupyena();
        printf("\n");

        printf("=== Sankalana-Vyavakalanabhyam Tests ===\n");
        test_sankalana_vyavakalanabhyam();
        printf("\n");

        printf("=== Mathematical Operator Tests ===\n");
        test_operators();
        printf("\n");

        printf("=== Central Dispatcher Tests ===\n");
        test_central_dispatcher();
        printf("\n");

        printf("=== Random Operation Tests ===\n");
        test_random_operations();
        printf("\n");

        printf("=== Dhvajanka Division Tests ===\n");
        test_dhvajanka_division();
        printf("\n");

        printf("=== Nikhilam Division Tests ===\n");
        test_nikhilam_division();
        printf("\n");

        printf("=== Enhanced Division Tests ===\n");
        test_enhanced_division();
        printf("\n");

        printf("=== Vestanam Tests ===\n");
        test_vestanam();
        printf("\n");

        break;

    default:
        printf("Invalid test number. Please choose a number between 1 and 15.\n");
        return 1;
    }

    // Print test summary
    print_test_summary();

    return 0;
}