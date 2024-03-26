#pragma once

#include "Bubble/Pipeline/Pipeline.h"
#include "Bubble/Renderer/Shader.h"


namespace Bubble
{
    class PBRPipeline :public Pipeline
    {
    public:
        virtual void LoadShaders(const std::unordered_map<int, std::string>& ShaderInformations)override final;
        virtual void BindTextureIndex(const std::unordered_map<int, std::vector<std::pair<int, std::string>>>& indexs)override final;

        virtual void BeginScene(const EditorCamera& camera)override final;
        virtual void BeginScene(const SceneCamera& camera, const glm::mat4& transform)override final;
        virtual void EndScene()override final;
        virtual void Draw_Forward(Ref<VertexArray> vertex, uint32_t count)override final;
        virtual void Draw_Deferred()override final;// 离线渲染
        virtual int Get_IDValue(int x, int y)override final; // 获取鼠标下的实体ID
        virtual uint32_t GetColorAttachmentRendererID()override final;

        virtual int GetEntityID(int FramebufferID, int AttachmentIndex, int mouseX, int mouseY)override final;

    private:
        Ref<Shader> GBufferShader;
    };
}