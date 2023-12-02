#include<iostream>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include<atomic>
#include<cassert>
#include <condition_variable>
#include <future>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <winsock2.h>

#include"ParserFlyweightFactory.h"
#pragma comment(lib, "ws2_32.lib") // Winsock Library

void handleClient(int clientSocket) {
    // 与客户端通信的代码
    // 接收数据、处理数据、发送响应
    //向客户端发送数据
   
    ParserFlyweightFactory flyweightFactory;
    
    while(true){
        //接收客户端传来的数据
        char clientMessage[2000];
        int recv_size;
        if((recv_size = recv(clientSocket, clientMessage, 2000, 0))>0){
            clientMessage[recv_size]='\0';
            std::string commandLine=std::string(clientMessage);
            auto tokens = split(commandLine);
            std::string command=tokens[0];
            std::shared_ptr<CommandParser> commandParser=flyweightFactory.getParser(command);
            std::string responseMessage="";
            if(commandParser==nullptr){
                responseMessage+="command "+commandLine+" does not exist!";
            }else{
                responseMessage=commandParser->parse(tokens);
            }
            send(clientSocket , responseMessage.c_str() , responseMessage.size() , 0);
        }

        
    }


}
int main() {
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    // const  char *message;

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        std::cout << "Failed. Error Code : " << WSAGetLastError() << std::endl;
        return 1;
    }
    //创建一个套接字
    if ((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        std::cout << "Could not create socket : " << WSAGetLastError() << std::endl;
    }

    //准备sockaddr_in结构
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //绑定
    if (bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR) {
        std::cout << "Bind failed with error code : " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }
    //读入logo
    std::ifstream ifs("logo");
    std::string line="";
    while(std::getline(ifs,line)){
        std::cout<<line<<std::endl;
    } 
    // 获取当前进程的PID
    DWORD pid = GetCurrentProcessId();
    std::string startMessage="[PID] DATE # Server started.";
    std::cout<<startMessage<<std::endl;
    std::string initMessage="[PID] DATE * The server is now ready to accept connections on port PORT";
    std::cout<<initMessage<<std::endl;
    //监听传入的连接
    listen(s , 3);
    while(true){
        //接受一个传入的连接
        // std::cout << "Waiting for incoming connections..." << std::endl;
        c = sizeof(struct sockaddr_in);
        int clientSocket = accept(s , (struct sockaddr *)&client, &c);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "accept failed with error code : " << WSAGetLastError() << std::endl;
            closesocket(s);
            WSACleanup();
            return 1;
        }
        // std::cout << "Connection accepted." << std::endl;
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach(); // 不阻塞主线程

    }
    return 0;
}
