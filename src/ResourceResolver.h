#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "NamingKey.h"

class ResourceResolver
{
public:
    void SetContentBasePath(std::filesystem::path contentBasePath);
    void SetArtBasePath(std::filesystem::path artBasePath);
    void SetModelFallbackBasePath(std::filesystem::path modelFallbackBasePath);
    void SetAnimationFallbackBasePath(std::filesystem::path animationFallbackBasePath);
    void SetTextureFallbackBasePath(std::filesystem::path textureFallbackBasePath);
    void SetNamingKey(NamingKey namingKey);

    [[nodiscard]] bool HasNamingKey() const;

    [[nodiscard]] std::optional<std::filesystem::path> ResolveAspModelPath(
        const std::string& modelName) const;

    [[nodiscard]] std::optional<std::filesystem::path> ResolvePrsAnimationPath(
        const std::string& animationName) const;

    [[nodiscard]] std::optional<std::filesystem::path> ResolveRawTexturePath(
        const std::string& textureName) const;

private:
    [[nodiscard]] std::optional<std::filesystem::path> ResolveResourcePath(
        const std::string& resourceName,
        const std::string& extension,
        const std::string& resourceKind,
        const std::filesystem::path& fallbackBasePath,
        bool tryNamingKeyFirst) const;

    [[nodiscard]] std::filesystem::path AppendExtensionIfNeeded(
        std::filesystem::path path,
        const std::string& extension) const;

    [[nodiscard]] std::optional<std::filesystem::path> FirstExistingPath(
        const std::vector<std::filesystem::path>& candidates,
        const std::string& resourceName,
        const std::string& extension,
        const std::string& resourceKind) const;

    [[nodiscard]] std::vector<std::filesystem::path> BuildDirectCandidates(
        const std::string& resourceName,
        const std::filesystem::path& fallbackBasePath) const;

    [[nodiscard]] std::vector<std::filesystem::path> BuildNamingKeyCandidates(
        const std::filesystem::path& resolvedRelativePath,
        const std::filesystem::path& fallbackBasePath) const;

private:
    std::filesystem::path m_contentBasePath{ "." };
    std::filesystem::path m_artBasePath{ "." };
    std::filesystem::path m_modelFallbackBasePath{ "." };
    std::filesystem::path m_animationFallbackBasePath{ "." };
    std::filesystem::path m_textureFallbackBasePath{ "." };

    NamingKey m_namingKey;
};
