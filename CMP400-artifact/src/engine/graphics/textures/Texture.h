#pragma once
#include "engine/system/Device.h"

namespace JEngine
{
	class Texture
	{
		// Members ********************************************************************************
	private:
		Device& m_device;

		VkImage m_textureImage;
		VkImageLayout m_textureImageLayout;
		VkDeviceMemory m_textureImageMemory;
		VkImageView m_imageView;
		VkSampler m_sampler;
		uint32_t m_width;
		uint32_t m_height;

		VkDeviceSize m_texelSize;
		uint32_t m_texelCount;

		VkFormat m_imageFormat;
		VkBufferUsageFlags m_usageFlags;

		// Methods ********************************************************************************
	public:
		Texture(
			Device& device,
			uint32_t imageWidth,
			uint32_t imageHeight,
			uint32_t texelCount,
			VkBufferUsageFlags usageFlags,
			VkFormat imageFormat
			// tiling mode,
			// sample count
		);

		/*void CreateTexture(
			uint32_t width,
			uint32_t height,
			VkFormat format,
			VkImageTiling tiling,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory
		);*/

		~Texture();
	};
}