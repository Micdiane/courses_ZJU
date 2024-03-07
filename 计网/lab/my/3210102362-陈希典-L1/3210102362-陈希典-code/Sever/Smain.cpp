#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    // 创建套接字
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // 绑定服务器地址和端口
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(2362);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        return -1;
    }

    // 监听连接
    if (listen(server_socket, 1) == -1) {
        std::cerr << "Error listening" << std::endl;
        return -1;
    }

    std::cout << "Waiting for a client to connect..." << std::endl;

    // 接受客户端连接
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);

    if (client_socket == -1) {
        std::cerr << "Error accepting client connection" << std::endl;
        return -1;
    }

    std::cout << "Client connected" << std::endl;

    // 接收和处理消息
    char buffer[1024];
    while (true) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break;
        }

        buffer[bytes_received] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }

    // 关闭套接字
    close(client_socket);
    close(server_socket);

    return 0;
}
