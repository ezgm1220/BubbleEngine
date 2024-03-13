 

# Getting Started

## Preamble

本教程用于将 Dear ImGui 集成到您的应用程序中。
也可参考我们的[常见问题](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md)和其他[维基](https://github.com/ocornut/imgui/wiki)页面。

在做任何事情之前，**构建并运行一个示例应用程序，玩一玩。
使用 Visual Studio，打开 `examples/imgui_examples.sln`。通常还会提供 XCode 项目和 Makefile。
由于我们支持多种系统，因此您可能没有安装所有相应的 SDK，但您应该可以在开箱后使用其中一些。


[例子](https://github.com/ocornut/imgui/tree/master/examples) 文件夹中的应用程序演示了 Dear ImGui 与各种常用窗口和图形 API 的配合使用。它们的设计尽可能简单明了。不过，在大多数情况下，大部分示例代码都与设置窗口和图形 API（又称设置基本应用程序）有关，而不是与设置 Dear ImGui 本身有关。

如果在应用程序中集成 Dear ImGui 时遇到问题，最简单的办法就是参考这些 [示例](https://github.com/ocornut/imgui/tree/master/examples)。

由于种种原因，我们的示例非常原始：我们不加载花哨的字体，而且 Demo 窗口通常无法从文件系统读取任何内容，因此我们的示例无法展示纹理等的使用。

## Game Loop?

Dear ImGui 有着游戏开发的背景，在这种背景下，应用程序需要以交互式帧速率（例如 60 FPS）持续更新，而且在 Dear ImGui 的运行和显示过程中，需要有一个底层的重图形应用程序。我们的示例一般就是这样结构的。虽然从技术上讲，在使用亲爱的 ImGui 时可以取消其中的一些假设（例如空闲、使用可变帧率、没有 "主视口"），但目前默认情况下并不支持这些用例，需要对系统和亲爱的 Imgui 有更深入的了解，因此不在本文讨论范围之内。

利用 GPU 渲染技术的典型游戏类应用程序会循环运行：

```C++
while (application_not_closed)
{
   // Poll events
   // Update application/game state
   // Render contents into a framebuffer
   // Swap/Present framebuffer to the screen
   // Wait some time (e.g. 1/60 of a second)
}
```

在您的示例应用程序中，我们明确尝试在交换/出现时与屏幕刷新同步，使应用程序以自然的屏幕刷新率刷新。成熟的应用程序可能会使用不同的计时机制来控制帧速率，但这不在我们的工作范围之内。

## Compiling/Linking

- (1) 决定拔出哪个分支：
  - master "或 "docking"，后者增加了对[Docking](https://github.com/ocornut/imgui/wiki/Docking) 和[Multi-viewports](https://github.com/ocornut/imgui/wiki/Multi-Viewports)的支持。
  - 两个分支都有维护，您可以随时轻松切换。
- (2) 将该版本库拉入你的项目的子模块中，或者直接下载/复制你的版本库并提交。
- (3) 将文件添加到您选择的项目或构建系统中，这样它们就会被编译并链接到您的应用程序中。
  - Add all source files in the root folder: `imgui/{*.cpp,*.h}`.
  - Add selected `imgui/backends/imgui_impl_xxxx{.cpp,.h}` files corresponding to the technology you use from the `imgui/backends/` folder (e.g. if your app uses SDL2 + DirectX11, add `imgui_impl_sdl2.cpp`, `imgui_impl_dx11.cpp` etc.). If your engine uses multiple APIs you may include multiple backends.
  - Visual Studio users: Add `misc/debugger/imgui.natvis` and `misc/debugger/imgui.natstepfilter` to improve the debugging experience.
  - std::string users: Add `misc/cpp/imgui_stdlib.*` to easily use InputText with std::string.

如果您的应用程序已经使用了您提取的后端应用程序接口，那么一切都应该已经编译和链接完成。

例如，如果您使用 DirectX11，则需要链接 d3d11.lib；如果您使用 SDL2，则需要获取该库（从其网站或 vcpkg 获取）并链接 SDL2.lib+SDL2main.lib，等等。如果您已经有一个应用程序在运行，那么根据定义，您应该不会有问题，包括相应的头文件和链接库。

如果你正在从头开始创建一个新的应用程序：虽然记录如何使用每个窗口/图形栈通常不在《亲爱的 ImGui》的范围之内，但你可以参考我们的 [examples/](https://github.com/ocornut/imgui/tree/master/examples) 文件夹，看看我们使用了哪些库/设置。有些人只是复制我们的示例来启动一个新的测试平台应用程序。为了方便提供易于编译的示例，多年来我们的资源库一直包含预编译的 Windows 版 GLFW 3.2（旧版本）。

## Setting up Dear ImGui & Backends

在列表之后，我们将显示相应的代码。

- (1) 在包含路径中添加 `imgui/`。包含主库（`#include "imgui.h"`）+ 后端（如`#include "imgui_impl_win32.h"`、`#include "imgui_impl_dx11.h"`）的头文件。
- (2) 使用 `ImGui::CreateContext()`创建亲爱的 ImGui 上下文。
- (3) (可选择)设置配置标志、加载字体和设置样式。
- (4) 初始化平台和渲染后端（例如 `ImGui_ImplWin32_Init()` + `ImGui_ImplDX11_Init()`）。
- (5) 主循环的开始：调用后端的`ImGui_ImplXXX_NewFrame（）`函数+调用`ImGui：：NewFrame（）`。
- (6) 主循环结束：调用 `ImGui::Render()` + 调用渲染后端的渲染函数（例如 `ImGui_ImplDX11_Render()`）。
- (7)大多数后端需要一些额外步骤来挂钩或转发事件。(例如，调用 `ImGui_ImplWin32_WndProcHandler`)
- (8) 调用后台关闭函数，并使用 `ImGui::DestroyContext()`销毁亲爱的 ImGui 上下文。
- (9) 在应用程序的输入逻辑中：您可以轮询
   `ImGui::GetIO().WantCaptureMouse`/`WantCaptureKeyboard`，以检查 Dear ImGui 是否想要阻止来自底层应用程序的鼠标/键盘输入。例如，当悬停窗口时，`WantCaptureMouse` 将被设置为 true，一种可能的策略是停止向主应用程序传递鼠标事件。



## Example: If you are using GLFW + OpenGL/WebGL

Full standalone example: [example_glfw_opengl3/main.cpp](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp)

Add to Includes:

```C++
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
```

Add to Initialization:

```C++
// Setup Dear ImGui context
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

// Setup Platform/Renderer backends(后端)
ImGui_ImplGlfw_InitForOpenGL(YOUR_WINDOW, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
ImGui_ImplOpenGL3_Init();
```

Add to start of main loop:

```C++
// (Your code calls glfwPollEvents())
// ...
// Start the Dear ImGui frame
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplGlfw_NewFrame();
ImGui::NewFrame();
ImGui::ShowDemoWindow(); // Show demo window! :)
```

Add to end of main loop:

```C++
// Rendering
// (Your code clears your framebuffer, renders your other stuff etc.)
ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
// (Your code calls glfwSwapBuffers() etc.)
```

Add to Shutdown:

```C++
ImGui_ImplOpenGL3_Shutdown();
ImGui_ImplGlfw_Shutdown();
ImGui::DestroyContext();
```

## Additional code to enable Docking

启用对接的附加代码

要使用 [Docking](https://github.com/ocornut/imgui/wiki/Docking) 功能，请拉取 `docking` 分支并启用配置标志：

```
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 
```



## Additional code to enable Multi-viewports

To use the [Multi-viewports](https://github.com/ocornut/imgui/wiki/Multi-Viewports) feature, pull the `docking` branch and enable the configuration flag:

```
io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; 
```



在渲染循环结束时，通常在渲染主视口之后，但在呈现/交换主视口之前：

```
// Update and Render additional Platform Windows
ImGui::UpdatePlatformWindows();
ImGui::RenderPlatformWindowsDefault();
```

请注意，根据您使用的后端（尤其是 OpenGL），可能还需要额外的调用。详情请查看适合您设置的 [示例项目](https://github.com/ocornut/imgui/tree/docking/examples)。