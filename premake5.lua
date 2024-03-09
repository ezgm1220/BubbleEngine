
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


include "Engine"
include "Editor"