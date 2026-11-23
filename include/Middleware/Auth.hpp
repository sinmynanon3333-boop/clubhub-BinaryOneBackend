#pragma once
#include <string>
struct  AuthMiddleWare
{
    struct  context
    {
        std::string role;
        int userId;
    };
    
};
