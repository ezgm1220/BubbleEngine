#include "bubblepch.h"
#include "PBR_Pipeline.h"

#include <glad/glad.h>

namespace Bubble
{

    void PBRPipeline::Init()
    {
        // 设置帧缓冲信息以及Shader信息
        {
            FramebufferSpecification fbSpec_GBuffer;
            fbSpec_GBuffer.Attachments = {FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F,FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth};
            fbSpec_GBuffer.Width = 1280;
            fbSpec_GBuffer.Height = 720;
            this->Set_Framebuffer(fbSpec_GBuffer, PID(GBufferFB));

            FramebufferSpecification fbSpec_Light;
            fbSpec_Light.Attachments = {FramebufferTextureFormat::RGBA16F};
            fbSpec_Light.Width = 1280;
            fbSpec_Light.Height = 720;
            this->Set_Framebuffer(fbSpec_Light, PID(LightFB));

            FramebufferSpecification fbSpec_SkyBox;
            fbSpec_SkyBox.Attachments = {FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::Depth};
            //fbSpec_SkyBox.Attachments = {FramebufferTextureFormat::RGBA16F};
            fbSpec_SkyBox.Width = 1280;
            fbSpec_SkyBox.Height = 720;
            this->Set_Framebuffer(fbSpec_SkyBox, PID(SkyBoxFB));

            std::unordered_map<int, std::string > shadersmap;
            shadersmap.insert({PID(GBufferFB),"assets/shaders/DeferedPipeline/GBufferPass.glsl"});
            shadersmap.insert({PID(LightFB),"assets/shaders/DeferedPipeline/LightingPass.glsl"});
            shadersmap.insert({PID(SkyBoxFB),"assets/shaders/Skybox.glsl"});
            this->LoadShaders(shadersmap);

            std::unordered_map<int, std::vector<std::pair<int, std::string>>> shaderinformation;
            std::vector<std::pair<int, std::string>> textureinformation_GBuffer = {
                            {0,"Albedo"},{1,"Normal"},{2,"Metallic"},{3,"Roughness"},{4,"AO"}};
            shaderinformation.insert({PID(GBufferFB),textureinformation_GBuffer});

            std::vector<std::pair<int, std::string>> textureinformation_Light = {
                            {0,"ColorMap"},{1,"PositionMap"},{2,"NormalMap"},{3,"MRAMap"},
                            { 4,"irradianceMap" },{5,"prefilterMap"},{6,"brdfLUT"}};
            shaderinformation.insert({PID(LightFB),textureinformation_Light});

            std::vector<std::pair<int, std::string>> textureinformation_SkyBox = {
                            {0,"RenderMap"},{1,"Skybox"}};
            shaderinformation.insert({PID(SkyBoxFB),textureinformation_SkyBox});


            this->BindTextureIndex(shaderinformation);
        }

        //this->SetViewportInformation(PID(SkyBoxFB), 0);

        this->SetViewportInformation(PID(SkyBoxFB), 0);

        this->SetEntityIDInformation(PID(GBufferFB), 4);
        //this->SetViewportInformation(PID(LightFB), 0);

        //this->GetSkybox_SixFaces("assets/SkyBox/Sky");
        //this->GetSkybox_Hdr("assets/shaders/GetCubeMap.glsl", "assets/SkyBox/spree_bank_4k.hdr", 512);
        this->GetSkybox_Hdr("assets/shaders/GetCubeMap.glsl", "assets/SkyBox/metro_noord_4k.hdr", 2048);
        this->GetIBL(128, 128, 5, 512);
    }

    void PBRPipeline::BeginScene(const EditorCamera& camera)
    {
        m_Framebuffers[PID(GBufferFB)]->Bind();
        m_Shader[PID(GBufferFB)]->Bind();
        m_Shader[PID(GBufferFB)]->SetMat4("ViewProjection", camera.GetViewProjection());
    }

