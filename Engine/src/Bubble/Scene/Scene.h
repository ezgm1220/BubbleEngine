#pragma once

#include "Bubble/Core/Timestep.h"
#include "Bubble/Core/UUID.h"
#include "Bubble/Camera/EditorCamera.h"

#include "Bubble/Pipeline/Pipeline.h"
#include "entt.hpp"

namespace Bubble
{

    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdateRuntime(Timestep ts, Ref<Pipeline> pipeline);
        void OnUpdateEditor(Timestep ts, EditorCamera& camera,Ref<Pipeline> pipeline);
        void OnViewportResize(uint32_t width, uint32_t height);

        Entity GetPrimaryCameraEntity();
    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };

}