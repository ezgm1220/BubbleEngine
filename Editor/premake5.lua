project "Editor"
	kind "ConsoleApp"-- 静态库
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	-- 设置编译后二进制目标的目标目录
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	-- 设置构建项目时放置对象和其他中间文件的目录
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	defines
	{

	}

	includedirs
	{
        "%{wks.location}/Engine/thirdparty/spdlog/include",
		"%{wks.location}/Engine/src",
		"%{wks.location}/Engine/thirdparty",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Engine"
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