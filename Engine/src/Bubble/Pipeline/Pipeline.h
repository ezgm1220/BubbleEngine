#pragma once
#include <vector>
#include <unordered_map>
#include "Bubble/Renderer/Framebuffer.h"
#include "Bubble/Renderer/VertexArray.h"
#include "Bubble/Renderer/Texture.h"
#include "Bubble/Renderer/Shader.h"
#include "Bubble/Camera/EditorCamera.h"
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
            GBuffer = 0, Deferred = 1
        };

        virtual void Init(int SlotsSize = 0) = 0;
        virtual void BeginScene(const EditorCamera& camera) = 0;
        virtual void EndScene() = 0;
        virtual void Draw_Forward(Ref<VertexArray> vertex, uint32_t count) = 0;// 前向渲染的接口
        virtual void Draw_Deferred() = 0;// 离线渲染
        virtual int Get_IDValue(int x, int y) = 0;
        virtual uint32_t GetColorAttachmentRendererID() = 0;

        void Set_Framebuffer(FramebufferSpecification& fbSpec, int id = -1);// 创建Framebuffer

        void Set_Shader(const std::string& filepath, int id = -1);
        Ref<Shader> Get_Shader(int id);

        //void Bind_BaseTexture(Texture& texture);// 物体最基本的Texture
        Ref<Framebuffer> Get_Framebuffer(int id = 0);// 返回Framebuffer的引用
        void Resize_FrameBuffer(uint32_t x, uint32_t y);

    protected:
        std::unordered_map<int,Ref<Framebuffer>>m_Framebuffers;
        std::unordered_map<int,Ref<Shader>>m_Shader;
        std::vector<Texture*>BaseTexture;
        int ViewportFB_ID = -1;
        int ViewportTexture_id = -1;
        int IDFB_ID=-1;
        int IDTexture_id=-1;
    };

}