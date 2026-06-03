#pragma once

#include <filesystem>
#include <string>

struct ViewerConfig
{
    std::filesystem::path contentBasePath{ "." };
    std::filesystem::path artBasePath{ "." };
    std::filesystem::path namingKeyPath{};

    std::filesystem::path modelFallbackBasePath{};
    std::filesystem::path animationFallbackBasePath{};
    std::filesystem::path textureFallbackBasePath{};

    std::string modelName{ "m_c_ebb_dg_pos_1" };
    std::string animationName{ "a_c_ebb_dg_fs0_at" };

    bool enableDebugLogging{};
    bool startWireframe{};
    bool startPaused{};
    bool startRotating{};

    static ViewerConfig LoadFromFile(const std::filesystem::path& path);
};
