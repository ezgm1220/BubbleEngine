#include "bubblepch.h"
#include "Bubble/Scene/Entity.h"

namespace Bubble
{

    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

}