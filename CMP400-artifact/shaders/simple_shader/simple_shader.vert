#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 uv;

layout(location = 0) out vec3 outColour;

layout(push_constant) uniform Push
{
	mat4 transform;
	mat4 normalMatrix;
} push;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0,-3.0,-1.0));
const float AMBIENT = 0.02;

void main()
{
	gl_Position = push.transform * vec4(position, 1.0);

	//mat3 normalMatrix = transpose(inverse(mat3(push.modelMat)));
	//vec3 normalWorldSpace = normalize(normalMatrix * normal);

	vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

	float lightIntensity = AMBIENT + max(dot(normalWorldSpace, DIRECTION_TO_LIGHT),0);

	outColour = lightIntensity*colour;
}