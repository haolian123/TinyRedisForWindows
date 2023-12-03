# 定义编译器
CC=g++
# 定义编译选项
CFLAGS=-g -std=c++11 
LDFLAGS=-lws2_32
# 定义共同依赖的头文件
COMMON_HEADERS=CommandParse.h FileCreator.h global.h ParserFlyweightFactory.h  RedisHelper.h SkipList.h 
# 默认目标
all: server.exe client.exe
# 编译server.exe
server.exe: server.cpp RedisServer.h $(COMMON_HEADERS)
	$(CC) $(CFLAGS) server.cpp  RedisHelper.cpp -o server.exe $(LDFLAGS)
# 编译client.exe
client.exe: client.cpp RedisClient.h $(COMMON_HEADERS)
	$(CC) $(CFLAGS) client.cpp  RedisHelper.cpp -o client.exe $(LDFLAGS)
# 清理目标
clean:
	rm -f *.exe
