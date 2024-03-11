
workspace "Bubble"-- 工作区
	architecture "x86_64"
	startproject "Editor"

	configurations{"Debug","Release"}-- 编译配置

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"-- 输出的文件名

-- 存放以后将要引用的include文件位置
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Engine/thirdparty/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Engine/thirdparty/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Engine/thirdparty/imgui"
IncludeDir["glm"] = "%{wks.location}/Engine/thirdparty/glm"
IncludeDir["stb_image"] = "%{wks.location}/Engine/thirdparty/stb_image"

group "Dependencies"
	include "Engine/thirdparty/GLFW"
	include "Engine/thirdparty/Glad"
	include "Engine/thirdparty/imgui"
group ""

include "Engine"
include "Editor"