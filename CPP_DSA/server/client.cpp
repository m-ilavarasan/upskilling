#include <iostream>
#include <string>
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

void close_socket(socket_t s)
{
#ifdef _WIN32
    closesocket(s);
#else
    close(s);
#endif
}

int main(int argc, char *argv[])
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }
#endif

    const char *serverIp = "127.0.0.1";
    const char *portStr = "54000";
    if (argc >= 2)
        serverIp = argv[1];
    if (argc >= 3)
        portStr = argv[2];

    int port = std::stoi(portStr);

    socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(static_cast<uint16_t>(port));
    inet_pton(AF_INET, serverIp, &servAddr.sin_addr);

    if (connect(sock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Connect failed" << std::endl;
        close_socket(sock);
        return 1;
    }

    std::cout << "Connected to " << serverIp << ":" << port << "\n";

    std::string msg;
    while (true)
    {
        std::cout << "> ";
        if (!std::getline(std::cin, msg))
            break;
        if (msg.empty())
            continue;
#ifdef _WIN32
        int sent = send(sock, msg.c_str(), static_cast<int>(msg.size()), 0);
#else
        ssize_t sent = send(sock, msg.c_str(), msg.size(), 0);
#endif
        if (sent <= 0)
            break;

        char buf[4096];
#ifdef _WIN32
        int rec = recv(sock, buf, sizeof(buf), 0);
#else
        ssize_t rec = recv(sock, buf, sizeof(buf), 0);
#endif
        if (rec <= 0)
            break;
        std::cout << "echo: " << std::string(buf, buf + rec) << std::endl;
    }

    close_socket(sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
