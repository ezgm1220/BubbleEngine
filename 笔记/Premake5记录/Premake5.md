lua基本常识

- 标识空白是任意的；
- 双破折号"--"开始单行注释。
- 括号"{"和"}"用于表示值列表

```lua
-- premake5.lua
workspace "HelloWorld"
   configurations { "Debug", "Release" }

project "HelloWorld"
   kind "ConsoleApp"
   language "C"
   targetdir "bin/%{cfg.buildcfg}"

   files { "**.h", "**.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
```



在premake，每一行语句实际上都是一个函数调用，但我们可以省略括号

```C++
workspace "HelloWorld"
configurations { "Debug", "Release" }
   
等价于
    
workspace("HelloWorld")
configurations({ "Debug", "Release" })   
   
```

但如果使用的不是简单的字符串或表格，则括号必须使用。如下：

```lua
local lang = "C++"
language (lang)  -- using a variable, needs parenthesis

workspace("HelloWorld" .. _ACTION) -- using string concatenation, needs parenthesis
```

## 路径指定

- 始终指定相对于脚本文件的路径。
- 始终使用正斜杠 ("/") 作为路径分隔符。Premake 会在生成输出文件时转换为适当的分隔符。



## Workspaces & Projects

### Workspaces

每次构建的顶层都有一个工作区，作为项目的容器。

您还可以在此级别指定额外的构建设置（定义、包含路径等），这些设置将同样由项目继承。

编译配置使用 configurations 函数指定，并且是必需的；

作为函数参数提供的工作区名称将用作生成的工作区文件的默认文件名，因此最好避免使用特殊字符（空格也可）。如果希望使用其他名称，请使用文件名函数指定。

自定义名字：

```lua
workspace "Hello World"
   filename "Hello"
   configurations { "Debug", "Release" }
```

### Projects

项目名称与工作区名称一样，用作生成项目文件的文件名，因此应避免使用特殊字符，或使用文件名函数提供不同的值。

每个项目都指定了一种类型，它决定了生成的输出类型，如控制台或窗口可执行文件，或共享库或静态库。kind 函数用于指定该值。

每个项目还指定其使用的编程语言，如 C++ 或 C#。语言函数用于设置该值。

```lua
project "MyProject"
  kind "ConsoleApp"
  language "C++"
```

### Locations

默认情况下，Premake 会将生成的工作区和项目文件放在与定义它们的脚本相同的目录下。

可以使用 location 功能更改输出位置。

```
workspace "MyWorkspace"
  configurations { "Debug", "Release" }
  location "build"

project "MyProject"
  location "build/MyProject"
```

与 Premake 中的所有路径一样，应指定相对于脚本文件的位置。
使用上面的示例和脚本，生成的工作区将放置在 C:\Code\MyProject\build 中，项目将放置在 C:\Code\MyProject\build\MyProject 中。

## Scopes & Inheritance

作用域的层次结构：
全局作用域包含工作区，工作区又包含项目。
外层作用域中的值会被内层作用域继承，因此工作空间会继承存储在全局作用域中的值，而项目则会继承存储在工作空间中的值。

```lua
-- global scope, all workspaces will receive these values
defines { "GLOBAL" }

workspace "MyWorkspaces"
  -- workspace scope inherits the global scope; the list value
  -- will now be { "GLOBAL", "WORKSPACE" }
  defines { "WORKSPACE" }

project "MyProject"
  -- project scope inherits from its workspace; the list value
  -- will now be { "GLOBAL", "WORKSPACE", "PROJECT" }
  defines { "PROJECT" }
```

有时，返回并向先前已声明的作用域添加值会很有帮助。您可以使用与最初声明时相同的方法来这样做：使用相同的名称调用工作区或项目。

```lua
-- declare my workspace
workspace "MyWorkspace"
  defines { "WORKSPACE1" }

-- declare a project or two
project "MyProject"
  defines { "PROJECT" }

-- re-select my workspace to add more settings, which will be inherited
-- by all projects in the workspace
workspace "MyWorkspace"
  defines { "WORKSPACE2" }  -- value is now { "WORKSPACE1", "WORKSPACE2" }
```

您还可以使用特殊的 "*"名称来选择当前作用域的父作用域或容器，而无需知道其名称。

```lua
-- declare my workspace
workspace "MyWorkspace"
  defines { "WORKSPACE1" }

-- declare a project or two
project "MyProject"
  defines { "PROJECT" }

-- re-select my workspace to add more settings
project "*"
  defines { "WORKSPACE2" }  -- value is now { "WORKSPACE1", "WORKSPACE2" }

-- re-select the global scope
workspace "*"
```

