#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 colour;

layout(location = 0) out vec3 outColour;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	outColour = colour;
}