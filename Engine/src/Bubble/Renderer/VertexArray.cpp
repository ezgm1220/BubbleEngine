#include "bubblepch.h"
#include "Bubble/Renderer/VertexArray.h"

#include "Bubble/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Bubble {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    BB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		BB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}