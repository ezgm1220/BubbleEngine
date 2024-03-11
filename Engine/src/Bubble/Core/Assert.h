#pragma once

#include "Bubble/Core/Base.h"
#include "Bubble/Core/Log.h"
#include <filesystem>

#ifdef BB_ENABLE_ASSERTS

    // Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
    // provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
    // 或者，我们可以对 "WITH_MSG "和 "NO_MSG "使用相同的 "默认 "信息，并通过连接格式化字符串来支持自定义格式化，
    // 而不是在默认信息中加入格式化内容
#define BB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { BB##type##ERROR(msg, __VA_ARGS__); BB_DEBUGBREAK(); } }
#define BB_INTERNAL_ASSERT_WITH_MSG(type, check, ...) BB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define BB_INTERNAL_ASSERT_NO_MSG(type, check) BB_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", BB_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define BB_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define BB_INTERNAL_ASSERT_GET_MACRO(...) BB_EXPAND_MACRO( BB_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, BB_INTERNAL_ASSERT_WITH_MSG, BB_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
// 目前至少接受条件和一个可选的附加参数（消息）
#define BB_ASSERT(...) BB_EXPAND_MACRO( BB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define BB_CORE_ASSERT(...) BB_EXPAND_MACRO( BB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define BB_ASSERT(...)
#define BB_CORE_ASSERT(...)
#endif