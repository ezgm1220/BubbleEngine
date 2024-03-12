#include "EditorLayer.h"
#include <imgui/imgui.h>

namespace Bubble
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({0.2f, 0.3f, 0.8f, 1.0f})
    {
    }
    void EditorLayer::OnAttach()
    {
    }
    void EditorLayer::OnDetach()
    {
    }
    void EditorLayer::OnUpdate(Timestep ts)
    {
        //BB_INFO("In EditorLayer");
        if(Input::IsKeyPressed(Key::A))
        {
            BB_INFO("AAAAAA");
        }
        RenderCommand::SetClearColor({227.0f / 256.0f, 180.0f / 256.0f, 184.0f / 256.0f, 1});
        RenderCommand::Clear();

    }
    void EditorLayer::OnImGuiRender()
    {
        // Note: Switch this to true to enable dockspace
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        //ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        //if(opt_fullscreen)
        //{
        //    ImGuiViewport* viewport = ImGui::GetMainViewport();
        //    ImGui::SetNextWindowPos(viewport->Pos);
        //    ImGui::SetNextWindowSize(viewport->Size);
        //    ImGui::SetNextWindowViewport(viewport->ID);
        //    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        //    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        //    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        //    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        //}

        //// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        //if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        //    window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        /*ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();*/

       // if(opt_fullscreen)
       //     ImGui::PopStyleVar(2);

       // // DockSpace
       // ImGuiIO& io = ImGui::GetIO();
       // ImGuiStyle& style = ImGui::GetStyle();
       // float minWinSizeX = style.WindowMinSize.x;
       // style.WindowMinSize.x = 370.0f;

       // if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
       // {
       //     ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
       //     ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
       // }

       // // 设置窗口最小大小
       // style.WindowMinSize.x = minWinSizeX;

       // if(ImGui::BeginMenuBar())
       // {
       //     if(ImGui::BeginMenu("File"))
       //     {
       //         // Disabling fullscreen would allow the window to be moved to the front of other windows, 
       //         // which we can't undo at the moment without finer window depth/z control.
       //         //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

       //         if(ImGui::MenuItem("New", "Ctrl+N"))
       //             //NewScene();

       //         if(ImGui::MenuItem("Open...", "Ctrl+O"))
       //             //OpenScene();

       //         if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
       //             //SaveSceneAs();

       //         if(ImGui::MenuItem("Exit")) Application::Get().Close();
       //         ImGui::EndMenu();
       //     }

       //     ImGui::EndMenuBar();
       // }

       //// m_SceneHierarchyPanel.OnImGuiRender();// 渲染场景UI

       // ImGui::Begin("Stats");

       // std::string name = "None";
       // /*if(m_HoveredEntity)
       //     name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
       // ImGui::Text("Hovered Entity: %s", name.c_str());*/

       // auto stats = Renderer2D::GetStats();
       // ImGui::Text("Renderer2D Stats:");
       // ImGui::Text("Draw Calls: %d", stats.DrawCalls);
       // ImGui::Text("Quads: %d", stats.QuadCount);
       // ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
       // ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

       // ImGui::End();

       // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
       // ImGui::Begin("Viewport");

       // auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
       // auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
       // auto viewportOffset = ImGui::GetWindowPos();
       // m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
       // m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

       // m_ViewportFocused = ImGui::IsWindowFocused();
       // m_ViewportHovered = ImGui::IsWindowHovered();
       // Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

       // ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
       // m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

       // uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
       // ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

        
        /*ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();*/
    }
    void EditorLayer::OnEvent(Event& e)
    {
    }
    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        return false;
    }
    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        
        return false;
    }
}

