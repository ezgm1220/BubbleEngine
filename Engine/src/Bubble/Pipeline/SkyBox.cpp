#include "bubblepch.h"
#include "SkyBox.h"

namespace Bubble
{

    glm::mat4 SkyBox::captureViews[6] = {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    glm::mat4 SkyBox::captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

    void SkyBox::SetShader(const std::string path)
    {
        Shader_path = path;
        m_Shader = Shader::Create(Shader_path);
    }

    void SkyBox::SetFrameBuffer(int CubeSize)
    {

    }

    void SkyBox::GetCubeMap_Hdr()
    {

    }

    void SkyBox::GetCubeMap_SixFaces()
    {
        m_CubeMap = CubeMap::Create(SixFaces_path);
    }

    
    uint32_t SkyBox::GetCubeMapID()
    {
        if(!m_CubeMap)
        {
            BB_CORE_WARN("SkyBox no have CubeMap!!!!!");
            return -1;
        }
        return m_CubeMap->GetRendererID();
    }

}

