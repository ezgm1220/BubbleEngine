#include "bubblepch.h"
#include "Renderer3D_NoBatch.h"
#include "Bubble/Tools/RenderApiState.h"
namespace Bubble
{

    namespace Render3DNoBatch
    {
        struct CubeVertexData
        {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 TexCoord;
        }; 
        struct SphereVertexData
        {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 TexCoord;
        };
        struct QuadVertexData
        {
            glm::vec3 Position;
            glm::vec2 TexCoord;
        };

        struct Renderer3DData
        {
            Ref<VertexArray>CubeVAO = nullptr;
            Ref<VertexArray>QuadVAO = nullptr;
            Ref<VertexArray>SphereVAO = nullptr;

            Ref<VertexBuffer>CubeVBO = nullptr;
            Ref<VertexBuffer>QuadVBO = nullptr; 
            Ref<VertexBuffer>SphereVBO = nullptr;

            unsigned int sphereIndexCount = 0;

            Renderer3D_NoBatch::Statistics Stats;
        };
    }

    static Render3DNoBatch::Renderer3DData s_Data;

    unsigned int mmap;

    void Renderer3D_NoBatch::Init()
    {
        Render3DNoBatch::CubeVertexData Cubevertexdata[36];
        {
            // back face
            Cubevertexdata[0].Position = {-1.0f, -1.0f, -1.0f};
            Cubevertexdata[1].Position = { 1.0f,  1.0f, -1.0f};
            Cubevertexdata[2].Position = { 1.0f, -1.0f, -1.0f};
            Cubevertexdata[3].Position = { 1.0f,  1.0f, -1.0f};
            Cubevertexdata[4].Position = {-1.0f, -1.0f, -1.0f};
            Cubevertexdata[5].Position = {-1.0f,  1.0f, -1.0f};
            // front face
            Cubevertexdata[6].Position =  {-1.0f, -1.0f,  1.0f};
            Cubevertexdata[7].Position =  { 1.0f, -1.0f,  1.0f};
            Cubevertexdata[8].Position =  { 1.0f,  1.0f,  1.0f};
            Cubevertexdata[9].Position =  { 1.0f,  1.0f,  1.0f};
            Cubevertexdata[10].Position = {-1.0f,  1.0f,  1.0f};
            Cubevertexdata[11].Position = {-1.0f, -1.0f,  1.0f};
            // left face
            Cubevertexdata[12].Position = {-1.0f,  1.0f,  1.0f};
            Cubevertexdata[13].Position = {-1.0f,  1.0f, -1.0f};
            Cubevertexdata[14].Position = {-1.0f, -1.0f, -1.0f};
            Cubevertexdata[15].Position = {-1.0f, -1.0f, -1.0f};
            Cubevertexdata[16].Position = {-1.0f, -1.0f,  1.0f};
            Cubevertexdata[17].Position = {-1.0f,  1.0f,  1.0f};
            // right face
            Cubevertexdata[18].Position = {1.0f,  1.0f,  1.0f};
            Cubevertexdata[19].Position = {1.0f, -1.0f, -1.0f}; 
            Cubevertexdata[20].Position = {1.0f,  1.0f, -1.0f};
            Cubevertexdata[21].Position = {1.0f, -1.0f, -1.0f};
            Cubevertexdata[22].Position = {1.0f,  1.0f,  1.0f};
            Cubevertexdata[23].Position = {1.0f, -1.0f,  1.0f};
            // bottom face
            Cubevertexdata[24].Position = {-1.0f, -1.0f, -1.0f};
            Cubevertexdata[25].Position = { 1.0f, -1.0f, -1.0f};
            Cubevertexdata[26].Position = { 1.0f, -1.0f,  1.0f};
            Cubevertexdata[27].Position = { 1.0f, -1.0f,  1.0f};
            Cubevertexdata[28].Position = {-1.0f, -1.0f,  1.0f};
            Cubevertexdata[29].Position = {-1.0f, -1.0f, -1.0f};
            // top face
            Cubevertexdata[30].Position = {-1.0f,  1.0f, -1.0f};
            Cubevertexdata[31].Position = { 1.0f,  1.0f , 1.0f};
            Cubevertexdata[32].Position = { 1.0f,  1.0f, -1.0f};
            Cubevertexdata[33].Position = { 1.0f,  1.0f,  1.0f};
            Cubevertexdata[34].Position = {-1.0f,  1.0f, -1.0f};
            Cubevertexdata[35].Position = {-1.0f,  1.0f,  1.0f};

            // Normal
            // back face
            Cubevertexdata[0]. Normal = { 0.0f,  0.0f, -1.0f};
            Cubevertexdata[1]. Normal = { 0.0f,  0.0f, -1.0f};
            Cubevertexdata[2]. Normal = { 0.0f,  0.0f, -1.0f};
            Cubevertexdata[3]. Normal = { 0.0f,  0.0f, -1.0f};
            Cubevertexdata[4]. Normal = { 0.0f,  0.0f, -1.0f};
            Cubevertexdata[5]. Normal = { 0.0f,  0.0f, -1.0f};
            // front face      Normal
            Cubevertexdata[6]. Normal = { 0.0f,  0.0f,  1.0f};
            Cubevertexdata[7]. Normal = { 0.0f,  0.0f,  1.0f};
            Cubevertexdata[8]. Normal = { 0.0f,  0.0f,  1.0f};
            Cubevertexdata[9]. Normal = { 0.0f,  0.0f,  1.0f};
            Cubevertexdata[10].Normal = { 0.0f,  0.0f,  1.0f};
            Cubevertexdata[11].Normal = { 0.0f,  0.0f,  1.0f};
            // left face       Normal
            Cubevertexdata[12].Normal = {-1.0f,  0.0f,  0.0f};
            Cubevertexdata[13].Normal = {-1.0f,  0.0f,  0.0f};
            Cubevertexdata[14].Normal = {-1.0f,  0.0f,  0.0f};
            Cubevertexdata[15].Normal = {-1.0f,  0.0f,  0.0f};
            Cubevertexdata[16].Normal = {-1.0f,  0.0f,  0.0f};
            Cubevertexdata[17].Normal = {-1.0f,  0.0f,  0.0f};
            // right face      Normal
            Cubevertexdata[18].Normal = { 1.0f,  0.0f,  0.0f};
            Cubevertexdata[19].Normal = { 1.0f,  0.0f,  0.0f};
            Cubevertexdata[20].Normal = { 1.0f,  0.0f,  0.0f};
            Cubevertexdata[21].Normal = { 1.0f,  0.0f,  0.0f};
            Cubevertexdata[22].Normal = { 1.0f,  0.0f,  0.0f};
            Cubevertexdata[23].Normal = { 1.0f,  0.0f,  0.0f};
            // bottom face     Normal
            Cubevertexdata[24].Normal = { 0.0f, -1.0f,  0.0f};
            Cubevertexdata[25].Normal = { 0.0f, -1.0f,  0.0f};
            Cubevertexdata[26].Normal = { 0.0f, -1.0f,  0.0f};
            Cubevertexdata[27].Normal = { 0.0f, -1.0f,  0.0f};
            Cubevertexdata[28].Normal = { 0.0f, -1.0f,  0.0f};
            Cubevertexdata[29].Normal = { 0.0f, -1.0f,  0.0f};
            // top face        Normal
            Cubevertexdata[30].Normal = { 0.0f,  1.0f,  0.0f};
            Cubevertexdata[31].Normal = { 0.0f,  1.0f,  0.0f};
            Cubevertexdata[32].Normal = { 0.0f,  1.0f,  0.0f};
            Cubevertexdata[33].Normal = { 0.0f,  1.0f,  0.0f};
            Cubevertexdata[34].Normal = { 0.0f,  1.0f,  0.0f};
            Cubevertexdata[35].Normal = { 0.0f,  1.0f,  0.0f};

            // Tex
            // back face
            Cubevertexdata[0].TexCoord = {0.0f, 0.0f};
            Cubevertexdata[1].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[2].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[3].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[4].TexCoord = {0.0f, 0.0f};
            Cubevertexdata[5].TexCoord = {0.0f, 1.0f};
            // front face      TexCoord     
            Cubevertexdata[6].TexCoord = {0.0f, 0.0f};
            Cubevertexdata[7].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[8].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[9].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[10].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[11].TexCoord = {0.0f, 0.0f};
            // left face       TexCoord     
            Cubevertexdata[12].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[13].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[14].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[15].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[16].TexCoord = {0.0f, 0.0f};
            Cubevertexdata[17].TexCoord = {1.0f, 0.0f};
            // right face      TexCoord     
            Cubevertexdata[18].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[19].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[20].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[21].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[22].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[23].TexCoord = {0.0f, 0.0f};
            // bottom face     TexCoord     
            Cubevertexdata[24].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[25].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[26].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[27].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[28].TexCoord = {0.0f, 0.0f};
            Cubevertexdata[29].TexCoord = {0.0f, 1.0f};
            // top face        TexCoord     
            Cubevertexdata[30].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[31].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[32].TexCoord = {1.0f, 1.0f};
            Cubevertexdata[33].TexCoord = {1.0f, 0.0f};
            Cubevertexdata[34].TexCoord = {0.0f, 1.0f};
            Cubevertexdata[35].TexCoord = {0.0f, 0.0f};
        }

        s_Data.CubeVAO = VertexArray::Create();

        s_Data.CubeVBO = VertexBuffer::Create(Cubevertexdata,36 * sizeof(Render3DNoBatch::CubeVertexData));

        s_Data.CubeVBO->SetLayout({
                { ShaderDataType::Float3, "a_Position"  },
                { ShaderDataType::Float3, "a_Normal"    },
                { ShaderDataType::Float2, "a_TexCoord"  }
            });
        s_Data.CubeVAO->AddVertexBuffer(s_Data.CubeVBO);

        //Ref<IndexBuffer> cubeIB = IndexBuffer::Create(CubeIndices, 36);// 创建EBO并绑定EBO数据
        //s_Data.CubeVAO->SetIndexBuffer(cubeIB);// 让VAO记录EBO的索引

        // 解绑
        s_Data.CubeVAO->Unbind();
        s_Data.CubeVBO->Unbind();;

        Render3DNoBatch::QuadVertexData Quadvertexdata[4];
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

        s_Data.QuadVBO = VertexBuffer::Create(Quadvertexdata, 4 * sizeof(Render3DNoBatch::QuadVertexData));

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


        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<uint32_t> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for(unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for(unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for(unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if(!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for(unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for(int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        s_Data.sphereIndexCount = indices.size();

        std::vector<float> data;
        for(unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if(normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if(uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        s_Data.SphereVAO = VertexArray::Create();
        s_Data.SphereVBO = VertexBuffer::Create(&data[0], data.size() * sizeof(float));
        s_Data.SphereVBO->SetLayout({
                { ShaderDataType::Float3, "a_Position"  },
                { ShaderDataType::Float3, "a_Normal"    },
                { ShaderDataType::Float2, "a_TexCoord"  }
            });
        s_Data.SphereVAO->AddVertexBuffer(s_Data.SphereVBO);
        Ref<IndexBuffer> sphIB = IndexBuffer::Create(&indices[0], indices.size());
        s_Data.SphereVAO->SetIndexBuffer(sphIB);

        s_Data.SphereVAO->Unbind();
        s_Data.SphereVBO->Unbind();
        sphIB->Unbind();
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
        pipeline->UnbindFramebuffer();

    }

    void Renderer3D_NoBatch::DrawQude()
    {
        RenderCommand::DrawIndexed(s_Data.QuadVAO, 6);
    }

    void Renderer3D_NoBatch::DrawCube(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, const glm::vec4& color, int entityID /*= -1*/)
    {
        //BB_CORE_INFO("Renderer3D_NoBatch::DrawCube()");
        pipeline->Get_Shader(ShaderID)->SetMat4("Transform", transform);
        pipeline->Get_Shader(ShaderID)->SetFloat4("BaseColor", color);
        pipeline->Get_Shader(ShaderID)->SetInt("EntityID", entityID);

        //RenderCommand::DrawIndexed(s_Data.CubeVAO, 36);
        RenderCommand::DrawArrays(s_Data.CubeVAO, 36);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer3D_NoBatch::DrawCube(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, const Ref<Texture2D>* textures, int TexturesSize, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/, int entityID /*= -1*/)
    {
        //BB_CORE_INFO("Renderer3D_NoBatch::DrawCube()");
        pipeline->Get_Shader(ShaderID)->SetMat4("Model", transform);
        pipeline->Get_Shader(ShaderID)->SetFloat4("BaseColor", tintColor);
        pipeline->Get_Shader(ShaderID)->SetInt("EntityID", entityID);
        pipeline->Get_Shader(ShaderID)->SetMat3("NormalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));

        for(int i = 0; i < TexturesSize; i++)
        {
            textures[i]->Bind(i);
        }

        //RenderCommand::DrawIndexed(s_Data.CubeVAO, 36);
        RenderCommand::DrawArrays(s_Data.CubeVAO, 36);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer3D_NoBatch::DrawCube()
    {
        //BB_CORE_INFO("Renderer3D_NoBatch::DrawCube()");
        //RenderCommand::DrawIndexed(s_Data.CubeVAO, 36);
        RenderCommand::DrawArrays(s_Data.CubeVAO, 36);
    }

    void Renderer3D_NoBatch::DrawSphere(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, const Ref<Texture2D>* textures, int TexturesSize, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/, int entityID /*= -1*/)
    {
        pipeline->Get_Shader(ShaderID)->SetMat4("Model", transform);
        pipeline->Get_Shader(ShaderID)->SetFloat4("BaseColor", tintColor);
        pipeline->Get_Shader(ShaderID)->SetInt("EntityID", entityID);
        pipeline->Get_Shader(ShaderID)->SetMat3("NormalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));

        for(int i = 0; i < TexturesSize; i++)
        {
            textures[i]->Bind(i);
        }

        RenderCommand::DrawIndexed_STRIP(s_Data.SphereVAO, s_Data.sphereIndexCount);
        //RenderCommand::DrawArrays(s_Data.SphereVAO, s_Data.sphereIndexCount);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer3D_NoBatch::DrawSprite(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, SpriteRendererComponent& src, int entityID /*= -1*/)
    {
        //BB_CORE_INFO("Renderer3D_NoBatch::DrawSprite");
        
        //DrawCube(pipeline, ShaderID, transform, src.Textures, 5, src.Color, entityID);
        DrawSphere(pipeline, ShaderID, transform, src.Textures, 5, src.Color, entityID);
        
    }

    void Renderer3D_NoBatch::Calculatelighting(const glm::vec3&CameraPos, Ref<Pipeline>pipeline)
    {
        pipeline->Calculatelighting_Begin();
        auto shader = pipeline->Calculatelighting(CameraPos);

        RenderCommand::DrawIndexed(s_Data.QuadVAO, 6);

        pipeline->Calculatelighting_End();
    }

    void Renderer3D_NoBatch::ShowSkyBox(Ref<Pipeline>pipeline, const glm::mat4& View, const glm::mat4& projection)
    {
        pipeline->ShowSkyBox_Begin();
        auto shader = pipeline->ShowSkyBox();
        shader->SetMat4("view", View);
        shader->SetMat4("projection", projection);
        //RenderCommand::Clear();
        //RenderCommand::DrawIndexed(s_Data.CubeVAO,36);
        RenderCommand::DrawArrays(s_Data.CubeVAO, 36);

        pipeline->ShowSkyBox_End();
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
