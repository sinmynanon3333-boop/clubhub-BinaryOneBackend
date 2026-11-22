#pragma once
#include <jwt-cpp/jwt.h>
#include <chrono>
#include <cstdlib>
namespace Jwt{
    const std::string jwtkey=std::getenv("jwt");
    
}