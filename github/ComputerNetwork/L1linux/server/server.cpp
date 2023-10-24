#include "Util.h"
using namespace std;

fd_t sock_fd;
sockaddr_in sock_addr_in{};
constexpr int max_connection = 20;
port_t default_port = 2362;
map<fd_t, client_t> client_list;
mutex mtx;

struct Args {
	fd_t conn_fd;
	map<fd_t, client_t> *client_list;
	mutex *mtx;
};

void conn_handler(Args args) {
	string helloStr{"Hello"};
	send(args.conn_fd, helloStr.c_str(), helloStr.length(), 0);
	
	char recv_buffer[BUFSIZE]={0};
	char send_buffer[BUFSIZE]={0};
	
	while (true)
	{
		auto length = recv(args.conn_fd, recv_buffer, BUFSIZE, 0);
		if(length <= 0) 
			cerr << "[server] recv() fails, errno is " << errno << endl;
		
		memset(send_buffer, 0, BUFSIZE);
		args.mtx->lock();
		switch (recv_buffer[0])
		{
			case DISCONNECT:{
				args.client_list->erase(args.conn_fd); // 从列表中删掉指定客户端
				cout << "[server] client#" << args.conn_fd << "disconnected\n";
			}
			break;
			
			case GET_TIME:{
				time_t t;
				time(&t);
				cout << "[server] client#" << args.conn_fd << "get time " << ctime(&t) << endl;
				
				send_buffer[0] = GET_TIME;
				sprintf(send_buffer + 1, "%s", ctime(&t));
				if(send(args.conn_fd, send_buffer, strlen(send_buffer), 0) < 0)
					cerr << "[server] send fails, errno is " << errno << endl;
				
			}
			break;	
			
		case GET_NAME:
			{
				cout << "[server] client#" << args.conn_fd << "get name " << endl;
				send_buffer[0] = GET_NAME;
				gethostname(send_buffer + 1, sizeof(send_buffer) - sizeof(char));
				if(send(args.conn_fd, send_buffer, strlen(send_buffer), 0) < 0)
					cerr << "[server] send fails, errno is " << errno << endl;
			}
			break;
			
			case GET_ACTIVE_LIST:{
				cout << "[server] client#" << args.conn_fd << "get active list " << endl;
				send_buffer[0] = GET_ACTIVE_LIST;
				
				for(const auto &client_list: *args.client_list){
					fd_t fd = client_list.first;
					inet_t ip = client_list.second.first;
					port_t port = client_list.second.second;
					string say = "[server] client#" + to_string(fd) + "@" + ip + ":" + to_string(port) + "\n";
					memcpy(send_buffer + strlen(send_buffer), say.c_str(), say.size());
				}
				
				if(send(args.conn_fd, send_buffer,strlen(send_buffer),0) < 0)
					cerr << "[server] send fails, errno is " << errno << endl;
			}
			break;
			
			case SEND_MSG:{
				string data = string(recv_buffer + 1);
				string ip = data.substr(0, data.find(":"));
				
				data = data.substr(data.find(":") + 1);
				port_t port = stoi(data.substr(0, data.find(":")));
				data = data.substr(data.find(":") + 2);
				cout << "[server] client#" << args.conn_fd << "send message to " << ip << ":" << port << endl;
				cout << "[server] message is " << data << endl;
				
				fd_t send_fd = -1;
				for(const auto &client_list: *args.client_list){ // 遍历 找到需要发送的客户端
					if(client_list.second.first == ip && client_list.second.second == port){
						send_fd = client_list.first;
						break;
					}
				}
				
				send_buffer[0] = SEND_MSG;
				if(send_fd == -1){
					sprintf(send_buffer + 1, "[server] The client you want to send to is not active\n");
				}else{
					sprintf(send_buffer + 1, "[server] success send %s", data.c_str());
				}
				
				char repost_buffer[BUFSIZE] = {0};
				repost_buffer[0] = FORWARD;
				sprintf(repost_buffer + 1, "[server] client#%d send message to you: %s", args.conn_fd, data.c_str());
				
				// 目标客户端转发消息
				if(send(args.conn_fd, send_buffer, strlen(send_buffer), 0) < 0)
					cerr << "[server] send fails, errno is " << errno << endl;
				
				// 源客户端回复确认信息
				if(send(send_fd, repost_buffer, strlen(repost_buffer), 0) < 0)
					cerr << "[server] send fails, errno is " << errno << endl;
				
			}
			break;
			default:
				break;
		}
		memset(recv_buffer,0,BUFSIZE);
		args.mtx->unlock();
	}
	
	
}

void *task(void *args) {
	conn_handler(*(Args *)args);
	return nullptr;
}

int main(){
	// 初始化 
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	sock_addr_in.sin_family = AF_INET;
	sock_addr_in.sin_port = htons(default_port);
	sock_addr_in.sin_addr.s_addr = htonl(INADDR_ANY); // 端口为本机所有地址
	
	bind(sock_fd, (sockaddr*)&sock_addr_in, sizeof(sock_addr_in));
	listen(sock_fd, max_connection);
	
	cout << "server is running !\n";
	
	while (true)
	{
		sockaddr_in client{};
		int client_addr_len = sizeof(client);
		
		//一直waiting 直到有端口呼叫
		int connection_fd = accept(sock_fd, (sockaddr*)&client, (socklen_t*)&client_addr_len);
		
		// 登记客户端信息
		client_list.insert(make_pair(connection_fd, client_t(inet_ntoa(client.sin_addr), ntohs(client.sin_port))));
		// 打印当前加入客户端信息
		cout << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " connected\n";
		pthread_t client_thread; // 申请一个客户端线程id
		Args args{connection_fd, &client_list, &mtx}; // 为这个客户端配置一把锁
		pthread_create(&client_thread, nullptr, task, (void*)&args); // 创建线程
	}
	
	
}
