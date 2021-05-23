#pragma once

#include "../../core/base_connection.hpp"
#include "connection.hpp"
#include "settings.hpp"

class http_server : public base_connection
{
public:
	explicit http_server(ui32 backlog, ui32 thread_pool_size);
	void run();
    void dispose(int signum);

private:
	void accept();
	void process();
	void accept_handler(const errc& err);
	void handleSignal(boost::system::error_code err_code, int signal);

private:
	asio_ctx context_{ 1 };
	acceptor acc_;
	connection_ptr conn_;
	endp_obj endpoint_;
	std::vector<sock_ptr_t> sockets;
	signal_set signals_;
	ui32 thread_pool_size_;
};