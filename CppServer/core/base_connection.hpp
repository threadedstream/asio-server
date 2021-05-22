#pragma once
#include "non_copyable.hpp"
#include "typedefs.hpp"
#include "logger.hpp"


class base_connection : private non_copyable
{
public:
	explicit base_connection(const std::string& addr="127.0.0.1", ui16 port=80, ui32 backlog = 5) :
		addr_(addr),
		port_(port),
		backlog_(backlog)
	{
	}

	virtual ~base_connection() {};
	

protected:
	//endp_obj endpoint_;
	std::string addr_;
	ui16 port_;
	int backlog_;
};