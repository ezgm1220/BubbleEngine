#include "bubblepch.h"
#include "Bubble/Renderer/Renderer3D.h"

#include "Bubble/Renderer/VertexArray.h"
#include "Bubble/Renderer/Shader.h"
#include "Bubble/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Bubble
{
    struct CubeVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;

        // Editor-only
        int EntityID;
    };

    struct Renderer3DData
    {
        static const uint32_t MaxCubes = 1000;
        static const uint32_t MaxVertices = MaxCubes * 8;
        static const uint32_t MaxIndices = MaxCubes * 36;
        static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

        Ref<VertexArray> CubeVertexArray;
        Ref<VertexBuffer> CubeVertexBuffer;
        //Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t CubeIndexCount = 0;
        CubeVertex* CubeVertexBufferBase = nullptr;
        CubeVertex* CubeVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = white texture

        glm::vec4 CubeVertexPositions[8];

        Renderer3D::Statistics Stats;
    };

    static Renderer3DData s_Data;

    void Renderer3D::Init(Ref<Pipeline>pipeline)
    {
        s_Data.CubeVertexArray = VertexArray::Create();

        s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CubeVertex));
        s_Data.CubeVertexBuffer->SetLayout({
                { ShaderDataType::Float3, "a_Position"     },
                { ShaderDataType::Float4, "a_Color"        },
                { ShaderDataType::Float2, "a_TexCoord"     },
                { ShaderDataType::Float,  "a_TexIndex"     },
                { ShaderDataType::Float,  "a_TilingFactor" },
                { ShaderDataType::Int,    "a_EntityID"     }
            });
        s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

        s_Data.CubeVertexBufferBase = new CubeVertex[s_Data.MaxVertices];

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

        uint32_t offset = 0;
        // 将所有的Index都预先填充上
        for(uint32_t i = 0; i < s_Data.MaxIndices; i += 36)
        {
            // 前面
            quadIndices[i +  0] = offset + 0;
            quadIndices[i +  1] = offset + 1;
            quadIndices[i +  2] = offset + 2;              
            quadIndices[i +  3] = offset + 0;
            quadIndices[i +  4] = offset + 2;
            quadIndices[i +  5] = offset + 3;
            
            // 下面
            quadIndices[i +  6] = offset + 0;
            quadIndices[i +  7] = offset + 1;
            quadIndices[i +  8] = offset + 5;
            quadIndices[i +  9] = offset + 0;
            quadIndices[i + 10] = offset + 5;
            quadIndices[i + 11] = offset + 4;

            // 右面
            quadIndices[i + 12] = offset + 1;
            quadIndices[i + 13] = offset + 5;
            quadIndices[i + 14] = offset + 6;
            quadIndices[i + 15] = offset + 1;
            quadIndices[i + 16] = offset + 6;
            quadIndices[i + 17] = offset + 2;

            //后面
            quadIndices[i + 18] = offset + 5;
            quadIndices[i + 19] = offset + 4;
            quadIndices[i + 20] = offset + 7;
            quadIndices[i + 21] = offset + 5;
            quadIndices[i + 22] = offset + 7;
            quadIndices[i + 23] = offset + 6;

            //左面
            quadIndices[i + 24] = offset + 4;
            quadIndices[i + 25] = offset + 0;
            quadIndices[i + 26] = offset + 3;
            quadIndices[i + 27] = offset + 4;
            quadIndices[i + 28] = offset + 3;
            quadIndices[i + 29] = offset + 7;

            // 上面
            quadIndices[i + 30] = offset + 3;
            quadIndices[i + 31] = offset + 5;
            quadIndices[i + 32] = offset + 6;
            quadIndices[i + 33] = offset + 3;
            quadIndices[i + 34] = offset + 6;
            quadIndices[i + 35] = offset + 7;

            offset += 8;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.CubeVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        for(uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;

        pipeline->Set_Shader("assets/shaders/Texture.glsl", 0);
        auto shader = pipeline->Get_Shader(0);
        shader->Bind();
        shader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
        //s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        //s_Data.TextureShader->Bind();
        //s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

        // Set first texture slot to 0
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        s_Data.CubeVertexPositions[0] = {-0.5f, -0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[1] = { 0.5f, -0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[2] = { 0.5f,  0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[3] = {-0.5f,  0.5f,  0.5f, 1.0f};
        s_Data.CubeVertexPositions[4] = {-0.5f, -0.5f, -0.5f, 1.0f};
        s_Data.CubeVertexPositions[5] = { 0.5f, -0.5f, -0.5f, 1.0f};
        s_Data.CubeVertexPositions[6] = { 0.5f,  0.5f, -0.5f, 1.0f};
        s_Data.CubeVertexPositions[7] = {-0.5f,  0.5f, -0.5f, 1.0f};
    }
    
    void Renderer3D::Shutdown()
    {
    }
    
    void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform, Ref<Pipeline>pipeline)
    {
    }
    
    void Renderer3D::BeginScene(const EditorCamera& camera, Ref<Pipeline>pipeline)
    {
        //glm::mat4 viewProj = camera.GetViewProjection();

        pipeline->BeginScene(camera);

       /* s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);*/

        StartBatch();
    }
    
    void Renderer3D::BeginScene(const OrthographicCamera& camera)
    {
    }
    
    void Renderer3D::EndScene(Ref<Pipeline>pipeline)
    {
        Flush(pipeline);
    }
    
    void Renderer3D::Flush(Ref<Pipeline>pipeline)
    {
        if(s_Data.CubeIndexCount == 0)
            return; // Nothing to draw

        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CubeVertexBufferPtr - (uint8_t*)s_Data.CubeVertexBufferBase);
        s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferBase, dataSize);

        // Bind textures
        for(uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);

        RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.CubeIndexCount);
        s_Data.Stats.DrawCalls++;
    }
    
    void Renderer3D::DrawCube(const glm::mat4& transform, const glm::vec4& color, Ref<Pipeline>pipeline, int entityID)
    {
        constexpr size_t quadVertexCount = 8;
        const float textureIndex = 0.0f; // White Texture
        constexpr glm::vec2 textureCoords[] = {{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },
                                               { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }};
        const float tilingFactor = 1.0f;

        if(s_Data.CubeIndexCount >= Renderer3DData::MaxIndices)
            NextBatch(pipeline);

        for(size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[i];
            s_Data.CubeVertexBufferPtr->Color = color;
            s_Data.CubeVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
            s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
            s_Data.CubeVertexBufferPtr->EntityID = entityID;
            s_Data.CubeVertexBufferPtr++;
        }

        s_Data.CubeIndexCount += 36;

        s_Data.Stats.CubeCount++;
    }
    
    void Renderer3D::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    Renderer3D::Statistics Renderer3D::GetStats()
    {
        return  s_Data.Stats;
    }
    
    void Renderer3D::StartBatch()
    {
        s_Data.CubeIndexCount = 0;
        s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

        s_Data.TextureSlotIndex = 1;
    }
    
    void Renderer3D::NextBatch(Ref<Pipeline>pipeline)
    {
        Flush(pipeline);
        StartBatch();
    }
}