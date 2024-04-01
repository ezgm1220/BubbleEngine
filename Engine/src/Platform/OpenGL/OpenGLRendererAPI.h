#pragma once

#include "Bubble/Renderer/RendererAPI.h"

namespace Bubble {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
        virtual void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t vertexCount = 0) override;
	};


}
