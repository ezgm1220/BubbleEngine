#pragma once

#include "Bubble/Pipeline/Pipeline.h"
#include "Bubble/Renderer/Shader.h"


namespace Bubble
{
    class PBRPipeline :public Pipeline
    {
    public:
        virtual void Init(int SlotsSize = 0)final;
        virtual void BeginScene(const EditorCamera& camera)final;
        virtual void EndScene()final;
        virtual void Draw_Forward(Ref<VertexArray> vertex, uint32_t count)final;
        virtual void Draw_Deferred()final;// 离线渲染
        virtual int Get_IDValue(int x, int y)final; // 获取鼠标下的实体ID
        virtual uint32_t GetColorAttachmentRendererID()final;

    private:
        Ref<Shader> GBufferShader;
    };
}