#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

class PropertiesFile
{
public:
	static PropertiesFile LoadFromFile(const std::filesystem::path& path);

	[[nodiscard]] bool HasKey(const std::string& key) const;

	[[nodiscard]] std::string GetString(
		const std::string& key,
		const std::string& defaultValue) const;

	[[nodiscard]] bool GetBool(
		const std::string& key,
		bool defaultValue) const;

	[[nodiscard]] float GetFloat(
		const std::string& key,
		float defaultValue) const;

private:
	void SetValue(std::string key, std::string value);

private:
	std::unordered_map<std::string, std::string> m_values;
};
