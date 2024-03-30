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
        //m_Framebuffer->Bind();

        //m_Shader->Bind();
        //m_Shader->SetInt("OriginalMap", 0);
        //m_Shader->SetMat4("projection", captureProjection);

        //m_HdrMap->Bind(0);

        //for(int i = 0; i < 6; ++i)
        //{
        //    m_Shader->SetMat4("view", captureViews[i]);
        //    /*m_Framebuffer->SetCubeFace(i, m_CubeMap->GetRendererID());
        //    RenderCommand::Clear();*/
        //    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_CubeMap->GetRendererID(), 0);
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //    Renderer3D_NoBatch::DrawCube();
        //}

        //m_Shader->Unbind();
        //m_Framebuffer->Unbind();

        unsigned int captureFBO;
        unsigned int captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        unsigned int envCubemap;
        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        for(unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        m_Shader->Bind();
        m_Shader->SetInt("OriginalMap", 0);
        m_Shader->SetMat4("projection", captureProjection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_HdrMap->GetRendererID());

        glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for(unsigned int i = 0; i < 6; ++i)
        {
            m_Shader->SetMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Renderer3D_NoBatch::DrawCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}