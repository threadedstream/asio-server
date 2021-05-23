#pragma once

#include "../../core/logger.hpp"
#include "../../core/non_copyable.hpp"
#include "../../core/dpconn.hpp"
#include "response.hpp"
#include "settings.hpp"
#include <sys/stat.h>

//NOTE: Temporary declarations
#define FILENAME_DEMO "F:/demo.cpp"
#define FILENAME "index.html"
#define TEMPLATE_DIR ".\\templates"
#define DBLOGFILE "F:/cppserver/CppServer/dblog.txt"

class connection :
        public boost::enable_shared_from_this<connection>,
        private non_copyable {
public:
    explicit connection(asio_ctx &context);

    inline sock_t &socket() { return socket_; }

    void start_processing();

    void closeConnection() { socket_.close(); }

private:
    sock_t socket_;
    std::string response_string;
    settings stx;
    response resp;
    boost::array<char, 8192> buffer_;
private:
    inline bool file_exists(const std::string &filename) {
        struct stat buffer;
        return (stat(filename.c_str(), &buffer) == 0);
    }
};