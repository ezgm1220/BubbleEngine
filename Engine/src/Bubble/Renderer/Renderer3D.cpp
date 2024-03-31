#include "bubblepch.h"
//#include "Bubble/Renderer/Renderer3D.h"
//
//#include "Bubble/Renderer/VertexArray.h"
//#include "Bubble/Renderer/Shader.h"
//#include "Bubble/Renderer/RenderCommand.h"
//
//#include <glm/gtc/matrix_transform.hpp>
//
//namespace Bubble
//{
//    namespace Render3DUseBatch
//    {
//        struct CubeVertex
//        {
//            glm::vec3 Position;
//            glm::vec4 Color;
//            glm::vec2 TexCoord;
//            float TexIndex;
//            float TilingFactor;
//
//            // Editor-only
//            int EntityID;
//        };
//
//        struct Renderer3DData
//        {
//            static const uint32_t MaxCubes = 500;
//            static const uint32_t MaxVertices = MaxCubes * 16;
//            static const uint32_t MaxIndices = MaxCubes * 36;
//            static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
//
//            Ref<VertexArray> CubeVertexArray;
//            Ref<VertexBuffer> CubeVertexBuffer;
//            //Ref<Shader> TextureShader;
//            Ref<Texture2D> WhiteTexture;
//
//            uint32_t CubeIndexCount = 0;
//            CubeVertex* CubeVertexBufferBase = nullptr;
//            CubeVertex* CubeVertexBufferPtr = nullptr;
//
//            std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
//            uint32_t TextureSlotIndex = 1; // 0 = white texture
//
//            glm::vec4 CubeVertexPositions[16];
//
//            Renderer3D::Statistics Stats;
//        };
//    }
//    
//
//    static Render3DUseBatch::Renderer3DData s_Data;
//
//    void Renderer3D::Init(Ref<Pipeline>pipeline)
//    {
//        s_Data.CubeVertexArray = VertexArray::Create();
//
//        s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(Render3DUseBatch::CubeVertex));
//        s_Data.CubeVertexBuffer->SetLayout({
//                { ShaderDataType::Float3, "a_Position"     },
//                { ShaderDataType::Float4, "a_Color"        },
//                { ShaderDataType::Float2, "a_TexCoord"     },
//                { ShaderDataType::Float,  "a_TexIndex"     },
//                { ShaderDataType::Float,  "a_TilingFactor" },
//                { ShaderDataType::Int,    "a_EntityID"     }
//            });
//        s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);
//
//        s_Data.CubeVertexBufferBase = new Render3DUseBatch::CubeVertex[s_Data.MaxVertices];
//
//        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
//
//        uint32_t offset = 0;
//        // 将所有的Index都预先填充上
//        for(uint32_t i = 0; i < s_Data.MaxIndices; i += 36)
//        {
//            // 前面
//            quadIndices[i + 0] = offset + 0;
//            quadIndices[i + 1] = offset + 1;
//            quadIndices[i + 2] = offset + 2;
//            quadIndices[i + 3] = offset + 0;
//            quadIndices[i + 4] = offset + 2;
//            quadIndices[i + 5] = offset + 3;
//
//            //后面
//            quadIndices[i + 6 ] = offset + 5;
//            quadIndices[i + 7 ] = offset + 4;
//            quadIndices[i + 8 ] = offset + 7;
//            quadIndices[i + 9 ] = offset + 5;
//            quadIndices[i + 10] = offset + 7;
//            quadIndices[i + 11] = offset + 6;
//
//                //左面
//            quadIndices[i + 12] = offset + 4;
//            quadIndices[i + 13] = offset + 0;
//            quadIndices[i + 14] = offset + 3;
//            quadIndices[i + 15] = offset + 4;
//            quadIndices[i + 16] = offset + 3;
//            quadIndices[i + 17] = offset + 7;
//
//            // 右面
//            quadIndices[i +  18] = offset + 1;
//            quadIndices[i +  19] = offset + 5;
//            quadIndices[i +  20] = offset + 6;
//            quadIndices[i +  21] = offset + 1;
//            quadIndices[i +  22] = offset + 6;
//            quadIndices[i +  23] = offset + 2;
//
//            // 上面
//            quadIndices[i + 24] = offset + 11;
//            quadIndices[i + 25] = offset + 10;
//            quadIndices[i + 26] = offset + 14;
//            quadIndices[i + 27] = offset + 11;
//            quadIndices[i + 28] = offset + 14;
//            quadIndices[i + 29] = offset + 15;
//
//             // 下面
//            quadIndices[i + 30] = offset + 8;   
//            quadIndices[i + 31] = offset + 9;   
//            quadIndices[i + 32] = offset + 13;   
//            quadIndices[i + 33] = offset + 8;   
//            quadIndices[i + 34] = offset + 13;   
//            quadIndices[i + 35] = offset + 12;   
//
//            offset += 16;
//        }
//
//        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
//        s_Data.CubeVertexArray->SetIndexBuffer(quadIB);
//        delete[] quadIndices;
//
//        s_Data.WhiteTexture = Texture2D::Create(1, 1);
//        uint32_t whiteTextureData = 0xffffffff;
//        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
//
//        int32_t samplers[s_Data.MaxTextureSlots];
//        for(uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
//            samplers[i] = i;
//
//        pipeline->Set_Shader("assets/shaders/Texture.glsl", 0);
//        auto shader = pipeline->Get_Shader(0);
//        shader->Bind();
//        shader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
//        //s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
//        //s_Data.TextureShader->Bind();
//        //s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
//
//        // Set first texture slot to 0
//        s_Data.TextureSlots[0] = s_Data.WhiteTexture;
//
//        s_Data.CubeVertexPositions[0] = {-0.5f, -0.5f,  0.5f, 1.0f};
//        s_Data.CubeVertexPositions[1] = { 0.5f, -0.5f,  0.5f, 1.0f};
//        s_Data.CubeVertexPositions[2] = { 0.5f,  0.5f,  0.5f, 1.0f};
//        s_Data.CubeVertexPositions[3] = {-0.5f,  0.5f,  0.5f, 1.0f};
//        s_Data.CubeVertexPositions[4] = {-0.5f, -0.5f, -0.5f, 1.0f};
//        s_Data.CubeVertexPositions[5] = { 0.5f, -0.5f, -0.5f, 1.0f};
//        s_Data.CubeVertexPositions[6] = { 0.5f,  0.5f, -0.5f, 1.0f};
//        s_Data.CubeVertexPositions[7] = {-0.5f,  0.5f, -0.5f, 1.0f};
//        s_Data.CubeVertexPositions[8] = {-0.5f, -0.5f,  0.5f, 1.0f};
//        s_Data.CubeVertexPositions[9] = {0.5f, -0.5f,  0.5f, 1.0f};
//        s_Data.CubeVertexPositions[10] = {0.5f,  0.5f,  0.5f, 1.0f};
//        s_Data.CubeVertexPositions[11] = {-0.5f,  0.5f,  0.5f, 1.0f};
//        s_Data.CubeVertexPositions[12] = {-0.5f, -0.5f, -0.5f, 1.0f};
//        s_Data.CubeVertexPositions[13] = {0.5f, -0.5f, -0.5f, 1.0f};
//        s_Data.CubeVertexPositions[14] = {0.5f,  0.5f, -0.5f, 1.0f};
//        s_Data.CubeVertexPositions[15] = {-0.5f,  0.5f, -0.5f, 1.0f};
//    }
//    
//    void Renderer3D::Shutdown()
//    {
//    }
//    
//    void Renderer3D::BeginScene(const SceneCamera& camera, const glm::mat4& transform, Ref<Pipeline>pipeline)
//    {
//        //BB_CORE_INFO("mincamera: {}",camera.GetProjection());
//        pipeline->BeginScene(camera,transform);
//        StartBatch();
//    }
//    
//    void Renderer3D::BeginScene(const EditorCamera& camera, Ref<Pipeline>pipeline)
//    {
//        //glm::mat4 viewProj = camera.GetViewProjection();
//
//        pipeline->BeginScene(camera);
//
//       /* s_Data.TextureShader->Bind();
//        s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);*/
//
//        StartBatch();
//    }
//    
//    void Renderer3D::EndScene(Ref<Pipeline>pipeline)
//    {
//        Flush(pipeline);
//    }
//    
//    void Renderer3D::Flush(Ref<Pipeline>pipeline)
//    {
//        if(s_Data.CubeIndexCount == 0)
//            return; // Nothing to draw
//
//        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CubeVertexBufferPtr - (uint8_t*)s_Data.CubeVertexBufferBase);
//        s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferBase, dataSize);
//
//        // Bind textures
//        for(uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
//            s_Data.TextureSlots[i]->Bind(i);
//
//        RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.CubeIndexCount);
//        s_Data.Stats.DrawCalls++;
//    }
//    
//    void Renderer3D::DrawCube(Ref<Pipeline>pipeline, const glm::mat4& transform, const glm::vec4& color, int entityID)
//    {
//        constexpr size_t quadVertexCount = 16;
//        const float textureIndex = 0.0f; // White Texture
//        constexpr glm::vec2 textureCoords[] = {{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },
//                                               { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
//                                               { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
//                                               { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }};
//        const float tilingFactor = 1.0f;
//
//        if(s_Data.CubeIndexCount >= Render3DUseBatch::Renderer3DData::MaxIndices)
//            NextBatch(pipeline);
//
//        for(size_t i = 0; i < quadVertexCount; i++)
//        {
//            s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[i];
//            s_Data.CubeVertexBufferPtr->Color = color;
//            s_Data.CubeVertexBufferPtr->TexCoord = textureCoords[i];
//            s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
//            s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
//            s_Data.CubeVertexBufferPtr->EntityID = entityID;
//            s_Data.CubeVertexBufferPtr++;
//        }
//
//        s_Data.CubeIndexCount += 36;
//
//        s_Data.Stats.CubeCount++;
//
//        BB_CORE_INFO("Renderer3D::DrawCube");
//    } 
//
//    void Renderer3D::DrawCube(Ref<Pipeline>pipeline, const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/, int entityID /*= -1*/)
//    {
//        constexpr size_t quadVertexCount = 16;
//        constexpr glm::vec2 textureCoords[] = {{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },
//                                               { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
//                                               { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
//                                               { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }};
//
//        if(s_Data.CubeIndexCount >= Render3DUseBatch::Renderer3DData::MaxIndices)
//            NextBatch(pipeline);
//
//        float textureIndex = 0.0f;
//        for(uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
//        {
//            if(*s_Data.TextureSlots[i] == *texture)
//            {
//                textureIndex = (float)i;
//                break;
//            }
//        }
//
//        if(textureIndex == 0.0f)
//        {
//            if(s_Data.TextureSlotIndex >= Render3DUseBatch::Renderer3DData::MaxTextureSlots)
//                NextBatch(pipeline);
//
//            textureIndex = (float)s_Data.TextureSlotIndex;
//            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
//            s_Data.TextureSlotIndex++;
//        }
//
//        for(size_t i = 0; i < quadVertexCount; i++)
//        {
//            s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[i];
//            s_Data.CubeVertexBufferPtr->Color = tintColor;
//            s_Data.CubeVertexBufferPtr->TexCoord = textureCoords[i];
//            s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
//            s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
//            s_Data.CubeVertexBufferPtr->EntityID = entityID;
//            s_Data.CubeVertexBufferPtr++;
//        }
//
//        s_Data.CubeIndexCount += 36;
//
//        s_Data.Stats.CubeCount++;
//
//    }
//
//    void Renderer3D::DrawCube(Ref<Pipeline>pipeline, const glm::mat4& transform, const Ref<Texture2D>* textures, int TexturesSize, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/, int entityID /*= -1*/)
//    {
//        /*constexpr size_t quadVertexCount = 16;
//        constexpr glm::vec2 textureCoords[] = {{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },
//                                               { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
//                                               { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
//                                               { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }};
//
//        if(s_Data.CubeIndexCount >= Renderer3DData::MaxIndices)
//            NextBatch(pipeline);
//
//        float textureIndex = 0.0f;
//        for(uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
//        {
//            if(*s_Data.TextureSlots[i] == *texture)
//            {
//                textureIndex = (float)i;
//                break;
//            }
//        }
//
//        if(textureIndex == 0.0f)
//        {
//            if(s_Data.TextureSlotIndex >= Renderer3DData::MaxTextureSlots)
//                NextBatch(pipeline);
//
//            textureIndex = (float)s_Data.TextureSlotIndex;
//            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
//            s_Data.TextureSlotIndex++;
//        }
//
//        for(size_t i = 0; i < quadVertexCount; i++)
//        {
//            s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[i];
//            s_Data.CubeVertexBufferPtr->Color = tintColor;
//            s_Data.CubeVertexBufferPtr->TexCoord = textureCoords[i];
//            s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
//            s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
//            s_Data.CubeVertexBufferPtr->EntityID = entityID;
//            s_Data.CubeVertexBufferPtr++;
//        }
//
//        s_Data.CubeIndexCount += 36;
//
//        s_Data.Stats.CubeCount++;*/
//    }
//
//    void Renderer3D::DrawSprite(Ref<Pipeline>pipeline, const glm::mat4& transform, SpriteRendererComponent& src, int entityID/*=-1*/)
//    {
//        BB_CORE_INFO("Renderer3D::DrawCube");
//        if(src.UseTexture)
//        {
//            //DrawCube(pipeline,transform, src.Textures,4, src.TilingFactor, src.Color, entityID);
//            DrawCube(pipeline,transform, src.Textures[2],src.TilingFactor, src.Color, entityID);
//        }
//        else
//            DrawCube(pipeline,transform, src.Color, entityID);
//    }
//
//    void Renderer3D::ResetStats()
//    {
//        memset(&s_Data.Stats, 0, sizeof(Statistics));
//    }
//
//    Renderer3D::Statistics Renderer3D::GetStats()
//    {
//        return  s_Data.Stats;
//    }
//    
//    void Renderer3D::StartBatch()
//    {
//        s_Data.CubeIndexCount = 0;
//        s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;
//
//        s_Data.TextureSlotIndex = 1;
//    }
//    
//    void Renderer3D::NextBatch(Ref<Pipeline>pipeline)
//    {
//        Flush(pipeline);
//        StartBatch();
//    }
//}