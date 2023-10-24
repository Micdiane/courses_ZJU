#include <iostream>
#include <cstring>
#include <string>
#include <winsock2.h>
#include <map>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#define BUFFSIZE 256 
//请求数据包
enum requestOrResponseType {
	TIME_REQUEST=1,	// 时间请求
	NAME_REQUEST,	// 名字请求
	LIST_REQUEST,	// 列表请求
	MESG_REQUEST, 	// 消息请求
	DISC_REQUEST,	// 连接请求
	TIME_RESPONSE,	
	NAME_RESPONSE,  
	LIST_RESPONSE,
	MESG_RESPONSE,
	INDICATE		// 指示
};

struct ClientInfo{
	SOCKET* clientSocket;
	string IP;
	int port;
	ClientInfo(){};
	ClientInfo(SOCKET* clientSocket, string IP, int port){
		this->clientSocket = clientSocket;
		this->IP = IP;
		this->port = port;
	}
};
// 客户端列表
map<int, ClientInfo> clients_map; // 编号 客户端套接字 IP 端口
// 响应数据包
// TIME 时间数据
// NAME 服务器名
// LIST 编号 IP 端口 
// MESG 不存在 ->错误代码和出错描述给src客户端
// 		存在 1 发消息给dest客户端 2 发消息给src客户端,转发成功
// CONN 返回端口给客户端,用于辨别客户端
DWORD WINAPI clientThread(LPVOID lpParameter){
	intptr_t index = reinterpret_cast<intptr_t>(lpParameter);
	SOCKET* clientSocket = clients_map[index].clientSocket;
	char recvBuffer[BUFFSIZE]={0};
	char sendBuffer[BUFFSIZE]={0};
	while(true){
		int recvFlag = recv(*clientSocket, recvBuffer, BUFFSIZE, 0);
		if(recvFlag > 0){
			int responseType = recvBuffer[0];
			cout << "recv: " << recvBuffer << "from client " << *clientSocket<< endl;
			cout << "IP " << clients_map[index].IP << " Port " << clients_map[index].port << " responseType " << responseType << endl;
			
			switch (responseType)
			{
				case TIME_REQUEST:{
					// 获取当前时间
					time_t now = time(0);
					char* time = ctime(&now);
					// 发送时间数据
					sendBuffer[0] = (char)TIME_RESPONSE;
					strcpy(sendBuffer + 1, time);
					int handle = send(*clientSocket, sendBuffer, BUFFSIZE, 0);
					if(handle == SOCKET_ERROR){
						cout << "send time response error" << endl;
					}
					
					break;
				}
				case NAME_REQUEST:{
					// 获取服务器名
					char* name = "Server";
					// 发送服务器名
					sendBuffer[0] = NAME_RESPONSE;
					strcpy(sendBuffer + 1, name);
					int handle = send(*clientSocket, sendBuffer, BUFFSIZE, 0);
					if(handle == SOCKET_ERROR){
						cout << "send name response error" << endl;
					}
					break;
				}
				case LIST_REQUEST:{
					// 获取客户端列表
					sendBuffer[0] = LIST_RESPONSE;
					//输出每个客户端的编号 IP 端口 
					for (auto it : clients_map) {
						int testValid = send(*it.second.clientSocket, "#", 2, 0);
						if(testValid == SOCKET_ERROR){
							clients_map.erase(it.first);
							continue;
						}
						int index = it.first;
						char index_str[10];  // 存储整数值的字符串形式
						sprintf(index_str, "[%d]", index);  // 将整数值转换为字符串
						strcat(sendBuffer, index_str);  // 将字符串连接到sendBuffer中
						strcat(sendBuffer, it.second.IP.c_str());  // 连接IP字符串
						char port_str[10];  // 存储端口值的字符串形式
						sprintf(port_str, ":%d\n", it.second.port);  // 将端口值转换为字符串
						strcat(sendBuffer, port_str);  // 将字符串连接到sendBuffer中
					}
					int handle = send(*clientSocket, sendBuffer, BUFFSIZE, 0);
					if(handle == SOCKET_ERROR){
						cout << "send list response error" << endl;
					}
					break;
				}

				case MESG_REQUEST:{
					// 获取请求数据
					int destIndex = recvBuffer[1];
					// cout << "destIndex: " << destIndex << endl;	
					char* mesg = recvBuffer + 2;
					// 判断目标客户端是否存在
					if(clients_map.find(destIndex) == clients_map.end()){
						// 目标客户端不存在
						sendBuffer[0] = MESG_RESPONSE;
						strcpy(sendBuffer + 1, "目标客户端不存在");
						int handle = send(*clientSocket, sendBuffer, BUFFSIZE, 0);
						if(handle == SOCKET_ERROR){
							cout << "send mesg response error" << endl;
						}
					}else{
						// 目标客户端存在
						// 获取目标客户端套接字
						SOCKET* destSocket = clients_map[destIndex].clientSocket;
						// 发送消息给目标客户端
						sendBuffer[0] = INDICATE;
						strcpy(sendBuffer + 1, mesg);
						int handle = send(*destSocket, sendBuffer, BUFFSIZE, 0);
						if(handle == SOCKET_ERROR){
							cout << "send mesg response error" << endl;
						}
						// 发送消息给源客户端
						sendBuffer[0] = MESG_RESPONSE;
						strcpy(sendBuffer + 1, mesg);
						handle = send(*clientSocket, sendBuffer, BUFFSIZE, 0);
						if(handle == SOCKET_ERROR){
							cout << "send mesg response error" << endl;
						}
					}
					break;
				}
				case DISC_REQUEST:{
					int Del_index = (int)recvBuffer[1]; 
					// cout << "Delindex: " << Del_index << endl;
					clients_map.erase(Del_index);
					break;
				}
			}
			cout << sendBuffer << endl;
		}
		memset(sendBuffer,0,sizeof(sendBuffer));
	}
	closesocket(*clientSocket);
	free(clientSocket);
	clients_map.erase(index);
	return 0;
}


int main() {
    WSADATA wsaData;	// 用于初始化 Winsock 库
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {  // 初始化 Winsock 库
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    SOCKET serverSocket; 				// 服务器套接字
    struct sockaddr_in serverAddress;	// 服务器地址
    // 创建套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); 		// AF_INET: IPv4, SOCK_STREAM: TCP
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket. Error code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置服务器地址
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(2362); // 指定监听的端口

    // 绑定套接字到指定地址和端口
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket. Error code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 开始监听连接
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on socket. Error code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server started. Listening for connections..." << std::endl;

    while (true) {
        // 接受客户端连接
		SOCKET* clientSocket = new(SOCKET);
		struct sockaddr_in clientAddress;
		int clientAddressLength = sizeof(clientAddress);
		// 主线程循环调用accept 直到返回一个有效的socket句柄
        *clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
		
		// 获取客户端地址信息,记录下该客户端句柄和连接状态、端口。
        char* clientIP = inet_ntoa(clientAddress.sin_addr);
        int clientPort = ntohs(clientAddress.sin_port);
		int temidx = clients_map.size();
        clients_map[temidx] = ClientInfo(clientSocket, clientIP, clientPort);
		std::cout << "Client connected. IP: " << clientIP << ", Port: " << clientPort << std::endl;

        // 然后创建一个子线程后继续调用accept()
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)clientThread, reinterpret_cast<LPVOID>(temidx), 0, NULL);
	}

    // 关闭服务器套接字
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}