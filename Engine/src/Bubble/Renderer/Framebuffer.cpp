#include "bubblepch.h"
#include "Bubble/Renderer/Framebuffer.h"

#include "Bubble/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Bubble {
	
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}

		BB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

