#pragma once

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


// Declare the data we want to send
extern WSABUF toSend;
