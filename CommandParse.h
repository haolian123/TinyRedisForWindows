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
    virtual std::string parse(std::vector<std::string>& tokens)=0;
};
std::shared_ptr<RedisClient> CommandParser::redisClient=std::make_shared<RedisClient>();

// select
class SelectParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<2){
            return "wrong number of arguments for SELECT.";
        }
        int index=0;
        try{
            index=std::stoi(tokens[1]);
        }catch(std::invalid_argument const &e){
            return tokens[1] +" is not a numeric type";
        }
        return redisClient->select(index);
    }
};

//set
class SetParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
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
class SetnxParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<3){
            return "wrong number of arguments for SETNX.";
        }
        return redisClient->setnx(tokens[1],tokens[2]);
    }
};
//setex
class SetexParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<3){
            return "wrong number of arguments for SETEX.";
        }
        return redisClient->setex(tokens[1],tokens[2]);
    }
};

//get
class GetParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<2){
            return "wrong number of arguments for GET.";
        }
        return redisClient->get(tokens[1]);
    }
};
//keys
class KeysParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        return redisClient->keys();
    }
};

// dbsize
class DBSizeParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        return redisClient->dbsize();
    }
};

// exists
class ExistsParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<2){
            return "wrong number of arguments for EXISTS.";
        }
        tokens.erase(tokens.begin());
        return redisClient->exists(tokens);
    }
};
//del
class DelParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<2){
            return "wrong number of arguments for DEL.";
        }
        tokens.erase(tokens.begin());
        return redisClient->del(tokens);
    }
};
//rename
class RenameParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<3){
            return "wrong number of arguments for RENAME.";
        }
        return redisClient->rename(tokens[1],tokens[2]);
    }
};
//incr
class IncrParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<2){
            return "wrong number of arguments for INCR.";
        }
        return redisClient->incr(tokens[1]);
    }
};
//incryby
class IncrbyParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<3){
            return "wrong number of arguments for INCRBY.";
        }
        int increment=0;
        try{
            increment=std::stoi(tokens[2]);
        }catch(std::invalid_argument const &e){
            return tokens[2] +" is not a numeric type";
        }
        return redisClient->incrby(tokens[1],increment);
    }
};
//incrbyfloat
class IncrbyfloatParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<3){
            return "wrong number of arguments for INCRBYFLOAT.";
        }
        double increment=0;
        try{
            increment=std::stod(tokens[2]);
        }catch(std::invalid_argument const &e){
            return tokens[2] +" is not a numeric type";
        }
        return redisClient->incrbyfloat(tokens[1],increment);
    }
};
//decr
class DecrParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<2){
            return "wrong number of arguments for DECR.";
        }
        return redisClient->decr(tokens[1]);
    }
};

//decrby
class DecrbyParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<3){
            return "wrong number of arguments for DECRBY.";
        }
        int increment=0;
        try{
            increment=std::stoi(tokens[2]);
        }catch(std::invalid_argument const &e){
            return tokens[2] +" is not a numeric type";
        }
        return redisClient->decrby(tokens[1],increment);
    }
};

//mset
class MSetParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<2){
            return "wrong number of arguments for MSET.";
        }
        tokens.erase(tokens.begin());
        return redisClient->mset(tokens);
    }
};

//mget
class MGetParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<2){
            return "wrong number of arguments for MGET.";
        }
        tokens.erase(tokens.begin());
        return redisClient->mget(tokens);
    }
};

//strlen
class StrlenParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<2){
            return "wrong number of arguments for STRLEN.";
        }
        return redisClient->strlen(tokens[1]);
    }
};
//append
class AppendParser:public CommandParser{
public:
    std::string parse(std::vector<std::string>& tokens){
        if(tokens.size()<3){
            return "wrong number of arguments for APPEND.";
        }
        return redisClient->append(tokens[1],tokens[2]);
    }
};
#endif
