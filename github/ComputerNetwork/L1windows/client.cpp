
#include <iostream>
#include <cstring>
#include <string>
#include <winsock2.h>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#define BUFFSIZE 256

// 请求数据包
enum RequestOrResponseType {
    TIME_REQUEST=1,   // 时间请求
    NAME_REQUEST,   // 名字请求
    LIST_REQUEST,   // 列表请求
    MESG_REQUEST,   // 消息请求
    CONN_REQUEST,   // 连接请求
    TIME_RESPONSE,
    NAME_RESPONSE,
    LIST_RESPONSE,
    MESG_RESPONSE,
    CONN_RESPONSE,
	INDICATE		// 指示
};

struct ClientInfo {
    SOCKET* clientSocket;
    string IP;
    int port;
    ClientInfo() {};
    ClientInfo(SOCKET* clientSocket, string IP, int port) {
        this->clientSocket = clientSocket;
        this->IP = IP;
        this->port = port;
    }
};
HANDLE hThread; //监听线程句柄
bool shouldExit = false;
bool isConnect = false;
std::queue<std::string> messageQueue;  // 消息队列，用于存储要发送给主线程的消息
std::mutex queueMutex;  // 互斥锁，用于保护消息队列的访问
std::condition_variable queueCondition;  // 条件变量，用于通知主线程有新的消息到达
SOCKET* temclientPtr ;
// 线程函数，用于处理接收到的消息
void RecvHandleThread(SOCKET* SocketClient)
{
    char recvBuff[BUFFSIZE] = { 0 };
    while (shouldExit == false) {
		// cout << "recv thread start\n";
        int recvHandler = 0;
        recvHandler = recv(*SocketClient, recvBuff, sizeof(recvBuff), 0);
		printf("recvbuffer %s\n", recvBuff);
		// int TYPE = int(recvBuff[0]);
		// cout << "Type is" << TYPE << endl;
		// cout << recvBuff << endl;
        if (recvHandler > 0) {
            // 将接收到的消息放入消息队列中
            messageQueue.push(std::string(recvBuff));
            queueCondition.notify_all();
        }        
		std::memset(recvBuff, 0, sizeof(recvBuff));
    }
    
}

void QueueThread() {
	cout << "queue thread start\n";
    // 检查消息队列是否有新的消息到达
    std::unique_lock<std::mutex> lock(queueMutex);
    queueCondition.wait(lock, [] { return !messageQueue.empty(); });

    // 处理消息队列中的消息
    while (!messageQueue.empty()) {
        std::string frontmsg = messageQueue.front();
        int type = frontmsg.c_str()[0];
        // cout << "type is " << type << endl;

        switch (type) {
            case TIME_RESPONSE: {
                cout << "[Client] time: " << frontmsg.substr(1) << endl;
                break;
            }
            case NAME_RESPONSE: {
                cout << "[Client] name: " << frontmsg.substr(1) << endl;
                break;
            }
            case LIST_RESPONSE: {
                cout << "[Client] list: " << frontmsg.substr(1) << endl;
                break;
            }
            case MESG_RESPONSE: {
                cout << "[Client] mesg: " << frontmsg.substr(0) << "sent successfully " << endl;
                break;
            }
            case CONN_RESPONSE: {
                cout << "[Client] conn: " << frontmsg.substr(1) << endl;
                break;
            }
            case INDICATE: {
                cout << "[Client] indicate: " << frontmsg.substr(0) << endl;
                break;
            }
            case '#': {
                break;
            }
            default: {
                cout << "[Client] wrong type\n";
                break;
            }
        }
        messageQueue.pop();
    }
    lock.unlock();
}

int InputThread(){
	
	 while (true) {
        if (!isConnect) {
            cout << "you have not connect to any server\n" \
                "menu:\n" \
                "1. connect to server\n" \
                "2. exit\n";
            int choice;
            std::cin >> choice;
            switch (choice) {
            case 1: {
				struct sockaddr_in serverAddress;
				int serverAddressLength;

				// 创建套接字
				SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
				if (clientSocket == INVALID_SOCKET) {
					std::cerr << "Failed to create socket. Error code: " << WSAGetLastError() << std::endl;
					WSACleanup();
					return 1;
				}
				// TODO 记得改回来
                cout << "please input server IP \n";
                string IP;
                // std::cin >> IP;
				IP = "127.0.0.1";
                cout << "please input server port \n";
                int port;
				port = 2362;
                // std::cin >> port;
				temclientPtr = &clientSocket;
                serverAddress.sin_family = AF_INET;
                serverAddress.sin_addr.s_addr = inet_addr(IP.c_str());
                serverAddress.sin_port = htons(port);
                serverAddressLength = sizeof(serverAddress);
                int handle = connect(clientSocket, (struct sockaddr*)&serverAddress, serverAddressLength);
                if (handle == SOCKET_ERROR) {
                    cout << "connect error\n";
                    break;
                }
                else {
                    isConnect = true;
                    // 然后创建一个接收数据的子线程，循环调用receive()，
                    // 如果收到了一个完整的响应数据包，就通过线程间通信（如消息队列）发送给主线程，
                    // 然后继续调用receive()，直至收到主线程通知退出。
                    thread recvThread(RecvHandleThread, &clientSocket);
					recvThread.detach();
                }
                break;
            }
            case 2: {
                cout << "bye\n";
                return 1;
            }
            default: {
                cout << "wrong choice\n";
                break;
            }
            }
        }
        else {
            cout << "menu:\n" \
				"1. get time\n" \
				"2. get name\n" \
				"3. get list\n" \
				"4. send message\n" \
				"5. disconnect\n" \
				"6. exit\n";
			int choice;
			std::cin >> choice;
			switch (choice) {
			case 1: {
				char buffer = (char)TIME_REQUEST;
				send(*temclientPtr,&buffer,sizeof(buffer), 0);
				break;
			}
			case 2: {
				char buffer = (char)NAME_REQUEST;
				send(*temclientPtr, &buffer, sizeof(buffer), 0);
				break;
			}
			case 3: {
				char buffer = (char)LIST_REQUEST;
				send(*temclientPtr, &buffer, sizeof(buffer), 0);
				break;
			}
			case 4: {
				char sendBuff[BUFFSIZE] = { 0 };
				int destIndex;
				sendBuff[0] = MESG_REQUEST;
				cout << "please input index of destination client\n";
				std::cin >> destIndex;
				sendBuff[1] = (char)destIndex;
				cout << "please input message\n";
				string message;
				std::cin >> message;
				std::memcpy(sendBuff + 2, message.c_str(), message.length());
				send(*temclientPtr, sendBuff, strlen(sendBuff), 0);
				break;
			}
			case 5: {
				// 通知并等待子线程关闭。
				char disconnect = (char)CONN_REQUEST;
				send(*temclientPtr, &disconnect, sizeof(disconnect), 0);
				isConnect = false;
				shouldExit = true;
				closesocket(*temclientPtr);
				//等待子线程关闭
				break;
			}
			case 6: {
				cout << "bye\n";
				goto final;
			}
			default: {
				cout << "wrong choice\n";
				break;
			}
			}
        }
    }
	final:
	// 关闭套接字
    closesocket(*temclientPtr);
    WSACleanup();
}
int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

	// 创建并启动用户输入处理线程
    std::thread inputThread(InputThread);

    // 创建并启动消息队列处理线程
    std::thread queueThread(QueueThread);

    // 等待用户输入处理线程和消息队列处理线程结束
    inputThread.join();
    queueThread.join();
   
    

    return 0;
}