#pragma once

#include "Bubble/Pipeline/Pipeline.h"
#include "Bubble/Renderer/Shader.h"


namespace Bubble
{
    class PBRPipeline :public Pipeline
    {
    public:

        virtual void Init()override;

        virtual void BeginScene(const EditorCamera& camera)override final;
        virtual void BeginScene(const SceneCamera& camera, const glm::mat4& transform)override final;
        virtual int DrawScene(const glm::mat4& transform, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1)override;
        virtual void EndScene()override final;
        virtual void Draw_Forward(Ref<VertexArray> vertex, uint32_t count)override final;
        virtual void ClearEntityID()override final;

        virtual bool Calculatelighting_Begin() override final;
        virtual Ref<Shader> Calculatelighting(const glm::vec3& CameraPos) override final;
        virtual void Calculatelighting_End() override final;

        virtual void ShowSkyBox_Begin() override final;
        virtual Ref<Shader> ShowSkyBox(const glm::mat4& View, const glm::mat4& projection)override final;
        virtual void ShowSkyBox_End()override final;

        virtual int GetEntityID(int mouseX, int mouseY)override final;

        virtual uint64_t Texture_DispalyViewport()override final;

    private:
        Ref<Shader> GBufferShader;

    };
}