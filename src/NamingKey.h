#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>

class NamingKey
{
public:
	static NamingKey LoadFromFile(const std::filesystem::path& path);

	[[nodiscard]] bool IsLoaded() const;

	[[nodiscard]] std::optional<std::filesystem::path> ResolveResourceName(
		const std::string& resourceName) const;

private:
	void AddTreeEntry(std::string key, std::filesystem::path localPath);

	[[nodiscard]] std::optional<std::filesystem::path> ResolveTreePath(
		const std::string& resourceName) const;

private:
	bool m_loaded{};

	std::unordered_map<std::string, std::filesystem::path> m_localPathsByKey;
};
