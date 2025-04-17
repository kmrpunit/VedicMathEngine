/**
 * vedicmath_platform.h - Cross-platform compatibility layer
 * 
 * This file provides platform-specific macros and functions to ensure
 * the Vedic Mathematics Library works correctly across different operating systems.
 */

 #ifndef VEDICMATH_PLATFORM_H
 #define VEDICMATH_PLATFORM_H
 
 #include <stddef.h>
 #include <stdlib.h>
 #include <string.h>
 
 // Platform detection
 #if defined(_WIN32) || defined(_WIN64)
     #define VEDICMATH_PLATFORM_WINDOWS
 #elif defined(__APPLE__)
     #define VEDICMATH_PLATFORM_MACOS
 #elif defined(__linux__)
     #define VEDICMATH_PLATFORM_LINUX
 #else
     #define VEDICMATH_PLATFORM_GENERIC
 #endif
 
 // API export/import macros for dynamic libraries (if needed)
 #if defined(VEDICMATH_PLATFORM_WINDOWS) && defined(VEDICMATH_BUILD_SHARED)
     #ifdef VEDICMATH_DLL_EXPORTS
         #define VEDICMATH_API __declspec(dllexport)
     #else
         #define VEDICMATH_API __declspec(dllimport)
     #endif
 #else
     #define VEDICMATH_API
 #endif
 
 // Safe string functions
 #ifdef VEDICMATH_PLATFORM_WINDOWS
     // Windows-specific safe string functions
     #define SAFE_SPRINTF sprintf_s
     #define SAFE_STRCPY(dest, size, src) strcpy_s(dest, size, src)
     #define SAFE_STRCAT(dest, size, src) strcat_s(dest, size, src)
     #define PLATFORM_STRDUP _strdup
 #else
     // Standard functions for other platforms
     #define SAFE_SPRINTF sprintf
     #define SAFE_STRCPY(dest, size, src) do { \
         strncpy(dest, src, (size)-1); \
         (dest)[(size)-1] = '\0'; \
     } while(0)
     #define SAFE_STRCAT(dest, size, src) strncat(dest, src, (size)-strlen(dest)-1)
     #define PLATFORM_STRDUP strdup
 #endif
 
 // Memory alignment macros
 #if defined(_MSC_VER)
     #define ALIGN8 __declspec(align(8))
 #elif defined(__GNUC__)
     #define ALIGN8 __attribute__((aligned(8)))
 #else
     #define ALIGN8
 #endif
 
 // Inline function directive
 #if defined(_MSC_VER)
     #define VEDICMATH_INLINE __forceinline
 #elif defined(__GNUC__)
     #define VEDICMATH_INLINE __attribute__((always_inline)) inline
 #else
     #define VEDICMATH_INLINE inline
 #endif
 
 // Platform-specific utility functions
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /**
  * Platform-independent string duplication
  */
 static VEDICMATH_INLINE char* vedicmath_strdup(const char* str) {
     if (str == NULL) {
         return NULL;
     }
     
     size_t len = strlen(str) + 1;
     char* dup = (char*)malloc(len);
     
     if (dup != NULL) {
         SAFE_STRCPY(dup, len, str);
     }
     
     return dup;
 }
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* VEDICMATH_PLATFORM_H */