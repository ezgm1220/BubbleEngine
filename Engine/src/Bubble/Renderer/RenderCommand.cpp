#include "bubblepch.h"
#include "Bubble/Renderer/RenderCommand.h"

namespace Bubble {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}