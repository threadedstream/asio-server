#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <ctime>
#include <cstdint>
#include <unordered_set>
#include <cassert>
#include <algorithm> 

class connection;

//usings
using namespace boost::asio;
using namespace boost::filesystem;
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

#define ADDR_FROM_STR(addr) ip::address::from_string(addr)
#define FORCE_INLINE inline
#define ADDR_TO_STR(sock_ptr) sock_ptr->remote_endpoint().address().to_string()

struct err_code {
	std::string cause;
};

//signed integers
typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

//unsigned integers
typedef uint64_t ui64;
typedef uint32_t ui32;
typedef uint16_t ui16;
typedef uint8_t  ui8;

//typealiases
using sock_ptr_t = boost::shared_ptr<boost::asio::ip::tcp::socket>;
using sock_t = boost::asio::ip::tcp::socket;
using acceptor = boost::asio::ip::tcp::acceptor;
using asio_service = boost::asio::io_service;
using asio_ctx = boost::asio::io_context;
using endp_obj = boost::asio::ip::tcp::endpoint;
using errc = boost::system::error_code;
using strnd = boost::asio::strand<boost::asio::io_context::executor_type>;
using connection_ptr = boost::shared_ptr<connection>;
using thread_ptr = boost::shared_ptr<boost::thread>;
