#pragma once
#define WIN32_LEAN_AND_MEAN

// Containers *************************************************************************************
#include <vector>
#include <unordered_map>
#include <string>
#include <set>
#include <unordered_set>
#include <array>

// Utilities **************************************************************************************
#include <cassert>
#include <iostream>
#include <type_traits>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <limits>
#include <memory>
#include <cstring>
#include <numeric>
#include <Windows.h>

// Vulkan *****************************************************************************************
#include <vulkan/vulkan.h>

// GLM ********************************************************************************************
//#include <glm/vec4.hpp>
//#include <glm/mat4x4.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// GLFW *******************************************************************************************
#include <GLFW/glfw3.h>

// JEngine ****************************************************************************************
#include "utils/Utils.h"

// Tiny OBJ Loader ********************************************************************************
//#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>

// ImGui ******************************************************************************************
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>