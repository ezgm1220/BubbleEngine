#include "bubblepch.h"
#include "SkyBox.h"
#include "Bubble/Renderer/Renderer3D_NoBatch.h"
#include "Bubble/Renderer/RenderCommand.h"

#include <glad/glad.h>

namespace Bubble
{

    glm::mat4 SkyBox::captureViews[6] = {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    glm::mat4 SkyBox::captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

    void SkyBox::SetHdrTexPath(const std::string path)
    {
        HdrTex_path = path;
        m_HdrTex = HDRTexture2D::Create(path);
    }

    void SkyBox::SetShader(const std::string path)
    {
        Shader_path = path;
        m_Shader = Shader::Create(Shader_path);
    }

    void SkyBox::SetFrameBuffer(int CubeSize)
    {
        m_Framebuffer = CubeMapFramebuffer::Create(CubeSize);
    }

    void SkyBox::GetEmptyCubeMap(const uint32_t mapsize)
    {
        CubeMapSize = mapsize;
        m_CubeMap = CubeMap::Create(mapsize,true);
    }

    void SkyBox::GetCubeMap_Hdr()
    {
        m_Framebuffer->Bind();
        m_Shader->Bind();
        m_Shader->SetInt("OriginalMap", 0);
        m_Shader->SetMat4("projection", captureProjection);
        m_HdrTex->Bind(0);

        for(unsigned int i = 0; i < 6; ++i)
        {
            m_Shader->SetMat4("view", captureViews[i]);
            m_Framebuffer->SetCubeFace(i, m_CubeMap->GetRendererID());

            RenderCommand::Clear();

            Renderer3D_NoBatch::DrawCube();
        }
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_Framebuffer->Unbind();

        m_CubeMap->SetMipMap();

    }

    void SkyBox::GetCubeMap_SixFaces()
    {
        m_CubeMap = CubeMap::Create(SixFaces_path);
    }

    
    void SkyBox::InitIBL(int IiaSize /*= 32*/, int PrfSize /*= 128*/, int PrfMipNum/*=5*/, int LUTSize /*= 512*/)
    {
        IrradianceSize = IiaSize;
        PrefilterSize = PrfSize;
        PrefilterMipMapNums = PrfMipNum;
        this->LUTSize = LUTSize;

        m_IrradianceMap = CubeMap::Create(IiaSize,false);
        m_PrefilterMap = CubeMap::Create(PrfSize,true);
        m_LUT = Texture2D::Create(LUTSize,LUTSize, TexDataType::RG16F, TexDataType::RG);

        m_IrradianceShader = Shader::Create("assets/shaders/IBL/Irradiance.glsl");
        m_PrefilterShader = Shader::Create("assets/shaders/IBL/Prefilter.glsl");
        m_LUTShader = Shader::Create("assets/shaders/IBL/LUT.glsl");
    }

    void SkyBox::GetIBL()
    {
        // Irradiance
        m_Framebuffer->Resize(IrradianceSize);
        m_IrradianceShader->Bind();
        m_IrradianceShader->SetInt("environmentMap", 0);
        m_IrradianceShader->SetMat4("projection", captureProjection);
        m_CubeMap->Bind();
        m_Framebuffer->Bind();
        for(unsigned int i = 0; i < 6; ++i)
        {
            m_IrradianceShader->SetMat4("view", captureViews[i]);
            m_Framebuffer->SetCubeFace(i, m_IrradianceMap->GetRendererID());
            RenderCommand::Clear();
            Renderer3D_NoBatch::DrawCube();
        }
        m_Framebuffer->Unbind();
        
        // Prefilter
        m_PrefilterMap->SetMipMap();
        m_Framebuffer->Resize(PrefilterSize);
        m_PrefilterShader->Bind();
        m_PrefilterShader->SetInt("environmentMap", 0);
        m_PrefilterShader->SetMat4("projection", captureProjection);
        m_CubeMap->Bind();
        for(unsigned int mip = 0; mip < PrefilterMipMapNums; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int newsize = static_cast<unsigned int>(PrefilterSize * std::pow(0.5, mip));
            m_Framebuffer->Resize(newsize);
            m_Framebuffer->Bind();
            float roughness = (float)mip / (float)(PrefilterMipMapNums - 1);
            m_PrefilterShader->SetFloat("roughness", roughness);
            for(unsigned int i = 0; i < 6; ++i)
            {
                m_PrefilterShader->SetMat4("view", captureViews[i]);
                m_Framebuffer->SetCubeFace(i, m_PrefilterMap->GetRendererID(),mip);
                RenderCommand::Clear();
                Renderer3D_NoBatch::DrawCube();
            }
        }
        m_Framebuffer->Unbind();

        // LUT
        m_Framebuffer->Resize(LUTSize);
        m_Framebuffer->Bind();
        m_LUTShader->Bind();
        RenderCommand::Clear();
        m_Framebuffer->Unbind();
    }

    uint32_t SkyBox::GetCubeMapID()
    {
        if(!m_CubeMap)
        {
            BB_CORE_WARN("SkyBox no have CubeMap!!!!!");
            return -1;
        }
        return m_CubeMap->GetRendererID();
    }

    uint32_t SkyBox::GetIrradianceMapID()
    {
        return m_IrradianceMap->GetRendererID();
    }

    uint32_t SkyBox::GetPrefilterMapID()
    {
        return m_PrefilterMap->GetRendererID();
    }

    uint32_t SkyBox::GetLUTID()
    {
        return m_LUT->GetRendererID();
    }

}

