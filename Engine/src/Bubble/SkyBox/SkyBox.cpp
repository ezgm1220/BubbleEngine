#include "bubblepch.h"
#include "SkyBox.h"
#include "Bubble/Renderer/Renderer3D_NoBatch.h"
#include "Bubble/Renderer/RenderCommand.h"

#include <glad/glad.h>
#include <stb_image.h>

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

    SkyBox::SkyBox(int CubeMapsize)
        :CubeMapSize(CubeMapsize)
    {
        m_Framebuffer = CubeMapFramebuffer::Create(CubeMapsize);
        m_CubeMap = CubeMap::Create(CubeMapSize, CubeMapSize);
    }

    void SkyBox::ChangeSkybox(std::string path)
    {
        HdrMap_path = path;
        ChangeHdrPath = true;
    }
    
    void SkyBox::SetShader(std::string path)
    {
        Shader_path = path;
        m_Shader = Shader::Create(Shader_path);
    }

    void SkyBox::LoadHdrMap(std::string path)
    {
        HdrMap_path = path;
        m_HdrMap = HDRTexture2D::Create(HdrMap_path);
    }

    void SkyBox::GetSkyBox()
    {
        m_Framebuffer->Bind();

        m_Shader->Bind();
        m_Shader->SetInt("OriginalMap", 0);
        m_Shader->SetMat4("projection", captureProjection);

        m_HdrMap->Bind(0);

        for(int i = 0; i < 6; ++i)
        {

            m_Shader->SetMat4("view", captureViews[i]);
            m_Framebuffer->SetCubeFace(i, m_CubeMap->GetRendererID());
            //RenderCommand::Clear();
            Renderer3D_NoBatch::DrawCube();
        }

        m_Shader->Unbind();
        m_Framebuffer->Unbind();
    }


}