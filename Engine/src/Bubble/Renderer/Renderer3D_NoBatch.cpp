#include "bubblepch.h"
#include "Renderer3D_NoBatch.h"

namespace Bubble
{

    namespace Render3DNoBatch
    {
        struct CubeVertex
        {
            glm::vec3 Position;
            glm::vec4 Color;
            glm::vec2 TexCoord;
            // Editor-only
            int EntityID;
        };

        struct Renderer3DData
        {
            static const uint32_t VerticesSize = 16;
            static const uint32_t IndicesSize = 36;

            Ref<VertexArray> CubeVertexArray;
            Ref<VertexBuffer> CubeVertexBuffer;

            std::array< CubeVertex, 16>CubeVertexBufferData;

            glm::vec4 CubeVertexPositions[16];
            glm::vec2 CubeTextureCoords[16];

            Renderer3D_NoBatch::Statistics Stats;
        };
    }

    static Render3DNoBatch::Renderer3DData s_Data;

    void Renderer3D_NoBatch::Init()
    {
        s_Data.CubeVertexArray = VertexArray::Create();

        s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.VerticesSize * sizeof(Render3DNoBatch::CubeVertex));
        s_Data.CubeVertexBuffer->SetLayout({
                { ShaderDataType::Float3, "a_Position"     },
                { ShaderDataType::Float4, "a_Color"        },
                { ShaderDataType::Float2, "a_TexCoord"     },
                { ShaderDataType::Int,    "a_EntityID"     }
            });
        s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

        uint32_t* quadIndices = new uint32_t[s_Data.IndicesSize];

        uint32_t offset = 0;
        // 将所有的Index都预先填充上
        for(uint32_t i = 0; i < s_Data.IndicesSize; i += 36)
        {
            // 前面
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 0;
            quadIndices[i + 4] = offset + 2;
            quadIndices[i + 5] = offset + 3;

            //后面
            quadIndices[i + 6] = offset + 5;
            quadIndices[i + 7] = offset + 4;
            quadIndices[i + 8] = offset + 7;
            quadIndices[i + 9] = offset + 5;
            quadIndices[i + 10] = offset + 7;
            quadIndices[i + 11] = offset + 6;

                //左面
            quadIndices[i + 12] = offset + 4;
            quadIndices[i + 13] = offset + 0;
            quadIndices[i + 14] = offset + 3;
            quadIndices[i + 15] = offset + 4;
            quadIndices[i + 16] = offset + 3;
            quadIndices[i + 17] = offset + 7;

            // 右面
            quadIndices[i + 18] = offset + 1;
            quadIndices[i + 19] = offset + 5;
            quadIndices[i + 20] = offset + 6;
            quadIndices[i + 21] = offset + 1;
            quadIndices[i + 22] = offset + 6;
            quadIndices[i + 23] = offset + 2;

            // 上面
            quadIndices[i + 24] = offset + 11;
            quadIndices[i + 25] = offset + 10;
            quadIndices[i + 26] = offset + 14;
            quadIndices[i + 27] = offset + 11;
            quadIndices[i + 28] = offset + 14;
            quadIndices[i + 29] = offset + 15;

             // 下面
            quadIndices[i + 30] = offset + 8;
            quadIndices[i + 31] = offset + 9;
            quadIndices[i + 32] = offset + 13;
            quadIndices[i + 33] = offset + 8;
            quadIndices[i + 34] = offset + 13;
            quadIndices[i + 35] = offset + 12;

            offset += 16;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.IndicesSize);
        s_Data.CubeVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;
       

