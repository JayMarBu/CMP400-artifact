#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

layout(location = 0) out vec3 outColour;

layout(set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projectionView;
} ubo;

float linearize_depth(float d,float zNear,float zFar)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

float inv_lerp(float a, float b, float v)
{
	return (v-a)/(b-a);
}

void main()
{
	vec4 pos = ubo.projectionView * vec4(position, 1.0);
	gl_Position = pos;

	outColour = vec3(colour.r,colour.g, colour.b);
}