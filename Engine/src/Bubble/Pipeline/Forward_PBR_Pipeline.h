#pragma once
#include "Bubble/Pipeline/Pipeline.h"


namespace Bubble
{
    class FPBR_Pipeline: public Pipeline
    {
    public:

        virtual void Init()override;

        virtual void BeginScene(const EditorCamera& camera) override;
        virtual void BeginScene(const SceneCamera& camera, const glm::mat4& transform) override;
        virtual int DrawScene(const glm::mat4& transform, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1)override;
        virtual void EndScene() override;
        virtual void Draw_Forward(Ref<VertexArray> vertex, uint32_t count) override;// 前向渲染的接口
        virtual void ClearEntityID() override;

        virtual bool Calculatelighting_Begin() override;
        virtual Ref<Shader> Calculatelighting(const glm::vec3& CameraPos) override;
        virtual void Calculatelighting_End() override;
        virtual void ShowSkyBox_Begin() override;
        virtual Ref<Shader> ShowSkyBox(const glm::mat4& View, const glm::mat4& projection) override;
        virtual void ShowSkyBox_End() override;


        virtual int GetEntityID(int mouseX, int mouseY) override;
        virtual uint64_t Texture_DispalyViewport() override;
    };
}