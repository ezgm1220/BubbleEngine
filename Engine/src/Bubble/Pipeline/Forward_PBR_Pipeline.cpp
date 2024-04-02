#include "bubblepch.h"
#include "Forward_PBR_Pipeline.h"

#include <glad/glad.h>

namespace Bubble
{

    void FPBR_Pipeline::Init()
    {
        FramebufferSpecification fbSpec_Forward;
        fbSpec_Forward.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth};
        fbSpec_Forward.Width = 1280;
        fbSpec_Forward.Height = 720;
        this->Set_Framebuffer(fbSpec_Forward, PID(ForwardPBRFB));

        FramebufferSpecification fbSpec_SkyBox;
        fbSpec_SkyBox.Attachments = {FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::Depth};
        //fbSpec_SkyBox.Attachments = {FramebufferTextureFormat::RGBA16F};
        fbSpec_SkyBox.Width = 1280;
        fbSpec_SkyBox.Height = 720;
        this->Set_Framebuffer(fbSpec_SkyBox, PID(SkyBoxFB));

        std::unordered_map<int, std::string > shadersmap;
        shadersmap.insert({PID(ForwardPBRFB),"assets/shaders/ForwardPBR.glsl"});
        shadersmap.insert({PID(SkyBoxFB),"assets/shaders/Skybox.glsl"});
        this->LoadShaders(shadersmap);

        std::unordered_map<int, std::vector<std::pair<int, std::string>>> shaderinformation;
        std::vector<std::pair<int, std::string>> textureinformation_GBuffer = {
                        {0,"albedoMap"},{1,"normalMap"},{2,"metallicMap"},{3,"roughnessMap"},{4,"aoMap"},
                        { 5,"irradianceMap" },{6,"prefilterMap"},{7,"brdfLUT"}};
        shaderinformation.insert({PID(ForwardPBRFB),textureinformation_GBuffer});

        std::vector<std::pair<int, std::string>> textureinformation_SkyBox = {
                            {0,"RenderMap"},{1,"Skybox"}};
        shaderinformation.insert({PID(SkyBoxFB),textureinformation_SkyBox});

        this->BindTextureIndex(shaderinformation);

        this->SetViewportInformation(PID(SkyBoxFB), 0);

        this->SetEntityIDInformation(PID(ForwardPBRFB), 1);

