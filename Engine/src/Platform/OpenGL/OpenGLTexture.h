#pragma once

#include "Bubble/Renderer/Texture.h"

#include <glad/glad.h>

namespace Bubble {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, TexDataType InternalFormat = TexDataType::RGBA8, TexDataType DataFormat = TexDataType::RGBA);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width;  }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
        virtual std::string GetPath()override { return m_Path; }
		
		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

        virtual void SetMipMap()const override {}

        virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		std::string m_Path;
        bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

    class OpenGLCubeMap : public CubeMap
    {
    public:
        OpenGLCubeMap(uint32_t MapSize,bool MipMap);
        OpenGLCubeMap(const std::string& path);
        virtual ~OpenGLCubeMap();

        virtual uint32_t GetWidth() const override { return m_CubeSize; }
        virtual uint32_t GetHeight() const override { return m_CubeSize; }
        virtual uint32_t GetRendererID() const override { return m_RendererID; }
        virtual std::string GetPath()override { return m_Path; }

        virtual void SetData(void* data, uint32_t size) override{}

        virtual void Bind(uint32_t slot = 0) const override;

        virtual bool IsLoaded() const override { return m_IsLoaded; }

        virtual void SetMipMap()const override;

        virtual bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLCubeMap&)other).m_RendererID;
        }
    private:
        std::string m_Path;
        bool m_IsLoaded = false;
        uint32_t m_CubeSize;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };

    class OpenGLHDRTexture2D : public HDRTexture2D
    {
    public:
        OpenGLHDRTexture2D(const std::string& path);
        virtual ~OpenGLHDRTexture2D();

        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetRendererID() const override { return m_RendererID; }
        virtual std::string GetPath()override { return m_Path; }

        virtual void SetData(void* data, uint32_t size) override {}

        virtual void Bind(uint32_t slot = 0) const override;

        virtual void SetMipMap()const override {}

        virtual bool IsLoaded() const override { return m_IsLoaded; }

        virtual bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLHDRTexture2D&)other).m_RendererID;
        }
    private:
        std::string m_Path;
        bool m_IsLoaded = false;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };
}
