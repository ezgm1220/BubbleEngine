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
        m_CubeMap = CubeMap::Create(mapsize);
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

    
    uint32_t SkyBox::GetCubeMapID()
    {
        if(!m_CubeMap)
        {
            BB_CORE_WARN("SkyBox no have CubeMap!!!!!");
            return -1;
        }
        return m_CubeMap->GetRendererID();
    }

}

