#include "bubblepch.h"
#include "Bubble/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Bubble {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		BB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}