可以把 "*"看成是 "通配符"，表示 "我的父容器中的所有项目 "或 "全局范围内的所有工作区"。

## Adding Source Files

您可以使用文件功能将文件（源代码、资源等）添加到项目中。

```lua
files {
   "hello.h",  -- you can specify exact names
   "*.c",      -- or use a wildcard...通配符
   "**.cpp"    -- ...and recurse into subdirectories 并递归到子目录
}
```

通配符 * 将匹配一个目录中的文件；
通配符 ** 将匹配一个目录中的文件，并向下搜索任何子目录。

路径应始终使用斜线 / 作为分隔符；Premake 会根据需要将其转换为相应平台的分隔符。

### Excluding Files

有时，你需要某个目录中的大部分文件，但不是全部。在这种情况下，可以使用 removefiles() 函数屏蔽掉少数例外文件。

```lua
files { "*.c" }
removefiles { "a_file.c", "another_file.c" }
```

排除也可以使用通配符。

```lua
files { "**.c" }
removefiles { "tests/*.c" }
```

有时，你可能想排除某个特定目录中的所有文件，但又不确定该目录在源代码树中的位置。

```lua
files { "**.c" }
removefiles { "**/Win32Specific/**" }
```

## Linking

与外部库的链接是通过links功能完成的

在指定库时，应省略系统特定的装饰，如前缀或文件扩展名。Premake 会根据目标平台自动合成正确的格式。唯一的例外是 Mac OS X 框架，它需要使用文件扩展名来标识。

要链接到同级项目（同一工作区中的项目），请使用项目名称。Premake 会根据当前平台和配置推导出正确的库路径和名称。

```lua
workspace "MyWorkspace"

   project "MyLibraryProject"
      -- ...project settings here...

   project "MyExecutableProject"
      -- ...project settings here...
      links { "MyLibraryProject" }
```

您可以使用 libdirs 函数告诉 Premake 在哪里搜索库。

`libdirs { "libs", "../mylibs" }`

如果需要查找某个库的位置，请使用 os.findlib 函数。

`libdirs { os.findlib("X11") }`

## Configurations & Platforms

配置是应用于构建的设置集合，包括标志和开关、头文件和库搜索目录等。每个工作区都定义了自己的配置名称列表；大多数集成开发环境提供的默认配置名称是 "调试 "和 "发布"。

这里的 "平台 "有点用词不当；我又一次沿用了 Visual Studio 的术语。实际上，平台只是另一套构建配置名称，为配置项目提供了另一个轴心。

```lua
configurations { "Debug", "Release" }
platforms { "Win32", "Win64", "Xbox360" }
```

设置完成后，列出的平台将出现在集成开发环境的平台列表中。因此，您可以选择 "调试 Win32 "构建，或 "发布 Xbox360 "构建，或两个列表的任意组合。
就像构建配置一样，平台名称本身没有任何意义。您可以通过使用过滤器功能应用设置来提供意义。

```lua
configurations { "Debug", "Release" }
platforms { "Win32", "Win64", "Xbox360" }

filter { "platforms:Win32" }
    system "Windows"
    architecture "x86"

filter { "platforms:Win64" }
    system "Windows"
    architecture "x86_64"

filter { "platforms:Xbox360" }
    system "Xbox360"
```

与构建配置不同，平台是完全可选的。如果不需要，只需不调用 platforms 函数，就会使用工具集的默认行为。
平台只是构建配置的另一种形式。你可以使用所有相同的设置，并适用相同的范围规则。
你可以在不使用平台的情况下使用 system 和 architecture() 设置，也可以在平台配置中使用其他非平台设置。
如果你曾经使用过 "调试静态"、"调试 DLL"、"发布静态 "和 "发布 DLL "等构建配置，那么平台确实可以简化事情。

```lua
configurations { "Debug", "Release" }
platforms { "Static", "DLL" }

filter { "platforms:Static" }
    kind "StaticLib"

filter { "platforms:DLL" }
    kind "SharedLib"
    defines { "DLL_EXPORTS" }
```

配置和平台列表现在可以按项目指定。例如，一个项目应为 Windows 构建，但不应为游戏控制台构建，那么就可以移除该平台：

