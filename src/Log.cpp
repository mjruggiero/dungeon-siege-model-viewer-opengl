#include "Log.h"

#include <iostream>

std::ostream& Log::Info()
{
	return std::cout;
}

std::ostream& Log::Warning()
{
	return std::cerr;
}

std::ostream& Log::Error()
{
	return std::cerr;
}