        this->GetSkybox_Hdr("assets/shaders/GetCubeMap.glsl", "assets/SkyBox/newport_loft.hdr", 2048);
        this->GetIBL(128, 128, 5, 512);
    }

    void FPBR_Pipeline::BeginScene(const EditorCamera& camera)
    {
        m_Framebuffers[PID(ForwardPBRFB)]->Bind();
        m_Shader[PID(ForwardPBRFB)]->Bind();
        m_Shader[PID(ForwardPBRFB)]->SetMat4("ViewProjection", camera.GetViewProjection());
    }
    void FPBR_Pipeline::BeginScene(const SceneCamera& camera, const glm::mat4& transform)
    {
        /*m_Framebuffers[PID(ForwardPBRFB)]->Bind();
        m_Shader[PID(ForwardPBRFB)]->Bind();
        m_Shader[PID(ForwardPBRFB)]->SetMat4("ViewProjection", camera.GetViewProjection());*/
    }

    int FPBR_Pipeline::DrawScene(const glm::mat4& transform, const glm::vec4& tintColor, int entityID)
    {
        m_Shader[PID(ForwardPBRFB)]->SetMat4("model", transform);
        m_Shader[PID(ForwardPBRFB)]->SetFloat4("BaseColor", tintColor);
        m_Shader[PID(ForwardPBRFB)]->SetInt("EntityID", entityID);
        m_Shader[PID(ForwardPBRFB)]->SetMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));

        m_Shader[PID(ForwardPBRFB)]->BindTexture(5, m_Skybox.GetIrradianceMapID());
        m_Shader[PID(ForwardPBRFB)]->BindTexture(6, m_Skybox.GetPrefilterMapID());
        m_Shader[PID(ForwardPBRFB)]->BindTexture(7, m_Skybox.GetLUTID());

        return PID(ForwardPBRFB);
    }

    void FPBR_Pipeline::EndScene()
    {
        m_Shader[PID(ForwardPBRFB)]->Unbind();
    }

    void FPBR_Pipeline::Draw_Forward(Ref<VertexArray> vertex, uint32_t count)
    {
        
    }

    void FPBR_Pipeline::ClearEntityID()
    {
        this->Get_Framebuffer(PID(ForwardPBRFB))->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();
        m_Framebuffers[PID(ForwardPBRFB)]->ClearAttachment(1, -1);
    }

    bool FPBR_Pipeline::Calculatelighting_Begin()
    {
        return false;
    }

    Bubble::Ref<Bubble::Shader> FPBR_Pipeline::Calculatelighting(const glm::vec3& CameraPos)
    {
        return nullptr;
    }

    void FPBR_Pipeline::Calculatelighting_End()
    {

    }

    void FPBR_Pipeline::ShowSkyBox_Begin()
    {
        /*m_Framebuffers[PID(ForwardPBRFB)]->Bind();
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBindTexture(GL_TEXTURE_2D, m_Framebuffers[PID(SkyBoxFB)]->GetColorAttachmentRendererID(0));
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_Framebuffers[PID(SkyBoxFB)]->GetSpecification().Width, m_Framebuffers[PID(SkyBoxFB)]->GetSpecification().Height);*/
        
        //m_Framebuffers[PID(SkyBoxFB)]->CopyFrameBufferAttachment(m_Framebuffers[PID(ForwardPBRFB)]->GetID(), (int)Framebuffer::FBAttachmentBufferType::COLORBuffer);
        m_Framebuffers[PID(SkyBoxFB)]->CopyFrameBufferAttachment_One(m_Framebuffers[PID(ForwardPBRFB)]->GetID(), 0);
        m_Framebuffers[PID(SkyBoxFB)]->CopyFrameBufferAttachment_All(m_Framebuffers[PID(ForwardPBRFB)]->GetID(), (int)Framebuffer::FBAttachmentBufferType::DEPTHBuffer);
    
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

    Bubble::Ref<Bubble::Shader> FPBR_Pipeline::ShowSkyBox(const glm::mat4& View, const glm::mat4& projection)
    {
        auto texid0 = m_Framebuffers[PID(SkyBoxFB)]->GetColorAttachmentRendererID(0);
        m_Shader[PID(SkyBoxFB)]->BindTexture(0, texid0);

        m_Shader[PID(SkyBoxFB)]->BindTexture(1, m_Skybox.GetCubeMapID());
        //m_Shader[PID(SkyBoxFB)]->BindTexture(1, m_Skybox.GetIrradianceMapID());
        //m_Shader[PID(SkyBoxFB)]->BindTexture(1, m_Skybox.GetPrefilterMapID());

        m_Shader[PID(SkyBoxFB)]->SetMat4("view", View);
        m_Shader[PID(SkyBoxFB)]->SetMat4("projection", projection);

        return m_Shader[PID(SkyBoxFB)];
    }

    void FPBR_Pipeline::ShowSkyBox_End()
    {
        m_Shader[PID(SkyBoxFB)]->Unbind();
        m_Framebuffers[PID(SkyBoxFB)]->Unbind();
    }

    int FPBR_Pipeline::GetEntityID(int mouseX, int mouseY)
    {
        m_Framebuffers[Entity_FramebufferID]->Bind();
        auto res = m_Framebuffers[Entity_FramebufferID]->ReadPixel(Entity_AttachmentIndex, mouseX, mouseY);
        m_Framebuffers[Entity_FramebufferID]->Unbind();
        return res;
    }

    uint64_t FPBR_Pipeline::Texture_DispalyViewport()
    {
        if(ViewportTexture_FramebufferID == -1 || ViewportTexture_AttachmentIndex == -1)
        {
            BB_CORE_WARN("PBRPipeline: No set Viewport Texture Information!!!!!!");
        }
        return m_Framebuffers[ViewportTexture_FramebufferID]->GetColorAttachmentRendererID(ViewportTexture_AttachmentIndex);
    }

}