```lua
workspace "MyWorkspace"
    configurations { "Debug", "Release" }
    platforms { "Windows", "PS3" }

project "MyProject"
    removeplatforms { "PS3" }
```

配置映射是一项相关功能，可将工作区级配置转换为项目级值，从而将具有不同配置和平台列表的项目合并到一个工作区中。例如，一个单元测试库可能配置了通用调试和发布配置。

````lua
project "UnitTest"
    configurations { "Debug", "Release" }

````

要在包含更复杂配置集的工作区中重复使用该测试项目，需要创建工作区配置到相应项目配置的映射。

```lua
workspace "MyWorkspace"
    configurations { "Debug", "Development", "Profile", "Release" }

project "UnitTest"
    configmap {
        ["Development"] = "Debug",
        ["Profile"] = "Release"
    }
```

需要注意的是，项目不能向工作区添加新配置。它们只能移除对现有工作区配置的支持，或将其映射到不同的项目配置。

## Filters

通过 Premake 的过滤系统，你可以将构建设置锁定到你希望它们出现的确切配置中。你可以根据特定的编译配置或平台、操作系统、目标操作等进行过滤。

下面的示例在工作区的 "调试 "构建配置中设置了名为 "DEBUG "的预处理器符号，而在 "发布 "配置中则设置了 "NDEBUG"。

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }

   filter "configurations:Debug"
      defines { "DEBUG" }

   filter "configurations:Release"
      defines { "NDEBUG" }
```

过滤器总是由两部分组成：前缀指定要过滤的字段，*模式*指定应接受该字段的哪些值。

过滤器遵循 Premake 脚本的伪声明风格：调用 filter() 会使过滤器条件 "激活"。随后出现在脚本中的所有设置都将被此条件过滤，直到新的过滤器或容器（工作区、项目）被激活。

```lua
-- All of these settings will appear in the Debug configuration
filter "configurations:Debug"
  defines { "DEBUG" }
  flags { "Symbols" }

-- All of these settings will appear in the Release configuration
filter "configurations:Release"
  defines { "NDEBUG" }
  optimize "On"

-- This is a sneaky bug (assuming you always want to link against these lib files).
-- Because the last filter set was Release. These libraries will only be linked for release.
-- To fix this place this after the "Deactivate" filter call below. Or before any filter calls.
links { "png", "zlib" }-- 因为未跳出过滤区域，所以只在Release下进行了链接

-- "Deactivate" the current filter; these settings will apply
-- to the entire workspace or project (whichever is active)
filter {}-- 跳出过滤区
  files { "**.cpp" }
```

过滤器在工作区或项目文件创建并写入磁盘时进行评估。当需要输出工作区 "调试 "构建配置的设置时，Premake 会评估筛选器列表，找出符合 "调试 "条件的筛选器。

使用上面的例子，Premake 会首先考虑过滤器 "configurations:Debug"。它会检查当前正在输出的配置名称，看是否与之匹配，然后将任何设置包含到下一次过滤器调用。

过滤器 "configurations:Release "将被跳过，因为模式 "Release "与当前正在生成的配置名称（"Debug"）不匹配。

**最后一个过滤器"{}"没有定义任何过滤条件，因此不会排除任何内容。在此过滤器之后应用的任何设置都将出现在工作区或项目中的所有配置中。**

过滤器也可以组合使用，用 "或 "或 "非 "修改，并使用模式匹配。有关更完整的说明和大量示例，请参阅过滤器。

## Build Settings

Premake 提供了一个不断增长的编译设置列表，你可以对其进行调整；下表列出了一些最常见的配置任务，并提供了相应函数的链接。如需可用设置和函数的完整列表，请参阅 Project API 和 Lua Library Additions。

|                                               |                                                              |
| --------------------------------------------- | ------------------------------------------------------------ |
| Specify the binary type (executable, library) | [kind](https://premake.github.io/docs/kind)                  |
| Specify source code files                     | [files](https://premake.github.io/docs/files), [removefiles](https://premake.github.io/docs/files) |
| Define compiler or preprocessor symbols       | [defines](https://premake.github.io/docs/defines)            |
| Locate include files                          | [includedirs](https://premake.github.io/docs/includedirs)    |
| Set up precompiled headers                    | [pchheader](https://premake.github.io/docs/pchheader), [pchsource](https://premake.github.io/docs/pchsource) |
| Link libraries, frameworks, or other projects | [links](https://premake.github.io/docs/links), [libdirs](https://premake.github.io/docs/libdirs) |
| Enable debugging information                  | [symbols](https://premake.github.io/docs/symbols)            |
| Optimize for size or speed                    | [optimize](https://premake.github.io/docs/optimize)          |
| Add arbitrary build flags                     | [buildoptions](https://premake.github.io/docs/buildoptions), [linkoptions](https://premake.github.io/docs/linkoptions) |
| Set the name or location of compiled targets  | [targetname](https://premake.github.io/docs/targetname), [targetdir](https://premake.github.io/docs/targetdir) |

## Command Line Arguments

### Actions and Options

Premake 提供了在项目脚本中使用 newaction 和 newoption 函数定义和处理新命令行参数的功能。

Premake 可识别两种类型的参数：`actions`和`options`。

动作表示 Premake 在运行时应执行的操作。例如，vs2013 操作表示应生成 Visual Studio 2013 项目文件。clean 操作会删除所有生成的文件。一次只能指定一个动作。

选项可修改操作的行为。例如，dotnet 选项用于更改生成文件中使用的 .NET 编译器集。选项可以接受一个值，如--dotnet=mono，也可以作为一个标志，如--with-opengl。
在脚本中，你可以使用 _ACTION 全局变量（一个字符串值）来识别当前的操作。你可以使用 _OPTIONS 表检查选项，该表包含一个键值对列表。键是选项标识符（"dotnet"），它引用了命令行值（"mono"），如果是无值选项，则为空字符串。

```lua
-- delete a file if the clean action is running
if _ACTION == "clean" then
   -- do something
