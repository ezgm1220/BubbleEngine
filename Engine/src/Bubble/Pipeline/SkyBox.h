#pragma once
#include "Bubble/Renderer/Texture.h"
#include "Bubble/Renderer/Framebuffer.h"
#include "Bubble/Renderer/Shader.h"
namespace Bubble
{
    class SkyBox
    {
    public:
        SkyBox() = default;

        void SetHdrTexPath(const std::string path);
        void SetHdrSixFacesPath(const std::string path) { SixFaces_path = path; }

        void SetShader(const std::string path);
        void SetFrameBuffer(int CubeSize);

        void GetEmptyCubeMap(const uint32_t mapsize);

        void GetCubeMap_Hdr();
        void GetCubeMap_SixFaces();

        uint32_t GetCubeMapID();

    private:
        std::string Shader_path;
        std::string HdrTex_path;
        std::string SixFaces_path;
        Ref<CubeMap>m_CubeMap;
        Ref<HDRTexture2D>m_HdrTex;
        Ref<CubeMapFramebuffer> m_Framebuffer;
        Ref<Shader> m_Shader;

    public:
        static glm::mat4 captureViews[6];
        static glm::mat4 captureProjection;
    };
}