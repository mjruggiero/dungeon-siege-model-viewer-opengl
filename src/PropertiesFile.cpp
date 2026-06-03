#include "PropertiesFile.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>

namespace
{
	std::string Trim(std::string value)
	{
		const auto isNotSpace = [](const unsigned char ch)
			{
				return !std::isspace(ch);
			};

		value.erase(
			value.begin(),
			std::find_if(value.begin(), value.end(), isNotSpace));

		value.erase(
			std::find_if(value.rbegin(), value.rend(), isNotSpace).base(),
			value.end());

		return value;
	}

	std::string ToLower(std::string value)
	{
		std::transform(
			value.begin(),
			value.end(),
			value.begin(),
			[](const unsigned char ch)
			{
				return static_cast<char>(std::tolower(ch));
			});

		return value;
	}

	bool IsCommentOrBlankLine(const std::string& line)
	{
		const std::string trimmed = Trim(line);

		return trimmed.empty() ||
			trimmed[0] == '#' ||
			trimmed[0] == '!';
	}

	bool TryParseBool(const std::string& text, bool& result)
	{
		const std::string lowered = ToLower(Trim(text));

		if (lowered == "true" || lowered == "yes" || lowered == "1" || lowered == "on")
		{
			result = true;
			return true;
		}

		if (lowered == "false" || lowered == "no" || lowered == "0" || lowered == "off")
		{
			result = false;
			return true;
		}

		return false;
	}
}

PropertiesFile PropertiesFile::LoadFromFile(const std::filesystem::path& path)
{
	PropertiesFile properties{};

	std::ifstream file(path);

	if (!file)
	{
		return properties;
	}

	std::string line;
	std::string currentSection;

	while (std::getline(file, line))
	{
		if (IsCommentOrBlankLine(line))
		{
			continue;
		}

		const std::string trimmedLine = Trim(line);

		if (trimmedLine.size() >= 2 && trimmedLine.front() == '[' && trimmedLine.back() == ']')
		{
			currentSection = Trim(trimmedLine.substr(1, trimmedLine.size() - 2));
			continue;
		}

		const std::size_t equalsPosition = trimmedLine.find('=');

		if (equalsPosition == std::string::npos)
		{
			continue;
		}

		std::string key = Trim(trimmedLine.substr(0, equalsPosition));
		std::string value = Trim(trimmedLine.substr(equalsPosition + 1));

		if (key.empty())
		{
			continue;
		}

		if (!currentSection.empty())
		{
			properties.SetValue(currentSection + "." + key, value);
		}

		properties.SetValue(std::move(key), std::move(value));
	}

	return properties;
}

bool PropertiesFile::HasKey(const std::string& key) const
{
	return m_values.find(key) != m_values.end();
}

std::string PropertiesFile::GetString(
	const std::string& key,
	const std::string& defaultValue) const
{
	const auto found = m_values.find(key);

	if (found == m_values.end())
	{
		return defaultValue;
	}

	return found->second;
}

bool PropertiesFile::GetBool(
	const std::string& key,
	const bool defaultValue) const
{
	const auto found = m_values.find(key);

	if (found == m_values.end())
	{
		return defaultValue;
	}

	bool parsedValue{};

	if (!TryParseBool(found->second, parsedValue))
	{
		return defaultValue;
	}

	return parsedValue;
}

float PropertiesFile::GetFloat(
	const std::string& key,
	const float defaultValue) const
{
	const auto found = m_values.find(key);

	if (found == m_values.end())
	{
		return defaultValue;
	}

	try
	{
		return std::stof(found->second);
	}
	catch (...)
	{
		return defaultValue;
	}
}

void PropertiesFile::SetValue(std::string key, std::string value)
{
	m_values[std::move(key)] = std::move(value);
}
