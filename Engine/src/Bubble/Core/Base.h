#pragma once

#include <memory>

#ifdef BB_DEBU
#define BB_DEBUGBREAK() __debugbreak()
#define BB_ENABLE_ASSERTS
#else
#define BB_DEBUGBREAK()
#endif

#define BB_EXPAND_MACRO(x) x
#define BB_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define BB_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Bubble
{

    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}

#include "Bubble/Core/Log.h"
#include "Bubble/Core/Assert.h"