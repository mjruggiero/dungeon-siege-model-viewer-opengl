#pragma once

#include <ostream>

class Log
{
public:
	static std::ostream& Debug();
	static std::ostream& Info();
	static std::ostream& Warning();
	static std::ostream& Error();

	static void SetDebugEnabled(bool enabled);
	static bool IsDebugEnabled();
};
