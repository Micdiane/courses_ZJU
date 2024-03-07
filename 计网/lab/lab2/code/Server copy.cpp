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

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        serverRunning = false;
        cout << "BYEBYE!" << endl;
    }
}

void MakeResponsePacket(char HeadType,char* buffer){
    
}

bool checkDoubleEnter(){
	// 检测来自Socket的输入流如果出现两个回车 那么返回true
}

string PhasePacket(){
	// 按照HTTP格式解析第1行，分离出方法、文件和路径名，其他头部字段根据需要读取
}

//解析出来的方法是GET
void GEThandle(){

}

//解析出来的方法是POST
void POSThandle(){
	std::vector<std::pair<std::string, std::string>> data;

    std::istringstream body_stream(request_body);
    std::string key_value_pair;
    while (std::getline(body_stream, key_value_pair, '&')) {
        std::istringstream pair_stream(key_value_pair);
        std::string key, value;
        if (std::getline(pair_stream, key, '=') && std::getline(pair_stream, value)) {
            data.emplace_back(key, value);
        }
    }

    // Process the extracted data
    for (const auto& pair : data) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        // You can perform any desired processing on the data here
    }
}

// 将消息推入buffer 并发送
void pushToBufferAndSend(){

}

// Function to parse the header of an HTTP GET request
void parse_get_request_header(SOCKET clientSocket,const std::string& request) {
    std::istringstream request_stream(request);
    std::string request_line;
    
    // Read the first line of the request, which contains the request method, URL, and HTTP version
    std::getline(request_stream, request_line);
    
    // Parse the request line
    std::string request_method;
    std::string request_url;
    std::string http_version;
    std::istringstream request_line_stream(request_line);
    request_line_stream >> request_method >> request_url >> http_version;

    // Output the parsed information
    std::cout << "Request Method: " << request_method << std::endl;
    std::cout << "Request URL: " << request_url << std::endl;
    std::cout << "HTTP Version: " << http_version << std::endl;

    // Now, you can handle the other headers if needed
    // To parse additional headers, loop through the lines in the request and extract header information.
		string filePath;
        if (request_url == "/test.html") {
            filePath = "code/websrc/html/test.html";
        } else if (request_url == "/logo.jpg") {
            filePath = "code/websrc/img/logo.jpg";
        } else if (request_url == "/test.txt") {
            filePath = "code/websrc/txt/test.txt";
        } else {
            // Handle 404 Not Found for other URLs
            const char* response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile not found.";
            send(clientSocket, response, strlen(response), 0);
            return;
        }

	
	// Use C standard library functions to read the file
        FILE* file = fopen(filePath.c_str(), "rb");
        if (file) {
			cout << "file found" << endl;
            // Determine the content length for the response
            fseek(file, 0, SEEK_END);
            long contentLength = ftell(file);
            fseek(file, 0, SEEK_SET);

            // Construct the HTTP response with proper headers
            string response = "HTTP/1.1 200 OK\r\n";
            response += "Content-Type: "; // Set the appropriate content type (e.g., text/html, image/jpeg, text/plain)
            if (request_url.find(".html") != string::npos) {
                response += "text/html";
            } else if (request_url.find(".jpg") != string::npos) {
                response += "image/jpeg";
            } else if (request_url.find(".txt") != string::npos) {
                response += "text/plain";
            } else {
                response += "application/octet-stream"; // Default content type
            }
            response += "\r\n";
            response += "Content-Length: " + to_string(contentLength) + "\r\n\r\n";

            response += "hello!";
			send(clientSocket, response.c_str(), response.size(), 0);
            fclose(file);

		} else {
			// Handle file not found
			const char* response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile not found.";
			send(clientSocket, response, strlen(response), 0);
		}


}




//不断读取客户端发送过来的字节，并检查其中是否连续出现了2个回车换行符
//如果出现，按照HTTP格式解析第1行，分离出方法、文件和路径名，其他头部字段根据需要读取。
void clientHandle(SOCKET* clientSocketptr, int clientIdx){
    char recvBuffer[PACKETSIZE+10] = {0};
	bool HandleFinish = false;
    while(!HandleFinish){
        int recv_handler = recv(*clientSocketptr, recvBuffer, PACKETSIZE, 0);
		if (recv_handler == SOCKET_ERROR) {
			std::cerr << "Failed to read data from socket. Error code: " << WSAGetLastError() << std::endl;
			closesocket(*clientSocketptr);
			WSACleanup();
			return ;
		}
		else if (recv_handler == 0) {
			std::cout << "Client disconnected." << std::endl;
			closesocket(*clientSocketptr);
			WSACleanup();
			return ;
		}
		else {
			std::cout << "Received data from client: " << recvBuffer << std::endl;
		}

		parse_get_request_header(*clientSocketptr, recvBuffer);
		HandleFinish = true;
	}
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


int main(){
	// FILE* file = fopen("code/websrc/txt/test.txt", "rb");
	// if(!file) cout << " cuole ";
	// return 0;

    // std::signal(SIGINT, signalHandler); // 登记一个信号
    Init();// 套接字初始化
	
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