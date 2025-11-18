#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <csignal>
#include <cstring>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET socket_t;
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int socket_t;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

static std::atomic<bool> keepRunning{true};

void init_sockets()
{
#ifdef _WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cerr << "WSAStartup failed: " << iResult << "\n";
        std::exit(1);
    }
#endif
}

void cleanup_sockets()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

void close_socket(socket_t s)
{
#ifdef _WIN32
    closesocket(s);
#else
    close(s);
#endif
}

void signal_handler(int)
{
    keepRunning = false;
}

void handle_client(socket_t clientSock, std::string clientAddr)
{
    std::cout << "Client connected: " << clientAddr << "\n";
    const int BUF_SIZE = 4096;
    char buffer[BUF_SIZE];
    while (keepRunning)
    {
#ifdef _WIN32
        int bytes = recv(clientSock, buffer, BUF_SIZE, 0);
#else
        ssize_t bytes = recv(clientSock, buffer, BUF_SIZE, 0);
#endif
        if (bytes <= 0)
            break;
        std::string received(buffer, buffer + bytes);
        std::cout << "Received from " << clientAddr << ": " << received << "\n";
        // Echo back
#ifdef _WIN32
        int sent = send(clientSock, received.c_str(), static_cast<int>(received.size()), 0);
#else
        ssize_t sent = send(clientSock, received.c_str(), received.size(), 0);
#endif
        if (sent <= 0)
            break;
    }
    std::cout << "Client disconnected: " << clientAddr << "\n";
    close_socket(clientSock);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, signal_handler);

    init_sockets();

    const char *host = "0.0.0.0";
    const char *portStr = "54000";
    if (argc >= 2)
        portStr = argv[1];

    int port = std::stoi(portStr);

    socket_t listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket\n";
        cleanup_sockets();
        return 1;
    }

    int opt = 1;
#ifdef _WIN32
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
#else
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host);
    serverAddr.sin_port = htons(static_cast<uint16_t>(port));

    if (bind(listenSock, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed\n";
        close_socket(listenSock);
        cleanup_sockets();
        return 1;
    }

    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed\n";
        close_socket(listenSock);
        cleanup_sockets();
        return 1;
    }

    std::cout << "Server listening on port " << port << "\n";

    while (keepRunning)
    {
        sockaddr_in clientAddr;
#ifdef _WIN32
        int addrLen = sizeof(clientAddr);
#else
        socklen_t addrLen = sizeof(clientAddr);
#endif
        socket_t clientSock = accept(listenSock, (sockaddr *)&clientAddr, &addrLen);
        if (clientSock == INVALID_SOCKET)
        {
            if (!keepRunning)
                break;
            std::cerr << "Accept failed\n";
            continue;
        }

        char addrBuf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, addrBuf, sizeof(addrBuf));
        std::string clientIP = addrBuf + (std::string(":") + std::to_string(ntohs(clientAddr.sin_port)));

        std::thread t(handle_client, clientSock, clientIP);
        t.detach();
    }

    std::cout << "Shutting down server...\n";
    close_socket(listenSock);
    cleanup_sockets();
    return 0;
}
