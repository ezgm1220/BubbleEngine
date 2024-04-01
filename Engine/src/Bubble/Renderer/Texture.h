#pragma once

#include <string>

#include "Bubble/Core/Base.h"

namespace Bubble {

    enum class TexDataType
    {
        RGB8 = 0, RGBA8, RGB16F, RGBA16F, RG16F,
        RGB, RGBA, RG
    };

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

        virtual bool IsLoaded() const = 0;

        virtual void SetMipMap()const = 0;

        virtual std::string GetPath() = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, TexDataType InternalFormat = TexDataType::RGBA8, TexDataType DataFormat = TexDataType::RGBA);
		static Ref<Texture2D> Create(const std::string& path);
	};

    class CubeMap : public Texture
    {
    public:
        static Ref<CubeMap> Create(const std::string& path);
        static Ref<CubeMap> Create(const uint32_t mapsize,bool MipMap = false);
    };

    class HDRTexture2D : public Texture
    {
    public:
        static Ref<HDRTexture2D> Create(const std::string& path);
    };
}