#include "bubblepch.h"
#include "Bubble/Renderer/Texture.h"

#include "Bubble/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Bubble {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height);
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

    Bubble::Ref<Bubble::HDRTexture2D> HDRTexture2D::Create(const std::string& path)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLHDRTexture2D>(path);
        }
    }

}