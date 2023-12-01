#ifndef COMMAND_PARSER
#define COMMAND_PARSER
#include<iostream>
#include <sstream>
#include <vector>
#include <string>
#include "global.h"
#include "RedisHelper.h"




class CommandParser{
protected:
    static std::shared_ptr<RedisClient>redisClient;
public:
    virtual std::string parse(const std::string& line)=0;
};
std::shared_ptr<RedisClient> CommandParser::redisClient=std::make_shared<RedisClient>();
// select
// class SelectParser:CommandParser{

// };

//set
class SetParser:CommandParser{
public:
    std::string parse(const std::string& line){
        auto tokens = split(line);
        if(tokens.size()<3){
            return "wrong number of arguments for SET.";
        }
        if(tokens.size()==4){
            if(tokens.back()=="NX"){
                return redisClient->set(tokens[1],tokens[2],NX);
            }else if(tokens.back()=="XX"){
                return redisClient->set(tokens[1],tokens[2],XX);
            }
        }
        return redisClient->set(tokens[1],tokens[2]);
    }
};

// //setnx
// class SetnxParser:CommandParser{

// }
class GetParser:CommandParser{
public:
    std::string parse(const std::string& line){
        auto tokens = split(line);
        if(tokens.size()<2){
            return "wrong number of arguments for GET.";
        }
        return redisClient->get(tokens[1]);
    }
};

#endif
/*


dbsize 

exists

del 

set
setnx
setex

rename

get

incr
incrby
incrbyfloat

decr
decrby

mset
mget

strlen

append



*/

// class ParserFlyweightFactory{
// public:
//     static CommandParser createPaser(int command){

//     }
// }