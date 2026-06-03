#include "ResourceResolver.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <optional>
#include <utility>

#include "Log.h"

namespace
{
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

    bool HasExtension(const std::filesystem::path& path, const std::string& expectedExtension)
    {
        return ToLower(path.extension().string()) == ToLower(expectedExtension);
    }

    std::optional<std::string> ExtractAnimationSetFolder(const std::string& animationName)
    {
        const std::string lowerName = ToLower(animationName);
        std::size_t searchPosition = 0;

        while (searchPosition < lowerName.size())
        {
            const std::size_t found = lowerName.find("_fs", searchPosition);

            if (found == std::string::npos)
            {
                return std::nullopt;
            }

            const std::size_t tokenStart = found + 1;
            std::size_t tokenEnd = lowerName.find('_', tokenStart);

            if (tokenEnd == std::string::npos)
            {
                tokenEnd = lowerName.size();
            }

            const std::string token = lowerName.substr(tokenStart, tokenEnd - tokenStart);

            if (token.size() >= 3 && token[0] == 'f' && token[1] == 's')
            {
                return token;
            }

            searchPosition = tokenEnd;
        }

        return std::nullopt;
    }

}

void ResourceResolver::SetContentBasePath(std::filesystem::path contentBasePath)
{
    m_contentBasePath = std::move(contentBasePath);
}

void ResourceResolver::SetArtBasePath(std::filesystem::path artBasePath)
{
    m_artBasePath = std::move(artBasePath);
}

void ResourceResolver::SetModelFallbackBasePath(std::filesystem::path modelFallbackBasePath)
{
    m_modelFallbackBasePath = std::move(modelFallbackBasePath);
}

void ResourceResolver::SetAnimationFallbackBasePath(std::filesystem::path animationFallbackBasePath)
{
    m_animationFallbackBasePath = std::move(animationFallbackBasePath);
}

void ResourceResolver::SetTextureFallbackBasePath(std::filesystem::path textureFallbackBasePath)
{
    m_textureFallbackBasePath = std::move(textureFallbackBasePath);
}

void ResourceResolver::SetNamingKey(NamingKey namingKey)
{
    m_namingKey = std::move(namingKey);
}

bool ResourceResolver::HasNamingKey() const
{
    return m_namingKey.IsLoaded();
}

std::optional<std::filesystem::path> ResourceResolver::ResolveAspModelPath(
    const std::string& modelName) const
{
    return ResolveResourcePath(modelName, ".asp", "ASP model", m_modelFallbackBasePath, true);
}

std::optional<std::filesystem::path> ResourceResolver::ResolvePrsAnimationPath(
    const std::string& animationName) const
{
    if (animationName.empty())
    {
        return std::nullopt;
    }

    Log::Info() << "Resolving PRS animation: " << animationName << std::endl;

    const std::optional<std::string> animationSetFolder =
        ExtractAnimationSetFolder(animationName);

    std::vector<std::filesystem::path> candidates;

    if (m_namingKey.IsLoaded())
    {
        const std::optional<std::filesystem::path> resolvedRelativePath =
            m_namingKey.ResolveResourceName(animationName);

        if (resolvedRelativePath)
        {
            Log::Info() << "NamingKey resolved " << animationName << " -> "
                << resolvedRelativePath->string() << std::endl;

            const std::vector<std::filesystem::path> namingKeyCandidates =
                BuildNamingKeyCandidates(*resolvedRelativePath, m_animationFallbackBasePath);

            candidates.insert(
                candidates.end(),
                namingKeyCandidates.begin(),
                namingKeyCandidates.end());

            if (animationSetFolder)
            {
                const std::filesystem::path parentPath = resolvedRelativePath->parent_path();
                const std::filesystem::path fileName = resolvedRelativePath->filename();
                const std::filesystem::path resolvedWithAnimationSet =
                    parentPath / *animationSetFolder / fileName;

                const std::vector<std::filesystem::path> animationSetCandidates =
                    BuildNamingKeyCandidates(resolvedWithAnimationSet, m_animationFallbackBasePath);

                candidates.insert(
                    candidates.end(),
                    animationSetCandidates.begin(),
                    animationSetCandidates.end());
            }
        }
        else
        {
            Log::Warning() << "NamingKey could not resolve: " << animationName << std::endl;
        }
    }
    else
    {
        Log::Warning() << "NamingKey is not loaded." << std::endl;
    }

    const std::vector<std::filesystem::path> directCandidates =
        BuildDirectCandidates(animationName, m_animationFallbackBasePath);

    candidates.insert(
        candidates.end(),
        directCandidates.begin(),
        directCandidates.end());

    if (animationSetFolder)
    {
        candidates.push_back(m_animationFallbackBasePath / *animationSetFolder / animationName);
    }

    if (const auto found = FirstExistingPath(candidates, animationName, ".prs", "PRS animation"))
    {
        return found;
    }

    Log::Warning() << "PRS animation unresolved after all candidates: "
        << animationName << std::endl;

    return std::nullopt;
}

