#include "bubblepch.h"
#include "PBR_Pipeline.h"

#include <glad/glad.h>

namespace Bubble
{

    void PBRPipeline::LoadShaders(const std::unordered_map<int, std::string>& ShaderInformations)
    {
        // 初始化Shader
        for(auto map : ShaderInformations)
        {
            m_Shader.insert({map.first,Shader::Create(map.second)});
        }        
    }

    void PBRPipeline::BindTextureIndex(const std::unordered_map<int, std::vector<std::pair<int, std::string>>>& indexs)
    {
        for(auto ShaderInformation : indexs)
        {
            auto& shader = m_Shader[ShaderInformation.first];
            shader->Bind();
            for(auto texture : ShaderInformation.second)
            {
                shader->SetInt(texture.second, texture.first);
            }
            shader->Unbind();
        }
    }

    void PBRPipeline::BeginScene(const EditorCamera& camera)
    {
        m_Framebuffers[PID(GBufferFB)]->Bind();
        m_Shader[PID(GBufferFB)]->Bind();
        m_Shader[PID(GBufferFB)]->SetMat4("u_ViewProjection", camera.GetViewProjection());
    }

    void PBRPipeline::BeginScene(const SceneCamera& camera, const glm::mat4& transform)
    {
        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

        m_Shader[PID(GBufferFB)]->Bind();
        m_Shader[PID(GBufferFB)]->SetMat4("u_ViewProjection", viewProj);

    }

    void PBRPipeline::EndScene()
    {
        m_Shader[PID(GBufferFB)]->Unbind();
    }

    void PBRPipeline::Draw_Forward(Ref<VertexArray> vertex, uint32_t count)
    {
        RenderCommand::DrawIndexed(vertex, count);
    }

    void PBRPipeline::ClearEntityID()
    {
        m_Framebuffers[PID(GBufferFB)]->ClearAttachment(3, -1);
    }

    void PBRPipeline::Calculatelighting_Begin()
    {
        if(!m_Framebuffers.count(PID(LightFB)))
        {
            BB_CORE_WARN("No set LightFB Framebuffer !!!!!");
            BB_ASSERT(1, "No set LightFB Framebuffer !!!!!");
        }
        m_Framebuffers[PID(LightFB)]->Bind();

        if(!m_Shader.count(PID(LightFB)))
        {
            BB_CORE_WARN("No set LightFB Shader !!!!!");
            BB_ASSERT(1, "No set LightFB Shader !!!!!");
        }
        m_Shader[PID(LightFB)]->Bind();
    }

    Ref<Shader> PBRPipeline::Calculatelighting()
    {
        auto texid = m_Framebuffers[PID(GBufferFB)]->GetColorAttachmentRendererID(0);
        m_Shader[PID(LightFB)]->BindTexture(0, texid);

        return m_Shader[PID(LightFB)];
    }

    void PBRPipeline::Calculatelighting_End()
    {
        m_Shader[PID(LightFB)]->Unbind();
        m_Framebuffers[PID(LightFB)]->Unbind();
    }

    void PBRPipeline::ShowSkyBox_Begin()
    {

        //glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Framebuffers[PID(LightFB)]->GetID());
        //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffers[PID(SkyBoxFB)]->GetID());
        //glBlitFramebuffer(0, 0, m_Framebuffers[PID(LightFB)]->GetSpecification().Width, m_Framebuffers[PID(LightFB)]->GetSpecification().Height, 0, 0, 
        //    m_Framebuffers[PID(SkyBoxFB)]->GetSpecification().Width, m_Framebuffers[PID(SkyBoxFB)]->GetSpecification().Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        m_Framebuffers[PID(SkyBoxFB)]->CopyFrameBufferAttachment(m_Framebuffers[PID(LightFB)]->GetID(),(int)Framebuffer::FBAttachmentBufferType::COLORBuffer);
        m_Framebuffers[PID(SkyBoxFB)]->CopyFrameBufferAttachment(m_Framebuffers[PID(GBufferFB)]->GetID(),(int)Framebuffer::FBAttachmentBufferType::DEPTHBuffer);

       /* glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Framebuffers[PID(GBufferFB)]->GetID());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffers[PID(SkyBoxFB)]->GetID());
        glBlitFramebuffer(0, 0, m_Framebuffers[PID(GBufferFB)]->GetSpecification().Width, m_Framebuffers[PID(GBufferFB)]->GetSpecification().Height, 0, 0,
            m_Framebuffers[PID(SkyBoxFB)]->GetSpecification().Width, m_Framebuffers[PID(SkyBoxFB)]->GetSpecification().Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);*/

        if(!m_Framebuffers.count(PID(SkyBoxFB)))
        {
            BB_CORE_WARN("No set LightFB Framebuffer !!!!!");
            BB_ASSERT(1, "No set LightFB Framebuffer !!!!!");
        }
        m_Framebuffers[PID(SkyBoxFB)]->Bind();

        if(!m_Shader.count(PID(SkyBoxFB)))
        {
            BB_CORE_WARN("No set LightFB Shader !!!!!");
            BB_ASSERT(1, "No set LightFB Shader !!!!!");
        }
        m_Shader[PID(SkyBoxFB)]->Bind();
    }

    Bubble::Ref<Bubble::Shader> PBRPipeline::ShowSkyBox()
    {
        auto texid0 = m_Framebuffers[PID(SkyBoxFB)]->GetColorAttachmentRendererID(0);
        m_Shader[PID(SkyBoxFB)]->BindTexture(0, texid0);
        
        m_Shader[PID(SkyBoxFB)]->BindTexture(1, m_Skybox.GetCubeMapID());

        return m_Shader[PID(SkyBoxFB)];
    }

    void PBRPipeline::ShowSkyBox_End()
    {
        m_Shader[PID(SkyBoxFB)]->Unbind();
        m_Framebuffers[PID(SkyBoxFB)]->Unbind();
    }

    int PBRPipeline::GetEntityID(int FramebufferID, int AttachmentIndex, int mouseX, int mouseY)
    {
        m_Framebuffers[FramebufferID]->Bind();
        return  m_Framebuffers[FramebufferID]->ReadPixel(AttachmentIndex, mouseX, mouseY);
        m_Framebuffers[FramebufferID]->Unbind();
    }

    uint64_t PBRPipeline::Texture_DispalyViewport()
    {
        if(ViewportTexture_FramebufferID == -1 || ViewportTexture_AttachmentIndex == -1)
        {
            BB_CORE_WARN("PBRPipeline: No set Viewport Texture Information!!!!!!");
        }
        return m_Framebuffers[ViewportTexture_FramebufferID]->GetColorAttachmentRendererID(ViewportTexture_AttachmentIndex);
    }

}

