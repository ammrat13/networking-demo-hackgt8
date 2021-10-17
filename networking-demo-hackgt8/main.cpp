// Win32 API
#include <Windows.h>


int main(void) {

    // Get the console handle so we can print
    HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (stdout == NULL || stdout == INVALID_HANDLE_VALUE) {
        // Can't print anything, so just die
        return 1;
    }

    WriteConsoleA(stdout, "Hello world!\n", 13, NULL, NULL);

    // Success
    return 0;
}
