#include <iostream>
#include <winsock2.h>
#include<string>
#pragma comment(lib, "ws2_32.lib") // Winsock Library

int main(int argc , char *argv[]) {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    std::string message;
    char serverMessage[2000];
    int recv_size;

    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        std::cout << "Failed. Error Code : " << WSAGetLastError() << std::endl;
        return 1;
    }
    
    std::cout << "Initialized." << std::endl;
    
    //创建套接字
    if ((s = socket(AF_INET , SOCK_STREAM , 0  )) == INVALID_SOCKET) {
        std::cout << "Could not create socket : " << WSAGetLastError() << std::endl;
    }

    std::cout << "Socket created." << std::endl;
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //连接到远程服务器
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0) {
        std::cout << "connect error." << std::endl;
        return 1;
    }
    
    std::cout << "Connected" << std::endl;
            
    while(true){
        //发送数据
        std::cout<<"127.0.0.1:8888> ";
        std::getline(std::cin,message);
        if (send(s , message.c_str() , message.size() , 0) < 0) {
            std::cout << "Send failed" << std::endl;
            break;
        }
        //接收服务器的数据
        if ((recv_size = recv(s, serverMessage, 2000, 0)) == SOCKET_ERROR) {
            std::cout << "recv failed" << std::endl;
        }


        //添加结束字符
        serverMessage[recv_size] = '\0';
        std::cout << serverMessage << std::endl;
    }


    closesocket(s);
    WSACleanup();

    return 0;
}
