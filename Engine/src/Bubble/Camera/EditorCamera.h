#pragma once

#include "Bubble/Camera/Camera.h"
#include "Bubble/Core/Timestep.h"
#include "Bubble/Events/Event.h"
#include "Bubble/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Bubble
{
    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        inline float GetDistance() const { return mDistance; }
        inline void SetDistance(float distance) { mDistance = distance; }

        inline void SetViewportSize(float width, float height) { mViewportWidth = width; mViewportHeight = height; UpdateProjection(); }

        const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
        glm::mat4 GetViewProjection() const { return mProjection * mViewMatrix; }

        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;
        const glm::vec3& GetPosition() const { return mPosition; }
        glm::quat GetOrientation() const;

        float GetPitch() const { return mPitch; }
        float GetYaw() const { return mYaw; }

        float GetNearPlane() const { return mNearClip; }
        float GetFarPlane() const { return mFarClip; }

        float GetFOV() const { return mFOV; }
        float GetAspect() const { return mAspectRatio; }

        void SetCenter(const glm::vec3& center);
    private:
        void UpdateProjection();
        void UpdateView();

        bool OnMouseScroll(MouseScrolledEvent& e);

        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        glm::vec3 CalculatePosition() const;

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;
    public:
        float mCameraSpeed = 1.0f;
    private:
        float mFOV = 45.0f, mAspectRatio = 1.778f, mNearClip = 0.1f, mFarClip = 1000.0f;

        glm::mat4 mViewMatrix;
        glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
        glm::vec3 mFocalPoint = {0.0f, 0.0f, 0.0f};

        glm::vec2 mInitialMousePosition = {0.0f, 0.0f};

        float mDistance = 10.0f;
        float mPitch = 0.0f, mYaw = 0.0f;

        float mViewportWidth = 1280, mViewportHeight = 720;

        bool bInit = true;
    };
}
