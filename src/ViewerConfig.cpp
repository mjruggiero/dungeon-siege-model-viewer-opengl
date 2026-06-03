#include "ViewerConfig.h"

#include "PropertiesFile.h"

ViewerConfig ViewerConfig::LoadFromFile(const std::filesystem::path& path)
{
    const PropertiesFile properties = PropertiesFile::LoadFromFile(path);

    ViewerConfig config{};

    config.contentBasePath = properties.GetString(
        "DungeonSiege.DataRoot",
        properties.GetString("content.basePath", config.contentBasePath.string()));

    config.artBasePath = properties.GetString(
        "DungeonSiege.ArtRoot",
        properties.GetString("art.basePath", config.artBasePath.string()));

    config.namingKeyPath = properties.GetString(
        "DungeonSiege.NamingKeyPath",
        properties.GetString("namingKey.path", config.namingKeyPath.string()));

    config.modelFallbackBasePath = properties.GetString(
        "Resources.ModelBasePath",
        properties.GetString("model.basePath", (config.artBasePath / "meshes").string()));

    config.animationFallbackBasePath = properties.GetString(
        "Resources.AnimationBasePath",
        properties.GetString("animation.basePath", (config.artBasePath / "animations").string()));

    config.textureFallbackBasePath = properties.GetString(
        "Resources.TextureBasePath",
        properties.GetString("texture.basePath", (config.artBasePath / "bitmaps").string()));

    config.modelName = properties.GetString(
        "Viewer.Model",
        properties.GetString("model.name", config.modelName));

    config.animationName = properties.GetString(
        "Viewer.Animation",
        properties.GetString("animation.name", config.animationName));

    config.enableDebugLogging = properties.GetBool(
        "Viewer.DebugLogging",
        properties.GetBool("debug.enabled", config.enableDebugLogging));

    config.startWireframe = properties.GetBool(
        "Viewer.StartWireframe",
        properties.GetBool("render.startWireframe", config.startWireframe));

    config.startPaused = properties.GetBool(
        "Viewer.StartPaused",
        properties.GetBool("animation.startPaused", config.startPaused));

    config.startRotating = properties.GetBool(
        "Viewer.StartRotating",
        properties.GetBool("camera.startRotating", config.startRotating));

    return config;
}
