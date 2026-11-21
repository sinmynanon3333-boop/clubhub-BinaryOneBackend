
cmake_minimum_required(VERSION 3.18)

project(Clubhub CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Asio
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-30-2
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR _none
)

# Crow
FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.2.0
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR _none
)

# SQLite ORM
FetchContent_Declare(
    sqlite_orm
    GIT_REPOSITORY https://github.com/fnc12/sqlite_orm.git
    GIT_TAG v1.9.1
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR _none
)

# Download dependencies
FetchContent_MakeAvailable(asio crow sqlite_orm)

# SQLite database
find_package(SQLite3 REQUIRED)

# Header-only dependencies
add_library(crow_deps INTERFACE)

target_include_directories(crow_deps INTERFACE
    ${asio_SOURCE_DIR}/asio/include
    ${crow_SOURCE_DIR}/include
    ${sqlite_orm_SOURCE_DIR}/include
)

target_compile_definitions(crow_deps INTERFACE
    ASIO_STANDALONE
)

# Main executable
add_executable(clubhub
    src/main.cpp
)

target_link_libraries(clubhub PRIVATE
    crow_deps
    SQLite3::SQLite3
)

# Windows networking libraries
if(WIN32)
    target_link_libraries(clubhub PRIVATE
        ws2_32
        mswsock
    )
endif()