end

-- use an option value in a configuration
targetdir ( _OPTIONS["outdir"] or "out" )
```

### Creating New Options

新的命令行选项是通过 newoption 函数创建的，该函数传递了一个完整描述选项的表格。最好用一些例子来说明这一点。
这是一个用于在 3D 应用程序中强制使用 OpenGL 的选项。它只是一个简单的标志，不带任何值。

```lua
newoption {
   trigger = "with-opengl",
   description = "Force the use of OpenGL for rendering, regardless of platform"
}
```

注意每个键值对后面的逗号；这是表格所需的 Lua 语法。一旦添加到脚本中，该选项就会出现在帮助文本中，您也可以在配置块中将触发器作为关键字使用。

```lua
filter { "options:with-opengl" }
   links { "opengldrv" }

filter { "not options:with-opengl" }
   links { "direct3ddrv" }
```

下一个示例显示了一个具有固定允许值的选项。和上面的例子一样，它的目的是让用户指定一个 3D API。

```lua
newoption {
   trigger = "gfxapi",
   value = "API",
   description = "Choose a particular 3D API for rendering",
   allowed = {
      { "opengl",    "OpenGL" },
      { "direct3d",  "Direct3D (Windows only)" },
      { "software",  "Software Renderer" }
   },
   default = "opengl"
}
```

和以前一样，这个新选项将被整合到帮助文本中，并附带对每个允许值的说明。Premake 会在启动时检查该选项的值，并在值无效时提示错误。**value** 字段出现在帮助文本中，目的是为用户提供预期值类型的线索。在这种情况下，帮助文本将如下所示：

```undefined
--gfxapi=API      Choose a particular 3D API for rendering; one of:
    opengl        OpenGL
    direct3d      Direct3D (Windows only)
    software      Software Renderer
```

与上面的例子不同，您现在可以在配置块中使用 *value* 作为关键字。

```lua
filter { "options:gfxapi=opengl" }
   links { "opengldrv" }

filter { "options:gfxapi=direct3d" }
    links { "direct3ddrv" }

filter { "options:gfxapi=software" }
    links { "softwaredrv" }
