//
// Created by Lenovo on 2023/10/23.
//

#ifndef COMPUTERNETWORK_LIB_H
#define COMPUTERNETWORK_LIB_H

#include <iostream>
#include <cstring>
#include <string>
#include <winsock2.h>
#include <map>
#include <thread>
#include <csignal>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mqrt.lib")
#define PACKETSIZE 256

//以下是对请求 或 响应符号的定义
#define TIME_REQUEST 1
#define NAME_REQUEST 2
#define LIST_REQUEST 3
#define MESG_REQUEST 4 	// 发送消息请求
#define DISC_REQUEST 5	// 断连请求

#define TIME_RESPONSE 6
#define NAME_RESPONSE 7
#define LIST_RESPONSE 8
#define MESG_RESPONSE 9 // 发送一条消息
#define DISC_RESPONSE 10

#define INDICATE	  11// 指示
//数据包格式定义 包头一个byte 然后是size 然后是data段 至多254个B


// 服务器用于记录ClientInfo的结构体
struct ClientInfo{
    SOCKET* clientSocket{};
    string IP;
    int port{};
    ClientInfo(){};
    ClientInfo(SOCKET* clientSocket ,string IP,int port){
        this->clientSocket = clientSocket;
        this->IP = IP ;
        this->port = port;
    }
};
// 客户端列表
map<int, ClientInfo> clients_map; // 编号 客户端套接字 IP 端口



#endif //COMPUTERNETWORK_LIB_H
