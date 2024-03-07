#include "Util.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <vector>

std::mutex mtx;
std::condition_variable cr;
std::list<Packet> msg_lst;

class Client {
private:
    sockaddr_in _server_address{};
	int 		temport ;
	std::string temip;
    int _socket_fd = -1; // socket file descriptor

    int getChoice() {
        int choice;
		if(isConnect){
				std::cout << "You have connected to " << temip << ":" << temport << std::endl;
		}
        std::cout << "Please choose from:\n"
                     "[1] Connect\n"
                     "[2] Disconnect\n"
                     "[3] Get time\n"
                     "[4] Get server name\n"
                     "[5] Get client list\n"
                     "[6] Send data to other client\n"
                     "[7] Exit\n"
                     "> ";
        std::cin >> choice;
        return choice;
    }

    void thread_handler() {
        char buffer[BUFSIZE];
        recv(_socket_fd, buffer, BUFSIZE, 0);

        while (true) {
            memset(buffer, 0, BUFSIZE);
            auto len = recv(_socket_fd, buffer, BUFSIZE, 0);
            if (len <= 0) continue;

            std::unique_lock<std::mutex> lck(mtx);
            Packet t{};

            if (buffer[0] == FORWARD) {
                std::cout << "Received message: " << buffer + 1 << std::endl << "> ";
                std::cout.flush();
            } else {
                t.type = static_cast<unsigned char>(buffer[0]);
                memcpy(t.data, buffer + 1, len - 1);
                msg_lst.push_back(t);
            }

            cr.notify_all();
        }
    }

    void printInfo(const std::string& info) {
        std::cout << info << std::endl;
    }

    void printError(const std::string& err) {
        std::cerr << err << std::endl;
    }

    bool isConnectionExists() const {
        return _socket_fd != -1;
    }

public:
    Client() = default;
	bool isConnect = false;

    ~Client() {
        if (isConnectionExists()) {
            close(_socket_fd);
        }
    }

    void runWhenConnect() {
        while (true) {
            int choice = getChoice();
			isConnect = isConnectionExists();
            if (choice != 1 && choice != 7 && ! isConnect) {
                printError("Please establish a connection first.");
                continue;
            }

            switch (choice) {
                case 1: { // Connect
                    if (isConnect) {
                        printInfo("c");
                        continue;
                    }

                    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
                    std::string ip;
                    int port;

                    printInfo("Enter server IP: ");
                    std::cin >> ip;
                    printInfo("Enter port: ");
                    std::cin >> port;

                    _server_address.sin_family = AF_INET;
                    _server_address.sin_port = htons(port);
                    _server_address.sin_addr.s_addr = inet_addr(ip.c_str());

                    if (connect(_socket_fd, (struct sockaddr*)&_server_address, sizeof(struct sockaddr)) == -1) {
                        printError("Failed to establish a connection.");
                        close(_socket_fd);
                        _socket_fd = -1;
                        break;
                    }

					temip = ip;
					temport = port;
					isConnect = true;
                    std::thread(&Client::thread_handler, this).detach();
                    break;
                } // Connect
                case 2: { // Disconnect
                    char fin = static_cast<char>(DISCONNECT);
                    if (send(_socket_fd, &fin, sizeof fin, 0) <= 0) {
                        printError("Cannot send final signal.");
                        break;
                    }
                    close(_socket_fd);
                    _socket_fd = -1;
                    printInfo("Connection closed.");
					isConnect = false;
                    break;
                } // Disconnect
                case 3: { // Get time
                    char buffer = static_cast<char>(PacketType::GET_TIME);
                    send(_socket_fd, &buffer, sizeof buffer, 0);

                    std::unique_lock<std::mutex> lck(mtx);
                    while (msg_lst.empty()) {
                        cr.wait(lck);
                    }

                    auto tmp = msg_lst.front();
                    std::cout << "Get time: " << tmp.data << std::endl;
                    msg_lst.pop_front();
                    break;
                } // Get time
                case 4: { // Get server name
                    char get_name = static_cast<char>(PacketType::GET_NAME);
                    send(_socket_fd, &get_name, sizeof get_name, 0);

                    std::unique_lock<std::mutex> lck(mtx);
                    while (msg_lst.empty()) {
                        cr.wait(lck);
                    }

                    auto tmp = msg_lst.front();
                    std::cout << "Get server name: " << tmp.data << std::endl;
                    msg_lst.pop_front();
                    break;
                } // Get server name
                case 5: { // Get client list
                    char get_list = static_cast<char>(PacketType::GET_ACTIVE_LIST);
                    send(_socket_fd, &get_list, sizeof get_list, 0);

                    std::unique_lock<std::mutex> lck(mtx);
                    while (msg_lst.empty()) {
                        cr.wait(lck);
                    }

                    auto tmp = msg_lst.front();
                    std::cout << "Get user list: " << tmp.data << std::endl;
                    msg_lst.pop_front();
                    break;
                } // Get client list
                case 6: { // Send message
                    char buffer[BUFSIZE] = {0};
                    std::string ip;
                    int port;

                    printInfo("Enter the destination IP: ");
                    std::cin >> ip;
                    printInfo("Enter the destination port: ");
                    std::cin >> port;

                    buffer[0] = SEND_MSG;
                    printInfo("Enter the message (end with #): ");
                    sprintf(buffer + 1, "%s:%d:\n", ip.c_str(), port);
                    char c = '\0';

                    while (true) {
                        std::cin >> c;
                        if (c == '#') break;
                        sprintf(buffer + strlen(buffer), "%c", c);
                    }

                    send(_socket_fd, buffer, sizeof buffer, 0);

                    std::unique_lock<std::mutex> lck(mtx);
                    while (msg_lst.empty()) {
                        cr.wait(lck);
                    }

                    auto tmp = msg_lst.front();
                    std::cout << "Message sent: " << tmp.data << std::endl;
                    msg_lst.pop_front();
                    break;
                } // Send message
                case 7: { // Exit
                    if (isConnectionExists()) {
                        char fin = static_cast<char>(DISCONNECT);
                        send(_socket_fd, &fin, sizeof fin, 0);
                        close(_socket_fd);
                    }
                    exit(0);
                } // Exit
                default: {
                    std::cout << "Invalid choice. Please enter a valid option." << std::endl;
                }
            }
        }
    }

	// void runWhenDisconnect(){
	// 	int choice = getChoice();
	// 	if (/* condition */)
	// 	{
	// 		/* code */
	// 	}
		
	// }
};

int main() {
    Client client;
    client.runWhenConnect();
}
