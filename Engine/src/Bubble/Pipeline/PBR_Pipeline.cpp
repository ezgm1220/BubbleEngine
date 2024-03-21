#include "bubblepch.h"
#include "PBR_Pipeline.h"

namespace Bubble
{

    void PBRPipeline::Init(const int SlotsSize)
    {
        // GBufferShader
        int32_t* samplers = new int32_t[SlotsSize];
        for(uint32_t i = 0; i < SlotsSize; i++)
            samplers[i] = i;
        GBufferShader = Shader::Create("assets/shaders/Texture.glsl");
        GBufferShader->Bind();
        GBufferShader->SetIntArray("u_Textures", samplers, SlotsSize);

        IDFB_ID = PID(GBuffer);
        IDTexture_id = 1;

        ViewportFB_ID = PID(GBuffer);
        ViewportTexture_id = 0;
    }

    void PBRPipeline::BeginScene(const EditorCamera& camera)
    {
        m_Shader[PID(GBuffer)]->Bind();
        m_Shader[PID(GBuffer)]->SetMat4("u_ViewProjection", camera.GetViewProjection());
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

}

