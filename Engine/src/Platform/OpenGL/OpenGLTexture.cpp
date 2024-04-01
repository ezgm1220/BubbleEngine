#include "bubblepch.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

namespace Bubble {

    static std::tuple< GLenum, GLenum>TexDataGetType(TexDataType InternalFormat, TexDataType DataFormat)
    {
        GLenum fir, sec;
        switch(InternalFormat)
        {
            case TexDataType::RGB8:fir = GL_RGB8; break;
            case TexDataType::RGBA8:fir = GL_RGBA8; break;
            case TexDataType::RGB16F:fir = GL_RGB16F; break;
            case TexDataType::RGBA16F:fir = GL_RGBA16F; break;
            case TexDataType::RG16F:fir = GL_RG16F; break;
        }
        switch(DataFormat)
        {
            case TexDataType::RGB:sec = GL_RGB; break;
            case TexDataType::RGBA:sec = GL_RGBA; break;
            case TexDataType::RG:sec = GL_RG; break;
        }
        return {fir,sec};
    }

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, TexDataType InternalFormat, TexDataType DataFormat)
		: m_Width(width), m_Height(height)
	{
        std::tie(m_InternalFormat, m_DataFormat)  = TexDataGetType(InternalFormat, DataFormat);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
        if(data)
        {
            m_IsLoaded = true;

            m_Width = width;
            m_Height = height;

            GLenum internalFormat = 0, dataFormat = 0;
            switch(channels)
            {
                case 4:
                {
                    internalFormat = GL_RGBA8;
                    dataFormat = GL_RGBA;
                    break;
                }
                case 3:
                {
                    internalFormat = GL_RGB8;
                    dataFormat = GL_RGB;
                    break;
                }
                case 2:
                {
                    internalFormat = GL_RG8;
                    dataFormat = GL_RG;
                    break;
                }
                case 1:
                {
                    internalFormat = GL_R8;
                    dataFormat = GL_RED;
                    break;
                }
            }

            m_InternalFormat = internalFormat;
            m_DataFormat = dataFormat;

            BB_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		BB_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

    OpenGLCubeMap::OpenGLCubeMap(uint32_t MapSize, bool MipMap)
        :m_CubeSize(MapSize)
    {
        m_InternalFormat = GL_RGB16F;
        m_DataFormat = GL_RGB;

        //glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
       
        //for(unsigned int i = 0; i < 6; ++i)
        //{
        //    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, MapSize, MapSize, 0, m_DataFormat, GL_FLOAT, nullptr);
        //}
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
        for(unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, MapSize, MapSize, 0, m_DataFormat, GL_FLOAT, nullptr);
        }
        if(MipMap)
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

    }

    static std::vector<std::string> faces
    {
        "/right.jpg",
        "/left.jpg",
        "/top.jpg",
        "/bottom.jpg",
        "/front.jpg",
        "/back.jpg"
    };

    OpenGLCubeMap::OpenGLCubeMap(const std::string& path)
    {
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

        stbi_set_flip_vertically_on_load(0);
        int width, height, nrChannels;
        for(unsigned int i = 0; i < faces.size(); i++)
        {
            auto p_path = path + faces[i];
            unsigned char* data = stbi_load(p_path.c_str(), &width, &height, &nrChannels, 0);
            if(data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                BB_CORE_WARN("Cubemap texture failed to load at path: {}", p_path);
                stbi_image_free(data);
            }
        }
        m_CubeSize = width;
        m_InternalFormat = GL_RGB;
        m_DataFormat = GL_RGB;
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    OpenGLCubeMap::~OpenGLCubeMap()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLCubeMap::Bind(uint32_t slot /*= 0*/) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }

    void OpenGLCubeMap::SetMipMap()const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    OpenGLHDRTexture2D::OpenGLHDRTexture2D(const std::string& path)
        :m_Path(path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        float* data = nullptr;
        {
            data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
        }
        if(data)
        {
            m_IsLoaded = true;

            m_Width = width;
            m_Height = height;

            m_InternalFormat = GL_RGB16F;
            m_DataFormat = GL_RGB;

            BB_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_FLOAT, data);

            stbi_image_free(data);
        }
    }

    OpenGLHDRTexture2D::~OpenGLHDRTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLHDRTexture2D::Bind(uint32_t slot /*= 0*/) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }

}