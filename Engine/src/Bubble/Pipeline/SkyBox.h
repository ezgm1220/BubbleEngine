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

        void InitIBL(int IiaSize = 32,int PrfSize = 128,int PrfMipNum=5, int LUTSize = 512);
        void GetIBL();

        uint32_t GetCubeMapID();
        uint32_t GetIrradianceMapID();
        uint32_t GetPrefilterMapID();
        uint32_t GetLUTID();

    private:
        std::string Shader_path;
        std::string HdrTex_path;
        std::string SixFaces_path;
        Ref<CubeMap>m_CubeMap;
        Ref<CubeMap>m_IrradianceMap;
        Ref<CubeMap>m_PrefilterMap;
        Ref<Texture2D>m_LUT;
        Ref<HDRTexture2D>m_HdrTex;
        Ref<CubeMapFramebuffer> m_Framebuffer;
        Ref<Shader> m_Shader;
        Ref<Shader> m_IrradianceShader;
        Ref<Shader> m_PrefilterShader;
        Ref<Shader> m_LUTShader;

    private:
        int CubeMapSize = -1;
        int IrradianceSize = -1;
        int PrefilterSize = -1;
        int PrefilterMipMapNums = 0;
        int LUTSize = -1;

    public:
        static glm::mat4 captureViews[6];
        static glm::mat4 captureProjection;
    };
}