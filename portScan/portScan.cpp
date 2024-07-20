#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

void scan_port(const std::string& host, unsigned short port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

    int result = connect(sock, (sockaddr*)&server_addr, sizeof(server_addr));
    if (result == 0) {
        std::cout << port << " : open" << std::endl;
    }

    closesocket(sock);
    WSACleanup();
}

void port_scan(const std::string& host, unsigned short start_port, unsigned short end_port) {
    std::vector<std::thread> threads;
    for (unsigned short port = start_port; port <= end_port; ++port) {
        threads.emplace_back(scan_port, host, port);
    }
    for (auto& thread : threads) {
        thread.join();
    }
}

int main() {
    std::string host;
    std::cout << "IP to scan: ";
    std::cin >> host;
    unsigned short start_port = 1;
    unsigned short end_port = 1024;

    port_scan(host, start_port, end_port);

    return 0;
}
