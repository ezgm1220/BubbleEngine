#include "bubblepch.h"
#include "Bubble/Scene/Scene.h"

#include "Bubble/Scene/Components.h"
#include "Bubble/Scene/ScriptableEntity.h"
#include "Bubble/Renderer/Renderer2D.h"
//#include "Bubble/Renderer/Renderer3D.h"
#include "Bubble/Renderer/Renderer3D.h"

#include <glm/glm.hpp>

#include "Bubble/Scene/Entity.h"
#include "Bubble/Tools/RenderApiState.h"

namespace Bubble
{

    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        return CreateEntityWithUUID(UUID(), name);
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
    {
        Entity entity = {m_Registry.create(), this};
        entity.AddComponent<IDComponent>(uuid);
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }

    void Scene::OnUpdateRuntime(Timestep ts, Ref<Pipeline> pipeline)
    {
        // Update scripts
        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
            {
                // TODO: Move to Scene::OnScenePlay
                if(!nsc.Instance)
                {
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->m_Entity = Entity{entity, this};
                    nsc.Instance->OnCreate();
                }

                nsc.Instance->OnUpdate(ts);
            });
        }

        // Render
        SceneCamera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {// 寻找主相机
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for(auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if(camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }

        if(mainCamera)
        {
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            Renderer3D::ClearEntityID(pipeline);

            if(group.size())
            {
                Renderer3D::ResetStats();

                // 获取场景GBuffer信息
                {
                    //Renderer3D::BeginScene(camera,pipeline);
                    Renderer3D::BeginScene(*mainCamera, cameraTransform, pipeline);

                    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
                    for(auto entity : group)
                    {
                        auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                        //Renderer3D::DrawSprite(pipeline, transform.GetTransform(), sprite, (int)entity);
                        Renderer3D::DrawSprite(pipeline, transform.GetTransform(), sprite, (int)entity);
                    }

                    //Renderer3D::EndScene(pipeline);
                    Renderer3D::EndScene(pipeline);
                }

                Renderer3D::Calculatelighting(glm::vec3(cameraTransform * glm::vec4(0.0, 0.0, 0.0, 1.0)), pipeline);
            }

            Renderer3D::ShowSkyBox(pipeline, glm::inverse(cameraTransform), mainCamera->GetProjection());
            pipeline->UnbindFramebuffer();
        }
        else
        {
            BB_CORE_INFO("No Main Camera");
        }

    }

    void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera, Ref<Pipeline> pipeline)
    {

        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        Renderer3D::ClearEntityID(pipeline);

        if(group.size())
        {
            Renderer3D::ResetStats();

            // 获取场景GBuffer信息
            {
                //Renderer3D::BeginScene(camera,pipeline);
                Renderer3D::BeginScene(camera, pipeline);

                auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
                for(auto entity : group)
                {
                    auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                    //Renderer3D::DrawSprite(pipeline, transform.GetTransform(), sprite, (int)entity);
                    Renderer3D::DrawSprite(pipeline, transform.GetTransform(), sprite, (int)entity);
                }

                //Renderer3D::EndScene(pipeline);
                Renderer3D::EndScene(pipeline);
            }

            Renderer3D::Calculatelighting(camera.GetPosition(),pipeline);  
        }

        Renderer3D::ShowSkyBox(pipeline,camera.GetViewMatrix(),camera.GetProjection());
        pipeline->UnbindFramebuffer();
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Resize our non-FixedAspectRatio cameras
        auto view = m_Registry.view<CameraComponent>();
        for(auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if(!cameraComponent.FixedAspectRatio)
                cameraComponent.Camera.SetViewportSize(width, height);
        }

    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_Registry.view<CameraComponent>();
        for(auto entity : view)
        {
            const auto& camera = view.get<CameraComponent>(entity);
            if(camera.Primary)
                return Entity{entity, this};
        }
        return {};
    }

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        // static_assert(false);
    }

    template<>
    void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        if(m_ViewportWidth > 0 && m_ViewportHeight > 0)
            component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {
    }

}