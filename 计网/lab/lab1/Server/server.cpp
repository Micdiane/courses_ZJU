//
// Created by Lenovo on 2023/10/23.
//

#include "Lib.h"

using namespace std;

// 用到的变量
SOCKET serverSocket;
struct sockaddr_in serverAddress;
// Global flag to indicate server termination
atomic<bool> serverRunning(true);
// bool serverRunning = true;
// Signal handler function for graceful termination

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        serverRunning = false;
        cout << "BYEBYE!" << endl;
    }
}
void MakeIndicatePacket(char HeadType,char* buffer){
    // 负责给DestClient发送Message
    if(HeadType == INDICATE){
		int len = strlen(buffer);
        memmove(buffer+2,buffer,len);
        buffer[0] = INDICATE;
        buffer[1] = (char)len;
    }
}
void MakeResponsePacket(char HeadType,char* buffer){
    //输入一个包的类型,组装出一个响应包,并通过buffer指针来显式返回;
    // 对于MSG请求,需要事先将内容放在buffer之中
    int len = strlen(buffer);
    memmove(buffer+2,buffer,len);
    buffer[1] = (char)len;
    buffer[0] = HeadType;
}

//刚获得的句柄要传递给子线程，子线程内部要使用该句柄发送和接收数据
void clientHandle(SOCKET* clientSocketptr, int clientIdx){
    char sendPacket[PACKETSIZE+10] = {0};
    char recvBuffer[PACKETSIZE+10] = {0};
    //发送一个hello
    strcpy(sendPacket,"hello");
    MakeResponsePacket(MESG_RESPONSE,sendPacket);
    send(*clientSocketptr,sendPacket,PACKETSIZE,0);

    while(serverRunning){
        //循环调用receive()，如果收到了一个完整的请求数据包，根据请求类型做相应的动作
        int recvFlag = recv(*clientSocketptr, recvBuffer, PACKETSIZE, 0);
        if(recvFlag > 0){
			char requestType = recvBuffer[0];
			// cout << "收到 " << recvBuffer << endl;
        	switch (requestType) {
                case TIME_REQUEST: { // 获取时间
					cout << "Time Request from Client" << clientIdx << endl;
                    time_t now = time(0);
                    char *time = ctime(&now);
                    strcpy(sendPacket, time);
                    MakeResponsePacket(TIME_RESPONSE, sendPacket);
                    send(*clientSocketptr,sendPacket, PACKETSIZE,0);
                    break;
                }
                case NAME_REQUEST: {
					cout << "Name Request from Client" << clientIdx << endl;
                    char hostname[MAX_COMPUTERNAME_LENGTH + 1];
                    DWORD size = sizeof(hostname);
                    GetComputerNameA(hostname, &size);
                    strcpy(sendPacket, hostname);
                    MakeResponsePacket(NAME_RESPONSE, sendPacket);
                    send(*clientSocketptr,sendPacket, PACKETSIZE,0);
                    break;
                }
                case LIST_REQUEST: {
					cout << "List Request from Client" << clientIdx << endl;
                    // 可能会遇到包长不够的情况,此时需要分多批发,目前不考虑这个问题
                    for (auto it: clients_map) {
                        int idx = it.first;
                        char idxstr[10];
                        sprintf(idxstr, "[%d]", idx);
                        strcat(sendPacket, idxstr);
                        strcat(sendPacket, it.second.IP.c_str());  // 连接IP字符串
                        char portstr[10];
                        sprintf(portstr, ":%d\n", it.second.port);  // 将端口值转换为字符串
                        strcat(sendPacket, portstr);
                    }
                    MakeResponsePacket(LIST_RESPONSE, sendPacket);
                    send(*clientSocketptr,sendPacket, PACKETSIZE,0);
                    break;
                }
                case MESG_REQUEST: {
					cout << "Message Request from Client" << clientIdx << endl;
                    int destIdx = recvBuffer[2];
					bool destNotValid = (clients_map.find(destIdx) == clients_map.end()) ;
                    if (destNotValid) {
                        // 不存在 发送一个错误响应
                        strcpy(sendPacket, "destination Client not exist!");
                        MakeResponsePacket(MESG_RESPONSE, sendPacket);
						send(*clientSocketptr, sendPacket, PACKETSIZE, 0);

                    } else {
                        strcpy(sendPacket, "message send successfully!");
                        MakeResponsePacket(MESG_RESPONSE, sendPacket);

						send(*clientSocketptr, sendPacket, PACKETSIZE, 0);
                        // 还要额外发送一个指示包
                    }
					if(!destNotValid){
						SOCKET *destSocket = clients_map[destIdx].clientSocket;
						char msgPack[256];
						memcpy(msgPack, recvBuffer + 3, PACKETSIZE);
						MakeIndicatePacket(INDICATE, msgPack);
						send(*destSocket,msgPack, PACKETSIZE,0);
					}
                    break;
                }
                case DISC_REQUEST: { // 收到断连请示 子线程结束
					cout << "Disconnect Request from Client" << clientIdx << endl;
                    int Del_idx = (int) recvBuffer[2];
                    clients_map.erase(Del_idx);
                    MakeResponsePacket(DISC_RESPONSE, sendPacket);
                    send(*clientSocketptr, sendPacket, PACKETSIZE, 0);
                    goto final;
                    break;
                }
            }
        memset(sendPacket,0,PACKETSIZE+10);
		}
    }
    final:
    // 结束时释放
    closesocket(*clientSocketptr);
    clients_map.erase(clientIdx);
}



