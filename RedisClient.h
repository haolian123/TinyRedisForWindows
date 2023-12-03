#include <iostream>
#include <winsock2.h>
#include<string>
#pragma comment(lib, "ws2_32.lib") // Winsock Library
#define DEFAULT_HOSTNAME "127.0.0.1"
#define DEFAULT_PORT 8888

class RedisClient{
private:
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in server;
    std::string message;
    char serverMessage[2000];
    int recvSize;
    std::string hostName="";
    int port;
    std::string PORT="";
private:
    bool initWinsock();

    bool createSocket();

    void initServer();

    bool getConnection();
public:
    RedisClient(const std::string&Host,int port);
    RedisClient();
    void start();
};



RedisClient::RedisClient():hostName(DEFAULT_HOSTNAME),port(DEFAULT_PORT){
    PORT=std::to_string(DEFAULT_PORT);
}
RedisClient::RedisClient(const std::string&Host,int port):hostName(Host),port(port){
    PORT=std::to_string(port);
}
void RedisClient::start(){
    if(!initWinsock()) return;

    if(!createSocket()) return;

    initServer();

    if(!getConnection()) return;

    while(true){
        //发送数据
        std::cout<<hostName<<":"<<PORT<<"> ";
        std::getline(std::cin,message);
        if (send(clientSocket , message.c_str() , message.size() , 0) < 0) {
            std::cout << "Send failed" << std::endl;
            break;
        }
        //接收服务器的数据
        if ((recvSize = recv(clientSocket, serverMessage, 2000, 0)) == SOCKET_ERROR) {
            std::cout << "recv failed" << std::endl;
        }


        //添加结束字符
        serverMessage[recvSize] = '\0';
        std::cout << serverMessage << std::endl;
    }


    closesocket(clientSocket);
    WSACleanup();
}

bool RedisClient::initWinsock(){
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        std::cout << "Failed. Error Code : " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool RedisClient::createSocket(){
    if ((clientSocket = socket(AF_INET , SOCK_STREAM , 0  )) == INVALID_SOCKET) {
        std::cout << "Could not create socket : " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

void RedisClient::initServer(){
    server.sin_addr.s_addr = inet_addr(hostName.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
}

bool RedisClient::getConnection(){
    if (connect(clientSocket , (struct sockaddr *)&server , sizeof(server)) < 0) {
        std::cout << "connect error." << std::endl;
        return false;
    }
    return true;
}
