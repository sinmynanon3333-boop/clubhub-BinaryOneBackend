
#include "crow.h"
#include "../include/database/Clubhub.hpp"
#include "../include/Controller/Student.hpp"

int main()
{
    crow::SimpleApp app;
    auto db=initDatabase();
    RegisterStudentRoute(app,db);
    app.port(8080).multithreaded().run();
}
