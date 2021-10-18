// Needed to use WinSock2 with Windows.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Win32 API
#include <Windows.h>

// Windows Sockets 2
// See: https://docs.microsoft.com/en-us/windows/win32/winsock/creating-a-basic-winsock-application
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")



int main(void) {

    // Get the console handle so we can print
    HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (stdout == NULL || stdout == INVALID_HANDLE_VALUE) {
        // Can't print anything, so just die
        return 1;
    }


    // Initialize the WinSock library
    // See: https://docs.microsoft.com/en-us/windows/win32/winsock/initializing-winsock
    WSAData wsaData;
    {
        int res = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (res != 0) {
            WriteConsoleA(stdout, "Failed to initialize WinSock2\n", 30, NULL, NULL);
            return 2;
        }
    }

    // Create a socket
    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock == INVALID_SOCKET) {
        WriteConsoleA(stdout, "Failed to create socket\n", 24, NULL, NULL);
        WSACleanup();
        return 3;
    }

    // Bind the socket
    // The address is given here
    SOCKADDR_IN addr = {
        AF_INET,
        0xb822, /* Port 8888 */
        INADDR_ANY
    };
    {
        int res = bind(serverSock, (SOCKADDR*)&addr, sizeof(addr));
        if (res == SOCKET_ERROR) {
            WriteConsoleA(stdout, "Failed to bind socket\n", 22, NULL, NULL);
            closesocket(serverSock);
            WSACleanup();
            return 4;
        }
    }

    // Listen on the socket
    {
        int res = listen(serverSock, SOMAXCONN);
        if (res == SOCKET_ERROR) {
            WriteConsoleA(stdout, "Failed to listen on socket\n", 27, NULL, NULL);
            closesocket(serverSock);
            WSACleanup();
            return 4;
        }
    }

    // Loop forever, accepting connections
    while (true) {

        WriteConsoleA(stdout, "Listening...\n", 13, NULL, NULL);

        // Accept a connection from the client
        SOCKET clientSock = accept(serverSock, NULL, NULL);
        if (clientSock == INVALID_SOCKET) {
            WriteConsoleA(stdout, "Failed to accept client socket\n", 31, NULL, NULL);
            WriteConsoleA(stdout, "Trying again\n\n", 14, NULL, NULL);
            continue;
        }

        WriteConsoleA(stdout, "Got connection\n", 15, NULL, NULL);

        // Send a string to the client
        send(clientSock, "Hello there!\n", 13, 0);

        // Close the connection
        closesocket(clientSock);

        // Go to the next loop
        WriteConsoleA(stdout, "\n", 2, NULL, NULL);
    }

    // Success
    // Remember to clean up
    closesocket(serverSock);
    WSACleanup();
    return 0;
}
