#pragma once
#include "engine/system/Device.h"
#include "engine/graphics/textures/Texture.h"

namespace JEngine
{
	class TextureManager
	{
		// Members ********************************************************************************
	private:
		Device& m_device;

		VkImage m_textureImage;
		VkDeviceMemory m_textureImageMemory;

		VkImageView m_textureImageView;

		VkSampler m_sampler;

		//std::unordered_map<std::string, unique_ptr<>>

		// Methods ********************************************************************************
	public:
		TextureManager(Device& device);
		~TextureManager();

		inline VkDescriptorImageInfo ImageInfo() const { return VkDescriptorImageInfo{ m_sampler, m_textureImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL}; }

	private:
		void CreateTextureImage();

		void CreateTexture(
			uint32_t width,
			uint32_t height,
			VkFormat format,
			VkImageTiling tiling,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory
		);
		
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		void createTextureImageView();

		VkImageView createImageView(VkImage image, VkFormat format);

		void CreateTextureSampler();
	};
}