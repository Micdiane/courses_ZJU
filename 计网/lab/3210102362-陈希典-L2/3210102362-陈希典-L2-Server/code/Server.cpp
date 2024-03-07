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
		closesocket(serverSocket);
    	WSACleanup();
    }
}



bool checkDoubleEnter(const char* data, int dataLength) {
    int count = 0;
    
    for (int i = 0; i < dataLength; i++) {
        if (data[i] == '\r') {
            // 如果找到回车符，检查下一个字符是否是换行符
            if (i + 1 < dataLength && data[i + 1] == '\n') {
                count++;
                i++; // 跳过换行符
            }
        }
        
        // 如果已经找到两个回车换行组合，返回true
        if (count == 2) {
            return true;
        }
    }
    
    return false;
}
void POSThandle(const std::string& request_url, SOCKET clientSocket, char *p) {
    bool isDopost = (request_url == "/dopost");

	string recvBuffer(p);
	
	// cout << recvBuffer ; 
    if (isDopost) {
        // 查找请求体结束标志 "\r\n\r\n" 的位置
        std::size_t bodyEndPos = recvBuffer.find("\r\n\r\n");
        if (bodyEndPos != std::string::npos) {
            // 从请求体结束标志位置开始解析表单字段
            std::string contentBody = recvBuffer.substr(bodyEndPos + 4);
			// cout << contentBody ;
            // 使用 & 符号分割表单字段
            std::vector<std::string> formData;
			std::vector<std::string> Values;
            size_t startPos = 0;
            while (startPos < contentBody.size()) {
                size_t endPos = contentBody.find("&", startPos);
                if (endPos == std::string::npos) {
                    endPos = contentBody.size();
                }
                formData.push_back(contentBody.substr(startPos, endPos - startPos));
                startPos = endPos + 1;
            }

            // 解析表单字段的键和值
            for (const std::string& field : formData) {
                size_t equalsPos = field.find("=");
                if (equalsPos != std::string::npos) {
                    std::string key = field.substr(0, equalsPos);
                    std::string value = field.substr(equalsPos + 1);
					Values.push_back(value);
                }
            }
			
			if(Values[0] == "3210102362" && Values[1] == "2362"){
				const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 22\r\n\r\nlogin in successfully!";
				send(clientSocket, response, strlen(response), 0);
			}else{
				const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 16\r\n\r\nlogin in failed!";
				send(clientSocket, response, strlen(response), 0);
			}
        } else {
            // 未找到请求体结束标志，处理错误
            const char* errorResponse = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nBad Request";
            send(clientSocket, errorResponse, strlen(errorResponse), 0);
        }
    } else {
        // 处理其他情况或错误
        const char* errorResponse = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nBad Request";
        send(clientSocket, errorResponse, strlen(errorResponse), 0);
    }
}





//解析出来的方法是GET
void GEThandle(string request_url,SOCKET clientSocket){
	// Define the content type based on the file extension
		std::string content_type;
		if (request_url.find(".html") != std::string::npos) {
			content_type = "text/html";
		} else if (request_url.find(".jpg") != std::string::npos) {
			// content_type = "text/plain";
			content_type = "image/jpeg";
		} else if (request_url.find(".txt") != std::string::npos) {
			content_type = "text/plain";
		} else {
			content_type = "text/plain"; // Default content type (plain text)
		}

		// Check if the request URL is empty, and if so, send a "hello" response
		if (request_url.empty() | request_url == "/") {
			const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 5\r\n\r\nhello";
			send(clientSocket, response, strlen(response), 0);
			return;
		}

		// Prepare the file path
		std::string filePath;
		if (request_url == "/test.html") {
			filePath = "code/websrc/html/test.html";
		} else if (request_url == "/logo.jpg") {
			// filePath = "code/websrc/txt/test.txt";
			filePath = "code/websrc/img/logo.jpg";
		} else if (request_url == "/test.txt") {
			// filePath = "code/websrc/img/logo.jpg";
			filePath = "code/websrc/html/test.html";
			// filePath = "code/websrc/txt/test.txt";
		} else {
			// Handle 404 Not Found for other URLs
			const char* response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile not found.";
			send(clientSocket, response, strlen(response), 0);
			return;
		}

		// Use C standard library functions to read the file
		FILE* file = fopen(filePath.c_str(), "rb");
		if (file) {
			// Determine the content length for the response
			fseek(file, 0, SEEK_END);
			long contentLength = ftell(file);
			// cout << " contentLength "<< contentLength << endl;
			fseek(file, 0, SEEK_SET);

			// Construct the HTTP response with proper headers
			std::string response = "HTTP/1.1 200 OK\r\n";
			response += "Content-Type: " + content_type + "\r\n";
			response += "Content-Length: " + std::to_string(contentLength) + "\r\n\r\n";

			int headerlen = strlen(response.c_str());
			char* sendbuff = new char[SENDBUFFSIZE];
			memset(sendbuff, 0, SENDBUFFSIZE);
			while (fread(sendbuff + headerlen, 1, contentLength, file) > 0 );
			memcpy(sendbuff, response.c_str(), headerlen);


			// Send the response header
			send(clientSocket, sendbuff, headerlen + contentLength + 10, 0);
			fclose(file);
		} else {
			// Handle file not found
			const char* response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile not found.";
			send(clientSocket, response, strlen(response), 0);
		}
}

//解析出来的方法是POST




void parse_request_header(SOCKET clientSocket, char * request) {
	

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

	if(request_method == "GET"){
		GEThandle(request_url,clientSocket);
	}else if(request_method == "POST"){
		POSThandle(request_url,clientSocket,request);
	}
}



//不断读取客户端发送过来的字节，并检查其中是否连续出现了2个回车换行符
//如果出现，按照HTTP格式解析第1行，分离出方法、文件和路径名，其他头部字段根据需要读取。
void clientHandle(SOCKET* clientSocketptr, int clientIdx){

	cout << "client " << clientIdx << clients_map[clientIdx].IP << " connected!" << endl; 

    char recvBuffer[2048] = {0};
	bool HandleFinish = false;
    while(!HandleFinish & serverRunning){ // serverRunning  用来通知子线程退出
        int recv_handler = recv(*clientSocketptr, recvBuffer, 2048, 0);
		if (recv_handler == SOCKET_ERROR) {
			std::cerr << "Failed to read data from socket. Error code: " 
				<< WSAGetLastError() << std::endl;
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
			if(checkDoubleEnter(recvBuffer,100)){
			parse_request_header(*clientSocketptr, recvBuffer);
			HandleFinish = true;
			}
		}
		
	}
	Sleep(1); // 很重要,因为HTML还有后续请求,这需要一定时间相应
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
    std::signal(SIGINT, signalHandler); // 登记一个信号,用来退出服务器
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
        
        // 然后创建一个子线程后继续调用accept()
		thread clientThread(clientHandle,clientSocket,temidx);
        clientThread.detach();
    }
	cout <<"Finish!" << endl;
    return 0;
}