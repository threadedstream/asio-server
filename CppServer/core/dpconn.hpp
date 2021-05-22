#pragma once

#include "typedefs.hpp"
#include "non_constructible.h"
#include "non_copyable.hpp"
#include "logger.hpp"


//Singleton class, denoting Database Connection
class dbconn : private non_copyable, private non_constructible {
public:
	
private:
	static dbconn* instance;
};
