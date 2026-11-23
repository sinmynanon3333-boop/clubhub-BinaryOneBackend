#pragma once
#include<string>
struct Token
{
    int id=0;  
    std::string tokenHash;
    std::string role="student";
    int OwerId=0;
    std::string createdAt;
    bool revoked=false;
};
