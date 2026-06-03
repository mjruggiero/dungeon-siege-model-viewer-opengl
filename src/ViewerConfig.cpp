#include "ViewerConfig.h"

#include "PropertiesFile.h"

namespace
{
    std::string GetStringOrLegacy(
        const PropertiesFile& properties,
        const std::string& propertyKey,
        const std::string& legacyKey,
        const std::string& defaultValue)
    {
        return properties.GetString(
            propertyKey,
            properties.GetString(legacyKey, defaultValue));
    }

    bool GetBoolOrLegacy(
        const PropertiesFile& properties,
        const std::string& propertyKey,
        const std::string& legacyKey,
        const bool defaultValue)
    {
        return properties.GetBool(
            propertyKey,
            properties.GetBool(legacyKey, defaultValue));
    }
}

ViewerConfig ViewerConfig::LoadFromFile(const std::filesystem::path& path)
{
    const PropertiesFile properties = PropertiesFile::LoadFromFile(path);

    ViewerConfig config{};

    config.contentBasePath = GetStringOrLegacy(
        properties,
        "dungeonSiege.dataRoot",
        "DungeonSiege.DataRoot",
        config.contentBasePath.string());

    config.artBasePath = GetStringOrLegacy(
        properties,
        "dungeonSiege.artRoot",
        "DungeonSiege.ArtRoot",
        config.artBasePath.string());

    config.namingKeyPath = GetStringOrLegacy(
        properties,
        "dungeonSiege.namingKeyPath",
        "DungeonSiege.NamingKeyPath",
        config.namingKeyPath.string());

    config.modelFallbackBasePath = GetStringOrLegacy(
        properties,
        "resources.modelBasePath",
        "Resources.ModelBasePath",
        (config.artBasePath / "meshes").string());

    config.animationFallbackBasePath = GetStringOrLegacy(
        properties,
        "resources.animationBasePath",
        "Resources.AnimationBasePath",
        (config.artBasePath / "animations").string());

    config.textureFallbackBasePath = GetStringOrLegacy(
        properties,
        "resources.textureBasePath",
        "Resources.TextureBasePath",
        (config.artBasePath / "bitmaps").string());

    config.modelName = GetStringOrLegacy(
        properties,
        "viewer.model",
        "Viewer.Model",
        config.modelName);

    config.animationName = GetStringOrLegacy(
        properties,
        "viewer.animation",
        "Viewer.Animation",
        config.animationName);

    config.enableDebugLogging = GetBoolOrLegacy(
        properties,
        "viewer.debugLogging",
        "Viewer.DebugLogging",
        config.enableDebugLogging);

    config.startWireframe = GetBoolOrLegacy(
        properties,
        "viewer.startWireframe",
        "Viewer.StartWireframe",
        config.startWireframe);

    config.startPaused = GetBoolOrLegacy(
        properties,
        "viewer.startPaused",
        "Viewer.StartPaused",
        config.startPaused);

    config.startRotating = GetBoolOrLegacy(
        properties,
        "viewer.startRotating",
        "Viewer.StartRotating",
        config.startRotating);

    return config;
}
