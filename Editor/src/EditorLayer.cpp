#include "EditorLayer.h"

namespace Bubble
{
    EditorLayer::EditorLayer()
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
    }
    void EditorLayer::OnImGuiRender()
    {
    }
    void EditorLayer::OnEvent(Event& e)
    {
    }
}