std::optional<std::filesystem::path> ResourceResolver::ResolveRawTexturePath(
    const std::string& textureName) const
{
    return ResolveResourcePath(textureName, ".raw", "RAW texture", m_textureFallbackBasePath, true);
}

std::optional<std::filesystem::path> ResourceResolver::ResolveResourcePath(
    const std::string& resourceName,
    const std::string& extension,
    const std::string& resourceKind,
    const std::filesystem::path& fallbackBasePath,
    const bool tryNamingKeyFirst) const
{
    if (resourceName.empty())
    {
        return std::nullopt;
    }

    Log::Info() << "Resolving " << resourceKind << ": " << resourceName << std::endl;

    const auto tryNamingKey = [&]() -> std::optional<std::filesystem::path>
    {
        if (!m_namingKey.IsLoaded())
        {
            Log::Warning() << "NamingKey is not loaded." << std::endl;
            return std::nullopt;
        }

        const std::optional<std::filesystem::path> resolvedRelativePath =
            m_namingKey.ResolveResourceName(resourceName);

        if (!resolvedRelativePath)
        {
            Log::Warning() << "NamingKey could not resolve: " << resourceName << std::endl;
            return std::nullopt;
        }

        Log::Info() << "NamingKey resolved " << resourceName << " -> "
            << resolvedRelativePath->string() << std::endl;

        const std::vector<std::filesystem::path> candidates =
            BuildNamingKeyCandidates(*resolvedRelativePath, fallbackBasePath);

        return FirstExistingPath(candidates, resourceName, extension, resourceKind);
    };

    const auto tryDirect = [&]() -> std::optional<std::filesystem::path>
    {
        const std::vector<std::filesystem::path> candidates =
            BuildDirectCandidates(resourceName, fallbackBasePath);

        return FirstExistingPath(candidates, resourceName, extension, resourceKind);
    };

    if (tryNamingKeyFirst)
    {
        if (const auto found = tryNamingKey())
        {
            return found;
        }

        if (const auto found = tryDirect())
        {
            return found;
        }
    }
    else
    {
        if (const auto found = tryDirect())
        {
            return found;
        }

        if (const auto found = tryNamingKey())
        {
            return found;
        }
    }

    Log::Warning() << resourceKind << " unresolved after all candidates: "
        << resourceName << std::endl;

    return std::nullopt;
}

std::filesystem::path ResourceResolver::AppendExtensionIfNeeded(
    std::filesystem::path path,
    const std::string& extension) const
{
    if (!HasExtension(path, extension))
    {
        path += extension;
    }

    return path;
}

std::optional<std::filesystem::path> ResourceResolver::FirstExistingPath(
    const std::vector<std::filesystem::path>& candidates,
    const std::string& resourceName,
    const std::string& extension,
    const std::string& resourceKind) const
{
    for (const std::filesystem::path& candidate : candidates)
    {
        const std::filesystem::path candidateWithExtension =
            AppendExtensionIfNeeded(candidate, extension);

        Log::Info() << "Trying " << resourceKind << " path for " << resourceName
            << ": " << candidateWithExtension.string() << std::endl;

        if (std::filesystem::exists(candidateWithExtension))
        {
            Log::Info() << "Resolved " << resourceKind << " path for " << resourceName
                << ": " << candidateWithExtension.string() << std::endl;
            return candidateWithExtension;
        }
    }

    return std::nullopt;
}

std::vector<std::filesystem::path> ResourceResolver::BuildDirectCandidates(
    const std::string& resourceName,
    const std::filesystem::path& fallbackBasePath) const
{
    const std::filesystem::path requestedPath{ resourceName };

    std::vector<std::filesystem::path> candidates;
    candidates.push_back(requestedPath);
    candidates.push_back(fallbackBasePath / requestedPath);
    candidates.push_back(m_artBasePath / requestedPath);
    candidates.push_back(m_contentBasePath / requestedPath);
    candidates.push_back(m_contentBasePath / "art" / requestedPath);

    return candidates;
}

std::vector<std::filesystem::path> ResourceResolver::BuildNamingKeyCandidates(
    const std::filesystem::path& resolvedRelativePath,
    const std::filesystem::path& fallbackBasePath) const
{
    std::vector<std::filesystem::path> candidates;
    candidates.push_back(m_artBasePath / resolvedRelativePath);
    candidates.push_back(m_contentBasePath / "art" / resolvedRelativePath);
    candidates.push_back(m_contentBasePath / resolvedRelativePath);
    candidates.push_back(fallbackBasePath / resolvedRelativePath);

    return candidates;
}
