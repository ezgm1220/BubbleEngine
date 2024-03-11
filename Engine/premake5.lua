project "Engine"
	kind "StaticLib"-- 静态库
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	-- 设置编译后二进制目标的目标目录
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	-- 设置构建项目时放置对象和其他中间文件的目录
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "bubblepch.h"
	pchsource "src/bubblepch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
        "thirdparty/stb_image/**.h",
		"thirdparty/stb_image/**.cpp",
		"thirdparty/glm/glm/**.hpp",
		"thirdparty/glm/glm/**.inl",
	}

	defines
	{
        "_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
        "BB_ENABLE_ASSERTS"
	}

	includedirs
	{
		"src",
		"thirdparty/spdlog/include",
        "%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

    links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "BB_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BB_RELEASE"
		runtime "Release"
		optimize "on"