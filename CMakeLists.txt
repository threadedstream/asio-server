cmake_minimum_required(VERSION 3.19)
project(asioserver)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS thread REQUIRED)

add_executable(asioserver CppServer/core/app.cpp CppServer/core/math.cpp
        CppServer/servers/http_server/connection.cpp CppServer/servers/http_server/response.cpp
        CppServer/servers/http_server/request.cpp CppServer/servers/http_server/http_server.cpp
        main.cpp)

target_link_libraries(asioserver PRIVATE Threads::Threads Boost::thread)
