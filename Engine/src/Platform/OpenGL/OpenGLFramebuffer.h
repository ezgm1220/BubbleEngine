#pragma once

#include "Bubble/Renderer/Framebuffer.h"

namespace Bubble {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
        int ReadPixel(FramebufferTextureFormat TextureType, int x, int y);

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

        virtual void ChangeColorAttachment(uint32_t OriIndex = 0, uint32_t NowID = 0, bool multisampled = false)override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { BB_CORE_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index]; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

        virtual uint32_t GetID() const override { return m_RendererID; }

        virtual void CopyFrameBufferAttachment_All(uint32_t FBID = -1,int AttachmentIndex = -1)override;

        virtual void CopyFrameBufferAttachment_One(uint32_t FBID = -1, int AttachmentIndex = -1)override;
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};

    class OpenGLCubeMapFramebuffer :public CubeMapFramebuffer
    {
    public:
        OpenGLCubeMapFramebuffer(int size);
        virtual ~OpenGLCubeMapFramebuffer();

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(int newsize)override;

        virtual void SetCubeFace(int Faceid, uint32_t CubeMapID, int mip = 0) override;
        virtual void SetTexture(uint32_t TexID)override;
    private:
        uint32_t m_FBOID = 0;
        uint32_t m_RBOID = 0;
        uint32_t m_RendererID = 0;
        int CubeMapSize = 0;
    };
}