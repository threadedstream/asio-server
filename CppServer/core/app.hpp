#pragma once
#include "../servers/http_server/http_server.hpp"


class App final
{
public:
	App() :
		server_http_(5, 5) {};
	FORCE_INLINE void run_http_server() { server_http_.run(); }
	FORCE_INLINE void run_chat_server() {  }
private:
	http_server server_http_;
	
};