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
	TIME_REQUEST,	// 时间请求
	NAME_REQUEST,	// 名字请求
	LIST_REQUEST,	// 列表请求
	MESG_REQUEST, 	// 消息请求
	CONN_REQUEST,	// 连接请求
	TIME_RESPONSE,
	NAME_RESPONSE,
	LIST_RESPONSE,
	MESG_RESPONSE,
	CONN_RESPONSE
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
HANDLE hThread; //监听线程句柄
bool shouldExit = false;
DWORD WINAPI RecvHandleThread(LPVOID lpParameter)
{
	SOCKET* SocketClient = (SOCKET*)lpParameter;
	char recvBuff[BUFFSIZE] = { 0 };
	while (true	) {
		int recvhandler = 0;
		recvhandler = recv(*SocketClient, recvBuff, sizeof(recvBuff), 0);
		if (recvhandler > 0) {
			cout << "\n[server] ";
			int response_type = int(recvBuff[0]);
			switch (response_type) {
			case TIME_RESPONSE:{
				cout << " TIME_RESPONSE:" << endl;
				cout << recvBuff + 1 << endl;
				break;
			}
			case NAME_RESPONSE:{
				cout << " NAME_RESPONSE:" << endl;
				cout << recvBuff + 1 << endl;
				break;
			}
			case LIST_RESPONSE:{
				cout << " LIST_RESPONSE:" << endl;
				cout << recvBuff + 1 << endl;
				break;
			}
			case MESG_RESPONSE:{
				cout << " MESG_RESPONSE:" << endl;
				cout << recvBuff + 1 << endl;
				break;
			}
			case CONN_RESPONSE:{
				cout << " CONN_RESPONSE:" << endl;
				cout << recvBuff + 1 << endl;
				break;
			}
			default:
				cout << endl;
				break;
			}
		}
		std::memset(recvBuff, 0, sizeof(recvBuff));
	}
}


int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }
	bool isConnect = false;
	SOCKET clientSocket;
    struct sockaddr_in serverAddress;
    int serverAddressLength;

    // 创建套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket. Error code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

	while(true){
		if(!isConnect){
			cout << "you have not connect to any server\n" \
					"menu:\n" \
					"1. connect to server\n" \
					"2. exit\n" ;
			int choice;
			std::cin >> choice;
			switch(choice){
				case 1:{
					cout << "please input server IP \n";
					string IP;
					std::cin >> IP;
					cout << "please input server port \n";
					int port;
					std::cin >> port;
					serverAddress.sin_family = AF_INET;
					serverAddress.sin_addr.s_addr = inet_addr(IP.c_str());
					serverAddress.sin_port = htons(port);
					serverAddressLength = sizeof(serverAddress);
					int handle = connect(clientSocket, (struct sockaddr*)&serverAddress, serverAddressLength);
					if(handle == SOCKET_ERROR){
						cout << "connect error\n";
						break;
					}else{
						cout << "connect success\n";
						isConnect = true;
						//然后创建一个接收数据的子线程，循环调用receive()，
						//如果收到了一个完整的响应数据包，就通过线程间通信（如消息队列）发送给主线程，
						//然后继续调用receive()，直至收到主线程通知退出。
						hThread = CreateThread(NULL, 0, RecvHandleThread, (LPVOID)&clientSocket, 0, NULL);
					}
					
					

					break;
				}
				case 2:{
					cout <<"bye\n";
					return 0;
				}
				default:{
					cout << "wrong choice\n";
					break;
				}
			}

		}else{
			
		}
	}


    

    // 在此处进行与服务器的通信

    // 关闭套接字
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}