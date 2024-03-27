#include "bubblepch.h"
#include "Renderer3D_NoBatch.h"

namespace Bubble
{

    namespace Render3DNoBatch
    {
        struct VertexData
        {
            glm::vec3 Position;
            glm::vec2 TexCoord;
        };

        struct Renderer3DData
        {
            Ref<VertexArray>CubeVAO = nullptr;
            Ref<VertexArray>QuadVAO = nullptr;

            Ref<VertexBuffer>CubeVBO = nullptr;
            Ref<VertexBuffer>QuadVBO = nullptr;

            Renderer3D_NoBatch::Statistics Stats;
        };
    }

    static Render3DNoBatch::Renderer3DData s_Data;

    void Renderer3D_NoBatch::Init()
    {
        s_Data.CubeVAO = VertexArray::Create();
        //glCreateVertexArrays(1, &m_RendererID);

        s_Data.CubeVBO = VertexBuffer::Create(16 * sizeof(Render3DNoBatch::VertexData));
        /*glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);*/

        s_Data.CubeVBO->SetLayout({
                { ShaderDataType::Float3, "a_Position"     },
                { ShaderDataType::Float2, "a_TexCoord"     }
            });
        s_Data.CubeVAO->AddVertexBuffer(s_Data.CubeVBO);

        Render3DNoBatch::VertexData Cubevertexdata[16];
        uint32_t cubeIndices[36];
        {
            Cubevertexdata[0].Position = {-0.5f, -0.5f,  0.5f};
            Cubevertexdata[1].Position = {0.5f, -0.5f,   0.5f};
            Cubevertexdata[2].Position = {0.5f,  0.5f,   0.5f};
            Cubevertexdata[3].Position = {-0.5f,  0.5f,  0.5f};
            Cubevertexdata[4].Position = {-0.5f, -0.5f, -0.5f};
            Cubevertexdata[5].Position = {0.5f, -0.5f,  -0.5f};
            Cubevertexdata[6].Position = {0.5f,  0.5f,  -0.5f};
            Cubevertexdata[7].Position = {-0.5f,  0.5f, -0.5f};
            Cubevertexdata[8].Position = {-0.5f, -0.5f,  0.5f};
            Cubevertexdata[9].Position = {0.5f, -0.5f,  0.5f};
            Cubevertexdata[10].Position = {0.5f,  0.5f,  0.5f};
            Cubevertexdata[11].Position = {-0.5f,  0.5f,  0.5f};
            Cubevertexdata[12].Position = {-0.5f, -0.5f, -0.5f};
            Cubevertexdata[13].Position = {0.5f, -0.5f, -0.5f};
            Cubevertexdata[14].Position = {0.5f,  0.5f, -0.5f};
            Cubevertexdata[15].Position = {-0.5f,  0.5f, -0.5f};

            Cubevertexdata[0].TexCoord = {0.0f, 0.0f};
            Cubevertexdata[1].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[2].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[3].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[4].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[5].TexCoord = {0.0f, 0.0f};
            Cubevertexdata[6].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[7].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[8].TexCoord = {0.0f, 0.0f};
            Cubevertexdata[9].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[10].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[11].TexCoord = {0.0f, 0.0f};
            Cubevertexdata[12].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[13].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[14].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[15].TexCoord = {0.0f, 1.0f};

            // 前面
            cubeIndices[0] = 0;
            cubeIndices[1] = 1;
            cubeIndices[2] = 2;
            cubeIndices[3] = 0;
            cubeIndices[4] = 2;
            cubeIndices[5] = 3;

            //后面
            cubeIndices[6] = 5;
            cubeIndices[7] = 4;
            cubeIndices[8] = 7;
            cubeIndices[9] = 5;
            cubeIndices[10] = 7;
            cubeIndices[11] = 6;

                //左面
            cubeIndices[12] = 4;
            cubeIndices[13] = 0;
            cubeIndices[14] = 3;
            cubeIndices[15] = 4;
            cubeIndices[16] = 3;
            cubeIndices[17] = 7;

            // 右面
            cubeIndices[18] = 1;
            cubeIndices[19] = 5;
            cubeIndices[20] = 6;
            cubeIndices[21] = 1;
            cubeIndices[22] = 6;
            cubeIndices[23] = 2;

            // 上面
            cubeIndices[24] = 11;
            cubeIndices[25] = 10;
            cubeIndices[26] = 14;
            cubeIndices[27] = 11;
            cubeIndices[28] = 14;
            cubeIndices[29] = 15;

             // 下面
            cubeIndices[30] = 8;
            cubeIndices[31] = 9;
            cubeIndices[32] = 13;
            cubeIndices[33] = 8;
            cubeIndices[34] = 13;
            cubeIndices[35] = 12;
        }

        s_Data.CubeVAO->Bind();
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(cubeIndices, 36);// 创建EBO并绑定EBO数据
        s_Data.CubeVAO->SetIndexBuffer(quadIB);// 让VAO记录EBO的索引
        s_Data.CubeVBO->SetData(Cubevertexdata, 16 * sizeof(Render3DNoBatch::VertexData));// 为VBO填充数据

        // 解绑
        s_Data.CubeVAO->Unbind();
        s_Data.CubeVBO->Unbind();
        quadIB->Unbind();




    }

    void Renderer3D_NoBatch::Shutdown()
    {

    }

    void Renderer3D_NoBatch::BeginScene(const SceneCamera& camera, const glm::mat4& transform, Ref<Pipeline>pipeline)
    {
        pipeline->BeginScene(camera, transform);
    }

    void Renderer3D_NoBatch::BeginScene(const EditorCamera& camera, Ref<Pipeline>pipeline)
    {
        pipeline->Get_Framebuffer(PID(GBuffer))->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();
        pipeline->Get_Framebuffer(PID(GBuffer))->ClearAttachment(3, -1);
        pipeline->BeginScene(camera);
    }

    void Renderer3D_NoBatch::EndScene(Ref<Pipeline>pipeline)
    {
        pipeline->UnbindFramebuffer();
        pipeline->EndScene();
    }

    void Renderer3D_NoBatch::DrawCube(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, const glm::vec4& color, int entityID /*= -1*/)
    {
        pipeline->Get_Shader(ShaderID)->SetMat4("Transform", transform);
        pipeline->Get_Shader(ShaderID)->SetFloat4("BaseColor", color);
        pipeline->Get_Shader(ShaderID)->SetInt("EntityID", entityID);

        RenderCommand::DrawIndexed(s_Data.CubeVAO, 36);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer3D_NoBatch::DrawCube(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, const Ref<Texture2D>* textures, int TexturesSize, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/, int entityID /*= -1*/)
    {
        pipeline->Get_Shader(ShaderID)->SetMat4("Transform", transform);
        pipeline->Get_Shader(ShaderID)->SetFloat4("BaseColor", tintColor);
        pipeline->Get_Shader(ShaderID)->SetInt("EntityID", entityID);

        for(int i = 0; i < TexturesSize; i++)
        {
            textures[i]->Bind(i);
        }

        RenderCommand::DrawIndexed(s_Data.CubeVAO, 36);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer3D_NoBatch::DrawSprite(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, SpriteRendererComponent& src, int entityID /*= -1*/)
    {
        DrawCube(pipeline, ShaderID, transform, src.Textures, 5, src.Color, entityID);
    }

    void Renderer3D_NoBatch::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    Bubble::Renderer3D_NoBatch::Statistics Renderer3D_NoBatch::GetStats()
    {
        return  s_Data.Stats;
    }

}
