#include "bubblepch.h"
#include <Bubble/Pipeline/Pipeline.h>

namespace Bubble
{

    void Pipeline::SetViewportInformation(int FrambufferID, int AttachmentIndex)
    {
        ViewportTexture_FramebufferID = FrambufferID;
        ViewportTexture_AttachmentIndex = AttachmentIndex;
    }

    void Pipeline::Set_Framebuffer(FramebufferSpecification& fbSpec, int id)
    {
        BB_CORE_ASSERT(id != -1, "Pipeline->Framebuffer->id no value");
        m_Framebuffers.insert({id,Framebuffer::Create(fbSpec)});
    }

    void Pipeline::Set_Shader(const std::string& filepath, int id /*= -1*/)
    {
        BB_CORE_ASSERT(id != -1, "Shader->id no value");
        m_Shader.insert({id,Shader::Create(filepath)});
    }

    Bubble::Ref<Bubble::Shader> Pipeline::Get_Shader(int id)
    {
        BB_CORE_ASSERT(m_Shader.count(id), "no find the Pipeline Shader");
        return m_Shader[id];
    }

    Bubble::Ref<Framebuffer> Pipeline::Get_Framebuffer(int id /*= 0*/)
    {
        return m_Framebuffers[id];
    }

    void Pipeline::Resize_FrameBuffer(uint32_t x, uint32_t y)
    {
        for(auto buffer : m_Framebuffers)
        {
            buffer.second->Resize(x, y);
        }
    }

    std::vector<Bubble::Ref<Bubble::Framebuffer>> Pipeline::GetFramebufferVector()
    {
        std::vector<Bubble::Ref<Bubble::Framebuffer>> v;
        for(auto buffer : m_Framebuffers)
        {
            v.emplace_back(buffer.second);
        }
        return v;
    }

    void Pipeline::UnbindFramebuffer()
    {
        for(auto buffer : m_Framebuffers)
        {
            buffer.second->Unbind();
            return;
        }
        BB_CORE_WARN("Bo framebuffer for you to unbind!!!!!!");
    }

}