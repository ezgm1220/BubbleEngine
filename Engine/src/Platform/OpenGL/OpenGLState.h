#pragma  once

#include <vector>
#include <tuple>

namespace Bubble
{

    class RenderAPIState
    {
    public:
        static void ActiveTexture(uint32_t* values);
        static void BindVertexArray(uint32_t* values);
        static void BindArrayBuffer(uint32_t* values);
        static void BindElementArray(uint32_t* values);
        static void BindDrawFrameBuffer(uint32_t* values);
        static void BindReadFrameBuffer(uint32_t* values);
        static std::tuple< uint32_t, uint32_t, uint32_t>GetVertexState();
        static std::tuple< uint32_t, uint32_t>GetFramebufferState();
    };
}