```

作为选项的最后一个例子，你可能想指定一个接受无约束值的选项，例如输出路径。只需省略允许值列表即可。

```lua
newoption {
   trigger     = "outdir",
   value       = "path",
   description = "Output directory for the compiled executable"
}
```

### Creating New Actions

操作的定义方式与选项大致相同，可以是简单的操作：

```lua
newaction {
   trigger     = "install",
   description = "Install the software",
   execute = function ()
      -- copy files, etc. here
   end
}
```

动作触发时要执行的实际代码应放在`execute()`函数中。

这是简单版本，非常适合不需要访问特定项目信息的一次性操作。有关编写更完整操作的教程，请参阅 [添加新操作](https://premake.github.io/docs/Adding-New-Action)。

## Using Modules

Premake 可通过使用第三方模块进行扩展。模块可以添加对新工具集、语言和框架以及全新功能的支持。请参阅 [模块](https://premake.github.io/community/modules)，了解社区已经创建的模块。

要使用模块，请下载或克隆该模块的软件源到 [Premake 的搜索路径之一](https://premake.github.io/docs/Locating-Scripts)，确保目标文件夹与模块的主脚本名称相同，例如 **qt/qt.lua**.

```undefined
$ git clone https://github.com/dcourtois/premake-qt qt
```

然后，只需在项目或[系统脚本](https://premake.github.io/docs/System-Scripts)中调用 `require()` 即可将其包含在内。

```lua
require "qt"
```

### Including a Module With Your Project

为方便起见，您可能希望在项目的源代码树中保留一份所需模块的副本。在这种情况下，您可以将它们放在任意位置，并在需要时提供相对路径。例如，如果您的主程序 **premake5.lua** 位于项目树的根目录，而模块位于名为 **build** 的文件夹中，那么您可以像下面这样加载它：

```lua
require "build/qt"
```

### System Modules

您也可以将模块放在 [Premake 搜索路径](https://premake.github.io/docs/Locating-Scripts) 的任何位置，例如 **~/.premake** 中。在这种情况下，不需要路径信息，只需调用

```lua
require "qt"
```

如果您想让所有项目都能使用某个模块，可以在 [system script](https://premake.github.io/docs/System-Scripts) 中调用 `require()`。在这种情况下，Premake 会在每次运行时自动加载模块，并提供模块的所有功能。

### Version Requirements

为确保与项目脚本的兼容性，有时需要为模块依赖关系设定一个最小版本或版本范围。Premake 包含 [修改版 Lua 的 `require()` 函数](https://premake.github.io/docs/require)，它接受版本测试作为第二个参数。

```lua
require("qt", ">=1.1")
```

更多信息和示例请参阅[`require()` 文档](https://premake.github.io/docs/require)。

# Reference

## Project Settions

### flags

```lua
flags { "flag_list" }
```

指定编译标志，以修改编译或链接过程。

flag_list 是一个字符串标志名称列表；有效标志列表见下文。标志值不区分大小写。特定平台或工具集不支持的标志将被忽略。

[参考链接](https://premake.github.io/docs/flags/)

### Tokens

值标记是用 %{} 序列包装的表达式。令牌可以访问一个或多个上下文对象，具体取决于它们在项目中的作用域：wks、prj、cfg 和 file。您可以在标记中访问这些上下文对象的所有字段。

```lua
%{wks.name}
%{prj.location}
%{cfg.targetdir}
```

您可以使用 wks、prj、cfg 和 file 分别表示当前工作区、项目、配置和文件配置。需要注意的是，这些值必须在您要替换的值类型的作用域内，否则对象将为零。在我有机会记录这些字段之前，你必须四处寻找可用的字段，但总的来说，它们与 API 名称（includedirs、location、flags 等）一致。

一些已知的标记（使用时可随时添加）：

```lua
wks.name
wks.location -- (location where the workspace/solution is written, not the premake-wks.lua file)

prj.name
prj.location -- (location where the project is written, not the premake-prj.lua file)
prj.language
prj.group

cfg.longname
cfg.shortname
cfg.kind
cfg.architecture
cfg.platform
cfg.system
cfg.buildcfg
cfg.buildtarget -- (see [target], below)
cfg.linktarget -- (see [target], below)
cfg.objdir

file.path
file.abspath
file.relpath
file.directory
file.reldirectory
file.name
file.basename -- (file part without extension)
file.extension -- (including '.'; eg ".cpp")

-- These values are available on build and link targets
-- Replace [target] with one of "cfg.buildtarget" or "cfg.linktarget"
--   Eg: %{cfg.buildtarget.abspath}
[target].abspath
[target].relpath
[target].directory
[target].name
[target].basename -- (file part without extension)
[target].extension -- (including '.'; eg ".cpp")
[target].bundlename
[target].bundlepath
[target].prefix
[target].suffix
```

### group

启动 "工作区组"，即包含一个或多个项目的虚拟文件夹。

`group("name")`

name 是虚拟文件夹的名称，应在集成开发环境中显示。可以用正斜杠分隔名称，创建嵌套组。

例子:

```lua
workspace "MyWorkspace"

-- 将项目 "Tests1 "和 "Tests2 "放入名为 "Tests "的虚拟文件夹中

group "Tests"

    project "Tests1"
      -- Tests1 stuff goes here

   project "Tests2"
      -- Tests2 stuff goes here

