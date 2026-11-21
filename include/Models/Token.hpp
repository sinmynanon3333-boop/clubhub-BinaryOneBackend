#include<string>
struct Token
{
    int id=0;  
    std::string tokenHash;
    int role=0;
    int OwerId=0;
    std::string createdAt;
    std::string experiedAt;
    bool revoked=false;
};
