#include "bubblepch.h"
#include "PBR_Pipeline.h"

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
        m_Shader[PID(GBuffer)]->Bind();
        m_Shader[PID(GBuffer)]->SetMat4("u_ViewProjection", camera.GetViewProjection());
    }

    void PBRPipeline::BeginScene(const SceneCamera& camera, const glm::mat4& transform)
    {
        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

        m_Shader[PID(GBuffer)]->Bind();
        m_Shader[PID(GBuffer)]->SetMat4("u_ViewProjection", viewProj);

    }

    void PBRPipeline::EndScene()
    {
        m_Shader[PID(GBuffer)]->Unbind();
    }

    void PBRPipeline::Draw_Forward(Ref<VertexArray> vertex, uint32_t count)
    {
        RenderCommand::DrawIndexed(vertex, count);
    }

    void PBRPipeline::Draw_Deferred()
    {

    }


    int PBRPipeline::GetEntityID(int FramebufferID, int AttachmentIndex, int mouseX, int mouseY)
    {
        m_Framebuffers[FramebufferID]->Bind();
        return  m_Framebuffers[FramebufferID]->ReadPixel(AttachmentIndex, mouseX, mouseY);
        m_Framebuffers[FramebufferID]->Unbind();
    }

    uint64_t PBRPipeline::Texture_DispalyViewport()
    {
        if(ViewportTexture_AttachmentIndex == -1 || ViewportTexture_AttachmentIndex == -1)
        {
            BB_CORE_WARN("PBRPipeline: No set Viewport Texture Information!!!!!!");
        }
        return m_Framebuffers[ViewportTexture_AttachmentIndex]->GetColorAttachmentRendererID(ViewportTexture_AttachmentIndex);
    }

}

