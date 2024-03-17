#pragma once

#include <filesystem>
#include "Bubble/Renderer/Texture.h"

namespace Bubble
{

    class ContentBrowserPanel
    {
    public:
        ContentBrowserPanel();

        void OnImGuiRender();
    private:
        std::filesystem::path m_CurrentDirectory;

        Ref<Texture2D> m_DirectoryIcon;
        Ref<Texture2D> m_FileIcon;
    };

}