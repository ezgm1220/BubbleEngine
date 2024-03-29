#pragma once

#include <Bubble/Renderer/Texture.h>
#include <Bubble/Renderer/Framebuffer.h>
#include <Bubble/Renderer/Shader.h>

namespace Bubble
{
    class SkyBox
    {
    public:
        SkyBox(){}
        SkyBox(int CubeMapsize);
        void ChangeSkybox(std::string path);
        void SetShader(std::string path);
        void LoadHdrMap(std::string path);
        void GetSkyBox();
        uint32_t GetCubeMapID() { return m_CubeMap->GetRendererID(); }

        Ref<CubeMapFramebuffer> m_Framebuffer;
    private:
        Ref<Shader> m_Shader;
    private:
        bool ChangeHdrPath = false;
        int CubeMapSize = 0;
        std::string Shader_path;
        std::string HdrMap_path;
        Ref<CubeMap>m_CubeMap;
        Ref<HDRTexture2D>m_HdrMap;
    public:
        static glm::mat4 captureViews[6];
        static glm::mat4 captureProjection;
    };

}
