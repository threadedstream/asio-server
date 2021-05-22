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

void connection::load_file(const std::string& filename)
{
	std::ifstream stream(filename, std::ios::in);
	
	if (connection::file_exists(filename))
	{
		stream.seekg(0, std::ios::end);
		size_t size = stream.tellg();
		stream.seekg(std::ios::beg);
		stream.read(buffer_.data(), size);
	}
	else
	{
		Logger::log(SEVERITY::ERR, "File does not exist\n");
	}
}

/*
	@Function for obtaining current time
*/
std::string connection::_now()
{
	std::stringstream strstream;

	auto time = std::time(nullptr);

	strstream << std::put_time(std::gmtime(&time), "%F %T%z") << "\n";

	return strstream.str();
}

//Main(so-called "driver") method
void connection::start_processing()
{
	Logger::log(SEVERITY::DEBUG, "Preparation of necessary resources\n");
	Logger::log(SEVERITY::DEBUG, "Attempt to initialize database connection\n");
	
	auto self(shared_from_this());
	//load_file(FILENAME);
	socket_.async_read_some(buffer(buffer_), [&, self](const errc& err, size_t length) {
		if (!err)
		{
			Logger::log(SEVERITY::DEBUG, "Message: " + std::string(buffer_.c_array()));
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
				else {
					Logger::log(SEVERITY::DEBUG, "Number of bytes sent: " + length + std::string("\n"));
				}
			});
		}
		else {
			Logger::log(SEVERITY::ERR, err.message());
		}
	});
}

void connection::write_completion_handler(const errc & err, size_t bytes_transferred)
{
		
}

void connection::read_completion_handler(const	errc & err, size_t bytes_transferred)
{
	
}