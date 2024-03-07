#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <mutex>
#include <condition_variable>
#include <thread>

bool fasttest=true ;
#define CONNECT 1
#define DISCONNECT 2
#define GETTIME 3
#define GETNAME 4
#define GETCLIENTLIST 5
#define SENDMESSAGE 6
#define EXIT 7

using namespace std;

void show_menu(int client_socket);

sockaddr_in server_address;

std::mutex mtx;
std::condition_variable cv;
bool connected = false;

// 接收数据的子线程
void receiveThread(int socket) {
    while (connected) {
        char buffer[1024];
        int bytesReceived = recv(socket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            // 连接已关闭或出错
            connected = false;
            std::unique_lock<std::mutex> lock(mtx);
            cv.notify_all();
            break;
        }

        // 处理接收到的数据
        // 你可以将数据放入消息队列，以便在主线程中处理
    }
}

bool connectToSever(const char* serverIP, int port){
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1){
        cerr << "创建Socket失败" << endl;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    if(connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr) ) == -1) {
//        thread receiver(receiveThread, clientSocket);

        close(clientSocket);
        return false;
    }
    return true;
}

void menuconnect(){
    char serverIP[100];
    int port;
    //记得删掉
    if(fasttest){
        connectToSever("127.0.0.1",2362);
    }
    else{
        printf("请输入服务器IP:");
        cin.getline(serverIP,sizeof(serverIP));
        printf("请输入服务器端口");
        cin >> port;
    }

    if(connectToSever(serverIP,port)){
        connected = true;
        // 创建接收数据的子线
        //连接成功的处理
    }else{

    }
}
//断开连接
void disconnect(){

}

//获取时间
void getTime(){

}

void getName(){

}

void sendMessage(){

}

void getClientList(){

}
int main() {
    // 创建套接字
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // 服务器地址和端口

//    server_address.sin_family = AF_INET;
//    server_address.sin_port = htons(12345);
//    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
//    // 连接到服务器
//    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
//        std::cerr << "Error connecting to server" << std::endl;
//        return -1;
//    }
//    std::cout << "Connected to the server" << std::endl;

    // 发送消息
    show_menu(client_socket);

    // 关闭套接字
    close(client_socket);

    return 0;
}

void show_menu(int client_socket) {
    int choice = 0;
    while (true) {
        if(connected){
            printf("已连接 %d %d!\n",server_address.sin_addr.s_addr,server_address.sin_port);
            printf("菜单选项:\n");
            printf("1. 连接\n");
            printf("2. 断开n");
            printf("3. 获取时间\n");
            printf("4. 获取名字\n");
            printf("5. 获取客户端功能列表\n");
            printf("6. 发送消息\n");
            printf("7. 退出\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            //处理部分
            switch (choice) {
                case CONNECT:
                    menuconnect();
                    break;
                case DISCONNECT:
                    disconnect();
                    break;
                case GETTIME:
                    getTime();
                    break;
                case GETNAME:
                    getName();
                    break;
                case GETCLIENTLIST:
                    getClientList();
                    break;
                case SENDMESSAGE:
                    sendMessage();
                    break;
                case EXIT:
                    printf("退出成功!\n");
                    return;
                default:
                    printf("输入非法,请重新选择!\n");
            }


            string message;

            send(client_socket, message.c_str(), message.size(), 0);
        }

        //没有建立连接的情况
        else{
            printf("未连接状态\n");
            printf("菜单选项:\n");
            printf("1. 连接\n");
            printf("7. 退出\n");
            scanf("%d",&choice);
            switch (choice) {
                case CONNECT :
                    menuconnect();
                    break;
                case EXIT:
                    printf("退出成功!\n");
                    return;
                default:
                    printf("输入非法,请重新选择!\n");
            }


        }

    }
}
