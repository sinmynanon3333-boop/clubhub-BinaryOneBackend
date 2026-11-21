
#include "crow.h"
#include <sqlite_orm/sqlite_orm.h>
#include <iostream>

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([] {
        return "ClubHub API is running!";
    });

    app.port(8080).multithreaded().run();
}
