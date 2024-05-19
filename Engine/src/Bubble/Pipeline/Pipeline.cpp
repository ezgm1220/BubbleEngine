#include "bubblepch.h"
#include <Bubble/Pipeline/Pipeline.h>

namespace Bubble
{

    void Pipeline::LoadShaders(const std::unordered_map<int, std::string>& ShaderInformations)
    {
         // 初始化Shader
        for(auto map : ShaderInformations)
        {
            m_Shader.insert({map.first,Shader::Create(map.second)});
        }
    }

    void Pipeline::ReLoadShaders(const std::unordered_map<int, std::string>& ShaderInformations)
    {
        for(auto map : ShaderInformations)
        {
            for(auto nmp : this->m_Shader)
            {
                auto k = nmp;
            }
            if(!m_Shader.count(map.first))
            {
                BB_CORE_ERROR("No find " + map.second);
            }
            else
            {
                m_Shader[map.first] = Shader::Create(map.second);
            }
        }
    }

    void Pipeline::BindTextureIndex(const std::unordered_map<int, std::vector<std::pair<int, std::string>>>& indexs)
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

    void Pipeline::SetViewportInformation(int FrambufferID, int AttachmentIndex)
    {
        ViewportTexture_FramebufferID = FrambufferID;
        ViewportTexture_AttachmentIndex = AttachmentIndex;
    }

    void Pipeline::SetEntityIDInformation(int FrambufferID, int AttachmentIndex)
    {
        Entity_FramebufferID = FrambufferID;
        Entity_AttachmentIndex = AttachmentIndex;
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

    void Pipeline::GetSkybox_SixFaces(const std::string path)
    {
        m_Skybox.SetHdrSixFacesPath(path);
        m_Skybox.GetCubeMap_SixFaces();
    }

    void Pipeline::GetSkybox_Hdr(const std::string Shaderpath, const std::string HdrTexpath, const uint32_t Mapsize)
    {
        m_Skybox.GetEmptyCubeMap(Mapsize);
        m_Skybox.SetShader(Shaderpath);
        m_Skybox.SetHdrTexPath(HdrTexpath);
        m_Skybox.SetFrameBuffer(Mapsize);
        m_Skybox.GetCubeMap_Hdr();
    }

    void Pipeline::GetIBL(int IiaSize /*= 32*/, int PrfSize /*= 128*/, int PrfMipNum /*= 5*/, int LUTSize /*= 512*/)
    {
        m_Skybox.InitIBL(IiaSize, PrfSize, PrfMipNum, LUTSize);
        m_Skybox.GetIBL();
    }

}