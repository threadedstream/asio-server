#include "connection.hpp"
#include <sstream>


connection::connection(asio_ctx& context) :
	socket_(context)
{
	stx.set_template_dir("templates");
	stx.set_req_file("dblog.txt");
	stx.set_static_dir("static");
	resp.set_template_dir(stx.template_dir());
	resp.set_static_dir(stx.static_dir());
}

//Main(so-called "driver") method
void connection::start_processing()
{
	auto self(shared_from_this());
	socket_.async_read_some(buffer(buffer_), [&, self](const errc& err, size_t length) {
		if (!err)
		{
			request req(buffer_.c_array());
			std::string str;
			
			str = resp.buildResponse(req);

			for (auto& ch : str)
			{
				if (isascii(ch))
				{
					response_string += ch;
				}
			}

			socket_.async_write_some(buffer(response_string), [&, self](const errc& err, size_t length) {
				if (err) {
					Logger::log(SEVERITY::ERR, err.message());
				}
			});
		}
		else {
			Logger::log(SEVERITY::ERR, err.message());
		}
	});
}

