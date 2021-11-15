#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 outColour;
layout(location = 1) out vec2 outTexCoord;

layout(set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projectionView;
	vec3 lightDir;
} ubo;

layout(push_constant) uniform Push
{
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;

const float AMBIENT = 0.02;

void main()
{
	gl_Position = ubo.projectionView * push.modelMatrix * vec4(position, 1.0);

	vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

	float lightIntensity = AMBIENT + max(dot(normalWorldSpace, ubo.lightDir),0);

	//outColour = lightIntensity*colour;
	outColour = colour;
	outTexCoord = uv;
}