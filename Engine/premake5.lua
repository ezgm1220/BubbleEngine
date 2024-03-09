project "Engine"
	kind "StaticLib"-- ��̬��
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	-- ���ñ���������Ŀ���Ŀ��Ŀ¼
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	-- ���ù�����Ŀʱ���ö���������м��ļ���Ŀ¼
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
		"src"
	}


	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "BE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BE_RELEASE"
		runtime "Release"
		optimize "on"