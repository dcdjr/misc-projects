// Enable modern Winsock features like inet_pton.
#define _WIN32_WINNT 0x0601

#include <stdio.h>      // printf, fprintf
#include <stdlib.h>     // malloc, free, exit
#include <winsock2.h>   // sockets: socket, connect, htons
#include <windows.h>    // threads, WinAPI types
#include <ws2tcpip.h>   // inet_pton, address helpers

#pragma comment(lib, "Ws2_32.lib")

// Set default port to scan
#define DEFAULT_PORT "27015"

// start with simple version to scan one ip.
int scan_port(char *port) {
    struct sockaddr_in target = {0};
    

    // Resolve the server address and port
    int iResult = getaddrinfo("127.0.0.1", port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        return 1;
    }
    
    SOCKET ConnectSocket = INVALID_SOCKET;

    ptr = result;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
        ptr->ai_protocol);
    
    // Connect to server.
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

    freeaddrinfo(result);
    
    if (iResult == 0) {
        closesocket(ConnectSocket);
        return 1;
    }
    closesocket(ConnectSocket);
    return 0;

}

// Main function
int main(void) {
    WSADATA wsaDATA;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaDATA);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    int scan_result = scan_port("443");

    // Cleanup
    WSACleanup();
    return 0;
}