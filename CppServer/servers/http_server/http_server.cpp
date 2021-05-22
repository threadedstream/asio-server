#include "http_server.hpp"


http_server::http_server(ui32 backlog, ui32 thread_pool_size) :
        base_connection("127.0.0.1", 80, backlog),
        endpoint_(ip::address::from_string(addr_), port_),
        thread_pool_size_(thread_pool_size),
        signals_(context_),
        acc_(context_) {

    process();
}


void http_server::run() {
    std::vector <thread_ptr> threads;
    for (size_t i = 0; i < thread_pool_size_; ++i) {

        thread_ptr thread(new boost::thread(
                boost::bind(&asio_ctx::run, &context_)));

        threads.emplace_back(thread);
    }

    for (size_t i = 0; i < thread_pool_size_; ++i) {
        threads[i]->join();
    }
}

void http_server::process() {
    Logger::log(SEVERITY::DEBUG, "Preparing mandatory resources...\n");
    Logger::log(SEVERITY::DEBUG, std::to_string(endpoint_.port()) + "\n");
    acc_.open(endpoint_.protocol());
    Logger::log(SEVERITY::DEBUG, "Connection is opened\n");
    errc err;
    acc_.bind(endpoint_, err);
    acc_.set_option(acceptor::reuse_address(true));
    if (err) {
        Logger::log(SEVERITY::ERR, err.message() + "\n");
        exit(-1);
    }

    signals_.add(SIGINT);
    signals_.add(SIGTERM);
#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif
    signals_.async_wait(boost::bind(&http_server::handle_stop, this));

    acc_.set_option(ip::tcp::acceptor::reuse_address(true), err);
    if (err) {
        Logger::log(SEVERITY::ERR, err.message());
    }
    Logger::log(SEVERITY::DEBUG, "Listening for incoming connections...\n");
    acc_.listen(backlog_, err);
    if (err) {
        Logger::log(SEVERITY::ERR, err.message());
    }
    accept();
}

void http_server::accept() {
    conn_.reset(new connection(context_));
    acc_.async_accept(conn_->socket(), boost::bind(&http_server::accept_handler, this,
                                                   placeholders::error));
}

void http_server::accept_handler(const errc &err) {
    if (!err) {
        conn_->start_processing();
    }
    accept();
}

void http_server::handle_stop() {
    acc_.close();
    context_.stop();
}
