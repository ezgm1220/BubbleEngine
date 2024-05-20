#pragma once
#include<vector>
#include <unordered_map>
#include "Bubble/Core/Base.h"
#include "Bubble/Renderer/VertexArray.h"
namespace Bubble
{
    struct VertexData
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
    };

    struct MeshData
    {
        static enum class DrawType
        {
            Arrays,
            Index,
            Index_STRIP,
        };
        std::vector<VertexData>m_data;
        Ref<VertexArray>VAO;
        Ref<VertexBuffer>VBO;
    };

    enum class MeshType
    {
        CUBE = 0, SPHERE_LOW, SPHERE_HIGH, BUNNY, CYLINDER, DOUBLESWORD,
        MAGNET, HAMMER
    };

    class Mesh
    {
    public:
        Mesh(){}
        Ref<VertexArray> DrawMesh(MeshType type);
        uint32_t Count(MeshType type);
    private:
        void LoadMesh(MeshType type);
        std::unordered_map< MeshType, MeshData*> m_data;
    };

    

    
}