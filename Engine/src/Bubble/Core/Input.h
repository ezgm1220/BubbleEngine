#pragma once

#include <glm/glm.hpp>

#include "Bubble/Core/KeyCodes.h"
#include "Bubble/Core/MouseCodes.h"

namespace Bubble
{

    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);
        static bool IsMouseButtonReleased(MouseCode button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}