-- Any project defined after the call to group() will go into that group. The
-- project can be defined in a different script though.

group "Tests"

    include "tests/tests1"
    include "tests/tests2"

-- Groups can be nested with forward slashes, like a file path.

group "Tests/Unit"

-- To "close" a group and put projects back at the root level use
-- an empty string for the name.

group ""

   project "TestHarness"
```

在首次声明项目时，组值会被锁定，但以后可以覆盖：

```lua
local prj = project "Tests1"
prj.group = "NotActuallyATest"

-- 或者为

project("Tests1").group = "NotActuallyATest"
```

### targetdir

设置编译后二进制目标的目标目录。

```lua
targetdir ("path")
```

默认情况下，生成的项目文件会将其编译输出放在与脚本相同的目录下。使用 `targetdir` 功能可以更改该位置。

path 是存放编译目标文件目录的文件系统路径。它是相对于当前执行的脚本文件指定的。

适用于项目配置。

This project separates its compiled output by configuration type.

```lua
project "MyProject"

  filter { "configurations:Debug" }
    targetdir "bin/debug"

  filter { "configurations:Release" }
    targetdir "bin/release"
```

### objdir

设置构建项目时放置对象和其他中间文件的目录。

```lua
objdir ("path")
```

默认情况下，中间文件将存储在项目同一目录下名为 "obj "的目录中。使用 `objdir` 函数可以更改该位置。

为了避免编译配置之间的冲突，Premake 会通过追加编译配置名称、平台名称或项目名称中的一个或多个来确保每个中间目录都是唯一的。你可以使用"!"前缀来阻止这种行为，并允许中间目录重叠。更多信息，请参阅下面的示例。

path 是存放对象文件和中间文件的目录，相对于当前执行的脚本文件指定。可以使用 [令牌](https://premake.github.io/docs/Tokens)。

使用名为 "obj"（默认）的目录存放中间文件。实际目录为 `obj/Debug` 和 `obj/Release`。

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }

project "MyProject"
   objdir "obj"
```

使用名为 "obj"（默认）的目录存放中间文件。实际目录为 `obj/Debug/x32`、`obj/Debug/x64`、`obj/Release/x32` 和 `obj/Release/x64`。

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
   platforms { "x32", "x64" }

project "MyProject"
   objdir "obj"
```

使用标记重新格式化路径。由于最终结果是唯一的，Premake 不会追加任何额外的目录。实际目录将是 `obj/x32_Debug`、 `obj/x64_Debug`、 `obj/x32_Release`、 `obj/x64_Release`。

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
   platforms { "x32", "x64" }

project "MyProject"
   objdir "obj/%{cfg.platform}_%{cfg.buildcfg}"
```

使用"!"前缀可强制使用 Visual Studio 提供的环境变量而不是 Premake 标记来指定目录。

```lua
workspace "MyWorkspace"
   configurations { "Debug", "Release" }
   platforms { "x32", "x64" }

project "MyProject"
   objdir "!obj/$(Platform)_$(Configuration)"
```

### includedirs

指定编译器的包含文件搜索路径。

```lua
includedirs { "paths" }
```

#### Parameters

`paths` specifies a list of include file search directories. Paths should be specified relative to the currently running script file.

#### Applies To

Project configurations.

#### Availability

Premake 4.0 or later.

#### Examples

Define two include file search paths.

```lua
includedirs { "../lua/include", "../zlib" }
```

You can also use wildcards to match multiple directories. The * will match against a single directory, ** will recurse into subdirectories as well.

```lua
includedirs { "../includes/**" }
```

### 



## Globals

### include

查找并执行另一个脚本文件（如果之前未运行过）。

```lua
include("path")
```

#### Parameters

`path` 是脚本文件或目录的文件系统路径。如果指定了目录，Premake 会在该目录中查找名为 `premake5.lua` 的文件，并在找到后运行它。

如果指定的文件或目录已被包含，调用将被忽略。如果要多次执行同一脚本，请使用 Lua 的 [dofile()](http://www.lua.org/manual/5.1/manual.html#pdf-dofile) 代替。

#### Return Value

包含的脚本返回的任何值都会传递给调用者。

#### Availability 

Premake 5.0 or later.

#### Examples 

```lua
-- runs "src/MyApplication/premake5.lua"
include "src/MyApplication"

-- runs "my_script.lua" just once
include "my_script.lua"
include "my_script.lua"
```

### 