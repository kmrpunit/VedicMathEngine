/**
 * Cross-Platform UTF-8 Console Configuration
 * 
 * Ensures proper display of Unicode characters (✓, ², °, etc.) 
 * across different operating systems and platforms.
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
    
    // Windows UTF-8 console setup
    void set_utf8_console(void) {
        // Method 1: Use system command (your current approach)
        system("chcp 65001 > nul");  // Set UTF-8 code page, suppress output
        
        // Method 2: Direct Windows API (more reliable, commented as alternative)
        /*
        SetConsoleOutputCP(CP_UTF8);  // Set output to UTF-8
        SetConsoleCP(CP_UTF8);        // Set input to UTF-8
        
        // Enable virtual terminal processing for ANSI escape sequences
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
        */
        
        // Method 3: Set file mode to UTF-8 (for file output compatibility)
        /*
        _setmode(_fileno(stdout), _O_U8TEXT);
        _setmode(_fileno(stderr), _O_U8TEXT);
        */
    }
    
#elif defined(__linux__)
    #include <locale.h>
    #include <langinfo.h>
    
    // Linux UTF-8 locale setup
    void set_utf8_console(void) {
        // Set locale to UTF-8 (most Linux systems default to this)
        char* locale_result = setlocale(LC_ALL, "");
        
        if (locale_result == NULL) {
            // Fallback to common UTF-8 locales
            if (setlocale(LC_ALL, "en_US.UTF-8") == NULL) {
                if (setlocale(LC_ALL, "C.UTF-8") == NULL) {
                    setlocale(LC_ALL, "POSIX");
                    printf("Warning: Could not set UTF-8 locale\n");
                }
            }
        }
        
        // Verify UTF-8 is active
        char* codeset = nl_langinfo(CODESET);
        if (codeset && strstr(codeset, "UTF-8") == NULL) {
            printf("Warning: Current codeset is %s, not UTF-8\n", codeset);
        }
        
        // Ensure stdout uses UTF-8
        printf("\033[0m"); // Reset terminal formatting
    }
    
#elif defined(__APPLE__)
    #include <locale.h>
    
    // macOS UTF-8 setup (usually already UTF-8 by default)
    void set_utf8_console(void) {
        // macOS Terminal.app defaults to UTF-8, but set explicitly
        setlocale(LC_ALL, "");
        
        // macOS-specific: Ensure Terminal supports Unicode
        const char* term = getenv("TERM");
        if (term && (strstr(term, "xterm") || strstr(term, "screen"))) {
            // Terminal supports Unicode, nothing extra needed
        } else {
            printf("Warning: Terminal may not fully support Unicode\n");
        }
    }
    
#elif defined(ESP32_PLATFORM)
    // ESP32 UTF-8 setup (for serial console output)
    void set_utf8_console(void) {
        // ESP32 serial console typically handles UTF-8 well
        // But some terminal emulators may need configuration
        
        // Send UTF-8 BOM (Byte Order Mark) to help terminal detection
        printf("\xEF\xBB\xBF"); // UTF-8 BOM
        
        // Print a test character to verify UTF-8 support
        printf("UTF-8 test: ✓\n");
        
        // Note: The receiving terminal (PuTTY, Arduino IDE Serial Monitor, etc.)
        // needs to be configured for UTF-8 as well
    }
    
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
    #include <locale.h>
    
    // BSD systems UTF-8 setup
    void set_utf8_console(void) {
        // BSD systems often default to UTF-8
        setlocale(LC_ALL, "");
        
        // Try common UTF-8 locales for BSD
        if (setlocale(LC_ALL, "en_US.UTF-8") == NULL) {
            if (setlocale(LC_ALL, "C.UTF-8") == NULL) {
                printf("Warning: Could not set UTF-8 locale on BSD system\n");
            }
        }
    }
    
#elif defined(__ANDROID__)
    #include <locale.h>
    
    // Android UTF-8 setup (for NDK applications)
    void set_utf8_console(void) {
        // Android typically uses UTF-8 by default
        setlocale(LC_ALL, "C.UTF-8");
        
        // Android log output is usually UTF-8 compatible
        printf("Android UTF-8 console initialized\n");
    }
    
#else
    // Generic POSIX/Unix fallback
    void set_utf8_console(void) {
        #include <locale.h>
        
        // Try to set UTF-8 locale
        if (setlocale(LC_ALL, "") == NULL) {
            if (setlocale(LC_ALL, "C.UTF-8") == NULL) {
                if (setlocale(LC_ALL, "en_US.UTF-8") == NULL) {
                    printf("Warning: Could not set UTF-8 locale\n");
                }
            }
        }
        
        // Set environment variables as fallback
        setenv("LC_ALL", "C.UTF-8", 0);
        setenv("LANG", "C.UTF-8", 0);
    }
#endif

/**
 * Universal UTF-8 console initialization
 * Call this once at the beginning of main()
 */
void initialize_utf8_console(void) {
    printf("Initializing UTF-8 console support...\n");
    set_utf8_console();
    
    // Test UTF-8 characters to verify setup
    printf("UTF-8 Test Characters:\n");
    printf("  Checkmark: ✓\n");
    printf("  Cross: ✗\n");
    printf("  Superscript 2: ²\n");
    printf("  Degree: °\n");
    printf("  Bullet: •\n");
    printf("  Arrow: →\n");
    printf("UTF-8 console initialization complete.\n\n");
}

/**
 * Platform-specific UTF-8 troubleshooting tips
 */
void print_utf8_troubleshooting(void) {
    printf("\n=== UTF-8 Troubleshooting ===\n");
    
#ifdef _WIN32
    printf("Windows:\n");
    printf("- If characters still appear garbled, try running: chcp 65001\n");
    printf("- Use Windows Terminal instead of Command Prompt for better Unicode support\n");
    printf("- Ensure console font supports Unicode (Consolas, Cascadia Code, etc.)\n");
    
#elif defined(__linux__)
    printf("Linux:\n");
    printf("- Check current locale: locale\n");
    printf("- Install UTF-8 locales: sudo locale-gen en_US.UTF-8\n");
    printf("- Set environment: export LANG=en_US.UTF-8\n");
    printf("- Ensure terminal supports UTF-8\n");
    
#elif defined(__APPLE__)
    printf("macOS:\n");
    printf("- Terminal.app should support UTF-8 by default\n");
    printf("- Check Terminal preferences: Text tab → Character encoding → UTF-8\n");
    printf("- iTerm2 also has excellent Unicode support\n");
    
#elif defined(ESP32_PLATFORM)
    printf("ESP32:\n");
    printf("- Configure your terminal emulator (PuTTY, Tera Term, etc.) for UTF-8\n");
    printf("- Arduino IDE Serial Monitor: Set to UTF-8 in preferences\n");
    printf("- ESP-IDF Monitor: Usually handles UTF-8 automatically\n");
    
#else
    printf("Generic Unix/POSIX:\n");
    printf("- Set LANG environment variable: export LANG=en_US.UTF-8\n");
    printf("- Check if UTF-8 locale is installed\n");
    printf("- Ensure terminal emulator supports UTF-8\n");
#endif
    
    printf("=============================\n\n");
}