#include "Util.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <vector>

sockaddr_in _server_address{};
using namespace std;
mutex mtx;
condition_variable cr;
list<Packet> msg_lst;
int _socket_fd = -1 ;
bool isConnect = false;
string temip;
int temport;
void thread_handler() {
	char buffer[BUFSIZE];
	recv(_socket_fd, buffer, BUFSIZE, 0);
	
	while (isConnect) {
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

int main(){
	while (true)
	{
		if(isConnect){
			while (true)
			{	// 显示菜单
				cout << "[NOW CONNECT]" << temip << ":" << temport << endl;
				cout << "Please choose from:\n"
				"[1] Connect\n"
				"[2] Disconnect\n"
				"[3] Get time\n"
				"[4] Get server name\n"
				"[5] Get client list\n"
				"[6] Send data to other client\n"
				"[7] Exit\n"
				"> ";
				int choice;
				cin >> choice;
				//对应功能选择
				switch (choice)
				{
					case 1: // 建立连接
					cout << " you have connected to "<< temip << ":" << temport << endl;
					break;
					
					case 2: {
						char fin = (char)DISCONNECT;
						if(send(_socket_fd,&fin,sizeof(fin),0) < 0){
							cerr << "[client] send fails, errno is " << errno << endl;
						}
						_socket_fd = -1;
						close(_socket_fd);
						isConnect = false;
						cout << "You have disconnected from " << temip << ":" << temport << endl;
						
					}
					// DisConnect
					break;
					
					case 3:{// GET time
						char buffer = (char)GET_TIME;
						send(_socket_fd,&buffer,sizeof(buffer),0);
						unique_lock<mutex> lck(mtx);//开一个锁
						while(msg_lst.empty()) cr.wait(lck);//等待
						auto t = msg_lst.front();
						msg_lst.pop_front(); // 取队首
						cout << "The time is " << t.data << endl;
					} 
					break;
					
					case 4: // GET NAME
					{
						char buffer = (char)GET_NAME;
						send(_socket_fd,&buffer,sizeof(buffer),0);
						unique_lock<mutex> lck(mtx);//开一个锁
						while(msg_lst.empty()) cr.wait(lck);//等待
						auto t = msg_lst.front();
						msg_lst.pop_front(); // 取队首
						cout << "The server name is " << t.data << endl;
						
					}
					break;
				case 5:
					{
						char buffer = (char)GET_ACTIVE_LIST;
						send(_socket_fd,&buffer,sizeof(buffer),0);
						unique_lock<mutex> lck(mtx);//开一个锁
						while(msg_lst.empty()) cr.wait(lck);//等待
						auto t = msg_lst.front();
						msg_lst.pop_front(); // 取队首
						cout << "The client list is \n" << t.data ;
					}break;
				case 6:
					{
						char buffer[BUFSIZE] = {0};
						string ip;
						int port;
						cout << "Please input the ip address of the client you want to send to: ";
						cin >> ip;
						cout << "Please input the port of the client you want to send to: ";
						cin >> port;
						cout << "Please input the message you want to send: ";
						cin >> buffer;
						string data = ip + ":" + to_string(port) + ":" + " " + buffer;
						char send_buffer[BUFSIZE] = {0};
						send_buffer[0] = (char)SEND_MSG ;
						memcpy(send_buffer + 1, data.c_str(), data.size());
						send(_socket_fd, send_buffer, strlen(send_buffer), 0);
						
						unique_lock<mutex> lck(mtx);//开一个锁
						while(msg_lst.empty()) cr.wait(lck);//等待
						auto t = msg_lst.front();
						msg_lst.pop_front(); // 取队首
						cout << "The message has been sent." << endl;
					}
					break;
				case 7:
					{
						char buffer = (char)EXIT;
						send(_socket_fd,&buffer,sizeof(buffer),0);
						_socket_fd = -1;
						close(_socket_fd);
						isConnect = false;
						cout << "You have disconnected from " << temip << ":" << temport << endl;
						cout << "Bye!" << endl;
						return 0;
					}
					break;
					default:{
						cout << "Invalid input!" << endl;
					}
					
					break;
				}
			}
		}else{
			// 显示菜单
			cout << "[NOW CONNECT]" << temip << ":" << temport << endl;
			cout << "Please choose from:\n"
			"[1] Connect\n"
			"[2] Exit\n"
			"> ";
			int choice;
			cin >> choice;	
			switch (choice)
			{
				case 1:{ // Connect
					cout << "Please input the ip address of the server: ";
					cin >> temip;
					cout << "Please input the port of the server: ";
					cin >> temport;
					
					_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
					_server_address.sin_family = AF_INET;
					_server_address.sin_port = htons(temport);
					_server_address.sin_addr.s_addr = inet_addr(temip.c_str());
					
					if(connect(_socket_fd,(struct sockaddr*)&_server_address,sizeof(struct sockaddr)) == -1){
						cerr << "Failed to establish a connection." << endl;
						close(_socket_fd);
						_socket_fd = -1;
						break;
					}
					isConnect = true;
					thread(&thread_handler).detach();
				}
				break;
				case 2:{
					cout << "Bye!" << endl;
					return 0;
				}
				break;
				
				default:{
					cout << "Invalid input!" << endl;
				}
				break;
			}
		}
	}
	
}
