#pragma once

#include "Bubble.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Bubble/Camera/EditorCamera.h"

#include "Bubble/Pipeline/Pipeline.h"
#include "Bubble/SkyBox/SkyBox.h"

namespace Bubble
{

    class EditorLayer : public Layer
    {
    public:
        EditorLayer(Ref<Pipeline> pipeline);
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        void OnEvent(Event& e) override;
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

        void NewScene();
        void OpenScene();
        void OpenScene(const std::filesystem::path& path);
        void SaveSceneAs();

        // 开始按钮
        void OnScenePlay();
        void OnSceneStop();

        // UI Panels
        void UI_Toolbar();
    private:
        Bubble::OrthographicCameraController m_CameraController;

        // Temp
        Ref<VertexArray> m_SquareVA;
        //Ref<Shader> m_FlatColorShader;
        //Ref<Framebuffer> m_Framebuffer;

        SkyBox m_SkyBox;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCamera;

        Entity m_HoveredEntity;

        bool m_PrimaryCamera = true;

        //Ref<Texture2D> m_CheckerboardTexture;

        EditorCamera m_EditorCamera;

        bool m_ViewportFocused = false, m_ViewportHovered = false;

        glm::vec2 m_ViewportSize = {0.0f, 0.0f};
        glm::vec2 m_ViewportBounds[2];

        glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};

        int m_GizmoType = -1;

        enum class SceneState
        {
            Edit = 0, Play = 1
        };
        SceneState m_SceneState = SceneState::Edit;

        // Editor resources
        Ref<Texture2D> m_IconPlay, m_IconStop;

        // 场景层次面板
        SceneHierarchyPanel m_SceneHierarchyPanel;
        // 资源管理器
        ContentBrowserPanel m_ContentBrowserPanel;
        // Pipeline
        Ref<Pipeline> m_pipeline;


    };

}