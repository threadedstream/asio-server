#pragma once
#include "../../core/utils.hpp"
#include "../../core/header.hpp"
#include "request.hpp"
#include "../../client/client.hpp"
#include "../../core/math.hpp"

class response {
public:
	
	enum class status_code {
		ok = 200,
		created = 201,
		accepted = 202,
		no_content = 204,
		multiple_choices = 300,
		moved_permanently = 301,
		moved_temporarily = 302,
		not_modified = 304,
		bad_request = 400,
		unauthorized = 401,
		forbidden = 403,
		not_found = 404,
		internal_server_error = 500,
		not_implemented = 501,
		bad_gateway = 502,
		service_unavailable = 503
	}code;

	response() : sock(ctx) {};
	std::string buildResponse(request& req);

	inline std::string response_content() const noexcept{ return content; } 
	//Note: Required for proper functioning
	inline void set_static_dir(const std::string& static_dir) noexcept { static_dir_ = static_dir; }
	inline void set_template_dir(const std::string& template_dir) noexcept { template_dir_ = template_dir; }
	std::string handle_math(request& req);
	void handle_post_request(request& req, err_code& err);
	void connect_handler(const errc & err_conn);
	void read_handler(const errc& err, size_t bytes);
	void write_handler(const errc& err, size_t bytes);
	std::string retrieve_user_data(request& req);
	void make_clean(std::string& text);
	
private:
	std::string load_template_file(request& req);

private:
	std::string content;
	asio_ctx ctx;
	sock_t sock;
	std::string template_dir_;
	std::string static_dir_;
	streambuf resp;

	//std::vector<const_buffer> to_buffers();
	//static response stock_response(status_code status);
};