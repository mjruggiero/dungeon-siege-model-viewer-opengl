#include "NamingKey.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

	std::string ToUpper(std::string value)
	{
		std::transform(
			value.begin(),
			value.end(),
			value.begin(),
			[](const unsigned char ch)
			{
				return static_cast<char>(std::toupper(ch));
			});

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

	std::vector<std::string> Split(const std::string& value, const char delimiter)
	{
		std::vector<std::string> parts;

		std::stringstream stream(value);
		std::string part;

		while (std::getline(stream, part, delimiter))
		{
			parts.push_back(part);
		}

		return parts;
	}

	bool StartsWithTreeAssignment(const std::string& line)
	{
		const std::string trimmed = Trim(line);

		return trimmed.rfind("TREE", 0) == 0 &&
			trimmed.find('=') != std::string::npos;
	}

	std::string StripQuotedComment(std::string value)
	{
		const std::size_t quotePosition = value.find('"');

		if (quotePosition != std::string::npos)
		{
			value = value.substr(0, quotePosition);
		}

		return Trim(value);
	}

	std::optional<std::pair<std::string, std::filesystem::path>> TryParseTreeLine(
		const std::string& line)
	{
		if (!StartsWithTreeAssignment(line))
		{
			return std::nullopt;
		}

		const std::size_t equalsPosition = line.find('=');

		if (equalsPosition == std::string::npos)
		{
			return std::nullopt;
		}

		std::string rhs = line.substr(equalsPosition + 1);

		const std::vector<std::string> parts = Split(rhs, ',');

		if (parts.size() < 2)
		{
			return std::nullopt;
		}

		std::string key = ToUpper(Trim(parts[0]));
		std::string localPath = StripQuotedComment(parts[1]);

		if (key.empty())
		{
			return std::nullopt;
		}

		return std::make_pair(std::move(key), std::filesystem::path{ localPath });
	}

	std::string NormalizeResourceNameForTreeLookup(std::string resourceName)
	{
		std::replace(resourceName.begin(), resourceName.end(), '\\', '/');

		const std::filesystem::path path = resourceName;
		resourceName = path.stem().string();

		std::replace(resourceName.begin(), resourceName.end(), '-', '_');

		return ToUpper(resourceName);
	}

	std::string NormalizeResourceFileName(std::string resourceName)
	{
		std::replace(resourceName.begin(), resourceName.end(), '\\', '/');

		const std::filesystem::path path = resourceName;

		return path.stem().string();
	}

	std::vector<std::string> BuildPrefixKeys(const std::string& normalizedResourceName)
	{
		const std::vector<std::string> tokens = Split(normalizedResourceName, '_');

		std::vector<std::string> keys;

		std::string current;

		for (const std::string& token : tokens)
		{
			if (token.empty())
			{
				continue;
			}

			if (!current.empty())
			{
				current += "_";
			}

			current += token;
			keys.push_back(current);
		}

		return keys;
	}
}

NamingKey NamingKey::LoadFromFile(const std::filesystem::path& path)
{
	NamingKey namingKey{};

	std::ifstream file(path);

	if (!file)
	{
		return namingKey;
	}

	std::string line;

	while (std::getline(file, line))
	{
		const std::string trimmed = Trim(line);

		if (trimmed.empty() || trimmed[0] == '#')
		{
			continue;
		}

		const auto parsed = TryParseTreeLine(trimmed);

		if (!parsed)
		{
			continue;
		}

		namingKey.AddTreeEntry(parsed->first, parsed->second);
	}

	namingKey.m_loaded = true;

	return namingKey;
}

bool NamingKey::IsLoaded() const
{
	return m_loaded;
}

std::optional<std::filesystem::path> NamingKey::ResolveResourceName(
	const std::string& resourceName) const
{
	if (!m_loaded || resourceName.empty())
	{
		return std::nullopt;
	}

	const std::optional<std::filesystem::path> treePath = ResolveTreePath(resourceName);

	if (!treePath)
	{
		return std::nullopt;
	}

	const std::string resourceFileName = NormalizeResourceFileName(resourceName);

	if (resourceFileName.empty())
	{
		return std::nullopt;
	}

	return *treePath / resourceFileName;
}

void NamingKey::AddTreeEntry(std::string key, std::filesystem::path localPath)
{
	m_localPathsByKey[ToUpper(std::move(key))] = std::move(localPath);
}

std::optional<std::filesystem::path> NamingKey::ResolveTreePath(
	const std::string& resourceName) const
{
	const std::string normalizedResourceName = NormalizeResourceNameForTreeLookup(resourceName);
	const std::vector<std::string> prefixKeys = BuildPrefixKeys(normalizedResourceName);

	if (prefixKeys.empty())
	{
		return std::nullopt;
	}

	std::filesystem::path resolvedPath;

	bool foundAnyPrefix = false;

	for (const std::string& key : prefixKeys)
	{
		const auto found = m_localPathsByKey.find(key);

		if (found == m_localPathsByKey.end())
		{
			continue;
		}

		if (!found->second.empty())
		{
			resolvedPath /= found->second;
		}

		foundAnyPrefix = true;
	}

	if (!foundAnyPrefix)
	{
		return std::nullopt;
	}

	return resolvedPath;
}
