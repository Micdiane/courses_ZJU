#include "../Lib.h"
using namespace std;
bool isConnect = false;
SOCKET nowSOCKETptr;
bool ClientRunning = true;
std::queue<char*> messageQueue;  // 消息队列，用于存储要发送给主线程的消息
std::mutex queueMutex;  // 互斥锁，用于保护消息队列的访问
std::condition_variable queueCondition;  // 条件变量，用于通知主线程有新的消息到达
int timecnt = 0;
bool msgIsArrived = false;

void Decodemsg(char* msg){
	char type = msg[0];
	char len  = msg[1];
	char* data = msg+2;
	cout << "[Server]\n";
	switch(type){
		case TIME_RESPONSE:{
			cout << "Time is " << data << endl;
			break;
		}
		case NAME_RESPONSE:{
			cout << "Name is " << data << endl;
			break;
		}
		case LIST_RESPONSE:{
			cout << "Client list is " << data << endl;
			break;
		}
		case MESG_RESPONSE:{
			cout << data << endl;
			break;
		}
		case DISC_RESPONSE:{
			cout << "Disconnect success\n";
			isConnect = false;
			break;
		}
		case INDICATE:{
			cout << "Receive " << data << endl;
			break;
		}
		default:{
			cout << "wrong type\n";
			break;
		}
	}   
}

void MakeRequestPacket(char HeadType,char* sendPacket){
	// 输入一个请求包的类型,组装一个请求包
	int len;
	switch(HeadType){
		case TIME_REQUEST:
			len = 0;
			break;
		case NAME_REQUEST:
			len = 0;
			break;
		case LIST_REQUEST:
			len = 0;
			break;
		case MESG_REQUEST:
			len = strlen(sendPacket) + 1;
			break;
		case DISC_REQUEST:
			len = 0;
			break;
	}
	sendPacket[0] = HeadType;
	sendPacket[1] = (char)len;
}



void ConnectMenu(){
	cout << "You have connected to server:\n" \
				"1. get time\n" \
				"2. get name\n" \
				"3. get list\n" \
				"4. send message\n" \
				"5. disconnect\n" \
				"6. exit\n";
	int choice;
	cin >> choice;
	char sendPacket[PACKETSIZE] = {0};
	switch(choice){
		case 1:{
			MakeRequestPacket(TIME_REQUEST,sendPacket);
			break;
		}
		case 2:{
			MakeRequestPacket(NAME_REQUEST,sendPacket);
			break;
		}
		case 3:{
			MakeRequestPacket(LIST_REQUEST,sendPacket);
			break;
		}
		case 4:{
			cout << "please input index of destination client\n";
			int destIndex;
			cin >> destIndex;
			sendPacket[2] = (char)destIndex;
			cout << "please input message\n";
			string message;
			cin >> message;
			memcpy(sendPacket+3, message.c_str(), message.length());	
			MakeRequestPacket(MESG_REQUEST,sendPacket);
			cout << sendPacket+3;
			break;
		}
		case 5:{
			MakeRequestPacket(DISC_REQUEST,sendPacket);
			closesocket(nowSOCKETptr);
			// 加入通知逻辑
			break;
		}
		case 6:{
			if(isConnect){
				cout << "Now disconnect to server\n";
				MakeRequestPacket(DISC_REQUEST,sendPacket);
			}
			break;
		}
		default:{
			cout << "wrong choice\n";
			break;
		}
	}
	cout << "timecnt = " << timecnt++ << endl;
	int handler = send(nowSOCKETptr,sendPacket,PACKETSIZE,0);
	// int handler = send(nowSOCKETptr,sendPacket,1,0);
}

// 接收数据 发送给主线程
void RecvHandleThread(){
    char recvBuff[PACKETSIZE] = {0};
	while(ClientRunning){
		// 一次接受一个包
		int handler = recv(nowSOCKETptr,recvBuff,PACKETSIZE,0);
		if(handler > 0){
			msgIsArrived = true	;
			
			char* msg = new char[PACKETSIZE];
			memcpy(msg,recvBuff,PACKETSIZE);
			lock_guard<mutex> lock(queueMutex);
			messageQueue.push(msg);
		}
	}
}


void Createsocket(){
	// 创建套接字
	struct sockaddr_in serverAddress;
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	// TODO 记得改回来
    cout << "please input server IP \n";
    string IP;
    std::cin >> IP;
	// IP = "127.0.0.1";
    cout << "please input server port \n";
    int port;
	// port = 2362;
    std::cin >> port;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(IP.c_str());
    serverAddress.sin_port = htons(port);
    int handle = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    nowSOCKETptr = clientSocket;
	if (handle == SOCKET_ERROR) {
        cout << "connect error\n";
    }else{
		isConnect = true;
		// 然后创建一个接收数据的子线程，循环调用receive()，
		thread recvThread(RecvHandleThread);
		recvThread.detach(); // 这里必须是detach 不然不会进行
	}

}

void disConnectMenu(){
	cout << "You have not connect to any server\n" \
                "menu:\n" \
                "1. connect to server\n" \
                "2. exit\n";
	int choice;
	cin >> choice;
	switch(choice){
		case 1:{
			Createsocket();
			break;
		}
		case 2:{
			cout << "Bye\n";
			exit(0);
		}
		default:{
			cout << "wrong choice\n";
			break;
		}
	}
}
void IOHandle(){
    // 获取用户输入,选择发送请求包
    if(isConnect)  ConnectMenu();
    	else disConnectMenu();
}



void Init(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	int sendBufferSize = 8192; // 设置发送缓冲区大小为 8192 字节
	int recvBufferSize = 8192; // 设置接收缓冲区大小为 8192 字节

	
}

// void test1(){
// 	//连接服务器
// 	struct sockaddr_in serverAddress;
// 	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	serverAddress.sin_family = AF_INET;
//     serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
//     serverAddress.sin_port = htons(2362);
//     connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
// 	char sendPacket[PACKETSIZE] = {0};
// 	MakeRequestPacket(TIME_REQUEST,sendPacket);
// 	for(int i = 0 ; i < 100 ;i++){
// 		send(clientSocket,sendPacket,PACKETSIZE,0);
// 		cout << "[1]send time packet" << i << endl;
// 	}
// }
// void test2(){
// 	//连接服务器
// 	struct sockaddr_in serverAddress;
// 	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	serverAddress.sin_family = AF_INET;
//     serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
//     serverAddress.sin_port = htons(2362);
//     connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
// 	char sendPacket[PACKETSIZE] = {0};
// 	MakeRequestPacket(TIME_REQUEST,sendPacket);
// 	for(int i = 0 ; i < 100 ;i++){
// 		send(clientSocket,sendPacket,PACKETSIZE,0);
// 		cout << "[2]send time packet" << i << endl;
// 	}
// }
int main(){
	
    
	Init();

	// thread t1(test1);
	// thread t2(test2);
	// t1.join();
	// t2.join();
	while(ClientRunning){
		// main线程检查消息队列是否有新的消息到达
		if(msgIsArrived){
			while(!messageQueue.empty()){
				lock_guard<mutex> lock(queueMutex);
				Decodemsg(messageQueue.front());
				messageQueue.pop();
			}
			msgIsArrived = false;
		}
		IOHandle();
	}
	


    return 0;
}