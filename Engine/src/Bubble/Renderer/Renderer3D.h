#pragma once

#include "Bubble/Renderer/OrthographicCamera.h"

#include "Bubble/Renderer/Texture.h"

#include "Bubble/Camera/SceneCamera.h"
#include "Bubble/Camera/EditorCamera.h"

#include "Bubble/Scene/Components.h"

#include "Bubble/Pipeline/Pipeline.h"

namespace Bubble
{

    class Renderer3D
    {
    public:
        static void Init(Ref<Pipeline>pipeline);
        static void Shutdown();

        static void BeginScene(const SceneCamera& camera, const glm::mat4& transform, Ref<Pipeline>pipeline);
        static void BeginScene(const EditorCamera& camera, Ref<Pipeline>pipeline);
        static void EndScene(Ref<Pipeline>pipeline);
        static void Flush(Ref<Pipeline>pipeline);

        // Primitives
        //static void DrawCube(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        //static void DrawCube(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void DrawCube(const glm::mat4& transform, const glm::vec4& color, Ref<Pipeline>pipeline,int entityID = -1);
       // static void DrawCube(const glm::mat4& transform, const Ref<Texture2D>& texture,const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

        //static void DrawRotatedCube(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        //static void DrawRotatedCube(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        //static void DrawRotatedCube(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
        //static void DrawRotatedCube(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));

        //static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

        // Stats
        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t CubeCount = 0;

            uint32_t GetTotalVertexCount() const { return CubeCount * 8; }
            uint32_t GetTotalIndexCount() const { return CubeCount * 36; }
        };
        static void ResetStats();
        static Statistics GetStats();
    private:
        static void StartBatch();
        static void NextBatch(Ref<Pipeline>pipeline);
    };

}