        s_Data.CubeVertexPositions[0] = {-0.5f, -0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[1] = {0.5f, -0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[2] = {0.5f,  0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[3] = {-0.5f,  0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[4] = {-0.5f, -0.5f, -0.5f, 1.0f};
        s_Data.CubeVertexPositions[5] = {0.5f, -0.5f, -0.5f, 1.0f};
        s_Data.CubeVertexPositions[6] = {0.5f,  0.5f, -0.5f, 1.0f};
        s_Data.CubeVertexPositions[7] = {-0.5f,  0.5f, -0.5f, 1.0f};
        s_Data.CubeVertexPositions[8] = {-0.5f, -0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[9] = {0.5f, -0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[10] = {0.5f,  0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[11] = {-0.5f,  0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[12] = {-0.5f, -0.5f, -0.5f, 1.0f};
        s_Data.CubeVertexPositions[13] = {0.5f, -0.5f, -0.5f, 1.0f};
        s_Data.CubeVertexPositions[14] = {0.5f,  0.5f, -0.5f, 1.0f};
        s_Data.CubeVertexPositions[15] = {-0.5f,  0.5f, -0.5f, 1.0f};

        s_Data.CubeTextureCoords[0] = {0.0f, 0.0f};
        s_Data.CubeTextureCoords[1] = {1.0f, 0.0f};
        s_Data.CubeTextureCoords[2] = {1.0f, 1.0f};
        s_Data.CubeTextureCoords[3] = {0.0f, 1.0f};
        s_Data.CubeTextureCoords[4] = {1.0f, 0.0f};
        s_Data.CubeTextureCoords[5] = {0.0f, 0.0f};
        s_Data.CubeTextureCoords[6] = {0.0f, 1.0f};
        s_Data.CubeTextureCoords[7] = {1.0f, 1.0f};
        s_Data.CubeTextureCoords[8] = {0.0f, 0.0f};
        s_Data.CubeTextureCoords[9] = {1.0f, 0.0f};
        s_Data.CubeTextureCoords[10] = {1.0f, 0.0f};
        s_Data.CubeTextureCoords[11] = {0.0f, 0.0f};
        s_Data.CubeTextureCoords[12] = {0.0f, 1.0f};
        s_Data.CubeTextureCoords[13] = {1.0f, 1.0f};
        s_Data.CubeTextureCoords[14] = {1.0f, 1.0f};
        s_Data.CubeTextureCoords[15] = {0.0f, 1.0f};

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
        pipeline->Get_Framebuffer(PID(GBuffer))->ClearAttachment(1, -1);
        pipeline->BeginScene(camera);
    }

    void Renderer3D_NoBatch::EndScene(Ref<Pipeline>pipeline)
    {
        
    }

    void Renderer3D_NoBatch::DrawCube(Ref<Pipeline>pipeline, const glm::mat4& transform, const glm::vec4& color, int entityID /*= -1*/)
    {
        for(size_t i = 0; i < s_Data.VerticesSize; i++)
        {
            s_Data.CubeVertexBufferData[i].Position = transform * s_Data.CubeVertexPositions[i];
            s_Data.CubeVertexBufferData[i].Color = color;
            s_Data.CubeVertexBufferData[i].TexCoord = s_Data.CubeTextureCoords[i];
            s_Data.CubeVertexBufferData[i].EntityID = entityID;
        }
        s_Data.Stats.CubeCount++;

        uint32_t dataSize = (uint32_t)(sizeof(Render3DNoBatch::CubeVertex)*s_Data.VerticesSize);
        s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferData.data(), dataSize);

        RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.IndicesSize);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer3D_NoBatch::DrawCube(Ref<Pipeline>pipeline, const glm::mat4& transform, const Ref<Texture2D>* textures, int TexturesSize, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/, int entityID /*= -1*/)
    {
        for(size_t i = 0; i < s_Data.VerticesSize; i++)
        {
            s_Data.CubeVertexBufferData[i].Position = transform * s_Data.CubeVertexPositions[i];
            s_Data.CubeVertexBufferData[i].Color = tintColor;
            s_Data.CubeVertexBufferData[i].TexCoord = s_Data.CubeTextureCoords[i];
            s_Data.CubeVertexBufferData[i].EntityID = entityID;
        }
        s_Data.Stats.CubeCount++;

        for(int i = 0; i < TexturesSize; i++)
        {
            textures[i]->Bind(i);
        }

        uint32_t dataSize = (uint32_t)(sizeof(Render3DNoBatch::CubeVertex) * s_Data.VerticesSize);
        s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferData.data(), dataSize);

        RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.IndicesSize);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer3D_NoBatch::DrawSprite(Ref<Pipeline>pipeline, const glm::mat4& transform, SpriteRendererComponent& src, int entityID /*= -1*/)
    {
        DrawCube(pipeline, transform, src.Textures,5, src.Color, entityID);
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
