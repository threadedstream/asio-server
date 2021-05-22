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

private:
    //methods
    void write_completion_handler(const errc &err, size_t bytes_transferred);

    void read_completion_handler(const errc &err, size_t bytes_transferred);

    void load_file(const std::string &filename);

    //void write_headers_to_file(const std::string& str,err_code& err);
    std::string _now();

private:
    //fields
    sock_t socket_;
    //strnd strand_;
    std::string response_string;
    settings stx;
    response resp;
    //boost::shared_ptr<dbconn> connInstance;
    boost::array<char, 8192> buffer_;
    std::ofstream dbwriter;

private:
    //methods
    inline bool file_exists(const std::string &filename) {
        struct stat buffer;
        return (stat(filename.c_str(), &buffer) == 0);
    }
};