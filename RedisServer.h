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
#include<windows.h>
#include <iomanip>
#include <chrono>
#include <ctime>
#include"ParserFlyweightFactory.h"
#include<signal.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library

class RedisServer{
private:
    WSADATA wsa;
    SOCKET serverSocket, newSocket;
    struct sockaddr_in server, client;
    int c;
    int port;
    bool stop=false;
    DWORD pid;
    std::string logoFilePath;
    RedisServer(int port=8888,std::string logoFilePath="logo"):port(port),logoFilePath(logoFilePath){
        pid = GetCurrentProcessId();
    }
    void handleClient(int clientSocket);
    // 信号处理函数
    // 控制台事件处理函数
    // static BOOL WINAPI ConsoleHandler(DWORD signal);
    static void signalHandler( int sig );
    bool initWinsock();
    void createSocket();
    void initServer();
    bool bindSocket();
    void printLogo();
    void printStartMessage();
    
    void replaceText(std::string& text,const std::string& toReplaceText, const std::string& replaceText);
    std::string getDate();
public:

    static std::shared_ptr<RedisServer> getInstance(){
        static std::shared_ptr<RedisServer> instance(new RedisServer());
        return instance;
    }
    void start();

};

bool RedisServer::initWinsock(){
    if(WSAStartup(MAKEWORD(2,2),&wsa) != 0){
        std::cout << "Failed. Error Code : " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

void RedisServer::createSocket(){
    serverSocket = socket(AF_INET , SOCK_STREAM , 0 ); 
    if(serverSocket==INVALID_SOCKET){
        std::cout << "Could not create socket : " << WSAGetLastError() << std::endl;
    }
    
}

void RedisServer::initServer(){
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
}

bool RedisServer::bindSocket(){
    if(bind(serverSocket ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR){
        std::cout << "Bind failed with error code : " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }
    return true;
}
void RedisServer::printLogo(){
    // 从文件读取并输出logo
    std::ifstream ifs(logoFilePath);
    std::string line="";
    while(std::getline(ifs,line)){
        replaceText(line,"PORT",std::to_string(port));
        replaceText(line,"PTHREAD_ID",std::to_string(pid));
        std::cout<<line<<std::endl;
    } 
}

void RedisServer::printStartMessage(){
    
    std::string startMessage="[PID] DATE # Server started.";
    std::string initMessage="[PID] DATE * The server is now ready to accept connections on port PORT";

    std::string PID=std::to_string(pid);
    std::string DATE=getDate();
    replaceText(startMessage,"PID",PID);
    replaceText(startMessage,"DATE",DATE);
    replaceText(initMessage,"PID",PID);
    replaceText(initMessage,"PORT",std::to_string(port));
    replaceText(initMessage,"DATE",DATE);

    
    std::cout<<startMessage<<std::endl;
    std::cout<<initMessage<<std::endl;
}
void RedisServer::start(){
    
    // 初始化Winsock库
    if(!initWinsock()){
        return ;
    }
    // 设置信号处理函数
    signal( SIGINT, signalHandler );
    // 创建一个套接字
    createSocket();
    // 准备sockaddr_in结构体
    initServer();
    // 绑定套接字到指定端口
    if (!bindSocket()) {
        return ;
    }
    printLogo();
    // 获取并输出当前进程的PID
    printStartMessage();
    // 监听传入的连接
    listen(serverSocket , 3);
    while(true){
        if(stop)
            break;
        c = sizeof(struct sockaddr_in);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&client, &c);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "accept failed with error code : " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return;
        }

        // 创建新线程处理客户端连接
        std::thread clientThread(&RedisServer::handleClient, this, clientSocket);
        clientThread.detach(); // 不阻塞主线程
    }

}

void RedisServer::handleClient(int clientSocket) {
    // 与客户端通信的代码
    // 接收数据、处理数据、发送响应
    //向客户端发送数据
    
    ParserFlyweightFactory flyweightFactory;
    
    while(true){
        if(stop)
            break;
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

void RedisServer::replaceText(std::string& text,const std::string& toReplaceText, const std::string& newText){
    int index=text.find(toReplaceText);
    if(index!=std::string::npos)
        // std::cout<<"toReplaceText="<<toReplaceText<<std::endl;
        text.replace(index,toReplaceText.size(),newText);
}

std::string RedisServer::getDate(){
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();
    // 转换为time_t类型
    auto now_c = std::chrono::system_clock::to_time_t(now);
    // 转换为tm结构
    std::tm* now_tm = std::localtime(&now_c);

    // 使用stringstream进行格式化
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%d %b %H:%M:%S");

    // 获取毫秒部分
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    // 添加毫秒到输出流
    oss << "." << std::setfill('0') << std::setw(3) << milliseconds.count();

    return oss.str();
}
void  RedisServer::signalHandler( int sig )
{
    if ( sig == SIGINT)
    {
        CommandParser::getRedisHelper()->flush();
        exit(0);
    }
}
