#pragma once

#include "../core/logger.hpp"

struct sync_client {
	sock_ptr_t socket_;
	std::string reqbuf;
	streambuf respbuf;
	std::string respstr;
	void initiate(err_code& err) {
		if (socket_->is_open()) {
			Logger::log(SEVERITY::DEBUG, "Attempting to write some data\n");
			sync_write(err);
			if (err.cause == "") {
				sync_read(err);
			}
			else {
				return;
			}
		}
		else {
			err.cause = "Socket is closed\n";
			return;
		}
	}

	void operator()(err_code& err) {
		initiate(err);
	}

private:
	void sync_write(err_code& err) {
		errc err_write;
		socket_->write_some(buffer(reqbuf, reqbuf.length()), err_write);
		if (err_write) {
			err.cause = err_write.message();
			return;
		}
	}
	void sync_read(err_code& err) {
		errc err_read;
		try {
			read(*socket_.get(), respbuf);
		}
		catch (const boost::system::system_error& err){
			std::cout << err.what();
		}
		auto bufs = respbuf.data();
		respstr = std::string(buffers_begin(bufs), buffers_begin(bufs) + respbuf.size());
		std::cout << &respbuf;
	}
};

struct async_client {
	sock_ptr_t socket_;
	std::string reqbuf;
	streambuf respbuf;
	void initiate(err_code& err) {
		if (socket_->is_open()) {
			return async_write(err);
		}
		else {
			err.cause = "Socket is closed\n";
		}
	}

	void operator()(err_code& err) {
		return initiate(err);
	}
private:
	void async_write(err_code& err) {
		socket_->async_write_some(buffer(reqbuf, reqbuf.length()), [&](const errc& err_write, size_t written){
			if (!err_write) {
				async_read(err);
			}
			else {
				err.cause = err_write.message();
			}
		});
	}

	void async_read(err_code& err) {
		async_read_until(*socket_.get(), respbuf, "\r\n\r\n", [&](const errc& err_read, size_t written) {
			if (!err_read) {
				Logger::log(SEVERITY::DEBUG, "Reading operation was successfully carried out\n");
				std::cout << &respbuf;
			}
			else {
				err.cause = err_read.message();
			}
		});
	}
};