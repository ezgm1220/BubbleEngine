#include "bubblepch.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Bubble {

	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils {

		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(std::vector<FramebufferTextureSpecification>& Attachments,bool multisampled, uint32_t* outID, uint32_t count)
		{
            for(int i = 0; i < count; ++i)
            {
                switch(Attachments[i].TextureFormat)
                {
                    case FramebufferTextureFormat::RGBA8:
                    case FramebufferTextureFormat::RED_INTEGER:
                    case FramebufferTextureFormat::RGBA16F:
                    case FramebufferTextureFormat::RGBA32I:
                    {
                        glCreateTextures(TextureTarget(multisampled), 1, &(outID[i]));
                        break;
                    }
                    case FramebufferTextureFormat::CUBEMAP_RGB16F:
                    {
                        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &(outID[i]));
                        break;
                    }
                }
            }

		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format,GLenum Type, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, Type, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

        static void AttachCubeMap(uint32_t id, int samples, GLenum internalFormat, GLenum format, GLenum Type, uint32_t width, uint32_t height, int index)
        {// 计算天空盒不用这样处理
            bool multisampled = samples > 1;
            if(multisampled)
            {
                //glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                //glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, Type, nullptr);

                for(unsigned int i = 0; i < 6; ++i)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, Type, nullptr);
                }

                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }

            //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			}

			return false;
		}

		static GLenum BubbleFBTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			case FramebufferTextureFormat::RGBA16F: return GL_RGBA16F;
			case FramebufferTextureFormat::RGBA32I: return GL_RGBA_INTEGER;
			}

			BB_CORE_ASSERT(false);
			return 0;
		}

	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.Samples > 1;

		// Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(m_ColorAttachmentSpecifications,multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
                switch(m_ColorAttachmentSpecifications[i].TextureFormat)
                {
                    case FramebufferTextureFormat::RGBA8:
                    {
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, m_Specification.Width, m_Specification.Height, i);
                        break;
                    }
                    case FramebufferTextureFormat::RGBA16F:
                    {
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, GL_FLOAT, m_Specification.Width, m_Specification.Height, i);
                        break;
                    }
                    case FramebufferTextureFormat::RGBA32I:
                    {
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA32I, GL_RGBA_INTEGER, GL_INT, m_Specification.Width, m_Specification.Height, i);
                        break;
                    }
                    case FramebufferTextureFormat::RED_INTEGER:
                    {
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, GL_UNSIGNED_BYTE, m_Specification.Width, m_Specification.Height, i);
                        break;
                    }
                    case FramebufferTextureFormat::CUBEMAP_RGB16F:
                    {
                        //Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, GL_UNSIGNED_BYTE, m_Specification.Width, m_Specification.Height, i);
                        break;
                    }
                }
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
            glCreateTextures(Utils::TextureTarget(multisample), 1, &m_DepthAttachment);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			BB_CORE_ASSERT(m_ColorAttachments.size() <= 5);
			GLenum buffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		BB_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			BB_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		BB_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        return ReadPixel(m_ColorAttachmentSpecifications[attachmentIndex].TextureFormat, x, y);
	}

    int OpenGLFramebuffer::ReadPixel(FramebufferTextureFormat TextureType, int x, int y)
    {
        
        switch(TextureType)
        {
            case FramebufferTextureFormat::RGBA8:
            {
                BB_CORE_ERROR("ReadPixel->FramebufferTextureFormat::RGBA8 No Define");
                break;
            }
            case FramebufferTextureFormat::RGBA16F:
            {
                BB_CORE_ERROR("ReadPixel->FramebufferTextureFormat::RGBA16F No Define");
                break;
            }
            case FramebufferTextureFormat::RGBA32I:  
            {
                glm::ivec4 pixelData_RGBA32I;
                glReadPixels(x, y, 1, 1, GL_RGBA_INTEGER, GL_INT, &pixelData_RGBA32I);
                //BB_CORE_INFO(pixelData_RGBA32I);
                return pixelData_RGBA32I.x;
                break;
            }
            case FramebufferTextureFormat::RED_INTEGER:
            {
                int pixelData_RED_INTEGER = -1;
                glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData_RED_INTEGER);
                return pixelData_RED_INTEGER;
                break;
            }
            case FramebufferTextureFormat::CUBEMAP_RGB16F:
            {
                BB_CORE_ERROR("ReadPixel->FramebufferTextureFormat::CUBEMAP_RGB16F No Define");
            }
        }
    }

    void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		BB_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

        auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];

        switch(m_ColorAttachmentSpecifications[attachmentIndex].TextureFormat)
        {
            case FramebufferTextureFormat::RGBA8:
            {
                BB_CORE_ERROR("ClearAttachment->FramebufferTextureFormat::RGBA8 No Define");
                break;
            }
            case FramebufferTextureFormat::RGBA16F:
            {
                BB_CORE_ERROR("ClearAttachment->FramebufferTextureFormat::RGBA16F No Define");
                break;
            }
            case FramebufferTextureFormat::RGBA32I:
            {
                glm::ivec4 value_RGBA32I(value, value, value, value);
                glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
                    Utils::BubbleFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value_RGBA32I);
                break;
            }
            case FramebufferTextureFormat::RED_INTEGER:
            {
                int value_RED_INTEGER = value;
                glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
                    Utils::BubbleFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value_RED_INTEGER);
                break;
            }
            case FramebufferTextureFormat::CUBEMAP_RGB16F:
            {
                BB_CORE_ERROR("ClearAttachment->FramebufferTextureFormat::CUBEMAP_RGB16F No Define");
                break;
            }
        }

		
		
	}

    void OpenGLFramebuffer::ChangeColorAttachment(uint32_t OriIndex /*= 0*/, uint32_t NowID /*= 0*/, bool multisampled /*= false*/)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + OriIndex, multisampled, NowID, 0);
    }

    void OpenGLFramebuffer::CopyFrameBufferAttachment(uint32_t FBID, int AttachmentIndex)
    {
        switch(AttachmentIndex)
        {
            case 0:
            {
                glBindFramebuffer(GL_READ_FRAMEBUFFER, FBID);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID);
                glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0,
                    m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
                break;
            }
            case 1:
            {
                glBindFramebuffer(GL_READ_FRAMEBUFFER, FBID);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID);
                glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0,
                    m_Specification.Width, m_Specification.Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
                break;
            }
            case -1:
            {
                BB_CORE_WARN("OpenGLFramebuffer::CopyFrameBufferAttachment -> No set AttachmentIndex");
                break;
            }
        }
    }

    OpenGLCubeMapFramebuffer::OpenGLCubeMapFramebuffer(int size)
        :CubeMapSize(size)
    {
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        /*unsigned int captureRBO;
        glGenFramebuffers(1, &m_RendererID);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);*/
    }

    OpenGLCubeMapFramebuffer::~OpenGLCubeMapFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
    }

    void OpenGLCubeMapFramebuffer::Invalidate()
    {
        if(m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
        }
        glCreateFramebuffers(1, &m_RendererID);
    }

    void OpenGLCubeMapFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, CubeMapSize, CubeMapSize);
    }

    void OpenGLCubeMapFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLCubeMapFramebuffer::Resize(int newsize)
    {
        CubeMapSize = newsize;
        //Invalidate();
    }

    void OpenGLCubeMapFramebuffer::SetCubeFace(int Faceid, uint32_t CubeMapID, int mip)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + Faceid, CubeMapID, mip);
    }

    void OpenGLCubeMapFramebuffer::SetTexture(uint32_t TexID)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexID, 0);
    }

}