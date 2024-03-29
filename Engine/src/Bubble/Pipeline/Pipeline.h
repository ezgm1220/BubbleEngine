#pragma once
#include <vector>
#include <unordered_map>
#include "Bubble/Renderer/Framebuffer.h"
#include "Bubble/Renderer/VertexArray.h"
#include "Bubble/Renderer/Texture.h"
#include "Bubble/Renderer/Shader.h"
#include "Bubble/Camera/EditorCamera.h"
#include "Bubble/Camera/SceneCamera.h"
#include "Bubble/Renderer/RenderCommand.h"

#define PID(x) static_cast<int>(Pipeline::Type::x)

namespace Bubble
{
    class Pipeline
    {
    public:
        Pipeline(){}
        ~Pipeline(){}

        enum class Type
        {
            Default = -1, GBuffer = 1, Light = 2, SkyBox = 3
        };

        virtual void LoadShaders(const std::unordered_map<int,std::string>& ShaderInformations) = 0;
        virtual void BindTextureIndex(const std::unordered_map<int, std::vector<std::pair<int, std::string>>>& indexs) = 0;

        virtual void BeginScene(const EditorCamera& camera) = 0;
        virtual void BeginScene(const SceneCamera& camera,const glm::mat4& transform) = 0;
        virtual void EndScene() = 0;
        virtual void Draw_Forward(Ref<VertexArray> vertex, uint32_t count) = 0;// 前向渲染的接口

        virtual void Calculatelighting_Begin()=0;
        virtual Ref<Shader> Calculatelighting() = 0;
        virtual void Calculatelighting_End() = 0;

        virtual int GetEntityID(int FramebufferID, int AttachmentIndex, int mouseX, int mouseY) = 0;
        virtual uint64_t Texture_DispalyViewport() = 0;

        void SetViewportInformation(int FrambufferID, int AttachmentIndex);

        void Set_Framebuffer(FramebufferSpecification& fbSpec, int id = -1);// 创建Framebuffer

        void Set_Shader(const std::string& filepath, int id = -1);
        Ref<Shader> Get_Shader(int id);

        //void Bind_BaseTexture(Texture& texture);// 物体最基本的Texture
        Ref<Framebuffer> Get_Framebuffer(int id = 0);// 返回Framebuffer的引用
        void Resize_FrameBuffer(uint32_t x, uint32_t y);

        std::vector<Ref<Framebuffer>> GetFramebufferVector();

        void UnbindFramebuffer();


    protected:
        std::unordered_map<int,Ref<Framebuffer>>m_Framebuffers;
        std::unordered_map<int,Ref<Shader>>m_Shader;
        int ViewportTexture_FramebufferID = -1;
        int ViewportTexture_AttachmentIndex = -1;
    };

}