#include "bubblepch.h"

#include "OpenGLState.h"

#include <glad/glad.h>

namespace Bubble
{
    void RenderAPIState::ActiveTexture(uint32_t* values)
    {
        glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)values);
    }

    void RenderAPIState::BindVertexArray(uint32_t* values)
    {
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)values);
    }

    void RenderAPIState::BindArrayBuffer(uint32_t* values)
    {
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)values);
    }

    void RenderAPIState::BindElementArray(uint32_t* values)
    {
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)values);
    }

    void RenderAPIState::BindDrawFrameBuffer(uint32_t* values)
    {
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, (GLint*)values);
    }

    void RenderAPIState::BindReadFrameBuffer(uint32_t* values)
    {
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, (GLint*)values);
    }

    std::tuple< uint32_t, uint32_t, uint32_t> RenderAPIState::GetVertexState()
    {
        uint32_t VertexArray,  ArrayBuffer, ElementArrya;
        RenderAPIState::BindVertexArray(&VertexArray);
        RenderAPIState::BindArrayBuffer(&ArrayBuffer);
        RenderAPIState::BindElementArray(&ElementArrya);
        return std::make_tuple(VertexArray, ArrayBuffer, ElementArrya);
    }

    std::tuple< uint32_t, uint32_t> RenderAPIState::GetFramebufferState()
    {
        uint32_t Read,  Draw;
        RenderAPIState::BindDrawFrameBuffer(&Draw);
        RenderAPIState::BindReadFrameBuffer(&Read);
        return std::make_tuple(Draw, Read);
    }

}