void Init(){
    WSADATA wsaData;	// 用于初始化 Winsock 库
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {  // 初始化 Winsock 库
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return;
    }
    serverSocket = socket(AF_INET,SOCK_STREAM,0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(2362); // 指定监听的端口

    // 绑定套接字到指定地址和端口
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket. Error code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return ;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on socket. Error code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return ;
    }
    cout << "Server start!" << endl ;
}

// 定期给全体客户端发包
void sendHeartbeat() {
    while (serverRunning) {

        for (auto& client : clients_map) {
            SOCKET* clientSocket = client.second.clientSocket;
            char heartbeatPacket[PACKETSIZE + 10] = {0};
            MakeResponsePacket(INDICATE, heartbeatPacket);

            // 发送一个空指示包
            if (send(*clientSocket, heartbeatPacket, PACKETSIZE, 0) == SOCKET_ERROR) {
                // Failed to send heartbeat, update clients_map
                int clientIdx = client.first;
                std::cout << "Heartbeat failed for Client " << clientIdx << std::endl;

                // 如果发送失败就清理掉
                closesocket(*clientSocket);
                clients_map.erase(clientIdx);
            }
        }
        // Wait for a specified interval before sending the next heartbeat
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

int main(){
    std::signal(SIGINT, signalHandler); // 登记一个信号
    Init();// 套接字初始化

	// serverRunning = true;
	// std::thread heartbeatThread(sendHeartbeat);
    // heartbeatThread.detach();
	
    while(serverRunning){

        SOCKET* clientSocket = new(SOCKET);
        struct sockaddr_in clientAddr;
        int clientAddrLength = sizeof(clientAddr);
        // 主线程循环调用accept 直到返回一个有效的socket句柄
        *clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLength);
        // 获取客户端地址信息,记录下该客户端句柄和连接状态、端口。
        char* clientIP = inet_ntoa(clientAddr.sin_addr);
        int clientPort = ntohs(clientAddr.sin_port);
        int temidx = clients_map.size();
        clients_map[temidx] = ClientInfo(clientSocket, clientIP, clientPort);
        std::cout << "Client connected. IP: " << clientIP << ", Port: " << clientPort << std::endl;

        // 然后创建一个子线程后继续调用accept()
		thread clientThread(clientHandle,clientSocket,temidx);
        clientThread.detach();
    }
	cout <<"finish ";
    // 关闭服务器套接字
    closesocket(serverSocket);
    WSACleanup();
}