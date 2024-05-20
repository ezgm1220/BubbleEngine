#include "bubblepch.h"
#include "Bubble/model/Mesh.h"
#include "Bubble/Model/tiny_obj_loader.h"

namespace Bubble
{

    static std::unordered_map< MeshType, std::string> MeshPath = {
        {MeshType::CUBE,"assets/Model/cube.obj"},
        {MeshType::SPHERE_LOW,"assets/Model/sphere8.obj"},
        {MeshType::SPHERE_HIGH,"assets/Model/sphere_smooth.obj"},
        {MeshType::BUNNY,"assets/Model/bunny.obj"},
        {MeshType::CYLINDER,"assets/Model/Cylinder.obj"},
        {MeshType::DOUBLESWORD,"assets/Model/Double_Sword_1.obj"},
        {MeshType::MAGNET,"assets/Model/Magnet.obj"},
        {MeshType::HAMMER,"assets/Model/TorHammer.obj"},
    };

    Ref<VertexArray> Mesh::DrawMesh(MeshType type)
    {
        if(m_data.count(type) == 0)
        {
            LoadMesh(type);
        }
        return m_data[type]->VAO;
    }

    uint32_t Mesh::Count(MeshType type)
    {
        return m_data[type]->m_data.size();
    }

    void Mesh::LoadMesh(MeshType type)
    {
        MeshData* meshdata = new MeshData;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;
        std::string filepath = MeshPath[type];
        bool ret = tinyobj::LoadObj(&attrib, &shapes, nullptr, &warn, &err, filepath.c_str());
       

        if(!ret)
        {
            exit(1);
        }

        // Loop over shapes
        for(size_t s = 0; s < shapes.size(); s++)
        {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
            {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                // Loop over vertices in the face.
                for(size_t v = 0; v < fv; v++)
                {
                    VertexData verdata;
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                    verdata.Position.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    verdata.Position.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    verdata.Position.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];


                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if(idx.normal_index >= 0)
                    {
                        verdata.Normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
                        verdata.Normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
                        verdata.Normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    }

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    if(idx.texcoord_index >= 0)
                    {
                        verdata.TexCoord.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                        verdata.TexCoord.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    }

                    meshdata->m_data.emplace_back(std::move(verdata));
                }
                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }
        }

        meshdata->VAO = VertexArray::Create();

        meshdata->VBO = VertexBuffer::Create(meshdata->m_data.data(), meshdata->m_data.size() * sizeof(VertexData));

        meshdata->VBO->SetLayout({
                { ShaderDataType::Float3, "a_Position"  },
                { ShaderDataType::Float3, "a_Normal"    },
                { ShaderDataType::Float2, "a_TexCoord"  }
            });
        meshdata->VAO->AddVertexBuffer(meshdata->VBO);

        meshdata->VAO->Unbind();
        meshdata->VBO->Unbind();

        m_data.insert({type,meshdata});
    }

}
