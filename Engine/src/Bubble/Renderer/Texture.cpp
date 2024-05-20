#include "bubblepch.h"
#include "Bubble/Renderer/Texture.h"

#include "Bubble/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Bubble {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, TexDataType InternalFormat, TexDataType DataFormat)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height, InternalFormat, DataFormat);
		}

		BB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
        if(path.empty())
        {
            return nullptr;
        }
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
		}

		BB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

    Bubble::Ref<Bubble::CubeMap> CubeMap::Create(const std::string& path)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLCubeMap>(path);
        }

        BB_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Bubble::Ref<Bubble::CubeMap> CubeMap::Create(const uint32_t mapsize, bool MipMap)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLCubeMap>(mapsize, MipMap);
        }

        BB_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Bubble::Ref<Bubble::HDRTexture2D> HDRTexture2D::Create(const std::string& path)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLHDRTexture2D>(path);
        }
    }
    Ref<Texture2D>DefaultTexture::AlbedoTexture ;
    Ref<Texture2D>DefaultTexture::NormalTexture ;
    Ref<Texture2D>DefaultTexture::MetallicTexture ;
    Ref<Texture2D>DefaultTexture::RoughnessTexture ;
    Ref<Texture2D>DefaultTexture::AOTexture ;
    void DefaultTexture::InitDefaultTexture()
    {
        AlbedoTexture = Texture2D::Create(1, 1);
        NormalTexture = Texture2D::Create(1, 1);
        MetallicTexture = Texture2D::Create(1, 1);
        RoughnessTexture = Texture2D::Create(1, 1);
        AOTexture = Texture2D::Create(1, 1);
        uint32_t AlbedoTextureData = 0xffffffff;
        uint32_t NormalTextureData = 0xffff8080;
        uint32_t MetallicTextureData = 0xffffffff;
        uint32_t RoughnessTextureData = 0xff000001;
        uint32_t AOTextureData = 0xffffffff;
        AlbedoTexture->SetData(&AlbedoTextureData, sizeof(uint32_t));
        NormalTexture->SetData(&NormalTextureData, sizeof(uint32_t));
        MetallicTexture->SetData(&MetallicTextureData, sizeof(uint32_t));
        RoughnessTexture->SetData(&RoughnessTextureData, sizeof(uint32_t));
        AOTexture->SetData(&AOTextureData, sizeof(uint32_t));
    }

}