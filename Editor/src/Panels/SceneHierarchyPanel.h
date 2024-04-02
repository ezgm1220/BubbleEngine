#pragma once

#include "Bubble/Core/Base.h"
#include "Bubble/Scene/Scene.h"
#include "Bubble/Scene/Entity.h"

namespace Bubble
{

    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void OnImGuiRender();

        Entity GetSelectedEntity() const { return m_SelectionContext; }

        void SetSelectedEntity(Entity entity);
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);
    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
    void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
}