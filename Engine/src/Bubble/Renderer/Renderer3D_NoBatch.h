#pragma once

#include "Bubble/Renderer/OrthographicCamera.h"

#include "Bubble/Renderer/Texture.h"

#include "Bubble/Camera/SceneCamera.h"
#include "Bubble/Camera/EditorCamera.h"

#include "Bubble/Scene/Components.h"

#include "Bubble/Pipeline/Pipeline.h"

namespace Bubble
{

    class Renderer3D_NoBatch
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const SceneCamera& camera, const glm::mat4& transform, Ref<Pipeline>pipeline);
        static void BeginScene(const EditorCamera& camera, Ref<Pipeline>pipeline);
        static void EndScene(Ref<Pipeline>pipeline);

        static void DrawCube(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
        static void DrawCube(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, const Ref<Texture2D>* textures, int TexturesSize, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

        static void DrawSprite(Ref<Pipeline>pipeline, int ShaderID, const glm::mat4& transform, SpriteRendererComponent& src, int entityID = -1);

        static void Calculatelighting(Ref<Pipeline>pipeline);

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
    };

}