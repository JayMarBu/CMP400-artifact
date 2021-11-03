#pragma once

// safe heap cleanup macros ***********************************************************************
#define CLEANUP(obj) if(obj != NULL) {delete obj; obj = NULL;}
#define CLEANUP_MAP(obj) for (auto pair : obj) {CLEANUP(pair.second);}obj.clear();

// utility macros *********************************************************************************
#define ASSERT_TYPE_INHERITANCE(_parent_ , _t_, _message_) static_assert(std::is_base_of<_parent_, _t_>::value == true, _message_);

#define REMOVE_COPY_CONSTRUCTOR(_class_) _class_(const _class_&) = delete; _class_ operator=(const _class_&) = delete;

#define SAFE_RUN_VULKAN_FUNC(_create_method_, _message_) if(_create_method_ != VK_SUCCESS) {throw std::runtime_error(_message_);}

#define VEC_IS_ZERO(_vec_) (glm::dot(_vec_, _vec_) > std::numeric_limits<float>::epsilon())

namespace JEngine
{
	// from: https://stackoverflow.com/a/57595105
	template <typename T, typename... Rest>
	void HashCombine(std::size_t& seed, const T& v, const Rest&... rest) 
	{
		seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
		(HashCombine(seed, rest), ...);
	};
}