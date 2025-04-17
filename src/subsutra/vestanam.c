/**
 * vestanam.c - Implementation of "By Osculation" sub-sutra
 * 
 * This sub-sutra provides methods for testing divisibility by applying
 * a factor to the last digit and adding/subtracting from the rest.
 */

 #include "vedicmath.h"

 /**
  * Get the osculation factor for a prime divisor
  * 
  * @param prime The prime number to find the factor for
  * @return The osculation factor, -1 if not a supported prime
  */
 static int get_osculation_factor(int prime) {
     switch (prime) {
         case 7:  return 2;  // Multiply last digit by 2, subtract from rest
         case 11: return 1;  // Alternating addition/subtraction of digits
         case 13: return 4;  // Multiply last digit by 4, add to rest
         case 17: return 5;  // Multiply last digit by 5, subtract from rest
         case 19: return 2;  // Multiply last digit by 2, add to rest
         case 23: return 7;  // Multiply last digit by 7, add to rest
         case 29: return 3;  // Multiply last digit by 3, add to rest
         case 31: return 3;  // Multiply last digit by 3, subtract from rest
         case 37: return 11; // Multiply last digit by 11, subtract from rest
         case 41: return 4;  // Multiply last digit by 4, subtract from rest
         case 43: return 13; // Multiply last digit by 13, add to rest
         case 47: return 14; // Multiply last digit by 14, subtract from rest
         case 53: return 16; // Multiply last digit by 16, subtract from rest
         default: return -1; // Not a supported prime
     }
 }
 
 /**
  * Get whether to add or subtract the osculation result
  * 
  * @param prime The prime number
  * @return 1 for addition, -1 for subtraction
  */
 static int get_osculation_operation(int prime) {
     switch (prime) {
         case 7:  return -1; // Subtract
         case 11: return 0;  // Special: alternating
         case 13: return 1;  // Add
         case 17: return -1; // Subtract
         case 19: return 1;  // Add
         case 23: return 1;  // Add
         case 29: return 1;  // Add
         case 31: return -1; // Subtract
         case 37: return -1; // Subtract
         case 41: return -1; // Subtract
         case 43: return 1;  // Add
         case 47: return -1; // Subtract
         case 53: return -1; // Subtract
         default: return 0;  // Not defined
     }
 }
 
 /**
  * Vestanam - "By Osculation"
  * 
  * Purpose: Test if a number is divisible by certain primes by applying
  * a factor to the last digit and adding/subtracting from the rest
  * 
  * When to use: For quickly determining divisibility by primes like 7, 13, 17, etc.
  * 
  * @param number The number to check
  * @param prime The prime divisor to test
  * @return 1 if divisible, 0 if not, -1 if prime not supported
  */
 int vestanam_divisibility(long number, int prime) {
     // Handle basic cases
     if (number == 0) return 1;  // 0 is divisible by everything
     
     // Take absolute value
     if (number < 0) number = -number;
     
     // Handle small primes with well-known divisibility tests
     if (prime == 2) return (number % 2 == 0) ? 1 : 0;
     if (prime == 3) {
         int sum = 0;
         long temp = number;
         while (temp > 0) {
             sum += temp % 10;
             temp /= 10;
         }
         return (sum % 3 == 0) ? 1 : 0;
     }
     if (prime == 5) return (number % 10 == 0 || number % 10 == 5) ? 1 : 0;
     
     // Get the osculation factor and operation
     int factor = get_osculation_factor(prime);
     int operation = get_osculation_operation(prime);
     
     if (factor == -1) return -1;  // Prime not supported
     
     // Special case for 11 (alternating sum)
     if (prime == 11) {
         int alternating_sum = 0;
         long temp = number;
         int position = 0;
         
         while (temp > 0) {
             if (position % 2 == 0) {
                 alternating_sum += temp % 10;
             } else {
                 alternating_sum -= temp % 10;
             }
             temp /= 10;
             position++;
         }
         
         return (alternating_sum % 11 == 0) ? 1 : 0;
     }
     
     // Apply osculation for other primes
     long temp = number;
     while (temp >= prime) {
         // Extract last digit
         int last_digit = temp % 10;
         
         // Remove last digit
         temp /= 10;
         
         // Apply osculation
         if (operation > 0) {
             // Add
             temp += last_digit * factor;
         } else {
             // Subtract
             temp -= last_digit * factor;
         }
         
         // Take absolute value if negative
         if (temp < 0) temp = -temp;
     }
     
     // Check if result is divisible by the prime
     return (temp % prime == 0) ? 1 : 0;
 }
 
 /**
  * Test if a number is divisible by 7
  * 
  * Special case of Vestanam
  * 
  * @param number The number to check
  * @return 1 if divisible by 7, 0 if not
  */
 int is_divisible_by_7(long number) {
     return vestanam_divisibility(number, 7);
 }
 
 /**
  * Test if a number is divisible by 13
  * 
  * Special case of Vestanam
  * 
  * @param number The number to check
  * @return 1 if divisible by 13, 0 if not
  */
 int is_divisible_by_13(long number) {
     return vestanam_divisibility(number, 13);
 }