#pragma once

#include "../servers/http_server/http_server.hpp"


class App final {
public:
    App() :
            server_http_(5, 5) {};

    FORCE_INLINE void run_http_server() { server_http_.run(); }

    void dispose(int signum) { server_http_.dispose(signum); }

private:
    http_server server_http_;

};