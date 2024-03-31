#include "bubblepch.h"
#include "Renderer3D_NoBatch.h"
#include "Bubble/Tools/RenderApiState.h"
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

    unsigned int mmap;

    void Renderer3D_NoBatch::Init()
    {
        Render3DNoBatch::VertexData Cubevertexdata[16];
        uint32_t CubeIndices[36];
        {
            Cubevertexdata[0].Position = {-1.0f, -1.0f,  1.0f};
            Cubevertexdata[1].Position = {1.0f, -1.0f,   1.0f};
            Cubevertexdata[2].Position = {1.0f,  1.0f,   1.0f};
            Cubevertexdata[3].Position = {-1.0f,  1.0f,  1.0f};
            Cubevertexdata[4].Position = {-1.0f, -1.0f, -1.0f};
            Cubevertexdata[5].Position = {1.0f, -1.0f,  -1.0f};
            Cubevertexdata[6].Position = {1.0f,  1.0f,  -1.0f};
            Cubevertexdata[7].Position = {-1.0f,  1.0f, -1.0f};
            Cubevertexdata[8].Position = {-1.0f, -1.0f,  1.0f};
            Cubevertexdata[9].Position = {1.0f, -1.0f,  1.0f};
            Cubevertexdata[10].Position = {1.0f,  1.0f,  1.0f};
            Cubevertexdata[11].Position = {-1.0f,  1.0f,  1.0f};
            Cubevertexdata[12].Position = {-1.0f, -1.0f, -1.0f};
            Cubevertexdata[13].Position = {1.0f, -1.0f, -1.0f};
            Cubevertexdata[14].Position = {1.0f,  1.0f, -1.0f};
            Cubevertexdata[15].Position = {-1.0f,  1.0f, -1.0f};

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
            CubeIndices[0] = 0;
            CubeIndices[1] = 1;
            CubeIndices[2] = 2;
            CubeIndices[3] = 0;
            CubeIndices[4] = 2;
            CubeIndices[5] = 3;

            //后面
            CubeIndices[6] = 5;
            CubeIndices[7] = 4;
            CubeIndices[8] = 7;
            CubeIndices[9] = 5;
            CubeIndices[10] = 7;
            CubeIndices[11] = 6;

                //左面
            CubeIndices[12] = 4;
            CubeIndices[13] = 0;
            CubeIndices[14] = 3;
            CubeIndices[15] = 4;
            CubeIndices[16] = 3;
            CubeIndices[17] = 7;

            // 右面
            CubeIndices[18] = 1;
            CubeIndices[19] = 5;
            CubeIndices[20] = 6;
            CubeIndices[21] = 1;
            CubeIndices[22] = 6;
            CubeIndices[23] = 2;

            // 上面
            CubeIndices[24] = 11;
            CubeIndices[25] = 10;
            CubeIndices[26] = 14;
            CubeIndices[27] = 11;
            CubeIndices[28] = 14;
            CubeIndices[29] = 15;

             // 下面
            CubeIndices[30] = 8;
            CubeIndices[31] = 9;
            CubeIndices[32] = 13;
            CubeIndices[33] = 8;
            CubeIndices[34] = 13;
            CubeIndices[35] = 12;
        }

        s_Data.CubeVAO = VertexArray::Create();

        s_Data.CubeVBO = VertexBuffer::Create(Cubevertexdata,16 * sizeof(Render3DNoBatch::VertexData));

        s_Data.CubeVBO->SetLayout({
                { ShaderDataType::Float3, "a_Position"     },
                { ShaderDataType::Float2, "a_TexCoord"     }
            });
        s_Data.CubeVAO->AddVertexBuffer(s_Data.CubeVBO);

        Ref<IndexBuffer> cubeIB = IndexBuffer::Create(CubeIndices, 36);// 创建EBO并绑定EBO数据
        s_Data.CubeVAO->SetIndexBuffer(cubeIB);// 让VAO记录EBO的索引

        // 解绑
        s_Data.CubeVAO->Unbind();
        s_Data.CubeVBO->Unbind();
        cubeIB->Unbind();

        Render3DNoBatch::VertexData Quadvertexdata[4];
        uint32_t QuadIndices[6];
        {
            Quadvertexdata[0].Position = {-1.f,-1.f,0.f};
            Quadvertexdata[1].Position = { 1.f,-1.f,0.f};
            Quadvertexdata[2].Position = { 1.f, 1.f,0.f};
            Quadvertexdata[3].Position = {-1.f, 1.f,0.f};

            Quadvertexdata[0].TexCoord = {0.f,0.f};
            Quadvertexdata[1].TexCoord = {1.f,0.f};
            Quadvertexdata[2].TexCoord = {1.f,1.f};
            Quadvertexdata[3].TexCoord = {0.f,1.f};

            QuadIndices[0] = 0;
            QuadIndices[1] = 1;
            QuadIndices[2] = 3;
            QuadIndices[3] = 1;
            QuadIndices[4] = 2;
            QuadIndices[5] = 3;
        }

        s_Data.QuadVAO = VertexArray::Create();

        s_Data.QuadVBO = VertexBuffer::Create(Quadvertexdata, 4 * sizeof(Render3DNoBatch::VertexData));

        s_Data.QuadVBO->SetLayout({
                { ShaderDataType::Float3, "a_Position"     },
                { ShaderDataType::Float2, "a_TexCoord"     }
            });
        s_Data.QuadVAO->AddVertexBuffer(s_Data.QuadVBO);

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(QuadIndices, 6);// 创建EBO并绑定EBO数据
        s_Data.QuadVAO->SetIndexBuffer(quadIB);// 让VAO记录EBO的索引

        // 解绑
        s_Data.QuadVAO->Unbind();
        s_Data.QuadVBO->Unbind();
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
        pipeline->BeginScene(camera);
    }

    void Renderer3D_NoBatch::EndScene(Ref<Pipeline>pipeline)
    {
        pipeline->UnbindFramebuffer();
        pipeline->EndScene();
    }

    void Renderer3D_NoBatch::ClearEntityID(Ref<Pipeline>pipeline)
    {
        pipeline->Get_Framebuffer(PID(GBufferFB))->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();
        pipeline->ClearEntityID();
    }

    void Renderer3D_NoBatch::DrawCube(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, const glm::vec4& color, int entityID /*= -1*/)
    {
        //BB_CORE_INFO("Renderer3D_NoBatch::DrawCube()");
        pipeline->Get_Shader(ShaderID)->SetMat4("Transform", transform);
        pipeline->Get_Shader(ShaderID)->SetFloat4("BaseColor", color);
        pipeline->Get_Shader(ShaderID)->SetInt("EntityID", entityID);

        RenderCommand::DrawIndexed(s_Data.CubeVAO, 36);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer3D_NoBatch::DrawCube(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, const Ref<Texture2D>* textures, int TexturesSize, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/, int entityID /*= -1*/)
    {
        //BB_CORE_INFO("Renderer3D_NoBatch::DrawCube()");
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

    void Renderer3D_NoBatch::DrawCube()
    {
        //BB_CORE_INFO("Renderer3D_NoBatch::DrawCube()");
        RenderCommand::DrawIndexed(s_Data.CubeVAO, 36);
    }

    void Renderer3D_NoBatch::DrawSprite(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, SpriteRendererComponent& src, int entityID /*= -1*/)
    {
        //BB_CORE_INFO("Renderer3D_NoBatch::DrawSprite");
        DrawCube(pipeline, ShaderID, transform, src.Textures, 5, src.Color, entityID);
    }

    void Renderer3D_NoBatch::Calculatelighting(Ref<Pipeline>pipeline)
    {
        pipeline->Calculatelighting_Begin();
        auto shader = pipeline->Calculatelighting();

        RenderCommand::DrawIndexed(s_Data.QuadVAO, 6);

        pipeline->Calculatelighting_End();
    }

    void Renderer3D_NoBatch::ShowSkyBox(Ref<Pipeline>pipeline, const glm::mat4& View, const glm::mat4& projection)
    {
        pipeline->ShowSkyBox_Begin();
        auto shader = pipeline->ShowSkyBox();
        shader->SetMat4("view", View);
        shader->SetMat4("projection", projection);

        RenderCommand::DrawIndexed(s_Data.CubeVAO,36);

        pipeline->ShowSkyBox_End();
        RenderCommand::Clear();
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
