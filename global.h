
#ifndef GLOBAL
#define GLOBAL
#include<iostream>
#include<unordered_map>
#include<sstream>
enum SET_MODEL{
    NONE,NX,XX
};

enum Command{
    SET,
    SETNX,
    SETEX,
    GET
};

static std::unordered_map<std::string,enum Command>commandMaps={
    {"set",SET},
    {"setnx",SETNX},
    {"setex",SETEX},
    {"get",GET}
};



static std::vector<std::string> split(const std::string &s, char delimiter=' ') {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}







#endif