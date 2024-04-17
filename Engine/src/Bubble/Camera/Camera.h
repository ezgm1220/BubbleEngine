#pragma once

#include <glm/glm.hpp>

namespace Bubble
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection)
            : m_Projection(projection)
        {
        }

        virtual ~Camera() = default;

        const glm::mat4& GetProjection() const { return m_Projection; }
        const glm::mat4& GetViewMatric() const { return m_ViewMatrix; }
    protected:
        glm::mat4 m_Projection = glm::mat4(1.0f);
        glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    };
}