#include "bubblepch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Bubble {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		BB_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		  

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		BB_CORE_ASSERT(status, "Failed to initialize Glad!");

		BB_CORE_INFO("OpenGL Info:");
		BB_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		BB_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		BB_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		BB_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Bubble requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		  

		glfwSwapBuffers(m_WindowHandle);
	}

}
