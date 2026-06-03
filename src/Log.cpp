#include "Log.h"

#include <iostream>
#include <streambuf>

namespace
{
	class NullBuffer : public std::streambuf
	{
	public:
		int overflow(int ch) override
		{
			return ch;
		}
	};

	NullBuffer g_nullBuffer;
	std::ostream g_nullStream(&g_nullBuffer);
	bool g_debugEnabled = false;
}

std::ostream& Log::Debug()
{
	if (g_debugEnabled)
	{
		return std::cout;
	}

	return g_nullStream;
}

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

void Log::SetDebugEnabled(bool enabled)
{
	g_debugEnabled = enabled;
}

bool Log::IsDebugEnabled()
{
	return g_debugEnabled;
}
