// Needed to use WinSock2 with Windows.h
// See: https://docs.microsoft.com/en-us/windows/win32/winsock/creating-a-basic-winsock-application
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Win32 API
#include <Windows.h>

// Windows Sockets 2
// See: https://docs.microsoft.com/en-us/windows/win32/winsock/creating-a-basic-winsock-application
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


// Include file for the data we want to send
// Defines `toSend`
#include "to_send.hpp"

// Macro for the version of IP we want to use
// Checked later on
#define USE_IPV6


// Entrypoint
int main(void) {

    // Get the console handle so we can print
    // See: https://docs.microsoft.com/en-us/windows/console/getstdhandle
    // See: https://docs.microsoft.com/en-us/windows/console/writeconsole
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
            WriteConsoleW(stdout, L"Failed to initialize WinSock2\n", 30, NULL, NULL);
            return 2;
        }
    }

    // Create a socket
    // Choose which protocol to use based on the preprocessor macro
    // See: https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
    // See: https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsasocketw
    SOCKET serverSock =
#if defined(USE_IPV4)
        WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
#elif defined(USE_IPV6)
        WSASocketW(AF_INET6, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
#endif
    if (serverSock == INVALID_SOCKET) {
        WriteConsoleW(stdout, L"Failed to create socket\n", 24, NULL, NULL);
        WSACleanup();
        return 3;
    }

    // Bind the socket
    // Use different addresses based on the protocol
    // See: https://docs.microsoft.com/en-us/windows/win32/winsock/binding-a-socket
    // See: https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-bind
    // See: https://docs.microsoft.com/en-us/windows/win32/winsock/sockaddr-2
#if defined(USE_IPV4)
    SOCKADDR_IN addr = {
        AF_INET,
        0xb822 /* Port 8888 in network order */,
        INADDR_ANY
    };
#elif defined(USE_IPV6)
    SOCKADDR_IN6 addr = {
        AF_INET6,
        0xb822, /* Port 8888 in network order */
        0,
        in6addr_any
    };
#endif
    {
        int res = bind(serverSock, (SOCKADDR*)&addr, sizeof(addr));
        if (res == SOCKET_ERROR) {
            WriteConsoleW(stdout, L"Failed to bind socket\n", 22, NULL, NULL);
            closesocket(serverSock);
            WSACleanup();
            return 4;
        }
    }

    // Listen on the socket
    // See: https://docs.microsoft.com/en-us/windows/win32/winsock/listening-on-a-socket
    // See: https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-listen
    {
        int res = listen(serverSock, SOMAXCONN);
        if (res == SOCKET_ERROR) {
            WriteConsoleW(stdout, L"Failed to listen on socket\n", 27, NULL, NULL);
            closesocket(serverSock);
            WSACleanup();
            return 4;
        }
    }

    // Loop forever, accepting connections
    // Note that this application is single-threaded. It can only handle one
    //  connection at a time. That's fine since we just send some data then
    //  close the connection.
    while (true) {

        WriteConsoleW(stdout, L"Listening...\n", 13, NULL, NULL);

        // Accept a connection from the client
        // See: https://docs.microsoft.com/en-us/windows/win32/winsock/accepting-a-connection
        // See: https://docs.microsoft.com/en-us/windows/win32/api/Winsock2/nf-winsock2-wsaaccept
        SOCKET clientSock = WSAAccept(serverSock, NULL, NULL, NULL, NULL);
        if (clientSock == INVALID_SOCKET) {
            WriteConsoleW(stdout, L"Failed to accept client socket\n", 31, NULL, NULL);
            WriteConsoleW(stdout, L"Trying again\n\n", 14, NULL, NULL);
            continue;
        }

        WriteConsoleW(stdout, L"Got connection\n", 15, NULL, NULL);

        // Send a string to the client
        // See: https://docs.microsoft.com/en-us/windows/win32/winsock/receiving-and-sending-data-on-the-server
        // See: https://docs.microsoft.com/en-us/windows/win32/api/Winsock2/nf-winsock2-wsasend
        {
            DWORD res;
            WSASend(clientSock, &toSend, 1, &res, 0, NULL, NULL);
        }

        // Close the connection
        // See: https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-closesocket
        closesocket(clientSock);

        // Go to the next loop
        WriteConsoleW(stdout, L"\n", 2, NULL, NULL);
    }
}
