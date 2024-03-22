#include "bubblepch.h"
#include <Bubble/Pipeline/Pipeline.h>

namespace Bubble
{

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

}