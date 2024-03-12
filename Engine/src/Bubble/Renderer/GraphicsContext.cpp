#include "bubblepch.h"
#include "Bubble/Renderer/GraphicsContext.h"

#include "Bubble/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Bubble {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		BB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}