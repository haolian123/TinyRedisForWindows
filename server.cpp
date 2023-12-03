#include"RedisServer.h"
#pragma comment(lib, "ws2_32.lib") // Winsock Library

int main(){
   RedisServer::getInstance()->start();    
}