#include <iostream>
#include <winsock2.h>
#include<string>
#include "RedisClient.h"
#pragma comment(lib, "ws2_32.lib") // Winsock Library

int main(){
    RedisClient redisClient;
    redisClient.start();
}