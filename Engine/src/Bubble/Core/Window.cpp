#include "bubblepch.h"
#include "Bubble/Core/Window.h"

#include "Platform/Windows/WindowsWindow.h"


namespace Bubble
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
        return CreateScope<WindowsWindow>(props);
    }

}