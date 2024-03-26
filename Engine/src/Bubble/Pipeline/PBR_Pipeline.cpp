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
        //m_Framebuffers[PID(GBuffer)]->Unbind();
    }

    void PBRPipeline::Draw_Forward(Ref<VertexArray> vertex, uint32_t count)
    {
        RenderCommand::DrawIndexed(vertex, count);
    }

    void PBRPipeline::Draw_Deferred()
    {

    }

    int PBRPipeline::Get_IDValue(int x, int y)
    {
       return m_Framebuffers[IDFB_ID]->ReadPixel(IDTexture_id, x, y);
    }

    uint32_t PBRPipeline::GetColorAttachmentRendererID()
    {
        return m_Framebuffers[ViewportFB_ID]->GetColorAttachmentRendererID(ViewportTexture_id);
    }

    int PBRPipeline::GetEntityID(int FramebufferID, int AttachmentIndex, int mouseX, int mouseY)
    {
        m_Framebuffers[FramebufferID]->Bind();
        return  m_Framebuffers[FramebufferID]->ReadPixel(AttachmentIndex, mouseX, mouseY);
    }

}