    void PBRPipeline::BeginScene(const SceneCamera& camera, const glm::mat4& transform)
    {
        m_Framebuffers[PID(GBufferFB)]->Bind();
        m_Shader[PID(GBufferFB)]->Bind();
        m_Shader[PID(GBufferFB)]->SetMat4("ViewProjection", camera.GetProjection() * glm::inverse(transform));
        //m_Shader[PID(ForwardPBRFB)]->SetFloat3("camPos", glm::vec3(transform * glm::vec4(0.0, 0.0, 0.0, 1.0)));

    }

    int PBRPipeline::DrawScene(const glm::mat4& transform, const glm::vec4& tintColor, int entityID)
    {
        m_Shader[PID(GBufferFB)]->SetMat4("Model", transform);
        m_Shader[PID(GBufferFB)]->SetFloat4("BaseColor", tintColor);
        m_Shader[PID(GBufferFB)]->SetInt("EntityID", entityID);
        m_Shader[PID(GBufferFB)]->SetMat3("NormalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));
        return PID(GBufferFB);
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
        this->Get_Framebuffer(PID(GBufferFB))->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();
        m_Framebuffers[PID(GBufferFB)]->ClearAttachment(4, -1);
    }

    bool PBRPipeline::Calculatelighting_Begin()
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
        return true;
    }

    Ref<Shader> PBRPipeline::Calculatelighting(const glm::vec3& CameraPos)
    {
        auto texid0 = m_Framebuffers[PID(GBufferFB)]->GetColorAttachmentRendererID(0);
        auto texid1 = m_Framebuffers[PID(GBufferFB)]->GetColorAttachmentRendererID(1);
        auto texid2 = m_Framebuffers[PID(GBufferFB)]->GetColorAttachmentRendererID(2);
        auto texid3 = m_Framebuffers[PID(GBufferFB)]->GetColorAttachmentRendererID(3);
        m_Shader[PID(LightFB)]->BindTexture(0, texid0);
        m_Shader[PID(LightFB)]->BindTexture(1, texid1);
        m_Shader[PID(LightFB)]->BindTexture(2, texid2);
        m_Shader[PID(LightFB)]->BindTexture(3, texid3);

        m_Shader[PID(LightFB)]->BindTexture(4, m_Skybox.GetIrradianceMapID());
        m_Shader[PID(LightFB)]->BindTexture(5, m_Skybox.GetPrefilterMapID());
        //m_Shader[PID(LightFB)]->BindTexture(5, m_Skybox.GetCubeMapID());
        m_Shader[PID(LightFB)]->BindTexture(6, m_Skybox.GetLUTID());


        m_Shader[PID(LightFB)]->SetFloat3("CameraPos", CameraPos);
        //BB_CORE_INFO(CameraPos);

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
        m_Framebuffers[PID(SkyBoxFB)]->CopyFrameBufferAttachment_All(m_Framebuffers[PID(LightFB)]->GetID(),(int)Framebuffer::FBAttachmentBufferType::COLORBuffer);
        m_Framebuffers[PID(SkyBoxFB)]->CopyFrameBufferAttachment_All(m_Framebuffers[PID(GBufferFB)]->GetID(),(int)Framebuffer::FBAttachmentBufferType::DEPTHBuffer);

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

    Bubble::Ref<Bubble::Shader> PBRPipeline::ShowSkyBox(const glm::mat4& View, const glm::mat4& projection)
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

    void PBRPipeline::ShowSkyBox_End()
    {
        m_Shader[PID(SkyBoxFB)]->Unbind();
        m_Framebuffers[PID(SkyBoxFB)]->Unbind();
    }

    int PBRPipeline::GetEntityID(int mouseX, int mouseY)
    {
        m_Framebuffers[Entity_FramebufferID]->Bind();
        auto res =  m_Framebuffers[Entity_FramebufferID]->ReadPixel(Entity_AttachmentIndex, mouseX, mouseY);
        m_Framebuffers[Entity_FramebufferID]->Unbind();
        return res;
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

