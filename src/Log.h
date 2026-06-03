#pragma once

#include <ostream>

class Log
{
public:
	static std::ostream& Info();
	static std::ostream& Warning();
	static std::ostream& Error();
};
